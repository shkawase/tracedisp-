/**
 * @file   TGeometryConfig.cc
 * @brief  geometry configuration class
 *
 * @date   Created       : 2016-11-25 16:58:31 JST
 *         Last Modified : 2016-11-28 15:17:35 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TGeometryConfig.h"

#include "traceUtil.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

using trace::TGeometryConfig;

namespace {
   template <typename T>
   void LoadOptionalScalar(const YAML::Node *node,const char* key, T *val) {
      if(const YAML::Node *p = node->FindValue(key)) {
      (*p) >> *val;
      }
   }
}

TGeometryConfig::TGeometryConfig()
   : fTargetCenterX(0.), fTargetCenterY(-4000.),
     fMagnetAngle(-30.), fExitAngle(-60.),
     fExitWindowX(2663.7), fExitWindowY(1746.8), fExitWindowW(3340.0),
     fNScale(4), fScaleSeparation(1000.), fScaleWidth(4000.), fScaleColor(7)
{}

TGeometryConfig* TGeometryConfig::GetInstance()
{
   static TGeometryConfig instance;
   return &instance;
}

void TGeometryConfig::LoadFile(const char* filename)
{
   if (!FileExists(filename)) {
      fprintf(stderr, "%s does not exist. Use default config.",filename);
      return;
   }

   ifstream ifs(filename);
   if(!ifs) {
      printf("Failed to load config file: %s\n",filename);
      return;
   }

   try {
      YAML::Node doc;
      YAML::Parser parser(ifs);
      parser.GetNextDocument(doc);

      LoadOptionalScalar(&doc,"MagnetAngle",&fMagnetAngle);
      LoadOptionalScalar(&doc,"ExitAngle",&fExitAngle);

      if(const YAML::Node *p = doc.FindValue("Target")) {
	 if(const YAML::Node *pCenter = p->FindValue("Center")) {
	    (*pCenter)[0] >> fTargetCenterX;
	    (*pCenter)[1] >> fTargetCenterY;
	 }
	 LoadOptionalScalar(p,"Width",&fExitWindowW);
      }
      if(const YAML::Node *p = doc.FindValue("ExitWindow")) {
	 if(const YAML::Node *pCenter = p->FindValue("Center")) {
	    (*pCenter)[0] >> fExitWindowX;
	    (*pCenter)[1] >> fExitWindowY;
	 }
	 LoadOptionalScalar(p,"Width",&fExitWindowW);
      }
      if(const YAML::Node *p = doc.FindValue("Scale")) {
	 LoadOptionalScalar(p,"N",&fNScale);
	 LoadOptionalScalar(p,"Separation",&fScaleSeparation);
	 LoadOptionalScalar(p,"Width",&fScaleWidth);
	 LoadOptionalScalar(p,"Color",&fScaleColor);
      }
   } catch (YAML::Exception& e) {
      printf("Error occurred while loading config file: %s\n%s\n",
	     filename,e.what());
   }
}
