
#include "Scanner.hh"
#include "DetectorFactory.hh"
#include "Utilities.hh"


using namespace std;

Scanner::Scanner( SCAN_TYPE in_scanType, const std::string& in_filePath, const DetectorFactory& in_detectorFactory )
	: m_detectorFactory( &in_detectorFactory )
	, m_type( in_scanType )
{
	LoadParameters(in_filePath);
	PrintParameters();
}

// ==============================================

Scanner::~Scanner()
{
}

// ==============================================

DETECTOR_TYPE
Scanner::GetDetectorTypeFromVolume( const std::string& logicalVolume ) const
{
	// cout << "Scanner::GetDetectorTypeFromVolume, my scan type: " << m_scanType << endl;
	if (m_type == SCAN_TYPE_CC || m_type == SCAN_TYPE_CCPET)				
	{
		const Detector* detector = m_detectorFactory->GetDetector( DETECTOR_TYPE_SCATTERER );
		if (IsInRightVolume(logicalVolume, detector->GetVolumes()))
			return DETECTOR_TYPE_SCATTERER;

		detector = m_detectorFactory->GetDetector( DETECTOR_TYPE_ABSORBER );
		if (IsInRightVolume(logicalVolume, detector->GetVolumes()))
			return DETECTOR_TYPE_ABSORBER;
	}

	else if (m_type == SCAN_TYPE_PET || m_type == SCAN_TYPE_PEM)
	{
		const Detector* detector = m_detectorFactory->GetDetector( DETECTOR_TYPE_ABSORBER );
		if (IsInRightVolume(logicalVolume, detector->GetVolumes()))
			return DETECTOR_TYPE_ABSORBER;
	}

	return DETECTOR_TYPE_NONE;
}

// ==============================================

const Detector*
Scanner::GetDetectorFromVolume( const std::string& logicalVolume ) const
{
	DETECTOR_TYPE dettype = GetDetectorTypeFromVolume( logicalVolume );
	const Detector* detector = m_detectorFactory->GetDetector( dettype );
// cout << "Scanner::GetDetectorFromVolume, detector: " << detector << " with type: " << type << endl;
	return detector;

//	return m_detectorFactory->GetDetector( type );
}

// ==============================================

int
Scanner::LoadParameters(const string& in_filePath)
{
	// Read general scanner parameters
	if (in_filePath == "")
	{
		return -2;
	}
	ifstream inputfile(in_filePath.c_str());

	if (!(inputfile.is_open()))
	{
		cout << "ERROR in " << __func__ << endl;
		cout << "file " << in_filePath << " not found or corrupted." << endl;
		exit(2);
	}
	int NumOfLines = 0;
	while(inputfile.good())
	{
		NumOfLines++;
		string line;
		getline (inputfile,line);

		if(inputfile.eof()) break;

		stringstream linestream;
		linestream << line;
		string par;

		while(linestream>>par)
		{
			if(par.find("#")!=string::npos || par.find("//")!=string::npos)
			{
//				cout << "ignoring comment" << endl;
				break;
			}
			if ( par=="coin_T" ) 
			{
				if(!(linestream >> m_coin_T)) 
				{
					cout << "ERROR in " << __func__ << endl;
					cout << "value for " << par << " is wrong or does not exist in line " 
						 << NumOfLines << " of "  <<  in_filePath  << endl;
				}
			}
		}
	}
	inputfile.close();

	// map<COINCIDENCE_TYPE, CutPars> 	m_cutsMap;
	COINCIDENCE_TYPE coincidenceType = COINCIDENCE_TYPE_NONE;
	if (m_type == SCAN_TYPE_PET || m_type == SCAN_TYPE_PEM || m_type == SCAN_TYPE_CCPET)
	{
		coincidenceType = COINCIDENCE_TYPE_PET;
		CutPars cutPars( coincidenceType );
		cutPars.LoadParameters(in_filePath);
		m_cutsMap.insert( std::pair<COINCIDENCE_TYPE, CutPars>(coincidenceType, cutPars) );
	}
	
	if (m_type == SCAN_TYPE_CC || m_type == SCAN_TYPE_CCPET)
	{
		coincidenceType = COINCIDENCE_TYPE_CC;
		CutPars cutPars( coincidenceType );
		cutPars.LoadParameters(in_filePath);
		m_cutsMap.insert( std::pair<COINCIDENCE_TYPE, CutPars>(coincidenceType, cutPars) );
	} 
}

// ==============================================

double
Scanner::GetSourceE( COINCIDENCE_TYPE in_type ) const
{ 
	map<COINCIDENCE_TYPE, CutPars>::const_iterator iter = m_cutsMap.find(in_type);
	if ( iter != m_cutsMap.end() )
	{
		return iter->second.sour_E;
	}
	return 0.0; 
}

// ==============================================

double
Scanner::GetRangeE( COINCIDENCE_TYPE in_type ) const
{ 
	map<COINCIDENCE_TYPE, CutPars>::const_iterator iter = m_cutsMap.find(in_type);
	if ( iter != m_cutsMap.end() )
	{
		return iter->second.rang_E;
	}
	return 0.0; 
}

// ==============================================

Scanner::CutPars::CutPars( COINCIDENCE_TYPE in_type )
	: m_type( in_type )
{
}

Scanner::CutPars::~CutPars()
{
}

int
Scanner::CutPars::LoadParameters(const string& in_filePath)
{
	// Read general scanner parameters
	if (in_filePath == "")
	{
		return -2;
	}
	ifstream inputfile(in_filePath.c_str());

	if (!(inputfile.is_open()))
	{
		cout << "ERROR in " << __func__ << endl;
		cout << "file " << in_filePath << " not found or corrupted." << endl;
		exit(2);
	}

	int NumOfLines = 0;
	while(inputfile.good())
	{
		NumOfLines++;
		string line;
		getline (inputfile,line);

		if(inputfile.eof()) break;

		stringstream linestream;
		linestream << line;
		string par;

		while(linestream>>par)
		{
			if(par.find("#")!=string::npos || par.find("//")!=string::npos)
			{
//				cout << "ignoring comment" << endl;
				break;
			}
			else if (    ( m_type == COINCIDENCE_TYPE_PET && par=="sour_E" )
				      || ( m_type == COINCIDENCE_TYPE_CC && par=="sour_E_CC" ) ) 
			{
				if(!(linestream >> sour_E)) 
				{
					cout << "ERROR in " << __func__ << endl;
					cout << "value for " << par << " is wrong or does not exist in line " 
						 << NumOfLines << " of "  <<  in_filePath  << endl;
				}
			}
			else if (    ( m_type == COINCIDENCE_TYPE_PET && par=="rang_E" )
				      || ( m_type == COINCIDENCE_TYPE_CC && par=="rang_E_CC" ) ) 
			{
				if(!(linestream >> rang_E)) 
				{
					cout << "ERROR in " << __func__ << endl;
					cout << "value for " << par << " is wrong or does not exist in line " 
						 << NumOfLines << " of " <<  in_filePath  <<  endl;
				}
			}
		}
	}
	inputfile.close();
	return 0;
}

// ==============================================

void
Scanner::PrintParameters() const
{
	/*
	cout << "Global Parameters: " << endl;
	cout << "coincidence time = " << coin_T << " ps" << endl;
	cout << "source energy = " << sour_E << " MeV" << endl;
	cout << "energy range = " << rang_E << " MeV" << endl;
	*/
}

// ==============================================


