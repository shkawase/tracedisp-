/**
 * @file   TGeneralConfig.h
 * @brief  general configuration class
 *
 * @date   Created       : 2016-11-25 10:44:12 JST
 *         Last Modified : 2016-11-25 14:48:02 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_FC6BFFF4_338D_4C86_A704_D86DA9AA65C2
#define INCLUDE_GUARD_UUID_FC6BFFF4_338D_4C86_A704_D86DA9AA65C2

#include <TAttLine.h>
#include <TAttText.h>

namespace trace {
   class TGeneralConfig;
}

class trace::TGeneralConfig {
public:
   TGeneralConfig(const char* filename = kDefaultConfigFile);
   ~TGeneralConfig();

   bool IsInsideViewPort(double x, double y) const;

   static const char* const kDefaultConfigFile;
   static const char* const kDefaultOutFile;
   static const char* const kDefaultGeoConfigFile;
   static const char* const kDefaultMagnetConfigFile;

   float GetXmin() const {return fXmin;};
   float GetXmax() const {return fXmax;};
   float GetYmin() const {return fYmin;};
   float GetYmax() const {return fYmax;};
   short GetCanvasW() const {return fCanvasW;}
   short GetCanvasH() const {return fCanvasH;}
   short GetLegendAlign() const {return fLegendAlign;}
   short GetLegendFont() const {return fLegendFont;}
   float GetLegendSize() const {return fLegendSize;}
   float GetLegendXOffset() const {return fLegendXOffset;}
   float GetLegendYOffset() const {return fLegendYOffset;}
   float GetLegendSpacing() const {return fLegendSpacing;}
   short GetTrajectoryStyle() const {return fTrajStyle;}
   short GetTrajectoryWidth() const {return fTrajWidth;}
   short GetTrajectoryMaxPoint() const {return fTrajMaxPoint;}
   float GetTrajectoryStepLength() const {return fTrajStepLength;}


   TAttLine GetTrajAttLine(Color_t color = kBlack) const
   { return TAttLine(color,fTrajStyle,fTrajWidth);}
   TAttText GetLegendAttText(Color_t color = kBlack) const
   { return TAttText(fLegendAlign,0.,color,fLegendFont,fLegendSize);}

   const char* GetOutFile() const {return fOutFile;}
   void SetOutFile(const char* val) {fOutFile = val;}
   const char* GetInputFile() const {return fInputFile;}
   void SetInputFile(const char* val) {fInputFile = val;}
   const char* GetGeometryConfigFile() const {return fGeoConfigFile;}
   void SetGeometryConfigFile(const char* val) {fGeoConfigFile = val;}
   const char* GetMagnetConfigFile() const {return fMagnetConfigFile;}
   void SetMagnetConfigFile(const char* val) {fMagnetConfigFile = val;}

   bool GetOverwrite() const {return fOverwrite;}
   void SetOverwrite(bool val = true) {fOverwrite = val;}

private:
   void LoadConfigFile(const char*);

   float fXmin;
   float fXmax;
   float fYmin;
   float fYmax;
   short fCanvasW;
   short fCanvasH;
   short fLegendAlign;
   short fLegendFont;
   float fLegendSize;
   float fLegendXOffset;
   float fLegendYOffset;
   float fLegendSpacing;
   short fTrajStyle;
   short fTrajWidth;
   short fTrajMaxPoint;
   float fTrajStepLength;

   bool fOverwrite;
   const char* fInputFile;
   const char* fOutFile;
   const char* fGeoConfigFile;
   const char* fMagnetConfigFile;
};

#endif // INCLUDE_GUARD_UUID_FC6BFFF4_338D_4C86_A704_D86DA9AA65C2
