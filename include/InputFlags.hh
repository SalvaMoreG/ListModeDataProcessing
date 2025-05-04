#ifndef __InputFlags_H__
#define __InputFlags_H__

#include "globalheaders.hh"

class InputFlags
{
public: 

    enum ENERGY_RESOLUTION_MODE
    {
        ERES_ZERO = 0,  // zero energy resolution
        ERES_GERARD,    // Gerard's number = ~1.7% @ 511 keV
        ERES_USER,      // energy resolution set by user
        ERES_OLD        // old way, by defining fano factor, #ionisation, etc... in DetParameters.txt
    };


	// singleton access
	static InputFlags*		Instance();
	static void 			Destroy();

	ENERGY_RESOLUTION_MODE	GetEresMode() const { return m_EresMode; }
	double                  GetUserEres() const { return m_userEres; }

	bool					DoComptonRecovery() const { return doComptonRecovery; }
		
	INPUT_MODE				GetInputMode() const { return theInputMode; }
	OUTPUT_MODE				GetOutputMode() const { return theOutputMode; }
	
	bool					DoClassification() const { return doClassification; }
	bool					DoHistograms() const { return doHistograms; }

	bool					DoRealHitPosOutput() const { return doRealHitPosOutput;	}

	bool                    SumAllStepsPerEvent() const { return sumAllStepsPerEvent; }

    bool                    DoGmSDTTree() const { return doGmSDTTree; }
    
    int                     CoincidenceVerbosity() const { return coincidenceVerbosity; }

private:

							InputFlags();
							~InputFlags();

	// static instance
	static InputFlags*		sInstance;

	// friend class (to access the data)
	friend class DoProcess;
    friend class ConvertROOTtoLM_DoProcess;
	friend class CoincidenceFinderClass;
	friend class ProcessDataClass;
	friend class TreeClass;
	friend class DetHitReconstruction;

	// private data
    ENERGY_RESOLUTION_MODE  m_EresMode;
    double                  m_userEres;

	bool					doComptonRecovery;
		
	INPUT_MODE				theInputMode;
	OUTPUT_MODE				theOutputMode;
	
	bool					doClassification;
	bool					doHistograms;

	bool					doRealHitPosOutput;
	bool					doWriteAddEventID;
	bool					doWriteAddSrcPosition;
    bool                    doWriteAddClassificationFlag;

    bool                    sumAllStepsPerEvent;

    bool                    doGmSDTTree;
    
    int                     coincidenceVerbosity;
}; 

#endif

