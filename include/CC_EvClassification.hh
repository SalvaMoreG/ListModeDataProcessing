#ifndef CC_EvClassification_H__
#define CC_EvClassification_H__

#include "globalheaders.hh"
#include "TreeClass.hh"
#include "EventClass.hh"
#include "Utilities.hh"
#include "EvClassificationClass.hh"

class CC_EvClassification : public EvClassificationClass
{
public:
						CC_EvClassification();
						~CC_EvClassification();
	
	int 				InitializeCounters();
	int 				InitializeGlobalCounters();
	int 				UpdateCounters(TreeClass* aTree, int StepIndex);
	int 				ClassifyEvent(TreeClass *aTree, std::map<int,EventClass> &theEventClassificationMap);
	int 				PrintClassification() const;

private:
	friend class CC_CoincidenceClassification;
	enum CCEvClassification
	{
		UNDEFINED,
		PE_null,
		CO_PE,
		nCO_PE,
		CO_COPE,
		nCO_COPE,
		CO_nCOPE,
		nCO_nCOPE,
		inco,
		PE_CO,
		scat_PE_null,
		scat_CO_PE,
		scat_nCO_PE,
		scat_CO_COPE,
		scat_nCO_COPE,
		scat_CO_nCOPE,
		scat_nCO_nCOPE,
		scat_inco,
		scat_PE_CO,
		multiphot,
		other
	};

	CCEvClassification 		aClassification; //classification variable
	int 					ID1,ID2;  //track ID number for original gammas
	bool 					scat1,scat2; //boolean to determine a scattered gamma1 or gamma2
	
	int NPE1_scatdet ,NPE2_scatdet; //counters for PhotoElectric interactions
	int NCO1_scatdet ,NCO2_scatdet; //counters for Compton interactions
	int NOTH1_scatdet,NOTH2_scatdet; //counters for Other interactions
	int NPE1_absdet  ,NPE2_absdet; //counters for PhotoElectric interactions
	int NCO1_absdet  ,NCO2_absdet; //counters for Compton interactions
	int NOTH1_absdet ,NOTH2_absdet; //counters for Other interactions
	
	int NE_PE_null;
	int NE_CO_PE;
	int NE_nCO_PE;
	int NE_CO_COPE;
	int NE_nCO_COPE;
	int NE_CO_nCOPE;
	int NE_nCO_nCOPE;
	int NE_inco;
	int NE_PE_CO;
	
	int NE_scat_PE_null;
	int NE_scat_CO_PE;
	int NE_scat_nCO_PE;
	int NE_scat_CO_COPE;
	int NE_scat_nCO_COPE;
	int NE_scat_CO_nCOPE;
	int NE_scat_nCO_nCOPE;
	int NE_scat_inco;
	int NE_scat_PE_CO;
	
	int NE_multiphot;
	int NE_other;
	
	int NE_tot;
	
	//verteces
	double Vx;
	double Vy;
	double Vz;
	double Annihil_Vx;
	double Annihil_Vy;
	double Annihil_Vz;

};




#endif // #ifdef CC_EvClassification_H__
