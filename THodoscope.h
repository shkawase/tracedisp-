/**
 * @file   THodoscope.h
 * @brief  hodoscope
 *
 * @date   Created       : 2016-11-29 15:46:38 JST
 *         Last Modified : 2016-11-29 16:16:28 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_B9B181E4_BFD5_48E1_A2E6_7168AF0996E2
#define INCLUDE_GUARD_UUID_B9B181E4_BFD5_48E1_A2E6_7168AF0996E2

#include "TDetector.h"

namespace trace {
   class THodoscope;
}

class trace::THodoscope : public TDetector {
public:
   virtual ~THodoscope();
private:
   THodoscope(const YAML::Node&);
   THodoscope(const THodoscope&);
   THodoscope& operator=(const THodoscope&);

   static const char *const kTag;

   friend class TDetector;
};

#endif // INCLUDE_GUARD_UUID_B9B181E4_BFD5_48E1_A2E6_7168AF0996E2
