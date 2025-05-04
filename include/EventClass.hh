#ifndef EventClass_H__
#define EventClass_H__

#include "globalheaders.hh"

class EventClass
{
public:
  					EventClass();
	virtual 		~EventClass(){};
  
	int 		classification;
	double 		Vx;
	double 		Vy;
	double 		Vz;
	double 		Annihil_Vx;
	double 		Annihil_Vy;
	double 		Annihil_Vz;
	double 		annihil_angle;
};


#endif // #define EventClass_H__
