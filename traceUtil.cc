#include "traceUtil.h"
#include "TGeneralConfig.h"
#include "TGeometryConfig.h"
#include "TSamuraiTracer.h"
#include "AddObjects.h"

#include <fstream>
#include <sys/stat.h>

#include <TStyle.h>
#include <TLatex.h>
#include <TMath.h>
#include <TPolyLine.h>
#include <TLine.h>
#include <TGraph.h>
#include <TEllipse.h>
#include <TObjArray.h>

namespace trace {

std::vector<trace_setting> LoadTraceSettings(const char* filename)
{
   std::vector<trace_setting> settings;
   std::ifstream ifs(filename);
   if (!ifs) return settings;

   std::string line;
   while(getline(ifs,line)) {
      trace_setting setting;
      const int result = sscanf(line.c_str(),"%d, %d, %d, %f, %f, %f, %*s",
				&setting.color,&setting.z,&setting.a,
				&setting.p,&setting.x,&setting.theta);
      if (result != 6) continue;

      const std::size_t first_comma = line.find_first_of('\'');
      const std::size_t last_comma = line.find_last_of('\'');
      if (first_comma == std::string::npos
	 || first_comma == last_comma) continue;
      setting.comment = line.substr(first_comma+1,last_comma - first_comma-1);
      settings.push_back(setting);
   }

   return settings;
}

bool TestBounds(TObjArray *bounds, double x, double y)
{
   for (Int_t i = 0, n = bounds->GetEntriesFast(); i != n; ++i) {
      TGraph *const g = (TGraph*)bounds->At(i);
      if(g->IsInside(x,y)) {
	 return true;
      }
   }

   return false;
}

template <class T>
void TranslatePolyLine(T *pl, double dx, double dy)
{
   const int N = pl->GetN();
   for (int i = 0; i != N; ++i) {
      const double x = (pl->GetX())[i];
      const double y = (pl->GetY())[i];
      pl->SetPoint(i,x+dx,y+dy);
   }
}

void Translate(Drawee_t *obj, Double_t dx, Double_t dy)
{
   if(TPolyLine *pl = dynamic_cast<TPolyLine*>(obj)) {
      TranslatePolyLine(pl,dx,dy);
   }
}

template <class T>
void RotatePolyLine(T *pl, double angle, double xo = 0, double yo = 0)
{
   const int N = pl->GetN();
   const double angle_rad = angle * TMath::DegToRad();
   for (int i = 0; i != N; ++i) {
      const double x = (pl->GetX())[i];
      const double y = (pl->GetY())[i];
      const double dx = x - xo;
      const double dy = y - yo;
      const double xx = xo + dx * cos(angle_rad) - dy * sin(angle_rad);
      const double yy = yo + dx * sin(angle_rad) + dy * cos(angle_rad);
      pl->SetPoint(i,xx,yy);
   }
}

void RotatePoint(Double_t *p, Double_t angle, Double_t xo = 0, Double_t yo = 0)
{
   const Double_t angle_rad = angle * TMath::DegToRad();
   Double_t x = p[0];
   Double_t y = p[1];
   const Double_t dx = x - xo;
   const Double_t dy = y - yo;
   const Double_t xx = xo + dx * cos(angle_rad) - dy * sin(angle_rad);
   const Double_t yy = yo + dx * sin(angle_rad) + dy * cos(angle_rad);
   p[0] = xx;
   p[1] = yy;
}

void RotateLine(TLine *l, Double_t angle, Double_t xo = 0, Double_t yo = 0)
{
   Double_t p1[2] = {l->GetX1(),l->GetY1()};
   Double_t p2[2] = {l->GetX2(),l->GetY2()};
   RotatePoint(p1,angle,xo,yo);
   RotatePoint(p2,angle,xo,yo);

   l->SetX1(p1[0]);
   l->SetY1(p1[1]);
   l->SetX2(p2[0]);
   l->SetY2(p2[1]);
}

void Rotate(Drawee_t *obj, Double_t angle, Double_t xo, Double_t yo)
{
   if(TPolyLine *pl = dynamic_cast<TPolyLine*>(obj)) {
      RotatePolyLine(pl,angle,xo,yo);
   } else if (TGraph *g = dynamic_cast<TGraph*>(obj)) {
      RotatePolyLine(g,angle,xo,yo);
   } else if (TLine *l = dynamic_cast<TLine*>(obj)) {
      RotateLine(l,angle,xo,yo);
   }
}

Drawee_t* MakeLine(double x1, double y1, double x2, double y2, TAttLine al)
{
   TLine *l = new TLine(x1,y1,x2,y2);
   al.Copy(*l);
   return l;
}

Drawee_t* MakePolyLine(int n, double *x, double *y, TAttLine al, TAttFill af)
{
   TPolyLine *pl = new TPolyLine(n,x,y);
   al.Copy(*pl);
   af.Copy(*pl);
   return pl;
}

Drawee_t* MakeCircle(double x, double y, double r, TAttLine al, TAttFill af)
{
   TEllipse *el = new TEllipse(x,y,r);
   al.Copy(*el);
   af.Copy(*el);
   return el;
}

Drawee_t *MakeRectangle(Double_t xc, Double_t yc, Double_t w, Double_t h,
		       Double_t angle, TAttLine al, TAttFill af)
{
   const Int_t NPOINT = 5;
   Double_t x[5];
   Double_t y[5];
   Double_t angle_rad = angle * TMath::DegToRad();

   for(int i = 0; i != 5; ++i) {
      Double_t xx = w/2 * ((i==1)||(i==2)?1:-1) + xc;
      Double_t yy = h/2 * (i&2?1:-1) + yc;
      x[i] = xx * cos(angle_rad) - yy * sin(angle_rad);
      y[i] = xx * sin(angle_rad) + yy * cos(angle_rad);
   }

   TPolyLine *pl = new TPolyLine(NPOINT,x,y);
   al.Copy(*pl);
   af.Copy(*pl);

   return pl;
}

Drawee_t *MakeLatexNDC(Double_t x, Double_t y, const char *text,
		      TAttText at)
{
   TLatex *latex = new TLatex(x,y,text);
   latex->SetNDC();
   at.Copy(*latex);
   return latex;
}

bool FileExists(const char* path)
{
   struct stat st;
   return !stat(path, &st);
}

void Draw(Drawee_t *drawee)
{
   if(TAttFill *af = dynamic_cast<TAttFill*>(drawee)) {
      if (af->GetFillStyle()) drawee->Draw("f");
   }
   drawee->Draw("same");
//   drawee->Draw("same");
   // DrawDXF(drawee); // TODO: will be implemented
}

const TGeneralConfig* InitConfig(int argc, char *argv[], int *errno)
{
   TGeneralConfig* conf = new TGeneralConfig();
   { /* analyze options */
      char opt;
      while((opt = getopt(argc,argv,"fg:hm:o:")) != -1){
	 switch (opt) {
	    case 'f':
	       conf->SetOverwrite();
	       break;
	    case 'o':
	       conf->SetOutFile(optarg);
	       break;
	    case 'g':
	       conf->SetGeometryConfigFile(optarg);
	       break;
	    case 'm':
	       conf->SetMagnetConfigFile(optarg);
	       break;
	    case 'h':
	       Usage();
	       exit(0);
	    default:
	       fprintf(stderr, "unknown option \"-%c\"\n", opt);
	       Usage();
	       *errno = -1;
	       return NULL;
	 }
      }
      argc -= optind;
      argv += optind;
   }

   if(!argc) {
      fprintf(stderr,"Input file not specified.\n");
      Usage();
      *errno = -1;
      return NULL;
   }

   conf->SetInputFile(argv[0]);

   if(!FileExists(conf->GetInputFile())) {
      fprintf(stderr,"Inputfile (%s) does not exist.\n",
	      conf->GetInputFile());
      *errno = -2;
      return NULL;
   }

   if(!conf->GetOverwrite() && FileExists(conf->GetOutFile())) {
      fprintf(stderr,"Outfile (%s) exists. Use -f option to overwrite.\n",
	      conf->GetOutFile());
      *errno = -2;
      return NULL;
   }

   printf("Input           = %s\n", conf->GetInputFile());
   printf("Output          = %s\n", conf->GetOutFile());
   printf("Geometry config = %s\n", conf->GetGeometryConfigFile());
   printf("Magnet config   = %s\n", conf->GetMagnetConfigFile());

   return conf;
}

void SetStyles()
{
   gStyle->SetOptStat(0);
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);
   gStyle->SetLabelSize(0,"XYZ");
   gStyle->SetTickLength(0.02,"XYZ");
}

