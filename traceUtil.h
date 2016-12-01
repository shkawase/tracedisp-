/**
 * @file   traceUtil.h
 * @brief
 *
 * @date   Created       : 2016-11-24 18:21:52 JST
 *         Last Modified : 2016-11-29 18:46:58 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_9E79A267_C218_4CF6_928F_157D4E7E36C5
#define INCLUDE_GUARD_UUID_9E79A267_C218_4CF6_928F_157D4E7E36C5

#include <TAttLine.h>
#include <TAttFill.h>
#include <TAttText.h>

class TObjArray;
class TObject;

namespace art {
   class TSamuraiTracer;
}

namespace trace {
   class TGeneralConfig;
   typedef TObject Drawee_t;

   static const TAttLine kDefaultAttLine(kBlack,1,1);
   static const TAttFill kDefaultAttFill(0,0);
   static const TAttLine kDashedLine(kBlack,2,1);
   static const TAttLine kDottedLine(kBlack,3,1);
   static const TAttLine kDashedDottedLine(kBlack,4,1);
   static const TAttText kDefaultAttText;

   struct trace_setting {
      int color;
      int z;
      int a;
      float p; // P/A
      float x;
      float theta;
      std::string comment;
   };
   typedef std::vector<trace_setting> SettingVec_t;

   std::vector<trace_setting> LoadTraceSettings(const char* filename);
   bool TestBounds(TObjArray *bounds, double x, double y);

   void Translate(Drawee_t *drawee, Double_t dx, Double_t dy);
   void Rotate(Drawee_t *drawee, Double_t angle, Double_t xo = 0, Double_t yo = 0);

   Drawee_t* MakeLine(double x1, double y1, double x2, double y2,
		      TAttLine al = kDefaultAttLine);
   Drawee_t* MakePolyLine(int n, double *x, double *y,
			  TAttLine al = kDefaultAttLine,
			  TAttFill af = kDefaultAttFill);
   Drawee_t* MakeCircle(double x, double y, double r,
			TAttLine al = kDefaultAttLine,
			TAttFill af = kDefaultAttFill);
   Drawee_t* MakeRectangle(Double_t xc, Double_t yc, Double_t w, Double_t h,
			   Double_t angle = 0,
			   TAttLine al = kDefaultAttLine,
			   TAttFill af = kDefaultAttFill);
   Drawee_t* MakeLatexNDC(Double_t x, Double_t y, const char *text,
			  TAttText at = kDefaultAttText);

   bool FileExists(const char* path);
   void Draw(Drawee_t *drawee);

   const TGeneralConfig* InitConfig(int argc, char* argv[], int *errno);
   void SetStyles();
   void Usage();

   void AddTrajectory(art::TSamuraiTracer *tracer, const trace_setting &setting,
		      TObjArray *drawees, TObjArray *bounds,
		      const TGeneralConfig *conf, int n);
}

#endif // INCLUDE_GUARD_UUID_9E79A267_C218_4CF6_928F_157D4E7E36C5
