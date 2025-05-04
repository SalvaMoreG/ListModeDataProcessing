
#ifndef _CSA_MYSIMPLEGMSDTTREE_H___
#define _CSA_MYSIMPLEGMSDTTREE_H___

#include "HitClass.hh"

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"

#include <vector>
#include <string>

const int MAX_N_STEPS = 1;    // only fill one step
const int MAX_N_CHARS = 20;
    
class MySimpleGmSDTTree
{
public:
					MySimpleGmSDTTree();
	virtual			~MySimpleGmSDTTree();
			

// METHODS
    void            InitTree( const std::string& in_rootfilename );
        // if the filename is empty, the output ROOT file will be "GmSDTTree_TEST.root"

    void            Fill();
    void            Fill( const HitClass& in_hit, int in_hit_index, int in_totHitsInEvent );

    void            Save();

// DATA ----
    int             m_TotEntriesCurrentTree;

    // HIT DATA
    Int_t           m_N_STEPS;
    Int_t           m_hit_EventID;
    Int_t           m_hit_TotHitsInEvent;
    Int_t           m_hit_Index;
    ULong64_t       m_hit_DetUnitID;
    Double_t        m_hit_Energy;

    Double_t        m_hit_PosX;
    Double_t        m_hit_PosY;
    Double_t        m_hit_PosZ;

    Double_t        m_hit_TimeMin;
    Double_t        m_hit_TimeMax;

    char            m_hit_SDType[100];
    
	Double_t        m_step_PosX[MAX_N_STEPS];   	//[N_STEPS]
	Double_t        m_step_PosY[MAX_N_STEPS];   	//[N_STEPS]
	Double_t        m_step_PosZ[MAX_N_STEPS];   	//[N_STEPS]
	
	Char_t          m_step_Particle[MAX_N_STEPS][MAX_N_CHARS];   //[N_STEPS]
	Char_t          m_step_FinalProcess[MAX_N_STEPS][MAX_N_CHARS];   		//[N_STEPS]

	Int_t           m_step_TrackID[MAX_N_STEPS];   			//[N_STEPS]
	Int_t           m_step_IsOriginalGamma[MAX_N_STEPS];   	//[N_STEPS]		

	// oh well..
	// Double_t        m_step_Energy[MAX_N_STEPS];   	//[N_STEPS]
	Double_t        m_step_EnergyAtCreation[MAX_N_STEPS];   	//[N_STEPS]
	// Char_t          m_step_InitialProcess[MAX_N_STEPS][MAX_N_CHARS];   		//[N_STEPS]
	
private:

	// private methods
	void			Initialize();

	// private data
	TTree* 		    mytree;
    TFile*          myRootFile;

};

#endif 
