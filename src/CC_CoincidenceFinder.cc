#include "CC_CoincidenceFinder.hh"

#include "CC_CoincidenceClassification.hh"

using namespace std;

CC_CoincidenceFinder::CC_CoincidenceFinder()
	: m_coincidenceClassification(NULL)
	, m_isFirst( true )
{
	m_coincClassificationMap.clear();
	m_coincidenceClassification = new CC_CoincidenceClassification();
}

// ====================================================

CC_CoincidenceFinder::~CC_CoincidenceFinder()
{
	delete m_coincidenceClassification;
}

// ====================================================

int 
CC_CoincidenceFinder::FindCoincidence(map<unsigned long long int, HitClass>& theLMTimeMap)
{
	if ( !m_scanner )
		return -1;
	
	map<unsigned long long int, HitClass> hitMap_scat;
	map<unsigned long long int, HitClass> hitMap_abs;
	unsigned long long int base_time = 0;

	int N_coinc = 0;
	int N_pairs = 0;

	// Loop over LM hits
	// cout << "CC_CoincidenceFinder::FindCoincidence, the LM time Map size: " << theLMTimeMap.size() << endl;
	for (map<unsigned long long int,HitClass>::iterator it = theLMTimeMap.begin();it != theLMTimeMap.end();it++) 
	{ 
		long long int Delta_T = it->first - base_time;
		if (Delta_T < 0) Delta_T *= -1;
		if ((hitMap_scat.size() + hitMap_abs.size()) == 0) //found first hit in coincidence map
		{ 
			base_time = it->first;
			if (it->second.detectorType == DETECTOR_TYPE_SCATTERER)
			{
				hitMap_scat[it->first] = it->second; 
			}
			else if (it->second.detectorType == DETECTOR_TYPE_ABSORBER) 
			{
				hitMap_abs[it->first] = it->second; 
			}
			else
			{
				cout << "CC_CoincidenceFinder::FindCoincidence, unknown det type: " << it->second.detectorType << endl;
			}
		}
		else if (Delta_T < m_scanner->GetCoincidenceTime()) //found coincident hit, adding to coinc map
		{ 
			// cout << "m_scanner->GetCoincidenceTime(): " << m_scanner->GetCoincidenceTime() << endl;
			bool merged = false;
			map<unsigned long long int, HitClass>& hitMap = it->second.detectorType==DETECTOR_TYPE_SCATTERER 
					? hitMap_scat : hitMap_abs;
			///*** loop to check if within merging distance ***///
			for(map<unsigned long long int,HitClass>::iterator coinc_it = hitMap.begin(); coinc_it != hitMap.end(); coinc_it++) 
			{
				double centroid_X = (coinc_it->second.X*coinc_it->second.E + it->second.E*it->second.X)
										/ (coinc_it->second.E + it->second.E);
				double centroid_Y = (coinc_it->second.Y*coinc_it->second.E + it->second.E*it->second.Y)
										/ (coinc_it->second.E + it->second.E);
				double centroid_Z = (coinc_it->second.Z*coinc_it->second.E + it->second.E*it->second.Z)
										/ (coinc_it->second.E + it->second.E);
				double dist_1 = Distance3DVector(centroid_X,centroid_Y,centroid_Z,it->second.X,it->second.Y,it->second.Z); 
							//distance centroid - new hit
				double dist_2 = Distance3DVector(centroid_X,centroid_Y,centroid_Z
												,coinc_it->second.X,coinc_it->second.Y,coinc_it->second.Z);
							//distance centroid - old hit
				if (coinc_it->second.detectorType == it->second.detectorType) // Only merge hits in same detector of course
				{
					const Detector* myDetector = m_scanner->GetDetector( it->second.detectorType );
					assert( myDetector );
					if (dist_1 < myDetector->merg_D && dist_2 < myDetector->merg_D) 
						//merge only if both hits are within acceptance radius from the centroid 
					{ 
						coinc_it->second.X = centroid_X;
						coinc_it->second.Y = centroid_Y;
						coinc_it->second.Z = centroid_Z;
						coinc_it->second.E += it->second.E; 
						merged = true;
						break;
					}
				}
			}
			if (!merged) 
			{
				if(it->second.detectorType == DETECTOR_TYPE_SCATTERER)
				{
					hitMap_scat[it->first] = it->second; 
				}
				else if(it->second.detectorType == DETECTOR_TYPE_ABSORBER) 
				{
					hitMap_abs[it->first] = it->second; 
				}
			}
		}
		else 
		{
			// if(aCoincidenceMap.size()==3) 
			//   { //trying compton recovery with 3 hits
			//     ComptonRecovery_3Hits(aCoincidenceMap);
			//   }
			if (hitMap_scat.size()==1 && hitMap_abs.size()==1)  //here we have two hits: potentially a good coincidence
			{ 
				N_pairs++;

				map<unsigned long long int,HitClass>::iterator coinc_it = hitMap_scat.begin();
				HitClass hit_scat = coinc_it->second; 	//saving first hit

				coinc_it = hitMap_abs.begin();
				HitClass hit_abs = coinc_it->second; 	//saving second hit

				/*
				cout << "fabs( (hit_scat.E + hit_abs.E) - m_scanner->GetSourceE(COINCIDENCE_TYPE_CC) ): " 
							<<  fabs( (hit_scat.E + hit_abs.E) - m_scanner->GetSourceE(COINCIDENCE_TYPE_CC) )
					<< " for range: "  << m_scanner->GetRangeE(COINCIDENCE_TYPE_CC) << endl;
				*/

				//apply energy condition
				if ( fabs( (hit_scat.E + hit_abs.E) - m_scanner->GetSourceE(COINCIDENCE_TYPE_CC) ) 
									< m_scanner->GetRangeE(COINCIDENCE_TYPE_CC)) 
				{ 
					CoincClass aCoinc;
					aCoinc.hit1 = hit_scat;
					aCoinc.hit2 = hit_abs;
					m_coincClassificationMap[N_coinc++] = aCoinc;
				}
			}
			hitMap_scat.clear(); 	//empty current coincidence map, ready for next coincidence
			hitMap_abs.clear(); 	//empty current coincidence map, ready for next coincidence
			it--; 	//the current hit is not part of the current coincidence, should be reconsidered for the next step
		}
	}  // for(map<unsigned long long int,HitClass>::iterator it = theLMTimeMap.begin();it != theLMTimeMap.end();it++) 

	cout << "found " << N_coinc << " prompts among " 
		 << N_pairs << " pairs from " << theLMTimeMap.size() << " reconstructed hits." << endl;
	theLMTimeMap.clear();

	return 0;
}

// ====================================================

int
CC_CoincidenceFinder::SaveCoincidencesAndClear( const string& in_outdirName  )
{
	string filename = in_outdirName + "/PROMPTS.out";
	SaveCoincidences( filename, m_coincClassificationMap, !m_isFirst );

	m_coincClassificationMap.clear();

	m_isFirst = false;

  	return 0;
}


// ====================================================

int
CC_CoincidenceFinder::Classify(const map<int,EventClass>& in_eventClassificationMap) 
{
	if (m_coincidenceClassification)
	{
		return m_coincidenceClassification->Classify( in_eventClassificationMap, m_coincClassificationMap, m_histograms );
	}
	return 0;
}

// ====================================================

int 
CC_CoincidenceFinder::PrintClassification() const
{
	if (m_coincidenceClassification)
	{
		return m_coincidenceClassification->Print();
	}
  	return 0;
}