void Usage()
{
   printf("usage: trace [-h] [-o <output>] [-g <geometry_config>] [-m <magnet_config>] <input>\n");
}

void AddTrajectory(art::TSamuraiTracer *tracer, const trace_setting &setting,
		   TObjArray *drawees, TObjArray *bounds,
		   const TGeneralConfig *conf, int n)
{
// TODO: make a new configuration class to remove passing of conf, yinit
// TODO: get rid of n
   const TGeometryConfig *geoConf = TGeometryConfig::GetInstance();

   const double xi[3] = {-geoConf->GetTargetCenterX() - setting.x,
			 geoConf->GetTargetCenterY(), 0.};
   const double p = setting.p * setting.a;
   const double theta_rad = setting.theta * TMath::DegToRad();

   const double pi[3] = {- p * sin(theta_rad), p * cos(theta_rad),0.};
   tracer->Trace(xi,pi,setting.z);

   std::vector<double> x(tracer->GetXArray());
   std::vector<double> y(tracer->GetYArray());

   const int NPOINTS = x.size();
   Int_t n_last = NPOINTS;
   for (size_t i = 0; i != x.size(); ++i) {
      if(TestBounds(bounds,x[i],y[i])
	 || !conf->IsInsideViewPort(x[i],y[i])) {
	 n_last = i;
	 break;
      }
   }

   const double flightlength =
      (NPOINTS == n_last) ? tracer->GetFlightLength() : 0./0.;
   printf("fl[%d] = %.1f mm\n",n,flightlength);

   drawees->Add(MakePolyLine(n_last,&x[0],&y[0],conf->GetTrajAttLine(setting.color)));
   AddLegend(drawees,conf,setting.comment.c_str(),setting.color);
}

}
