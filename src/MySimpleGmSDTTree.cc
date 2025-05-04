
#include "MySimpleGmSDTTree.h"
#include <iostream>

using namespace std;

const int MAX_BYTES = 500000000; //gdl

MySimpleGmSDTTree::MySimpleGmSDTTree()
    : m_TotEntriesCurrentTree(0)
	, mytree(0)
    , myRootFile(0)
{
	Initialize();
}

MySimpleGmSDTTree::~MySimpleGmSDTTree()
{
	// delete mytree;
    delete myRootFile;
    myRootFile = 0;
    mytree = 0;
}

void
MySimpleGmSDTTree::Fill()
{
	mytree->Fill();
}

void
MySimpleGmSDTTree::Fill( const HitClass& in_hit, int in_hit_index, int in_totHitsInEvent )
{
    m_N_STEPS = 0;      // for the moment, not stored
    m_hit_EventID = in_hit.ev;
    m_hit_TotHitsInEvent = in_totHitsInEvent;
    m_hit_Index = in_hit_index;
    m_hit_DetUnitID = 0;        // TODO: ???
    m_hit_Energy = in_hit.E;    // MeV

    // cout << "Hit pos: " << in_hit.X << " " << in_hit.Y << " " << in_hit.Z << endl;
    m_hit_PosX = in_hit.X;
    m_hit_PosY = in_hit.Y;
    m_hit_PosZ = in_hit.Z;

    m_hit_TimeMin = in_hit.T;
    m_hit_TimeMax = in_hit.T;

    //  string tmpStr = in_hit.volumeName;
    //  m_hit_SDType = tmpStr.c_str();
    // (See GAMOS-3: GmSDTTreeUA.cc) (Sometimes C and C++ are insane, especially with characters)
    //
    sprintf(m_hit_SDType,"%s",in_hit.volumeName.c_str());

    mytree->Fill();
}

void
MySimpleGmSDTTree::Save()
{
    cout << "myRootFile: during Save: " << myRootFile << endl;
    TFile* theTreeFile = mytree->GetCurrentFile();
    cout << "testing theTreeFile: " << theTreeFile << endl;
    myRootFile->cd();
    mytree->Write();
    myRootFile->Close();

    //  mytree->Write();
}

void
MySimpleGmSDTTree::InitTree( const string& in_rootfilename )
{
    string filename( in_rootfilename );
    if (filename.size() < 2)
        filename = "GmSDTTree_TEST.root";
    myRootFile = new TFile( filename.c_str(), "RECREATE");
    cout << "myRootFile: during constructor: " << myRootFile << endl;

    //  mytree = new TChain("MySimpleGmSDTTree");
    mytree = new TTree("GmSDTTree", "");

    cout << "mytree does exist: " << mytree << endl;

    mytree->Branch("N_STEPS",&m_N_STEPS,"N_STEPS/I");
    mytree->Branch("hit_EventID",  &m_hit_EventID,"hit_EventID/I");
    mytree->Branch("hit_TotHitsInEvent", &m_hit_TotHitsInEvent,"hit_TotHitsInEvent/I");
    mytree->Branch("hit_Index", &m_hit_Index,"hit_Index/I");
    mytree->Branch("hit_DetUnitID", &m_hit_DetUnitID,"hit_DetUnitID/l");
    mytree->Branch("hit_Energy", &m_hit_Energy,"hit_Energy/D");
    mytree->Branch("hit_PosX", &m_hit_PosX,"hit_PosX/D");
    mytree->Branch("hit_PosY", &m_hit_PosY,"hit_PosY/D");
    mytree->Branch("hit_PosZ", &m_hit_PosZ,"hit_PosZ/D");
    mytree->Branch("hit_TimeMin", &m_hit_TimeMin,"hit_TimeMin/D");
    mytree->Branch("hit_TimeMax", &m_hit_TimeMax,"hit_TimeMax/D");

    //  mytree->Branch("hit_SDType", &m_hit_SDType);
    //  theTree->Branch("hit_SDType",&hit_SDType[0],"hit_SDType/C");
    //  mytree->Branch("hit_SDType",&m_hit_SDType[0],"hit_SDType/C");

    mytree->Branch("hit_SDType",&m_hit_SDType,"hit_SDType/C");

	mytree->Branch("step_PosX",&m_step_PosX[0],"step_PosX[N_STEPS]/D");
	mytree->Branch("step_PosY",&m_step_PosY[0],"step_PosY[N_STEPS]/D");
	mytree->Branch("step_PosZ",&m_step_PosZ[0],"step_PosZ[N_STEPS]/D");
    
    mytree->Branch("step_Particle",&m_step_Particle[0][0],"step_Particle[N_STEPS][20]/B");
    mytree->Branch("step_FinalProcess",&m_step_FinalProcess[0][0],"step_FinalProcess[N_STEPS][20]/B");
    
    mytree->Branch("step_TrackID",&m_step_TrackID[0],"step_TrackID[N_STEPS]/I");
	mytree->Branch("step_IsOriginalGamma",&m_step_IsOriginalGamma[0],"step_IsOriginalGamma[N_STEPS]/I"); 
    
    mytree->Branch("step_EnergyAtCreation",&m_step_EnergyAtCreation[0], 
                   "step_EnergyAtCreation[N_STEPS]/D");
}


