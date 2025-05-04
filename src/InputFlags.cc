
#include "InputFlags.hh"

using namespace std;

// A bit ugly way to make sure that InputFlags is only created once....
InputFlags* InputFlags::sInstance = 0;	// initialize pointer


InputFlags*
InputFlags::Instance()
{
	if (sInstance == NULL)
	{
		sInstance = new InputFlags();
	}
	return sInstance;
}

void
InputFlags::Destroy()
{
	delete sInstance;
	sInstance = NULL;
}


InputFlags::InputFlags()
    : m_EresMode(ERES_GERARD)
    , m_userEres(0.0)
	, doComptonRecovery( true )
	, theInputMode( IN_ROOT )
	, theOutputMode( OUT_LMANDPROMPTS )
	, doClassification( false )
	, doHistograms( false )
	, doRealHitPosOutput( false )
	, doWriteAddEventID( false )
	, doWriteAddSrcPosition( false )
    , doWriteAddClassificationFlag( false )
    , sumAllStepsPerEvent( false )
    , doGmSDTTree( false )
    , coincidenceVerbosity(0)
{
	if (sInstance != 0) 
	{
		cout << "ERROR! InputFlags can only be called once" << endl;
	}

	sInstance = this;
}
							
InputFlags::~InputFlags()
{
}


