#ifndef CoincidenceClassificationClass_H__
#define CoincidenceClassificationClass_H__

#include "globalheaders.hh"
#include "EventClass.hh"
#include "CoincClass.hh"
#include "HistoClass.hh"
#include "Scanner.hh"

class CoincidenceClassificationClass
{
public:
						CoincidenceClassificationClass() {}
	virtual				~CoincidenceClassificationClass(){}

	virtual int 		Classify(const std::map<int,EventClass>& theEventClassificationMap
							, std::map<int,CoincClass>& aCoincClassificationMap
							, HistoClass* io_histograms) = 0;

	virtual int 		InitializeCounters() = 0;
	virtual int 		Print() const = 0;

protected:


};

#endif // #ifdef CoincidenceClassificationClass_H__
