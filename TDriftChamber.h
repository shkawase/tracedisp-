/**
 * @file   TDriftChamber.h
 * @brief  drift chamber
 *
 * @date   Created       : 2016-11-29 17:53:19 JST
 *         Last Modified : 2016-11-29 17:55:00 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_C72D64AE_39C6_4242_A12E_C86CC742CFB2
#define INCLUDE_GUARD_UUID_C72D64AE_39C6_4242_A12E_C86CC742CFB2

#include "TDetector.h"

namespace trace {
   class TDriftChamber;
}

class trace::TDriftChamber : public TDetector {
public:
   virtual ~TDriftChamber();
private:
   TDriftChamber(const YAML::Node&);
   TDriftChamber(const TDriftChamber&);
   TDriftChamber& operator=(const TDriftChamber&);

   static const char* const kTag;
   friend class TDetector;
};

#endif // INCLUDE_GUARD_UUID_C72D64AE_39C6_4242_A12E_C86CC742CFB2
