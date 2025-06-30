
#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TObjArray.h"

using namespace std;

int main( int argc, char* argv[] )
{
	string fname;
	if (argc > 1)
		fname = argv[1];
	else
	{
		cout << "Give GAMOS root file: " << endl;
		cin >> fname;
	}

	TFile* file0 = TFile::Open( fname.c_str() );
	TTree* GmDataTTree = (TTree*)file0->Get("GmDataTTree");
	if ( !GmDataTTree )
	{
		cout << "ERROR! GmDataTTree not found. Maybe the file is not there: " << fname << endl;
		cout << "Or the format is wrong?" << endl;
		return 1;
	}

	int Event_EventID;
	double Event_InitialPosX;
	double Event_InitialPosY;
	double Event_InitialPosZ;
	
	string Event_InitialLogicalVolume;
	string* Event_InitialLogicalVolume_ptr = &Event_InitialLogicalVolume;

    TObjArray* mylist = GmDataTTree->GetListOfBranches();
    if (mylist->FindObject("Event_InitialLogicalVolume"))
	{
        cout << "Found Event_InitialLogicalVolume" << endl;
		GmDataTTree->SetBranchAddress("Event_EventID", &Event_EventID);
		GmDataTTree->SetBranchAddress("Event_InitialLogicalVolume", &Event_InitialLogicalVolume_ptr);
		GmDataTTree->SetBranchAddress("Event_InitialPosX", &Event_InitialPosX);
		GmDataTTree->SetBranchAddress("Event_InitialPosY", &Event_InitialPosY);
		GmDataTTree->SetBranchAddress("Event_InitialPosZ", &Event_InitialPosZ);
	
		int nentries = GmDataTTree->GetEntries();
		cout << "nentries: " << nentries << endl;
		for (int i = 0; i < nentries && i < 5; i++)
		{
			long long int ientry = GmDataTTree->GetEntry(i);	
			cout << i << " " << ientry << " " << Event_EventID 
                 << " xyz: " << Event_InitialPosX << " " << Event_InitialPosY << " " << Event_InitialPosZ 
				 << " where: " << Event_InitialLogicalVolume
				 << endl;
		}
	}
	else
	{
        cout << "Could Not Find Event_InitialLogicalVolume!" << endl;
	}
  

	return 0;
}
	

