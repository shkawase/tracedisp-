/**
 * @file   THodoscope.cc
 * @brief  hodoscope
 *
 * @date   Created       : 2016-11-29 15:58:57 JST
 *         Last Modified : 2016-11-29 18:33:50 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "THodoscope.h"

#include "traceUtil.h"

#include <TObjArray.h>
#include <yaml-cpp/yaml.h>

using trace::THodoscope;

const char* const trace::THodoscope::kTag = "!trace/detector:Hodo";

THodoscope::THodoscope(const YAML::Node &node)
{
   std::string name;
   const YAML::Node &nname = node["Name"];
   nname >> name;

   float centerX, centerY;
   const YAML::Node &center = node["Center"];
   center[0] >> centerX;
   center[1] >> centerY;

   SetTitle(TString::Format("%s (%.2f, %.2f)",
			    name.c_str(),centerX,centerY));

   { /* make effective area */
      float sizeX, sizeY;
      const YAML::Node &p = node["EffectiveArea"];
      const YAML::Node &size = p["Size"];
      size[0] >> sizeX;
      size[1] >> sizeY;

      fObjects->Add(MakeRectangle(centerX,centerY,sizeX,sizeY));
   }

   { /* make table */
      float sizeX, sizeY, distance;
      const YAML::Node &p = node["Table"];
      const YAML::Node &size = p["Size"];
      size[0] >> sizeX;
      size[1] >> sizeY;
      const YAML::Node &dist = p["Distance"];
      dist >> distance;

      fObjects->Add(MakeRectangle(centerX,centerY+distance+sizeY/2,
				  sizeX,sizeY));
   }

   { /* make ch (what is ch??) */
      float sizeX, sizeY, distance;
      const YAML::Node &p = node["Ch"];
      const YAML::Node &size = p["Size"];
      size[0] >> sizeX;
      size[1] >> sizeY;
      const YAML::Node &dist = p["Distance"];
      dist >> distance;

      fObjects->Add(MakeRectangle(centerX,centerY+distance,
				  sizeX,sizeY,0,
				  TAttLine(kRed,1,1),
      				  TAttFill(0,0)));
   }
}

THodoscope::~THodoscope(){}

