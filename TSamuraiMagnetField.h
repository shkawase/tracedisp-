/**
 * @file   TSamuraiMagnetField.h
 * @brief  Vector Field represented by 3D Matrix (for SAMURAI magnet field)
 *
 * @date   Created       : 2016-11-18 13:37:11 JST
 *         Last Modified : 2016-11-28 15:44:49 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_5DCC58A8_96F9_4CD7_8978_28232C6B0F3B
#define INCLUDE_GUARD_UUID_5DCC58A8_96F9_4CD7_8978_28232C6B0F3B

namespace art {
   class TSamuraiMagnetField;
}

////////////////////////////////////////////////////////////
///
/// coordinate => x: beam left, y: upward, z: downstream
/// (the same definition as H.Sato san's field calculation)
///

class art::TSamuraiMagnetField {
public:
   TSamuraiMagnetField(const char* filename, double scale = 1.,
		       int nx = 301, int ny = 81, int nz = 301,
		       double dx = 10, double dy = 10, double dz = 10);
   virtual ~TSamuraiMagnetField();

   void Eval(double x, double y, double z,
	     double *bx, double *by, double *bz) const;
   double GetScale() const {return fScale;}
   void SetScale(double scale) {fScale = scale;};
   void ResetScale() {fScale = 1.;};
   double GetCentralField();
   bool IsGood() const {return fIsGood;};

private:
   static const int kDimension = 3; // = Bx, By, Bz
   float    ****fField;             // [kDimension][fNx][fNy][fNz]
   const int    fNx;                // number of x grid
   const int    fNy;                // number of y grid
   const int    fNz;                // number of z grid
   const double fDx;                // mesh size x
   const double fDy;                // mesh size y
   const double fDz;                // mesh size z

   double fScale;
   bool fIsGood;

   bool FindCell(double,double,double,
		   int*,int*,int*,double*,double*,double*) const;
   void Interpolate(int,int,int,double,double,double,
		    double*,double*,double*) const;

   TSamuraiMagnetField(const TSamuraiMagnetField&); // undefined
   TSamuraiMagnetField& operator=(const TSamuraiMagnetField&); // undefined
};

#endif // INCLUDE_GUARD_UUID_5DCC58A8_96F9_4CD7_8978_28232C6B0F3B
