#ifndef ChargeSharingClass_H__
#define ChargeSharingClass_H__

#include "globalheaders.hh"
#include "HitClass.hh"
#include "ChargeCircleClass.hh"
#include "LocalPixelPositionClass.hh"

#include <map>
#include <string>

/*
struct ChargeSharingLUT
{
  int denominator;
  std::map<int,double> RadiusMap;
};

class ChargeSharingClass 
{
private:
  ChargeSharingClass(std::ifstream &f_in);
  static ChargeSharingClass* theInstance;

public:
  static ChargeSharingClass* GetInstance(std::string filename);
  ~ChargeSharingClass(){delete theLUT; delete theCircle;};

  int CalcChargeFractionIn9NeighborPixels(double pix_pitch, bool debug);
  int CalcUnitIDOf9NeighborPixels(std::string centralID);
  int SplitChargeToNeighborPixels(std::vector<HitClass> &NeighborList, LocalPixelPositionClass* PixPos);
  double GetChargeSharingRadius(double depth, double thick);

  ChargeSharingLUT *theLUT;
  ChargeCircleClass *theCircle;

};
*/

#endif // #define ChargeSharingClass_H__
