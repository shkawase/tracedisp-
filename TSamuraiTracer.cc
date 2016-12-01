/**
 * @file   TSamuraiTracer.cc
 * @brief  Trajectory tracer in SAMURAI Magnet
 *
 * @date   Created       : 2016-11-22 10:49:14 JST
 *         Last Modified : 2016-11-29 11:27:27 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TSamuraiTracer.h"

#include "TSamuraiMagnetField.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

using art::TSamuraiTracer;

TSamuraiTracer::TSamuraiTracer()
   : fNMaxPoint(0), fStep(0.), fRotationAngle(0.),
     fEndPlaneAngle(0.), fEndPlaneDistance(0.), fField(NULL),
     fFlightLength(0.),
     fPosition(3,0.), fMomentum(3,0.), fB(3,0.),
     fRNew(3,0.), fDX(3,0.), fP1(3,0.), fP2(3,0.), fPNew(3,0.),
     fStatus(-1)
{
}

TSamuraiTracer::~TSamuraiTracer()
{
   delete fField;
}

bool TSamuraiTracer::LoadField(const char* filename, double scale,
			       int nx, int ny, int nz,
			       double dx, double dy, double dz)
{
   fField = new TSamuraiMagnetField(filename,scale,
				    nx,ny,nz,dx,dy,dz);
   if(!fField->IsGood()) {
      printf("TSamuraiTracer::LoadField() : Failed to load magnet field.\n");
      delete fField;
      fField = NULL;
      fStatus = -2;
      return false;
   }

   printf("TSamuraiTracer::LoadField() : filename = %s\n", filename);
   fStatus = 0;
   return true;
}

void TSamuraiTracer::SetMaxPoint(int nMaxPoint)
{
   fNMaxPoint = nMaxPoint;
   fX.resize(fNMaxPoint);
   fX.clear();
   fY.resize(fNMaxPoint);
   fY.clear();
   fZ.resize(fNMaxPoint);
   fZ.clear();
}

namespace {
   inline double xyMag2(const std::vector<double>& vec)
   {
      return vec[0] * vec[0] + vec[1] * vec[1];
   }

   inline double xyMag(const std::vector<double>& vec)
   {
      return sqrt(xyMag2(vec));
   }

   inline double mag2(const std::vector<double>& vec)
   {
      return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
   }

   inline double mag(const std::vector<double>& vec)
   {
      return sqrt(mag2(vec));
   }

   inline double phi(const std::vector<double>& vec)
   {
      return mag2(vec) == 0. ? 0. : atan2(vec[1],vec[0]);
   }

   inline double theta(const std::vector<double>& vec)
   {
      const double xymag = xyMag(vec);
      return xymag == 0. ? 0. : atan2(xymag,vec[2]);
   }


   inline void deflect(const std::vector<double>& pi,
		       const std::vector<double>& pb,
		       const std::vector<double>& b,
		       double pxy, double step, double charge,
		       double *pf)
   {
      const double c = 2.99792458e+8;
      const double pi_mag = mag(pi);
      const double factor = step * c * charge / pi_mag * 1e-9;

      const double dPhi =
	 factor * (pb[2] * (pb[0]*b[0]+pb[1]*b[1]) /pxy/pxy - b[2]);
      const double dTheta =
	 factor * (pb[0]*b[1]-pb[1]*b[0]) / pxy;

      const double pf_theta = theta(pi) + dTheta;
      const double pf_phi   = phi(pi) + dPhi;

      pf[0] = pi_mag * sin(pf_theta) * cos(pf_phi);
      pf[1] = pi_mag * sin(pf_theta) * sin(pf_phi);
      pf[2] = pi_mag * cos(pf_theta);
   }
}

bool TSamuraiTracer::Trace(const double xi[], const double pi[], double charge)
{
   {  /* initialize temporary variables */
      std::copy(xi,xi+3,fPosition.begin());
      std::copy(pi,pi+3,fMomentum.begin());
      fX.clear();
      fY.clear();
      fZ.clear();
      fB.assign(3,0.);
      fRNew.assign(3,0.);
      fDX.assign(3,0.);
      fP1.assign(3,0.);
      fP2.assign(3,0.);
      fPNew.assign(3,0.);
      fCharge = charge;
      fFlightLength = 0./0.;
   }

   for (int i = 0; i != fNMaxPoint; ++i)
   {
      TraceOneStep();
      const double d = DistanceToEndPlane();
      if (d < 0.) {
	 const double pi = 3.14159265359;
	 const double deg2rad = pi / 180.;
	 const double c = cos(fEndPlaneAngle * deg2rad);
	 const double s = sin(fEndPlaneAngle * deg2rad);
	 const double p = mag(fMomentum);
	 const double pdotn = -fMomentum[0]*s + fMomentum[1]*c;
	 fFlightLength = (i+1) * fStep + d*p/pdotn;
	 return true;
      }
   }

   return false;
}