void
MySimpleGmSDTTree::Initialize()
{
	// Initialize bloody pointers....
	// VERY IMPORTANT!!!!!!!!!!!!!!!!!!!! POINTERS ARE EXPECTED TO BE NULL AT INIT!!!!!!!!!!!!!

}

// ====================================================================================
/*
******************************************************************************
*Tree    :GmSDTTree : GmSDTTree                                              *
******************************************************************************
*Br    0 :N_STEPS   : N_STEPS/I                                              *
*............................................................................*
*Br    1 :N_CHAR    : N_CHAR/I                                               *
*............................................................................*
*Br    2 :hit_EventID : hit_EventID/I                                        *
*............................................................................*
*Br    3 :hit_TotHitsInEvent : hit_TotHitsInEvent/I                          *
*............................................................................*
*Br    4 :hit_Index : hit_Index/I                                            *
*............................................................................*
*Br    5 :hit_DetUnitID : hit_DetUnitID/l                                    *
*............................................................................*
*Br    6 :hit_Energy : hit_Energy/D                                          *
*............................................................................*
*Br    7 :hit_PosX  : hit_PosX/D                                             *
*............................................................................*
*Br    8 :hit_PosY  : hit_PosY/D                                             *
*............................................................................*
*Br    9 :hit_PosZ  : hit_PosZ/D                                             *
*............................................................................*
*Br   10 :hit_TimeMin : hit_TimeMin/D                                        *
*............................................................................*
*Br   11 :hit_TimeMax : hit_TimeMax/D                                        *
*............................................................................*
*Br   12 :hit_SDType : hit_SDType/C                                          *
*............................................................................*
*Br   13 :step_IsOriginalGamma : step_IsOriginalGamma[N_STEPS]/I             *
*............................................................................*
*Br   14 :step_Energy : step_Energy[N_STEPS]/D                               *
*............................................................................*
*Br   15 :step_EnergyAtCreation : step_EnergyAtCreation[N_STEPS]/D           *
*............................................................................*
*Br   16 :step_PosX : step_PosX[N_STEPS]/D                                   *
*............................................................................*
*Br   17 :step_PosY : step_PosY[N_STEPS]/D                                   *
*............................................................................*
*Br   18 :step_PosZ : step_PosZ[N_STEPS]/D                                   *
*............................................................................*
*Br   19 :step_Time : step_Time[N_STEPS]/D                                   *
*............................................................................*
*Br   20 :step_Particle : step_Particle[N_STEPS][20]/B                       *
*............................................................................*
*Br   21 :step_TrackID : step_TrackID[N_STEPS]/I                             *
*............................................................................*
*Br   22 :step_InitialProcess : step_InitialProcess[N_CHAR]/B                *
*............................................................................*
*Br   23 :step_FinalProcess : step_FinalProcess[N_CHAR]/B                    *
*............................................................................*
*Br   24 :step_CreatorProcess : step_CreatorProcess[N_CHAR]/B                *
*............................................................................*

*/

