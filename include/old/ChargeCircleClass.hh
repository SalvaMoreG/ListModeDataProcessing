#ifndef ChargeCircleClass_H__
#define ChargeCircleClass_H__

#include "globalheaders.hh"
#include "Utilities.hh"

#include <string>

class ChargeCircleClass
{
public:

  ChargeCircleClass(){};
  ~ChargeCircleClass(){};
  int InitCircle(double newR, double x, double y, double z)
  {
    R = newR;
    Ox = x;
    Oy = y;
    Oz = z;
    return 0;
  };
  
  double R;
  double Ox,Oy,Oz;
  double Fo,FN,FNE,FE,FES,FS,FSW,FW,FWN;
  std::string IDo,IDN,IDNE,IDE,IDES,IDS,IDSW,IDW,IDWN;
  
};


#endif // #define ChargeCircleClass_H__
