/**
 * @file   TSamuraiTracer.h
 * @brief  Trajectory tracer in SAMURAI Magnet
 *
 * @date   Created       : 2016-11-22 10:01:14 JST
 *         Last Modified : 2016-11-29 10:37:37 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_17117C21_DF6C_4A68_8A3F_21591906DCCE
#define INCLUDE_GUARD_UUID_17117C21_DF6C_4A68_8A3F_21591906DCCE

#include <vector>

namespace art {
   class TSamuraiTracer;
   class TSamuraiMagnetField;
}

class art::TSamuraiTracer {
public:
   TSamuraiTracer();
   ~TSamuraiTracer();

   void SetEndPlane(double dist, double angle) {
      SetEndPlaneDistance(dist);
      SetEndPlaneAngle(angle);
   };
   void SetEndPlaneDistance(double dist) { fEndPlaneDistance = dist;}
   void SetEndPlaneAngle(double angle) { fEndPlaneAngle = angle;}
   void SetMaxPoint(int nMaxPoint);
   void SetStepLength(double step) {fStep = step;};
   void SetRotationAngle(double angle) {fRotationAngle = angle;}

   bool LoadField(const char* filename, double scale = 1.,
		  int nx = 301, int ny = 81, int nz = 301,
		  double dx = 10, double dy = 10, double dz = 10);

   // trace trajectory. returns true if the trajectory reached the end plane.
   bool Trace(const double xi[], const double pi[], double charge = 1);
   double GetFlightLength() const {return fFlightLength;};
   const std::vector<double>& GetXArray() const {return fX;};
   const std::vector<double>& GetYArray() const {return fY;};
   const std::vector<double>& GetZArray() const {return fZ;};

   double GetCentralField() const;
   void ScaleCentralFieldTo(double field);

   bool IsGood() const {return !fStatus;}

private:
   int    fNMaxPoint;
   double fStep;             // step length (mm)
   double fRotationAngle;    // rotation angle for SAMURAI Magnet (deg)
   double fEndPlaneAngle;    // angle of end plane (deg)
   double fEndPlaneDistance; // distance of end plane (mm)

   TSamuraiMagnetField *fField;
   std::vector<double> fX; // should be std::array<double> in C++11
   std::vector<double> fY; // should be std::array<double> in C++11
   std::vector<double> fZ; // should be std::array<double> in C++11
   double fFlightLength;

   /* temporary variables */
   std::vector<double> fPosition;
   std::vector<double> fMomentum;
   std::vector<double> fB;
   std::vector<double> fRNew;
   std::vector<double> fDX;
   std::vector<double> fP1;
   std::vector<double> fP2;
   std::vector<double> fPNew;

   double fCharge;
   int fStatus; // TODO: define status code

   void TraceOneStep();
   double DistanceToEndPlane() const;
   void ReadMagneticFieldAt(const std::vector<double> &x,
			    double *b) const;

   TSamuraiTracer(const TSamuraiTracer&);            // undefined
   TSamuraiTracer& operator=(const TSamuraiTracer&); // undefined
};

#endif // INCLUDE_GUARD_UUID_17117C21_DF6C_4A68_8A3F_21591906DCCE
