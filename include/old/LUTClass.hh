#ifndef LUTClass_H__
#define LUTClass_H__

#include "globalheaders.hh"

class LUTClass
{
private:
  LUTClass(const std::string& LUTFileName);
  static LUTClass* theInstance;

public:
  static LUTClass* GetInstance(const std::string& LUTFileName);
  ~LUTClass(){};

  double GetEfraction(double x0_over_d);
  double GetTrigTime(double x0_over_d, double Etrig_over_Etot);

  int Nsteps_X0; //number of sampled x0/d values where x0 is the impact point along the E-field direction and d is the voxel thickness
  int Nsteps_E; //number of

  //LUT tables depend on 
  //- the absolute thickness of the voxel along the E-field direction,
  //- the mobility of electrons and holes
  //- the lifetime of electrons and holes
  //we save these values here for future checks (mostly for debugging) about the selected LUT
  double thickness; 
  double mu_e;
  double mu_h;
  double tau_e;
  double tau_h;

protected:
  //actual containers of LUT energy fraction and trigger time: pointers for dynamic memory
  double *frac_E;
  double *trig_T;

  int GetIndexForX0(double x0_over_d);
  int GetIndexForE(double Etrig_over_Etot);

};


#endif // #define LUTClass_H__
