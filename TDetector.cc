/**
 * @file   TDetector.cc
 * @brief  detector class for trace
 *
 * @date   Created       : 2016-11-29 14:57:06 JST
 *         Last Modified : 2016-11-29 18:29:18 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TDetector.h"
#include "THodoscope.h"
#include "TDriftChamber.h"
#include "traceUtil.h"

#include <TObjArray.h>
#include <TClass.h>
#include <yaml-cpp/yaml.h>

using trace::TDetector;

Float_t TDetector::fOriginX = 0.;
Float_t TDetector::fOriginY = 0.;
Float_t TDetector::fAngle = 0.;

TDetector::TDetector()
{
   fObjects = new TObjArray;
   fObjects->SetOwner(kTRUE);
}

TDetector::~TDetector()
{
   delete fObjects;
}

trace::TDetector* TDetector::Create(const YAML::Node &node)
{
   const std::string &tag = node.Tag();
   TDetector *detector = NULL;
   if (tag == THodoscope::kTag) {
      detector = new THodoscope(node);
   } else if (tag == TDriftChamber::kTag) {
      detector = new TDriftChamber(node);
   } else if (tag == "?"){
      fprintf(stderr,"Error in <TDetector::Create>: Tag is missing.\n");
      return NULL;
   } else {
      fprintf(stderr,"Error in <TDetector::Create>: Unknown tag: \"%s\"\n",tag.c_str());
      return NULL;
   }

   for(Int_t i = 0; i != detector->fObjects->GetEntriesFast(); ++i) {
      Translate(detector->fObjects->At(i),fOriginX,fOriginY);
      Rotate(detector->fObjects->At(i),fAngle,fOriginX,fOriginY);
   }

   return detector;
}

void TDetector::Draw(Option_t *opt)
{
   for(Int_t i = 0; i != fObjects->GetEntriesFast(); ++i) {
      TObject *obj = fObjects->At(i);
      if(obj->InheritsFrom(TAttFill::Class())) {
	 obj->Draw("f");
      }
      obj->Draw(opt);
   }
}
