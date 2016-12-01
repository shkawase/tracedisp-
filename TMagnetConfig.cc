/**
 * @file   TMagnetConfig.cc
 * @brief  magnet configuration class
 *
 * @date   Created       : 2016-11-28 15:15:53 JST
 *         Last Modified : 2016-11-28 16:59:31 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TMagnetConfig.h"

#include "traceUtil.h"

#include <fstream>
#include <cmath>

#include <yaml-cpp/yaml.h>

using trace::TMagnetConfig;

namespace {
   template <typename T>
   void LoadOptionalScalar(const YAML::Node *node,const char* key, T *val) {
      if(const YAML::Node *p = node->FindValue(key)) {
      (*p) >> *val;
      }
   }
}

TMagnetConfig::TMagnetConfig()
   : fFieldFile(""), fCentralField(0.),
     fCentralFieldIsDefined(false), fIsGood(false)
{
}

TMagnetConfig* TMagnetConfig::GetInstance()
{
   static TMagnetConfig instance;
   return &instance;
}

void TMagnetConfig::LoadFile(const char *filename)
{
   if (!FileExists(filename)) {
      fprintf(stderr, "Error: %s does not exist.\n",filename);
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

      LoadOptionalScalar(&doc,"File",&fFieldFile);
      if(const YAML::Node *p = doc.FindValue("CentralField")) {
	 (*p) >> fCentralField;
	 fCentralFieldIsDefined = true;
      }

      fIsGood = true;
   } catch (YAML::Exception& e) {
      printf("Error occurred while loading config file: %s\n%s\n",
	     filename,e.what());
   }
}

bool TMagnetConfig::CentralFieldIsDefined() const
{
   return fCentralFieldIsDefined;
}
