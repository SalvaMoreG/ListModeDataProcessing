#ifndef PET_EvClassification_H__
#define PET_EvClassification_H__

#include "globalheaders.hh"
#include "TreeClass.hh"
#include "EventClass.hh"
#include "Utilities.hh"

#include "EvClassificationClass.hh"


class PET_EvClassification : public EvClassificationClass
{
public:
					PET_EvClassification();
					~PET_EvClassification();
	
	int InitializeCounters();
	int InitializeGlobalCounters();
	int UpdateCounters(TreeClass* aTree, int StepIndex);
	int ClassifyEvent(TreeClass *aTree, std::map<int,EventClass> &theEventClassificationMap);
	int PrintClassification() const;

    enum PETEvClassification
    {
        UNDEFINED = 0,
        scat,
        PEPE,
        PECOPE,
        COPECOPE,
        PEinco,
        incoinco,
        Rayl
    };

private:
	friend class PET_CoincidenceClassification;
	
	PETEvClassification 	m_eventClassification; //classification variable
	int 					ID1,ID2;  //track ID number for original gammas
	bool 					scat1,scat2; //boolean to determine a scattered gamma1 or gamma2
	bool 					Rayl1,Rayl2; //boolean to determine a rayleigh gamma1 or gamma2


	int NPE1,NPE2; //counters for PhotoElectric interactions
	int NCO1,NCO2; //counters for Compton interactions
	
	int NE_tot;
	
	int NE_scat;
	int NE_scat_PEPE;
	int NE_scat_PECOPE;
	int NE_scat_COPECOPE;
	int NE_scat_PEinco;
	int NE_scat_incoinco;
	
	int NE_PEPE;
	int NE_PECOPE;
	int NE_COPECOPE;
	int NE_PEinco;
	int NE_incoinco;
	
	//verteces
	double Vx;
	double Vy;
	double Vz;
	double Annihil_Vx;
	double Annihil_Vy;
	double Annihil_Vz;
	
//	double _mindiff;
};



#endif // #ifdef PET_EvClassification_H__
