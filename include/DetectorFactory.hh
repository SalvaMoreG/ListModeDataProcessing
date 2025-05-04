
#ifndef _DETECTORFACTORY_H__
#define _DETECTORFACTORY_H__

#include "globalheaders.hh"
#include "HitClass.hh"

//	#include "LocalPixelPositionClass.hh"		
	// <-- terrible stuff, but for the moment (for old-time's sake) I let it in...

#include <map>
#include <vector>
#include <string>

class Detector;

// ********************************************* DetectorFactory Class ********************************
// DetectoryFactory returns a Detector according to type (SCATTERER or ABSORBER)
// How it does this (creating it or getting it from storage) is hidden.
// ****************************************************************************************************
class DetectorFactory
{
public:
	// ctor and dtor
							DetectorFactory( const std::string& in_filePath );
							~DetectorFactory();

	// Get/Set	
	const Detector* 		CreateDetector( DETECTOR_TYPE in_type );		// creates detector if it does not exist (reading DetParameters file)
	const Detector* 		GetDetector( DETECTOR_TYPE in_type ) const;		// gets detector if it exists, if not, deletes

private:

	// private methods
	void					Finalize();

	// data
	std::string				m_filePath;
	typedef std::map<DETECTOR_TYPE, Detector*>  DetectorMap;
	DetectorMap				m_detectorMap;
};

// *************************************************** Detector Class *********************************
// The Detector class stores typical (electronic and mechanic) parameters of a detector.
// It can be of type DETECTOR_TYPE_ABSORBER or DETECTOR_TYPE_SCATTERER
// It reads the parameters from a file. 
// In the case of DETECTOR_TYPE_ABSORBER, the parameters in the file are: 
// 			- like the variable names in the class, e.g, "meas_T" 
//			- OR like the variable names in the class with postfix "_abs", e.g. "meas_T_abs"
// In the case of DETECTOR_TYPE_SCATTERER, the parameters in the file are: 
//			- like the variable names in the class with postfix "_scat", e.g. "meas_T_scat"
// ****************************************************************************************************

class Detector
{
public:
					Detector(DETECTOR_TYPE in_type);
	virtual			~Detector();

	inline const std::vector<std::string>& 	GetVolumes() const { return m_volumes; }
	inline DETECTOR_TYPE 					GetType() const { return m_type; }

	inline bool 	PassTriggerThreshold(const double& in_E) const 			 
					{ 
						return (in_E > trig_E); 
					} 
	inline bool 	PassTriggerThreshold(const HitClass& in_myHit) const  
					{ 
						return (in_myHit.E > trig_E); 
					}
	bool 			ExceedMaxOverlapTime(const long long unsigned int&, const HitClass& in_hitClass) const; 

	// TODO: Should be private:
	int 			meas_T;
	int 			dead_T;
	
	double 			trig_E;
	double 			fwhm_E;
	
	double 			electronic_sigma_E;
	
	double 			time_resolution;
	
	double 			merg_D;
	
	double 			fano_F;
	double 			ioni_E;
	
//	axis 			Efield_direction;
//	double 			pixel_thick;
	
	std::vector<std::string> 	m_volumes;

	// LocalPixelPositionClass*  thePixPos;	// <-- un-used stuff, but for the moment (for old-time's sake) I let it in...
											// It's related to charge-sharing, but I think we should redesign it. 
											// 	

private:
	friend class 	DetectorFactory;

	// private methods
	int				LoadParameters(const std::string& in_FilePath);

	// private data
	DETECTOR_TYPE	m_type;
};


#endif
