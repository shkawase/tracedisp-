/**
 * @file   TDetector.h
 * @brief  detector class for trace
 *
 * @date   Created       : 2016-11-29 14:51:17 JST
 *         Last Modified : 2016-11-29 17:23:39 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_9905DE04_B61D_48A9_8598_36B6326F2B7A
#define INCLUDE_GUARD_UUID_9905DE04_B61D_48A9_8598_36B6326F2B7A

#include "traceUtil.h"
#include <TNamed.h>

namespace trace {
   class TDetector;
}

namespace YAML {
   class Node;
}

class trace::TDetector : public TNamed {
public:
   static TDetector* Create(const YAML::Node &node);
   static void SetOrigin(Float_t x, Float_t y) {fOriginX = x; fOriginY = y;};
   static void SetAngle(Float_t angle) {fAngle = angle;};
   virtual ~TDetector();
   virtual void Draw(Option_t *);
protected:
   TDetector();
   static Float_t fOriginX;
   static Float_t fOriginY;
   static Float_t fAngle;
   TObjArray *fObjects;
};

#endif // INCLUDE_GUARD_UUID_9905DE04_B61D_48A9_8598_36B6326F2B7A
