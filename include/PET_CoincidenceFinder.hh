#ifndef PET_CoincidenceFinder_H__
#define PET_CoincidenceFinder_H__

#include "HitClass.hh"
#include "CoincClass.hh"
#include "CoincidenceFinderClass.hh"
#include "CoincidenceClassificationClass.hh"

#include <map>

class PET_CoincidenceFinder : public CoincidenceFinderClass
{
public:
					PET_CoincidenceFinder();
	virtual			~PET_CoincidenceFinder();
	
	int 			FindCoincidence(std::map<unsigned long long int, HitClass>& theLMTimeMap);
	int 			Classify(const std::map<int,EventClass>& theEventClassificationMap);
	int				SaveCoincidencesAndClear( const std::string& in_outdirName );
	int 			PrintClassification() const;

private: 
	// private methods
	void			HandleCompletedCoincidenceMap( int& io_N_clusters, std::map<int,int>& io_counters
						, std::map<unsigned long long int, HitClass>& io_aCoincidenceMap
						, int& io_N_pairs, int& io_N_coinc, std::map<int,CoincClass>& io_aCoincClassificationMap );

	std::map<int ,CoincClass> 				m_coincClassificationMap;
	CoincidenceClassificationClass* 	m_coincidenceClassification;
	bool			m_isFirst;
};


#endif // #ifdef PET_CoincidenceFinder_H__
