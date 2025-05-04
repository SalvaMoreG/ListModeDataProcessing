#ifndef CCPET_CoincidenceFinder_H__
#define CCPET_CoincidenceFinder_H__

#include "HitClass.hh"
#include "CoincClass.hh"
#include "CoincidenceFinderClass.hh"
#include "CoincidenceClassificationClass.hh"

#include <map>

class CCPET_CoincidenceFinder : public CoincidenceFinderClass
{
public:
					CCPET_CoincidenceFinder();
	virtual			~CCPET_CoincidenceFinder();
	
	int 			FindCoincidence(std::map<unsigned long long int, HitClass>& theLMTimeMap);
	int 			Classify(const std::map<int,EventClass>& theEventClassificationMap);
	int				SaveCoincidencesAndClear( const std::string& in_outdirName );
	int 			PrintClassification() const;


private: 
	// private methods
	/*
	void			HandleCompletedCoincidenceMap( int& io_N_clusters, map<int,int>& io_counters
						, map<unsigned long long int, HitClass>& io_aCoincidenceMap
						, int& io_N_pairs, int& io_N_coinc, map<int,CoincClass>& io_aCoincClassificationMap );
	*/

	bool				HandleCandidates( std::map<unsigned long long int,HitClass>& io_hitMap_scat
										, std::map<unsigned long long int,HitClass>& io_hitMap_abs );

	bool				HandleCCCandidate( std::map<unsigned long long int,HitClass>& io_hitMap_scat
										 , std::map<unsigned long long int,HitClass>& io_hitMap_abs
										 , CoincClass& io_coincidence ) const;
	bool				HandlePETCandidate(   std::map<unsigned long long int,HitClass>& io_hitMap_abs
											, CoincClass& io_coincidence ) const;

	std::map<int,CoincClass> m_coincCCClassificationMap;
	std::map<int,CoincClass> m_coincPETClassificationMap;

	std::map<int,CoincClass> m_coincOnlyCCClassificationMap;
	std::map<int,CoincClass> m_coincOnlyPETClassificationMap;

	bool				m_isFirst;
};


#endif // #ifdef CCPET_CoincidenceFinder_H__
