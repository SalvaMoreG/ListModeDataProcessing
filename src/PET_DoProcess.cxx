
#include "DoProcess.hh"

#include <iostream>
#include <sys/stat.h>

#include "TString.h"

using namespace std;

int main( int argc, char *argv[] )
{
	PET_DoProcess process;
	int err = process.Execute( argc, argv );
	return err;
}

// ************************************************************

PET_DoProcess::PET_DoProcess()
{
	m_scanType = SCAN_TYPE_PET;
}

PET_DoProcess::~PET_DoProcess()
{
}

void
PET_DoProcess::DoCreateHistoos()
{
	cout << "PET_DoProcess::DoCreateHistoos()" << endl;

	//definition of histograms
	theGlobalHistograms = new HistoClass();

	theGlobalHistograms->Add1DHisto("E_pairs","Energy for hit pairs in time coincidence",1300,0,1.3);

	if (InputFlags::Instance()->DoClassification())
	{
		theGlobalHistograms->Add1DHisto("dist_hits_sameCdTe_perEv","distance between same CdTe hits per event",2000,0,200);
		theGlobalHistograms->Add1DHisto("N_hits_sameCdTe_perEv","Number of same CdTe hits per event",10,0,10);
		theGlobalHistograms->Add1DHisto("N_ComptonPerAbsPhoton",
			"Number of Compton Interactions Per Absorbed Primary Photon",20,0,20);
		
		theGlobalHistograms->Add1DHisto("E_scat","Energy for scat coincidence hits",1300,0,1.3);
		theGlobalHistograms->Add1DHisto("E1_scat","Energy for scat coincidence hit 1",1000,0,1);
		theGlobalHistograms->Add1DHisto("E2_scat","Energy for scat coincidence hit 2",1000,0,1);
		theGlobalHistograms->Add2DHisto("E1_vs_E2_scat","Energy for scat coincidence hit 1 and hit 2",1000,0,1,1000,0,1);
		
		theGlobalHistograms->Add1DHisto("Dist_LOR_Vtx","Distance from LOR to primary vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_AnnihilVtx","Distance from LOR to annihilation vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_Vtx_Rayl","Distance from LOR to primary vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_AnnihilVtx_Rayl","Distance from LOR to annihilation vertex",2000,0,200);
		
		theGlobalHistograms->Add1DHisto("Dist_LOR_Vtx_rand","Distance from LOR to primary vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_AnnihilVtx_rand","Distance from LOR to annihilation vertex",2000,0,200);
		
		theGlobalHistograms->Add1DHisto("Dist_LOR_Vtx_scat","Distance from LOR to primary vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_AnnihilVtx_scat","Distance from LOR to annihilation vertex",2000,0,200);
		
		theGlobalHistograms->Add1DHisto("Dist_LOR_Vtx_PEPE","Distance from LOR to primary vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_AnnihilVtx_PEPE","Distance from LOR to annihilation vertex",2000,0,200);
		
		theGlobalHistograms->Add1DHisto("Dist_LOR_Vtx_PECOPE","Distance from LOR to primary vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_AnnihilVtx_PECOPE","Distance from LOR to annihilation vertex",2000,0,200);
		
		theGlobalHistograms->Add1DHisto("Dist_LOR_Vtx_COPECOPE","Distance from LOR to primary vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_AnnihilVtx_COPECOPE","Distance from LOR to annihilation vertex",2000,0,200);
		
		theGlobalHistograms->Add1DHisto("Dist_LOR_Vtx_PEinco","Distance from LOR to primary vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_AnnihilVtx_PEinco","Distance from LOR to annihilation vertex",2000,0,200);
		
		theGlobalHistograms->Add1DHisto("Dist_LOR_Vtx_incoinco","Distance from LOR to primary vertex",2000,0,200);
		theGlobalHistograms->Add1DHisto("Dist_LOR_AnnihilVtx_incoinco","Distance from LOR to annihilation vertex",2000,0,200);
	}
}


