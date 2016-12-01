/**
 * @file   AddObjects.cc
 * @brief
 *
 * @date   Created       : 2016-11-24 18:14:43 JST
 *         Last Modified : 2016-11-29 18:53:14 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "AddObjects.h"
#include "traceUtil.h"
#include "TGeometryConfig.h"
#include "TDetector.h"
#include "TGeneralConfig.h"

#include <TH2F.h>
#include <TGraph.h>
#include <TObjArray.h>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace trace {

void AddMagnet(TObjArray *drawees, TObjArray *bounds)
{
   const TGeometryConfig* geoConf = TGeometryConfig::GetInstance();
   const double magnet_angle = geoConf->GetMagnetAngle();

   { /* pit */
      const Double_t pit_w = 10000;
      const Double_t pit_d = 10000;
      drawees->Add(MakeRectangle(0,0,pit_w,pit_d,0,
				trace::kDottedLine));
   }

   /* rotating table */
   drawees->Add(MakeCircle(0.,0.,4300.,trace::kDottedLine));

   /* dipole magnet */
   const Double_t magnet_w = 6700.;
   const Double_t magnet_d = 3500.;
   const Int_t n_sy = 6;
   for (int i = 0; i != 4 ; ++i) { /* side yoke */
      int xsgn = i % 2 ? 1:-1;
      int ysgn = i / 2 ? 1:-1;
      Double_t sy_x[n_sy];
      Double_t sy_y[n_sy];
      sy_x[0] = magnet_w/2. - 700.;
      sy_y[0] = -magnet_d/2.;
      sy_x[1] = magnet_w/2. - 1600.;
      sy_y[1] = -1200./2.;
      sy_x[2] =  sy_x[1];
      sy_y[2] = -200.;
      sy_x[3] = magnet_w/2.;
      sy_y[3] = sy_y[2];
      sy_x[4] = sy_x[3];
      sy_y[4] = sy_y[0];
      sy_x[5] = sy_x[0];
      sy_y[5] = sy_y[0];
      for (int j = 0; j!=n_sy; ++j) {
	 sy_x[j] *= xsgn;
	 sy_y[j] *= ysgn;
      }
      TGraph *sy = new TGraph(n_sy,sy_x,sy_y);
      sy->SetFillColor(kBlue);
      sy->SetFillStyle(3004);
      Rotate(sy,magnet_angle);
      drawees->Add(sy);
      bounds->Add(sy);
   }

   /* pole */
   drawees->Add(MakeCircle(0.,0.,1000.));
   drawees->Add(MakeCircle(0.,0.,1580.));

   { /* field cramp */
      const Double_t fc_w = 5200;
      const Double_t fc_d = 250;
      TObject *field_cramp_u = MakeRectangle(0,-1625,fc_w,fc_d);
      Rotate(field_cramp_u,magnet_angle);
      TObject *field_cramp_d = field_cramp_u->Clone();
      Rotate(field_cramp_u,180.);
      drawees->Add(field_cramp_u);
      drawees->Add(field_cramp_d);
   }

   { /* magnet center line */
      TObject *mc = MakeLine(0,-magnet_d/2.,0,magnet_d/2.,
			     trace::kDashedDottedLine);
      Rotate(mc,magnet_angle);
      drawees->Add(mc);
   }
}

void AddAxes(double xmin, double ymin, double xmax, double ymax,
	     TObjArray *drawees)
{
   /* frame */
   drawees->Add(new TH2F("frame","",1,xmin,xmax,1,ymin,ymax));

   /* axes */
   drawees->Add(MakeLine(xmin,0,xmax,0,trace::kDashedLine));
   drawees->Add(MakeLine(0,ymin,0,ymax,trace::kDashedLine));
}


void AddExitObjects(TObjArray *drawees, TObjArray*)
{
   const TGeometryConfig* geoConf = TGeometryConfig::GetInstance();
   const double magnet_angle = geoConf->GetMagnetAngle();
   const double exit_angle = geoConf->GetExitAngle();

   /* center stand */
   {
      const TAttLine centerstand_al(kMagenta,1,1);
      drawees->Add(MakeRectangle(0,1845,280,50,magnet_angle,
				centerstand_al));
      drawees->Add(MakeRectangle(125,1870.+1672.312/2.,15,1672.321,magnet_angle,
				centerstand_al));
   }

   /* exit window */
   const Double_t CPwinX = geoConf->GetExitWindowX();
   const Double_t CPwinY = geoConf->GetExitWindowY();
   const Double_t CPwinW = geoConf->GetExitWindowW();
   {
      Drawee_t *ew = MakeRectangle(CPwinX,CPwinY,CPwinW,0);
      Rotate(ew,exit_angle,CPwinX,CPwinY);
      drawees->Add(ew);
   }

   /* 60 deg. scale */
   const Int_t   n_scale = geoConf->GetScaleN();
   const double  scale_w   = geoConf->GetScaleWidth();
   const double  scale_sep = geoConf->GetScaleSeparation();
   const Style_t scale_color = geoConf->GetScaleColor();

   for (int i = 1; i != n_scale + 1; ++i) {
      Double_t scale_x[2] = {CPwinX-scale_w/2.,CPwinX+scale_w/2.};
      Double_t scale_y = CPwinY+scale_sep*i;
      Drawee_t *scale = MakeLine(scale_x[0],scale_y,
				 scale_x[1],scale_y,
				 TAttLine(scale_color,1,1));
      Rotate(scale,exit_angle,CPwinX,CPwinY);
      drawees->Add(scale);
   }

   /* 60 deg. line */
   const double stdline_length = 5000.;
   Drawee_t *sixty_deg = MakeLine(CPwinX,CPwinY,
				  CPwinX,CPwinY+stdline_length,
				  TAttLine(scale_color,3,1));
   Rotate(sixty_deg,exit_angle,CPwinX,CPwinY);
   drawees->Add(sixty_deg);
}

void AddDetectors(TObjArray *drawees, const TGeneralConfig *conf){
   const TGeometryConfig* geoConf = TGeometryConfig::GetInstance();
   TDetector::SetOrigin(geoConf->GetExitWindowX(),geoConf->GetExitWindowY());
   TDetector::SetAngle(geoConf->GetExitAngle());

   std::ifstream ifs("detector.conf");
   YAML::Node doc;
   YAML::Parser parser(ifs);
   parser.GetNextDocument(doc);

   for(size_t i = 0; i!=doc.size(); ++i) {
      TDetector *detector = TDetector::Create(doc[i]);
      if(!detector) continue;
      drawees->Add(detector);
      AddLegend(drawees,conf,detector->GetTitle());
   }
}

static float legendXOffset = 0.;
static float legendYOffset = 0.;
static float legendSpacing = 0.1;

void AddLegend(TObjArray *drawees, const TGeneralConfig *conf,
	       const char* latex, Color_t color)
{
   drawees->Add(MakeLatexNDC(trace::legendXOffset,trace::legendYOffset,
			     latex,conf->GetLegendAttText(color)));
   ForwardLegend();
}

void ForwardLegend()
{
   trace::legendYOffset -= trace::legendSpacing;
}

void SetLegendXOffset(float val) {legendXOffset = val;}
void SetLegendYOffset(float val) {legendYOffset = val;}
void SetLegendSpacing(float val) {legendSpacing = val;}
float GetLegendXOffset() {return legendXOffset;}
float GetLegendYOffset() {return legendYOffset;}
float GetLegendSpacing() {return legendSpacing;}

}
