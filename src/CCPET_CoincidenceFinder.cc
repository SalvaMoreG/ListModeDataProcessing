
#include "CCPET_CoincidenceFinder.hh"
#include "InputFlags.hh"

using namespace std;

// ====================================================

CCPET_CoincidenceFinder::CCPET_CoincidenceFinder()
	: m_isFirst( true )
{
	m_coincPETClassificationMap.clear();
	m_coincCCClassificationMap.clear();
}

// ====================================================

CCPET_CoincidenceFinder::~CCPET_CoincidenceFinder()
{
}

// ====================================================

int CCPET_CoincidenceFinder::FindCoincidence(map<unsigned long long int, HitClass>& theLMTimeMap) 
{
	cout << "starting CCPET_CoincidenceFinder::FindCoincidence " << endl;

	if ( !m_scanner )
		return -1;

	map <unsigned long long int, HitClass> hitMap_scat;
	map <unsigned long long int, HitClass> hitMap_abs;
	unsigned long long int base_time = 0;

	int N_coinc = 0;

	cout << "LM map size = " << theLMTimeMap.size() << endl;

	// Loop over LM hits
	for (map<unsigned long long int,HitClass>::iterator it = theLMTimeMap.begin(); it != theLMTimeMap.end(); it++) 
	{ 
		long long int Delta_T = (it->first) - base_time;

		if (Delta_T < 0) Delta_T *= -1;

		if ((hitMap_scat.size() + hitMap_abs.size()) == 0) 	//found first hit in coincidence map
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
				cout << "CCPET_CoincidenceFinder::FindCoincidence, unknown det type: " 
					 << it->second.detectorType << endl;
			}
		}

		else if (Delta_T < m_scanner->GetCoincidenceTime()) // Found coincident hit, adding to coinc map
		{ 
			bool merged = false;
			map<unsigned long long int, HitClass>& hitMap = it->second.detectorType==DETECTOR_TYPE_SCATTERER 
					? hitMap_scat : hitMap_abs;
			///*** loop to check if within merging distance ***///
			for(map<unsigned long long int,HitClass>::iterator coinc_it = hitMap.begin(); 
					coinc_it != hitMap.end(); coinc_it++) 
			{
				double centroid_X = (coinc_it->second.X*coinc_it->second.E 
					+ it->second.E*it->second.X)/(coinc_it->second.E + it->second.E);
				double centroid_Y = (coinc_it->second.Y*coinc_it->second.E 
					+ it->second.E*it->second.Y)/(coinc_it->second.E + it->second.E);
				double centroid_Z = (coinc_it->second.Z*coinc_it->second.E 
					+ it->second.E*it->second.Z)/(coinc_it->second.E + it->second.E);
				double dist_1 = Distance3DVector(centroid_X,centroid_Y,centroid_Z,it->second.X,it->second.Y,it->second.Z); 
							//distance centroid - new hit
				double dist_2 = Distance3DVector(centroid_X,centroid_Y,centroid_Z
										,coinc_it->second.X,coinc_it->second.Y,coinc_it->second.Z);
							//distance centroid - old hit

				if (coinc_it->second.detectorType == it->second.detectorType) // Only merge hits in same detector of course
				{
					const Detector* myDetector = m_scanner->GetDetector( it->second.detectorType );
					assert( myDetector );
					if(dist_1 < myDetector->merg_D && dist_2 < myDetector->merg_D) 
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
		else // if aCoincidenceMap.size > 0 and current hit T lies outside of latest time in coincidenceMap
		{
			if ( HandleCandidates( hitMap_scat, hitMap_abs ) )
			{
				N_coinc++;
			}

			hitMap_scat.clear(); 	// Empty current coincidence map, ready for next coincidence
			hitMap_abs.clear(); 	// Empty current coincidence map, ready for next coincidence
			it--; 	// The current hit is not part of the current coincidence, should be reconsidered for the next step
		}

	}  // for(map<unsigned long long int,HitClass>::iterator it = theLMTimeMap.begin();it != theLMTimeMap.end();it++)

	// See whether there is anything left to do...
	if (hitMap_abs.size() > 0) 
	{
		if ( HandleCandidates( hitMap_scat, hitMap_abs )  )
		{
			N_coinc++;
		}
	}

	cout << "CCPET_CoincidenceFinder::FindCoincidence Found " << N_coinc 
		 << " prompts from " << theLMTimeMap.size() << " reconstructed hits." << endl;

	theLMTimeMap.clear();

	return 0;
}

// ====================================================

bool
CCPET_CoincidenceFinder::HandleCandidates( map<unsigned long long int,HitClass>& io_hitMap_scat
										 , map<unsigned long long int,HitClass>& io_hitMap_abs )
{
	bool found = false;
	CoincClass coincidence;
	if ( io_hitMap_scat.size() == 1 && io_hitMap_abs.size() == 1)
	{
		// Look for only Compton camera (CC)
		if ( HandleCCCandidate( io_hitMap_scat, io_hitMap_abs, coincidence ) )
		{
			int nc = m_coincOnlyCCClassificationMap.size();
			m_coincOnlyCCClassificationMap[nc] = coincidence;
			found = true;
		}
	}
	else if (      io_hitMap_scat.size() == 0 
				&& (io_hitMap_abs.size() == 2 || io_hitMap_abs.size() == 3) )
	{
		// Look for only PET
		if ( HandlePETCandidate( io_hitMap_abs, coincidence ) )
		{
			int nc = m_coincOnlyPETClassificationMap.size();
			m_coincOnlyPETClassificationMap[nc] = coincidence;
			found = true;
		}
	}
	else if ( io_hitMap_scat.size() == 1 && io_hitMap_abs.size() == 2)
	{
		// Could be only PET OR only CC: Do NOTHING (we could never account for all hits of this event)
		// (Both in CC_CoincidenceFinder and PET_CoincidenceFinder
		// , events with more hits than belonging to a coincidence are ignored.)
	}
	else if (      io_hitMap_scat.size() == 1 
				&& (io_hitMap_abs.size() == 3 || io_hitMap_abs.size() == 4) )
	{
		// Look for PET AND CC
		// If we don't find a CC, what's the use of looking for a PET? 
		//	It's all hopeless, we might as well give up, why were we even born?
		if (    HandleCCCandidate( io_hitMap_scat, io_hitMap_abs, coincidence ) 
				&& (io_hitMap_abs.size() == 2 || io_hitMap_abs.size() == 3) )
		{
			CoincClass coincidence2;
			if ( HandlePETCandidate( io_hitMap_abs, coincidence2 ))
			{
				assert ( m_coincPETClassificationMap.size() == m_coincCCClassificationMap.size() );
				int idx = m_coincPETClassificationMap.size();
				m_coincCCClassificationMap[idx] = coincidence;
				m_coincPETClassificationMap[idx] = coincidence2;
				found = true;
			}
		}
	}
	else
	{
		// NOTHING (well, maybe dark matter but we cannot see that)
	}

	/*
	if (found)
		cout << "CCPET_CoincidenceFinder::HandleCandidates found: " << found << endl; 
	else
		cout << "CCPET_CoincidenceFinder::HandleCandidates NOT found: " << found << endl; 
	*/

	return found;
}

// ====================================================

bool
CCPET_CoincidenceFinder::HandleCCCandidate( map<unsigned long long int,HitClass>& io_hitMap_scat
		, map<unsigned long long int,HitClass>& io_hitMap_abs
		, CoincClass& io_coincidence ) const
{
	map<unsigned long long int,HitClass>::iterator scat_coinc_it = io_hitMap_scat.begin();
	HitClass hit_scat = scat_coinc_it->second; 	//saving first hit

	for (map<unsigned long long int,HitClass>::iterator abs_coinc_it = io_hitMap_abs.begin(); 
					abs_coinc_it != io_hitMap_abs.end(); abs_coinc_it++) 
	{
		HitClass hit_abs = abs_coinc_it->second; 	//saving second hit

		if (   m_scanner 
			&& fabs((hit_scat.E + hit_abs.E) - m_scanner->GetSourceE(COINCIDENCE_TYPE_CC)) 
							< m_scanner->GetRangeE(COINCIDENCE_TYPE_CC)) //apply energy condition
		{ 
			io_coincidence.hit1 = hit_scat;
			io_coincidence.hit2 = hit_abs;
	
			// leaving loop, so we can erase iterator without problems
			io_hitMap_scat.erase( scat_coinc_it );
			io_hitMap_abs.erase( abs_coinc_it );	
			return true;
		}
	}
	return false;
}

bool
CCPET_CoincidenceFinder::HandlePETCandidate( map<unsigned long long int,HitClass>& io_hitMap_abs, CoincClass& io_coincidence ) const
{
	// All CC hits (when found before) are already deleted from the map...
	// if size == 3
		// 3 PET hits: try ComptonRecovery...
	// size == 2
		// 2 PET hits

	// Trying compton recovery with 3 hits
	if ( io_hitMap_abs.size() == 3 && InputFlags::Instance()->DoComptonRecovery() ) 
	{ 	
		ComptonRecovery_3Hits( io_hitMap_abs, COINCIDENCE_TYPE_PET );
	}

	// Here we have two hits: potentially a good coincidence
	if ( io_hitMap_abs.size() == 2 ) 
	{ 
		map<unsigned long long int,HitClass>::iterator coinc_it = io_hitMap_abs.begin();
		HitClass hit1 = coinc_it->second; //saving first hit
		coinc_it++;
		HitClass hit2 = coinc_it->second; //saving second hit
		if (m_histograms)
		{
			m_histograms->Fill1DHisto("E_pairs", hit1.E);
			m_histograms->Fill1DHisto("E_pairs", hit2.E);
		}

		if (   m_scanner
			&& fabs(hit1.E - m_scanner->GetSourceE(COINCIDENCE_TYPE_PET)) < m_scanner->GetRangeE(COINCIDENCE_TYPE_PET) 
			&& fabs(hit2.E - m_scanner->GetSourceE(COINCIDENCE_TYPE_PET)) < m_scanner->GetRangeE(COINCIDENCE_TYPE_PET) ) 
				//apply energy condition
		{ 
			io_coincidence.hit1 = hit1;
			io_coincidence.hit2 = hit2;

			return true;
		}
	}
	return false;
}

// ====================================================

int
CCPET_CoincidenceFinder::SaveCoincidencesAndClear( const string& in_outdirName )
{
	/*
	if (m_isFirst)
		cout << "CCPET_CoincidenceFinder::SaveCoincidencesAndClear IS FIRST: " << m_isFirst << endl;
	else 
		cout << "CCPET_CoincidenceFinder::SaveCoincidencesAndClear NOT FIRST: " << m_isFirst << endl;
	*/

	string filename = in_outdirName + "/PET_PROMPTS.out";
	SaveCoincidences( filename, m_coincOnlyPETClassificationMap, !m_isFirst );

	filename = in_outdirName + "/CC_PROMPTS.out";
	SaveCoincidences( filename, m_coincOnlyCCClassificationMap, !m_isFirst );

	filename = in_outdirName + "/CCPET_PET_PROMPTS.out";
	SaveCoincidences( filename, m_coincPETClassificationMap, !m_isFirst );

	filename = in_outdirName + "/CCPET_CC_PROMPTS.out";
	SaveCoincidences( filename, m_coincCCClassificationMap, !m_isFirst );

	m_coincOnlyPETClassificationMap.clear();
	m_coincOnlyCCClassificationMap.clear();

	m_coincPETClassificationMap.clear();
	m_coincCCClassificationMap.clear();

	m_isFirst = false;

  	return 0;
}

// ====================================================

int
CCPET_CoincidenceFinder::Classify(const map<int,EventClass>& in_eventClassificationMap)
{
	/*
	if (m_coincidenceClassification)
	{
		m_coincidenceClassification->Classify( in_eventClassificationMap, m_coincClassificationMap, m_histograms );
	}
	*/
	return 0;
}

// ====================================================

int 
CCPET_CoincidenceFinder::PrintClassification() const
{
  	return 0;
}








