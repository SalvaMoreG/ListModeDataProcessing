#ifndef _SCANNER_H__
#define _SCANNER_H__

#include "globalheaders.hh"
#include "DetectorFactory.hh"

enum SCAN_TYPE
{
	SCAN_TYPE_NONE = 0,
	SCAN_TYPE_PET, 		// PET 
	SCAN_TYPE_PEM, 		// PEM
	SCAN_TYPE_CC,		// Compton camera
	SCAN_TYPE_CCPET
};

enum COINCIDENCE_TYPE
{
	COINCIDENCE_TYPE_NONE = 0,
	COINCIDENCE_TYPE_PET, 	// PET or PEM
	COINCIDENCE_TYPE_CC		// Compton camera
};

// ***************************************************************************************************************
// A scanner has 1 or more detectors, depending on the scan-type.
// For SCAN_TYPE_PET and SCAN_TYPE_PEM: there is only 1 detector, with detector-type which MUST be DETECTOR_TYPE_ABSORBER
// For SCAN_TYPE_CC: there are 2 detectors, with detector-types a CHOICE of DETECTOR_TYPE_SCATTERER or DETECTOR_TYPE_ABSORBER
// ***************************************************************************************************************

class Scanner
{
public:
	// ctor and dtor
							Scanner( SCAN_TYPE in_scanType, const std::string& in_filePath
										, const DetectorFactory& in_detectorFactory );
	virtual					~Scanner();

	// access methods
	inline SCAN_TYPE		GetType() const { return m_type; }
	const Detector*			GetDetector( DETECTOR_TYPE in_type ) const 
							{
								return m_detectorFactory->GetDetector( in_type );
							}

	DETECTOR_TYPE			GetDetectorTypeFromVolume( const std::string& logicalVolume ) const;
	const Detector* 		GetDetectorFromVolume( const std::string& logicalVolume ) const;

	inline int 				GetCoincidenceTime() const { return m_coin_T; }
	double 					GetSourceE( COINCIDENCE_TYPE in_type ) const;
	double 					GetRangeE( COINCIDENCE_TYPE in_type ) const;

protected:

private:
	// methods
	int						LoadParameters(const std::string& in_filePath);
	void					PrintParameters() const;

	// data
	const DetectorFactory*	m_detectorFactory;
	SCAN_TYPE				m_type;
	int						m_coin_T;

	class CutPars
	{
		public: 
									CutPars( COINCIDENCE_TYPE in_type );
									~CutPars();

			int						LoadParameters(const std::string& in_filePath);

			double					sour_E;
			double					rang_E;
		private:
			COINCIDENCE_TYPE			m_type;
	};

	// TODO: we can have more than one cuts
	// e.g. CUTS_PET and/or CUTS_CC
	std::map<COINCIDENCE_TYPE, CutPars> 		m_cutsMap;
};

 
#endif

