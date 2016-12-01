/**
 * @file   TGeneralConfig.cc
 * @brief  general configuration class
 *
 * @date   Created       : 2016-11-25 10:46:29 JST
 *         Last Modified : 2016-11-25 18:36:04 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TGeneralConfig.h"
#include "traceUtil.h"

#include <fstream>
#include <TStyle.h>

#include <yaml-cpp/yaml.h>

using trace::TGeneralConfig;

const char* const TGeneralConfig::kDefaultConfigFile("tracedisp.conf");
const char* const TGeneralConfig::kDefaultOutFile("trace.pdf");
const char* const TGeneralConfig::kDefaultGeoConfigFile("geometry.conf");
const char* const TGeneralConfig::kDefaultMagnetConfigFile("magnet.conf");

namespace {
   template <typename T>
   void LoadOptionalScalar(const YAML::Node *node,const char* key, T *val) {
      if(const YAML::Node *p = node->FindValue(key)) {
      (*p) >> *val;
      }
   }
}

TGeneralConfig::TGeneralConfig(const char* filename)
   : fXmin(-8000), fXmax(8000), fYmin(-6000), fYmax(10000),
     fCanvasW(640), fCanvasH(640),
     fLegendAlign(12), fLegendFont(gStyle->GetTextFont()), fLegendSize(0.018),
     fLegendXOffset(0.12), fLegendYOffset(0.87), fLegendSpacing(0.0225),
     fTrajStyle(1), fTrajWidth(1), fTrajMaxPoint(500), fTrajStepLength(50),
     fOverwrite(false),
     fInputFile(""), fOutFile(kDefaultOutFile),
     fGeoConfigFile(kDefaultGeoConfigFile),
     fMagnetConfigFile(kDefaultMagnetConfigFile)
{
   if (!FileExists(filename)) {
      fprintf(stderr, "%s does not exist. Use default config.",filename);
      return;
   }

   LoadConfigFile(filename);
}

void TGeneralConfig::LoadConfigFile(const char* filename)
{
   ifstream ifs(filename);
   if(!ifs) {
      printf("Failed to load config file: %s\n",filename);
      return;
   }

   try {
      YAML::Node doc;
      YAML::Parser parser(ifs);
      parser.GetNextDocument(doc);

      if(const YAML::Node *pVP = doc.FindValue("ViewPort")) {
	 if(const YAML::Node *pXRange = pVP->FindValue("XRange")) {
	    (*pXRange)[0] >> fXmin;
	    (*pXRange)[1] >> fXmax;
	    if (fXmax < fXmin) {
	       std::swap(fXmin,fXmax);
	    }
	 }
	 if(const YAML::Node *pYRange = pVP->FindValue("YRange")) {
	    (*pYRange)[0] >> fYmin;
	    (*pYRange)[1] >> fYmax;
	    if (fYmax < fYmin) {
	       std::swap(fYmin,fYmax);
	    }
	 }
      }
      if(const YAML::Node *pCanvas = doc.FindValue("Canvas")) {
	 LoadOptionalScalar(pCanvas,"Width",&fCanvasW);
	 LoadOptionalScalar(pCanvas,"Height",&fCanvasH);
      }
      if(const YAML::Node *pLegend = doc.FindValue("Legend")) {
	 LoadOptionalScalar(pLegend,"Align",&fLegendAlign);
	 LoadOptionalScalar(pLegend,"Font",&fLegendFont);
	 LoadOptionalScalar(pLegend,"Size",&fLegendSize);
	 LoadOptionalScalar(pLegend,"Spacing",&fLegendSpacing);
	 if(const YAML::Node *pOffset = pLegend->FindValue("Offset")) {
	    (*pOffset)[0] >> fLegendXOffset;
	    (*pOffset)[1] >> fLegendYOffset;
	 }
      }
      if(const YAML::Node *pTraj = doc.FindValue("Trajectory")) {
	 LoadOptionalScalar(pTraj,"Style",&fTrajStyle);
	 LoadOptionalScalar(pTraj,"Width",&fTrajWidth);
	 LoadOptionalScalar(pTraj,"MaxPoint",&fTrajMaxPoint);
	 LoadOptionalScalar(pTraj,"StepLength",&fTrajStepLength);
      }
   } catch (YAML::Exception& e) {
      printf("Error occurred while loading config file: %s\n%s\n",
	     filename,e.what());
   }
}

bool TGeneralConfig::IsInsideViewPort(double x, double y) const
{
   return (fXmin < x && x < fXmax
	   && fYmin < y && y < fYmax);
}
