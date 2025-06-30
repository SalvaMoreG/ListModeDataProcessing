
#include "PET_CoincidenceFinder.hh"

#include "PET_CoincidenceClassification.hh"

#include "InputFlags.hh"

using namespace std;

// ====================================================

PET_CoincidenceFinder::PET_CoincidenceFinder()
	: m_coincidenceClassification(NULL)
	, m_isFirst( true )
{
	m_coincClassificationMap.clear();
	m_coincidenceClassification = new PET_CoincidenceClassification();
}

// ====================================================

PET_CoincidenceFinder::~PET_CoincidenceFinder()
{
	delete m_coincidenceClassification;
}

// ====================================================

int PET_CoincidenceFinder::FindCoincidence(map<unsigned long long int, HitClass>& theLMTimeMap) 
{
	cout << "starting PET_CoincidenceFinder::FindCoincidence " << endl;

	if ( !m_scanner )
		return -1;

	map<unsigned long long int, HitClass> hitMap;
	unsigned long long int base_time = 0;

	int N_coinc = 0;
	int N_clusters = 0;
	int N_pairs = 0;
	
	map<int,int> counters;
	
	cout << "LM map size = " << theLMTimeMap.size() << endl;
	// Loop over LM hits
	for (map<unsigned long long int,HitClass>::iterator it = theLMTimeMap.begin(); it != theLMTimeMap.end(); it++) 
	{ 
		long long int Delta_T = (it->first) - base_time;
        
        // 2023-03-03 Threshold (already done for ROOT input but not for LM input)
        // 
        const Detector* myDetector = m_scanner->GetDetector( it->second.detectorType );
        if ( !myDetector->PassTriggerThreshold(it->second) )
            continue;   // skip this hit

        // Tiiming
		if (Delta_T < 0) Delta_T *= -1;
		if (hitMap.size() == 0)  //found first hit in coincidence map
		{ 
			base_time = it->first;
			hitMap[it->first] = it->second; 
		}
		else if (Delta_T < m_scanner->GetCoincidenceTime()) //found coincident hit, adding to coinc map
		{ 
			bool merged = false;
			///*** loop to check if within merging distance ***///
			for (map<unsigned long long int,HitClass>::iterator coinc_it = hitMap.begin(); 
					coinc_it != hitMap.end(); coinc_it++) 
			{
				double centroid_X = (coinc_it->second.X*coinc_it->second.E + it->second.E*it->second.X)
										/ (coinc_it->second.E + it->second.E);
				double centroid_Y = (coinc_it->second.Y*coinc_it->second.E + it->second.E*it->second.Y)
										/ (coinc_it->second.E + it->second.E);
				double centroid_Z = (coinc_it->second.Z*coinc_it->second.E + it->second.E*it->second.Z)
										/ (coinc_it->second.E + it->second.E);
				double dist_1 = Distance3DVector(centroid_X,centroid_Y,centroid_Z
												, it->second.X, it->second.Y, it->second.Z); 
					//distance centroid - new hit
				double dist_2 = Distance3DVector(centroid_X,centroid_Y,centroid_Z
												, coinc_it->second.X, coinc_it->second.Y, coinc_it->second.Z);
					//distance centroid - old hit

				const Detector* myDetector = m_scanner->GetDetector( it->second.detectorType );
				// cout << "it->second.detectorType: " << it->second.detectorType << endl;
				assert( myDetector );
				if (dist_1 < myDetector->merg_D && dist_2 < myDetector->merg_D) 
						//merge only if both hits are within acceptance radius from the centroid 
				{ 
					coinc_it->second.X = centroid_X;
					coinc_it->second.Y = centroid_Y;
					coinc_it->second.Z = centroid_Z;
					/*
					if (coinc_it->second.E < it->second.E) 
					{ 
						coinc_it->second.X = it->second.X;
						coinc_it->second.Y = it->second.Y;
						coinc_it->second.Z = it->second.Z;
					}
					*/
					coinc_it->second.E += it->second.E; 

					merged = true;
					break;
				}
			}

	 	 	// if (!merged && hitMap.size() < 10) // if hitmap gets too big it's useless anyway and p goes very slow.
                // Doesn't help much in speeding up, and needs to be tested more...
                
            if (!merged) 
            {
                hitMap[it->first] = it->second; //create new hit if not merged
            }

		}
		else // if hitMap.size > 0 and current hit T lies outside of latest time in coincidenceMap
		{
			HandleCompletedCoincidenceMap( N_clusters, counters, hitMap, N_pairs, N_coinc, m_coincClassificationMap );

			hitMap.clear(); //empty current candidates map, ready for next coincidence
			it--; // the current hit is not part of the current coincidence, should be reconsidered for the next step
		}

	}  // for(map<unsigned long long int,HitClass>::iterator it = theLMTimeMap.begin();it != theLMTimeMap.end();it++)

	// See whether there is anything left to do...
	if (hitMap.size() > 0) 
	{
		HandleCompletedCoincidenceMap( N_clusters, counters, hitMap, N_pairs, N_coinc, m_coincClassificationMap );
	}

	cout << "found " << N_coinc << " prompts among " << N_pairs << " pairs from " << N_clusters
		<< " coinc hit clusters and " << theLMTimeMap.size() << " reconstructed hits." << endl;

	theLMTimeMap.clear();

	return 0;
}

