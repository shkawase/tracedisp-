/**
 * @file   AddObjects.h
 * @brief
 *
 * @date   Created       : 2016-11-24 18:36:40 JST
 *         Last Modified : 2016-11-29 17:25:22 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_793E2F03_85E3_422D_9746_38B3A0991ED6
#define INCLUDE_GUARD_UUID_793E2F03_85E3_422D_9746_38B3A0991ED6

class TObjArray;


#include <Rtypes.h>

namespace trace {
class TGeneralConfig;

void AddAxes(double xmin, double ymin, double xmax, double ymax,
	     TObjArray *drawees);
void AddMagnet(TObjArray *drawees, TObjArray *bounds);
void AddExitObjects(TObjArray *drawees, TObjArray *bounds);
void AddDetectors(TObjArray *drawees, const TGeneralConfig *conf);
void AddLegend(TObjArray *drawees, const TGeneralConfig *conf,
	       const char* latex, Color_t color = 1);
void ForwardLegend();

void SetLegendXOffset(float val);
void SetLegendYOffset(float val);
void SetLegendSpacing(float val);
float GetLegendXOffset();
float GetLegendYOffset();
float GetLegendSpacing();
}

#endif // INCLUDE_GUARD_UUID_793E2F03_85E3_422D_9746_38B3A0991ED6
