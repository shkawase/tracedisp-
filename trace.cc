#include "TSamuraiTracer.h"
#include "traceUtil.h"
#include "TGeneralConfig.h"
#include "TGeometryConfig.h"
#include "TMagnetConfig.h"
#include "AddObjects.h"
#include "TDetector.h"

#include <TCanvas.h>
#include <TObjArray.h>

#include <fstream>
#include <yaml-cpp/yaml.h>

int main(int argc, char* argv[])
{
   using namespace trace;
   int errno = 0;
   const TGeneralConfig *const gconf = InitConfig(argc,argv,&errno);
   if(errno) {
      exit(errno);
   }

   TGeometryConfig *const geoConf = TGeometryConfig::GetInstance();
   geoConf->LoadFile(gconf->GetGeometryConfigFile());

   TMagnetConfig *const magConf = TMagnetConfig::GetInstance();
   magConf->LoadFile(gconf->GetMagnetConfigFile());
   if (!magConf->IsGood()) {
      return -3;
   }

   SetLegendXOffset(gconf->GetLegendXOffset());
   SetLegendYOffset(gconf->GetLegendYOffset());
   SetLegendSpacing(gconf->GetLegendSpacing());

   TObjArray drawees;
   drawees.SetOwner(kTRUE);
   TObjArray bounds;

   SetStyles();

   AddAxes(gconf->GetXmin(),gconf->GetYmin(),
	   gconf->GetXmax(),gconf->GetYmax(),&drawees);
   AddMagnet(&drawees,&bounds);
   AddExitObjects(&drawees,&bounds);

   /* trajectory */
   art::TSamuraiTracer *tracer = new art::TSamuraiTracer;
   tracer->LoadField(magConf->GetFieldFile());
   if (!tracer->IsGood()) {
      return -4;
   }

   if(magConf->CentralFieldIsDefined()) {
      tracer->ScaleCentralFieldTo(magConf->GetCentralField());
   }
   tracer->SetMaxPoint(gconf->GetTrajectoryMaxPoint());
   tracer->SetStepLength(gconf->GetTrajectoryStepLength());
   tracer->SetRotationAngle(geoConf->GetMagnetAngle());

   tracer->SetEndPlaneAngle(-60.);
   tracer->SetEndPlaneDistance(6750);

   {
      const TString &b = TString::Format("#it{B}_{#it{z}}(0,0,0) = %.2f T",
					 tracer->GetCentralField());
      AddLegend(&drawees, gconf, b.Data());
   }

   AddDetectors(&drawees,gconf);

   ForwardLegend();

   const SettingVec_t settings = LoadTraceSettings(gconf->GetInputFile());
   if (settings.empty()) {
      fprintf(stderr,"Cannot load input file: %s\n",gconf->GetInputFile());
      return -1;
   }

   for(SettingVec_t::const_iterator it = settings.begin();
       it != settings.end(); ++it) {
      const int n = it - settings.begin();
      AddTrajectory(tracer,*it,&drawees,&bounds,gconf,n);
   }

   TCanvas *const canvas = new TCanvas("canvas","canvas",
				       gconf->GetCanvasW(),gconf->GetCanvasH());
   for(Int_t i = 0; i != drawees.GetEntriesFast(); ++i) {
      Draw(drawees.At(i));
   }
   canvas->SaveAs(gconf->GetOutFile());
   return 0;
}
