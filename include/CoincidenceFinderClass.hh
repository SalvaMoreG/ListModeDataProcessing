#ifndef CoincidenceFinderClass_H__
#define CoincidenceFinderClass_H__

#include "globalheaders.hh"
#include "HitClass.hh"
#include "CoincClass.hh"
#include "EventClass.hh"

#include "HistoClass.hh"
#include "Scanner.hh"

#include <map>
#include <string>

class CoincidenceFinderClass
{
public:
						CoincidenceFinderClass();
	virtual				~CoincidenceFinderClass(); 
	
	virtual int 		FindCoincidence(std::map<unsigned long long int, HitClass> &theLMTimeMap) = 0;
	virtual int 		Classify(const std::map<int,EventClass>& theEventClassificationMap) = 0;
	virtual	int			SaveCoincidencesAndClear( const std::string& in_outdirName ) = 0;
	virtual int 		PrintClassification() const = 0;

	inline void			SetHistograms( HistoClass* in_histograms ) { m_histograms = in_histograms; }
	inline void			SetScanner( const Scanner* in_scanner ) { m_scanner = in_scanner; }

protected:

	// methods
	virtual HitClass 	FindBestComptonAngle(const HitClass& hit1, const HitClass& hit2, const HitClass& hit3, bool verbose=0) const;
	virtual int 		ComptonRecovery_3Hits(std::map<unsigned long long int, HitClass>& aCoincidenceMap, COINCIDENCE_TYPE in_coincidenceType) const;
	virtual	int			SaveCoincidences( const std::string& in_filename, const std::map<int, CoincClass>& in_coincClassificationMap, bool doAppend ) const;

	// data
	HistoClass*			m_histograms;
	const Scanner*		m_scanner;
};


#endif // #ifdef CoincidenceFinderClass_H__
