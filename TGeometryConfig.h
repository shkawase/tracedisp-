/**
 * @file   TGeometryConfig.h
 * @brief  geometry configuration class
 *
 * @date   Created       : 2016-11-25 14:33:58 JST
 *         Last Modified : 2016-11-28 12:23:38 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_FA8F778C_7789_407D_B3CD_8B8F7D298314
#define INCLUDE_GUARD_UUID_FA8F778C_7789_407D_B3CD_8B8F7D298314

namespace trace {
   class TGeometryConfig;
}

class trace::TGeometryConfig {
public:
   static TGeometryConfig* GetInstance();
   void LoadFile(const char* filename);

   float GetTargetCenterX() const {return fTargetCenterX;}
   void  SetTargetCenterX(float x) {fTargetCenterX = x;}
   float GetTargetCenterY() const {return fTargetCenterY;}
   void  SetTargetCenterY(float y) {fTargetCenterY = y;}
   float GetMagnetAngle() const {return fMagnetAngle;}
   void  SetMagnetAngle(float angle) {fMagnetAngle = angle;}
   float GetExitAngle() const {return fExitAngle;}
   void  SetExitAngle(float angle) {fExitAngle = angle;}
   float GetExitWindowX() const {return fExitWindowX;}
   void  SetExitWindowX(float x) {fExitWindowX = x;}
   float GetExitWindowY() const {return fExitWindowY;}
   void  SetExitWindowY(float y) {fExitWindowY = y;}
   float GetExitWindowW() const {return fExitWindowW;}
   void  SetExitWindowW(float w) {fExitWindowW = w;}

   short GetScaleN() const {return fNScale;}
   void  SetScaleN(short n) {fNScale = n;}
   float GetScaleSeparation() const {return fScaleSeparation;}
   void  SetScaleSeparation(float sep) {fScaleSeparation = sep;}
   float GetScaleWidth() const {return fScaleWidth;}
   void  SetScaleWidth(float w) {fScaleWidth = w;}
   short GetScaleColor() const {return fScaleColor;}
   void  SetScaleColor(short color) {fScaleColor = color;}

private:
   TGeometryConfig();
   TGeometryConfig(const TGeometryConfig&);            // undefined
   TGeometryConfig& operator=(const TGeometryConfig&); // undefined

   float fTargetCenterX;
   float fTargetCenterY;
   float fMagnetAngle;
   float fExitAngle;
   float fExitWindowX;
   float fExitWindowY;
   float fExitWindowW;
   short fNScale;
   float fScaleSeparation;
   float fScaleWidth;
   short fScaleColor;
};

#endif // INCLUDE_GUARD_UUID_FA8F778C_7789_407D_B3CD_8B8F7D298314
