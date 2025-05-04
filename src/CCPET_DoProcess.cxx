
#include "DoProcess.hh"

#include "Utilities.hh"

using namespace std;

int main( int argc, char *argv[] )
{
	CCPET_DoProcess process;
	int err = process.Execute( argc, argv );

	return err;
}

// ************************************************************

CCPET_DoProcess::CCPET_DoProcess()
{
	m_scanType = SCAN_TYPE_CCPET;
}

CCPET_DoProcess::~CCPET_DoProcess()
{
}

void
CCPET_DoProcess::DoCreateHistoos()
{
	cout << "CCPET_DoProcess::DoCreateHistoos()" << endl;

	//definition of histograms
	theGlobalHistograms = new HistoClass();

	theGlobalHistograms->Add1DHisto("E_pairs","Energy for hit pairs in time coincidence",1500, 0, 1.5);
}


