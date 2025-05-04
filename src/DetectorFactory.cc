
#include "DetectorFactory.hh"

using namespace std;

// ********************************************* DetectorFactory Class ********************************

DetectorFactory::DetectorFactory( const std::string& in_filePath )
	: m_filePath( in_filePath )
{
}
	
// ==================================
						
DetectorFactory::~DetectorFactory()
{
	Finalize();
}

// ==================================

const Detector*
DetectorFactory::CreateDetector( DETECTOR_TYPE in_type )
{
	const Detector* c_detector = GetDetector( in_type );
	if (c_detector != NULL)
		return c_detector;

	Detector* detector = new Detector( in_type );

	assert( detector );
	m_detectorMap.insert( std::pair<DETECTOR_TYPE, Detector*>(in_type, detector) ); 
	detector->LoadParameters(m_filePath);

	return detector;
}


// ==================================

const Detector*
DetectorFactory::GetDetector( DETECTOR_TYPE in_type ) const
{
	if (in_type == DETECTOR_TYPE_NONE)
	{	
		return NULL;
	}

	DetectorMap::const_iterator iter = m_detectorMap.find(in_type);
	if ( iter != m_detectorMap.end() )
	{
		return iter->second;
	}
	return NULL;
}

// ==================================

void
DetectorFactory::Finalize()
{
	DetectorMap::iterator mapiter;
	for (mapiter = m_detectorMap.begin(); mapiter != m_detectorMap.end(); mapiter++)
	{
		Detector* detector = ((*mapiter).second);
		delete detector;
	}
	m_detectorMap.clear();
}

// ********************************************************** Detector Class *********************************
// ***********************************************************************************************************

Detector::Detector(DETECTOR_TYPE in_type)
    : meas_T(0)
    , dead_T(0)
    , trig_E(0)
    , fwhm_E(0)
    , electronic_sigma_E(0)
    , time_resolution(0)
    , merg_D(0)
    , fano_F(0)
    , ioni_E(0)
	, m_type( in_type )
{
	//	: thePixPos(NULL)		// <--- whatever
}

Detector::~Detector()
{
}

bool 
Detector::ExceedMaxOverlapTime(const unsigned long long int& in_evTime, const HitClass& in_myHit) const
{
	return ((in_evTime - in_myHit.T) > (meas_T+dead_T));
}

int
Detector::LoadParameters(const string& in_filePath)
{
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

	while (inputfile.good())
	{
		NumOfLines++;
		string line;
		getline (inputfile,line);

		if(inputfile.eof()) break;

		stringstream linestream;
		linestream << line;
		string par;

		while(linestream >> par)
		{
			if(par.find("#")!=string::npos || par.find("//")!=string::npos)
			{
//				cout << "ignoring comment" << endl;
				break;
			}

			if (  m_type == DETECTOR_TYPE_ABSORBER  && (par=="meas_T" || par=="meas_T_abs") 
			   || m_type == DETECTOR_TYPE_SCATTERER && (par=="meas_T_scat")  ) 
			{
				if (!(linestream >> meas_T)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
			}
			else if (   m_type == DETECTOR_TYPE_ABSORBER  && (par=="dead_T" || par=="dead_T_abs")
				|| m_type == DETECTOR_TYPE_SCATTERER && (par=="dead_T_scat")  ) 
			{
				if(!(linestream >> dead_T)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
			}
			else if (   m_type == DETECTOR_TYPE_ABSORBER  && (par=="trig_E" || par=="trig_E_abs")
				|| m_type == DETECTOR_TYPE_SCATTERER && (par=="trig_E_scat")  ) 
			{
				if(!(linestream >> trig_E)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
			}
			else if (   m_type == DETECTOR_TYPE_ABSORBER  && (par=="fwhm_E" || par=="fwhm_E_abs")
				|| m_type == DETECTOR_TYPE_SCATTERER && (par=="fwhm_E_scat")  ) 
			{
				if(!(linestream >> fwhm_E)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
			}
			else if (   m_type == DETECTOR_TYPE_ABSORBER  && (par=="electronic_sigma_E" || par=="electronic_sigma_E_abs")
				|| m_type == DETECTOR_TYPE_SCATTERER && (par=="electronic_sigma_E_scat")  ) 
			{
				if(!(linestream >> electronic_sigma_E)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
			}
			else if (   m_type == DETECTOR_TYPE_ABSORBER  && (par=="time_resolution" || par=="time_resolution_abs")
				|| m_type == DETECTOR_TYPE_SCATTERER && (par=="time_resolution_scat")  ) 
			{
				if(!(linestream >> time_resolution)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
			}
			else if (   m_type == DETECTOR_TYPE_ABSORBER  && (par=="merg_D" || par=="merg_D_abs")
				|| m_type == DETECTOR_TYPE_SCATTERER && (par=="merg_D_scat")  )
			{
				if(!(linestream >> merg_D)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
			}
			else if (   m_type == DETECTOR_TYPE_ABSORBER  && (par=="fano_F" || par=="fano_F_abs")
				|| m_type == DETECTOR_TYPE_SCATTERER && (par=="fano_F_scat")  )
			{
				if(!(linestream >> fano_F)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
			}
			else if (   m_type == DETECTOR_TYPE_ABSORBER  && (par=="ioni_E" || par=="ioni_E_abs")
				|| m_type == DETECTOR_TYPE_SCATTERER && (par=="ioni_E_scat")  )
			{
				if(!(linestream >> ioni_E)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
			}
			else if (   m_type == DETECTOR_TYPE_ABSORBER  && (par=="volumes" || par=="volumes_abs")
				|| m_type == DETECTOR_TYPE_SCATTERER && (par=="volumes_scat")  ) 
			{
				string volume;
				if(!(linestream >> volume)) 
				{
					cout<<"ERROR in "<<__func__<<endl;
					cout<<"value for "<<par<<" is wrong or does not exist in line "<<NumOfLines<<" of "<<in_filePath<<endl;
				}
				else 
				{
					m_volumes.clear();
// cout << "adding volume: " << volume << endl;
					m_volumes.push_back(volume);
					while(linestream>>volume)
					{
						m_volumes.push_back(volume);
					}
				}
			}
		}
	}
}





