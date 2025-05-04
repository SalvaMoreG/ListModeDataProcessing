#ifndef CC_CoincidenceClassification_H__
#define CC_CoincidenceClassification_H__

#include "EventClass.hh"
#include "CoincClass.hh"
#include "CoincidenceClassificationClass.hh"

class CC_CoincidenceClassification : public CoincidenceClassificationClass
{
public:
  			CC_CoincidenceClassification();
  			~CC_CoincidenceClassification();

  int 		Classify(const std::map<int,EventClass>& aEventClassificationMap
			   				, std::map<int,CoincClass>& aCoincClassificationMap
							, HistoClass* io_histograms);

  int 		InitializeCounters();
  int 		Print() const;

private:

  int 		NC_prompt;
  int 		NC_good;
  int 		NC_rand;
  int 		NC_scat;
  int 		NC_BackScat;
  int 		NC_others;

  int 		NC_PE_null;
  int 		NC_CO_PE;
  int 		NC_nCO_PE;
  int 		NC_CO_COPE;
  int 		NC_nCO_COPE;
  int 		NC_CO_nCOPE;
  int 		NC_nCO_nCOPE;
  int 		NC_inco;
  int 		NC_PE_CO;

  int 		NC_scat_PE_null;
  int 		NC_scat_CO_PE;
  int 		NC_scat_nCO_PE;
  int 		NC_scat_CO_COPE;
  int 		NC_scat_nCO_COPE;
  int 		NC_scat_CO_nCOPE;
  int 		NC_scat_nCO_nCOPE;
  int 		NC_scat_inco;
  int 		NC_scat_PE_CO;

  int 		NC_multiphot;
  int 		NC_other;
};


#endif // #ifdef CC_CoincidenceClassification_H__
