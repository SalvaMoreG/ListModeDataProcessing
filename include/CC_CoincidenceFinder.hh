#ifndef CC_CoincidenceFinder_H__
#define CC_CoincidenceFinder_H__

#include "HitClass.hh"
#include "CoincClass.hh"
#include "CoincidenceFinderClass.hh"
#include "CoincidenceClassificationClass.hh"

#include <map>

class CC_CoincidenceFinder : public CoincidenceFinderClass
{
public:
  					CC_CoincidenceFinder();
  					~CC_CoincidenceFinder();

	int 			FindCoincidence(std::map<unsigned long long int, HitClass> &theLMTimeMap);
	int 			Classify(const std::map<int,EventClass>& theEventClassificationMap);
	int				SaveCoincidencesAndClear( const std::string& in_outdirName );
	int 			PrintClassification() const;

private:
	std::map<int, CoincClass> 			m_coincClassificationMap;
	CoincidenceClassificationClass* 	m_coincidenceClassification;
	bool			m_isFirst;
};


#endif // #ifdef CC_CoincidenceFinder_H__