// ====================================================

void
PET_CoincidenceFinder::HandleCompletedCoincidenceMap( int& io_N_clusters, map<int,int>& io_counters
		, map<unsigned long long int, HitClass>& io_hitMap
		, int& io_N_pairs, int& io_N_coinc, map<int,CoincClass>& io_aCoincClassificationMap )
{
	assert( m_scanner );
	if ( !m_scanner )
		return;

	io_N_clusters++;
	if (io_counters.find(io_hitMap.size()) == io_counters.end()) 
		io_counters[io_hitMap.size()] = 1;
	else 
		io_counters[io_hitMap.size()]++;

	// Trying compton recovery with 3 hits
	if (io_hitMap.size() == 3 && InputFlags::Instance()->DoComptonRecovery() ) 
	{ 	
		// int oldsize = io_hitMap.size();
		ComptonRecovery_3Hits( io_hitMap, COINCIDENCE_TYPE_PET );
		// cout << "ComptonRec, oldsize: " << oldsize << " newsize: " << io_hitMap.size() << endl;
	}

	// Here we have two hits: potentially a good coincidence
	if ( io_hitMap.size() == 2 ) 
	{ 
		io_N_pairs++;
		map<unsigned long long int,HitClass>::iterator coinc_it = io_hitMap.begin();
		HitClass hit1 = coinc_it->second; //saving first hit
		coinc_it++;
		HitClass hit2 = coinc_it->second; //saving second hit
		if (m_histograms)
		{
			m_histograms->Fill1DHisto("E_pairs", hit1.E);
			m_histograms->Fill1DHisto("E_pairs", hit2.E);
		}

		if (   fabs(hit1.E - m_scanner->GetSourceE(COINCIDENCE_TYPE_PET)) 
                      < m_scanner->GetRangeE(COINCIDENCE_TYPE_PET) 
			&& fabs(hit2.E - m_scanner->GetSourceE(COINCIDENCE_TYPE_PET)) 
                      < m_scanner->GetRangeE(COINCIDENCE_TYPE_PET) ) 
				//apply energy condition
		{ 
			if ( m_scanner->GetType() != SCAN_TYPE_PEM || (hit1.Y * hit2.Y < 0))  
				// PEM accepts only hits firing the two opposite paddles
			{
				CoincClass aCoinc;
				aCoinc.hit1 = hit1;
				aCoinc.hit2 = hit2;
				io_aCoincClassificationMap[io_N_coinc++] = aCoinc;
			}
			
			if (InputFlags::Instance()->CoincidenceVerbosity() > 0)
                if (hit1.ev == hit2.ev)
                    cout << hit1.ev << " COINC_VERDICT:  " << " OK " << endl;
                else
                    cout << hit1.ev << " COINC_VERDICT:  " << " OK RANDOM (hit2.ev: " 
                         << hit2.ev << ")" << endl;
		}
		else if (InputFlags::Instance()->CoincidenceVerbosity() > 0)
        {
            cout << hit1.ev << " COINC_VERDICT:  " << " wrong energies: " 
                 << " E1: " << hit1.E << " E2: " << hit2.E  << endl;
        }
	}
	else if (InputFlags::Instance()->CoincidenceVerbosity() > 0)
    {
		map<unsigned long long int,HitClass>::iterator coinc_it = io_hitMap.begin();
		HitClass hit1 = coinc_it->second;
        cout << hit1.ev << " COINC_VERDICT:  " << " #hits > 2 : " << io_hitMap.size() << endl;
    }
}

// ====================================================

int
PET_CoincidenceFinder::SaveCoincidencesAndClear( const string& in_outdirName )
{
	string filename = in_outdirName + "/PROMPTS.out";
	SaveCoincidences( filename, m_coincClassificationMap, !m_isFirst );

	m_coincClassificationMap.clear();

	m_isFirst = false;

  	return 0;
}

// ====================================================

int
PET_CoincidenceFinder::Classify(const map<int,EventClass>& in_eventClassificationMap)
{
	if (m_coincidenceClassification)
	{
		m_coincidenceClassification->Classify( in_eventClassificationMap, m_coincClassificationMap, m_histograms );
	}
	return 0;
}

// ====================================================

int 
PET_CoincidenceFinder::PrintClassification() const
{
	if (m_coincidenceClassification)
	{
		return m_coincidenceClassification->Print();
	}
  	return 0;
}



