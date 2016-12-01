/**
 * @file   TDriftChamber.cc
 * @brief  drift chamber
 *
 * @date   Created       : 2016-11-29 17:54:14 JST
 *         Last Modified : 2016-11-29 18:36:43 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TDriftChamber.h"

#include "traceUtil.h"

#include <TObjArray.h>
#include <yaml-cpp/yaml.h>

using trace::TDriftChamber;

const char* const trace::TDriftChamber::kTag = "!trace/detector:DC";

TDriftChamber::TDriftChamber(const YAML::Node &node)
{
   std::string name;
   const YAML::Node &nname = node["Name"];
   nname >> name;

   float posX, posY;
   const YAML::Node &pos = node["Position"];
   pos[0] >> posX;
   pos[1] >> posY;

   SetTitle(TString::Format("%s (%.2f, %.2f)",
			    name.c_str(),posX,posY));

   float centerX, centerY;
   { /* make chassis */
      float sizeX, sizeY;
      const YAML::Node &p = node["Chassis"];
      const YAML::Node &size = p["Size"];
      size[0] >> sizeX;
      size[1] >> sizeY;

      centerX = posX;
      centerY = posY + sizeY/2;

      fObjects->Add(MakeRectangle(centerX,centerY,
				  sizeX,sizeY));
   }

   { /* make effective area */
      float sizeX, sizeY;
      const YAML::Node &p = node["EffectiveArea"];
      const YAML::Node &size = p["Size"];
      size[0] >> sizeX;
      size[1] >> sizeY;

      fObjects->Add(MakeRectangle(centerX,centerY,sizeX,sizeY,0,
				  TAttLine(0,0,1),
				  TAttFill(kOrange,1001)));
   }

   { /* make table */
      float sizeX, sizeY;
      const YAML::Node &p = node["Table"];
      const YAML::Node &size = p["Size"];
      size[0] >> sizeX;
      size[1] >> sizeY;

      fObjects->Add(MakeRectangle(centerX,centerY,
				  sizeX,sizeY,0,
				  TAttLine(kRed,1,1),
				  TAttFill(0,0)));
   }
}

TDriftChamber::~TDriftChamber(){}
