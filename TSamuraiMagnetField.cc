/**
 * @file   TSamuraiMagnetField.cc
 * @brief  SAMURAI Magnet Field
 *
 * @date   Created       : 2016-11-18 13:40:41 JST
 *         Last Modified : 2016-11-28 16:17:32 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TSamuraiMagnetField.h"

#include <fstream>
#include <cmath>

using art::TSamuraiMagnetField;

TSamuraiMagnetField::TSamuraiMagnetField(const char* filename, double scale,
					 int nx, int ny, int nz,
					 double dx, double dy, double dz)
   : fField(NULL), fNx(nx), fNy(ny), fNz(nz), fDx(dx), fDy(dy), fDz(dz),
     fScale(scale), fIsGood(false)
{
   /* ny should be odd */
   if (fNy % 2 == 0) {
      printf("TSamuraiMagnetField::TSamuraiMagnetField(): ny must be odd but it is even.\n");
      return;
   }

   const size_t expectedFileSize =
      sizeof(float) * fNx * fNy * fNz * kDimension;

   std::ifstream ifs(filename,std::ios::binary);
   if (!ifs) {
      printf("TSamuraiMagnetField::TSamuraiMagnetField(): Cannot open file: %s\n",filename);
      return;
   }

   { /* check whether file size is correct */
      ifs.seekg(0, std::fstream::end);
      const size_t endPos = ifs.tellg();
      ifs.clear();
      ifs.seekg(0, std::fstream::beg);
      const size_t beginPos = ifs.tellg();
      const size_t fileSize = endPos - beginPos;

      if (fileSize != expectedFileSize) {
	 printf("TSamuraiMagnetField::TSamuraiMagnetField(): File size mismatch!\n");
	 printf("File size = %lu while %lu expected\n",fileSize,expectedFileSize);
	 return;
      }
   }

   fField          = new float***[fNx];
   fField[0]       = new float** [fNx*fNy];
   fField[0][0]    = new float*  [fNx*fNy*fNz];
   fField[0][0][0] = new float   [fNx*fNy*fNz*kDimension];
   for (int i = 0; i != fNx; ++i) {
      fField[i] = fField[0] + i * fNy;
      for (int j = 0; j != fNy; ++j) {
	 fField[i][j] = fField[0][0] + i * fNy * fNz + j * fNz;
	 for (int k = 0; k != fNz; ++k) {
	    fField[i][j][k] =
	       fField[0][0][0]
	       + i * fNy * fNz * kDimension
	       + j       * fNz * kDimension
	       + k             * kDimension;
	 }
      }
   }
   ifs.read((char*)(fField[0][0][0]), expectedFileSize);

   fIsGood = true;
}

TSamuraiMagnetField::~TSamuraiMagnetField()
{
   if(fField) {
      delete [] fField[0][0][0];
      delete [] fField[0][0];
      delete [] fField[0];
      delete [] fField;
      fField = NULL;
   }
}

void TSamuraiMagnetField::Eval(double x, double y, double z,
			       double *bx, double *by, double *bz) const
{
   int i,j,k;    // identifiers of the cell
   double p,q,r; // local coordinate in the cell

   if (!FindCell(x,y,z,&i,&j,&k,&p,&q,&r)) {
      // the field should be weak out of the domain
      *bx = 0;
      *by = 0;
      *bz = 0;
      return;
   }

   Interpolate(i,j,k,p,q,r,bx,by,bz);
}

namespace {
   inline void DivRem(double x, double y, int *div, double *rem) {
      *div = x/y;
      *rem = x - (*div) * y;
   }
}

bool TSamuraiMagnetField::FindCell(double x, double y, double z,
				     int *i, int *j, int *k,
				     double *p, double *q, double *r) const
{
   DivRem(fabs(x),fDx,i,p); // x is symmetric
   DivRem(y,fDy,j,q);             // y is not symmetric
   *j += (fNy / 2);                     // shift y index by half of fNy
   DivRem(fabs(z),fDz,k,r); // z is symmetric

   *p /= fDx;
   *q /= fDy;
   *r /= fDz;

//   printf("i,j,k,p,q,r=(%d,%d,%d), (%.2f,%.2f,%.2f)\n",*i,*j,*k,*p,*q,*r);

   /* boundary check */
   return ((0 <= *i) && (*i < fNx - 1)
	   && (0 <= *j) && (*j < fNy - 1)
	   && (0 <= *k) && (*k < fNz - 1));
}

void TSamuraiMagnetField::Interpolate(int i, int j, int k,
				      double p, double q, double r,
				      double *bx, double *by, double *bz) const
{
   double c[3];

   /* trilinear interpolation */
   for (size_t axis = 0; axis != kDimension; ++axis) {
      const double c00 = (1-p)*fField[i][j][k][axis]     + p*fField[i+1][j][k][axis];
      const double c01 = (1-p)*fField[i][j][k+1][axis]   + p*fField[i+1][j][k+1][axis];
      const double c10 = (1-p)*fField[i][j+1][k][axis]   + p*fField[i+1][j+1][k][axis];
      const double c11 = (1-p)*fField[i][j+1][k+1][axis] + p*fField[i+1][j+1][k+1][axis];

      const double c0 = (1-q)*c00 + q*c10;
      const double c1 = (1-q)*c01 + q*c11;

      c[axis] = (1-r)*c0 + r*c1;
   }

   *bx = fScale * c[0];
   *by = fScale * c[1];
   *bz = fScale * c[2];
}

double TSamuraiMagnetField::GetCentralField()
{
   // returns B(upward) at magnet center
   return IsGood() ? fField[0][fNy/2][0][1] * fScale : 0.;
}