void TSamuraiTracer::TraceOneStep()
{
   const std::vector<double>& p0 = fMomentum;
   const std::vector<double>& r0 = fPosition;
   std::copy(fMomentum.begin(),fMomentum.end(),fPNew.begin());

   ReadMagneticFieldAt(r0,&fB[0]);

   deflect(p0,p0,fB,xyMag(p0),fStep,fCharge,&fP1[0]);

   const int N_ITERATION = 2;
   for (int i = 0; i != N_ITERATION; ++i) {
      if (i) ReadMagneticFieldAt(fRNew,&fB[0]);

      deflect(p0,fP1,fB,xyMag(fPNew),fStep,fCharge,&fP2[0]);

      fPNew[0] = (fP1[0] + fP2[0])/2;
      fPNew[1] = (fP1[1] + fP2[1])/2;
      fPNew[2] = (fP1[2] + fP2[2])/2;

      fDX[0] = p0[0] + fPNew[0];
      fDX[1] = p0[1] + fPNew[1];
      fDX[2] = p0[2] + fPNew[2];
      const double mult = fStep / mag(fDX);
      fRNew[0] = r0[0] + fDX[0] * mult;
      fRNew[1] = r0[1] + fDX[1] * mult;
      fRNew[2] = r0[2] + fDX[2] * mult;
   }

   std::copy(fRNew.begin(),fRNew.end(),fPosition.begin());
   std::copy(fPNew.begin(),fPNew.end(),fMomentum.begin());
   fX.push_back(fRNew[0]);
   fY.push_back(fRNew[1]);
   fZ.push_back(fRNew[2]);
}

namespace {
   void Rotate2D(double x, double y, double angle,
		double *xrot, double *yrot)
   {
      const double s = sin(angle);
      const double c = cos(angle);
      const double xold = x;
      const double yold = y;
      *xrot = c*xold - s*yold;
      *yrot = s*xold + c*yold;
   }
}

void TSamuraiTracer::ReadMagneticFieldAt(const std::vector<double> &x,
					 double *b) const
{
   const double pi = 3.14159265359;
   const double deg2rad = pi / 180.;
   double xrot, yrot;
   Rotate2D(x[0],x[1],-fRotationAngle * deg2rad,&xrot,&yrot);
   fField->Eval(xrot,x[2],yrot,b,b+2,b+1);
   b[0] = -b[0];
   Rotate2D(b[0],b[1], fRotationAngle * deg2rad,&b[0],&b[1]);
}

double TSamuraiTracer::DistanceToEndPlane() const
{
   const double pi = 3.14159265359;
   const double deg2rad = pi / 180.;
   const double c = cos(fEndPlaneAngle * deg2rad);
   const double s = sin(fEndPlaneAngle * deg2rad);
   return fEndPlaneDistance - (-fPosition[0]*s + fPosition[1]*c);
}

double TSamuraiTracer::GetCentralField() const
{
   return fField->GetCentralField();
}

void TSamuraiTracer::ScaleCentralFieldTo(double field)
{
   fField->ResetScale();
   const double newScale = field / fField->GetCentralField();
   fField->SetScale(newScale);
}
