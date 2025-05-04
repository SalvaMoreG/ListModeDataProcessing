#ifndef HitClass_H__
#define HitClass_H__

#include "globalheaders.hh"
//	#include "LocalPixelPositionClass.hh"
#include "Utilities.hh"

class HitClass
{
public:
					HitClass();
	virtual 		~HitClass(){};

	int 			PrintHit();
	// int 			RotateAndTranslate(HitClass hit0);
	//	int 			GetLocalPosition(LocalPixelPositionClass* PixPos);
	
	// TODO: no copy constructor and assign operator defined (so default ctor and operator are used. 
	//			Only works correctly when there are no pointers)
	
	long long int   ev;
	double 			E;

	double 			X;
	double 			Y;
	double 			Z;

	double 			locX;
	double 			locY;
	double 			locZ;

	double			realX;		// 2015 05 06: not possible to fill correctly...
	double			realY;
	double			realZ;

	double			sourceX;
	double			sourceY;
	double			sourceZ;

//	double 			depth;
//	double 			thickness;
    /*
	double 			Phi;
	double 			Theta;
	double 			Psi;
    */
    
	unsigned long long int T;
	unsigned long long int T_trigger;

	std::string 	UnitID;
	bool 			merged;
	DETECTOR_TYPE 	detectorType;
	std::string		volumeName;
	std::string 	primary_process;
	bool 			bypass_trigger;
	bool 			is_neighbor_hit;
	double 			E_central;
};


#endif // #define HitClass_H__
