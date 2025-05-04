#ifndef PET_CoincidenceClassification_H__
#define PET_CoincidenceClassification_H__

#include "globalheaders.hh"
#include "EventClass.hh"
#include "CoincClass.hh"
#include "HistoClass.hh"
#include "Utilities.hh"
#include "CoincidenceClassificationClass.hh"
#include "PET_EvClassification.hh"

class PET_CoincidenceClassification : public CoincidenceClassificationClass
{
public:
			PET_CoincidenceClassification();
			~PET_CoincidenceClassification();

  int 		Classify(const std::map<int,EventClass>& anEventClassificationMap
						, std::map<int,CoincClass>& aCoincClassificationMap
						, HistoClass* in_histograms);

  int 		InitializeCounters();
  int 		Print() const;

private:
  int 		NC_prompt;
  int 		NC_good;
  int 		NC_scat;
  int	 	NC_Rayl;
  int 		NC_rand;
  int 		NC_PEPE;
  int 		NC_PECOPE;
  int 		NC_COPECOPE;
  int 		NC_PEinco;
  int 		NC_incoinco;
  int 		NC_lostE;

  int 		NC_good_LOR_Vtx;
  int 		NC_bad_LOR_Vtx;
  int 		NC_good_LOR_annihilVtx;
  int 		NC_bad_LOR_annihilVtx;
};


#endif // #ifdef PET_CoincidenceClassification_H__
