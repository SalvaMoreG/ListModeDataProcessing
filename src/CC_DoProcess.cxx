
#include "DoProcess.hh"

using namespace std;

int main(int argc, char *argv[])
{
	CC_DoProcess process;
	int err = process.Execute(argc,argv);

	return err;
}

// ************************************************************


CC_DoProcess::CC_DoProcess()
{
	m_scanType = SCAN_TYPE_CC;
}

CC_DoProcess::~CC_DoProcess()
{
}

void
CC_DoProcess::DoCreateHistoos()
{
	//definition of histograms
	// no histograms needed

	return;
}
