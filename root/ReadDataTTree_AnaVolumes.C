TFile* file;
TTree* mytree;
TCanvas* m_c1;

TChain* mychain;

#include <string>

//	#include<vector> <---- does not work very well in ROOT
//	#include<algorithm>

void Main()
{
	mychain = new TChain("GmDataTTree");
	cout << "give name of root file" << endl;
	string fname; cin >> fname;
	mychain->Add( fname.c_str() );

	Int_t nentries = (Int_t) mychain->GetEntries();
	cout << "nentries: " << nentries << endl;

	if (nentries == 0)
	{
		cout << "Error: empty root" << endl;
		exit(1);
	}

	// Load parts of tree in memory
	int			 	event_EventID;
	vector<string>* step_InitialLogicalVolume;
	vector<double>* step_AccumulatedEnergyDeposited;
	vector<double>* step_InitialTouchablePosX;
	vector<double>* step_InitialTouchablePosY;
	vector<double>* step_InitialTouchablePosZ;
	vector<double>* step_FinalPosX;
	vector<double>* step_FinalPosY;
	vector<double>* step_FinalPosZ;

	mychain->SetBranchAddress("Event_EventID",     &event_EventID);

	mychain->SetBranchAddress("Step_InitialLogicalVolume", &step_InitialLogicalVolume);
	mychain->SetBranchAddress("Step_AccumulatedEnergyDeposited", &step_AccumulatedEnergyDeposited);

	mychain->SetBranchAddress("Step_InitialTouchablePosX", &step_InitialTouchablePosX);
	mychain->SetBranchAddress("Step_InitialTouchablePosY", &step_InitialTouchablePosY);
	mychain->SetBranchAddress("Step_InitialTouchablePosZ", &step_InitialTouchablePosZ);

	mychain->SetBranchAddress("Step_FinalPosX", &step_FinalPosX);
	mychain->SetBranchAddress("Step_FinalPosY", &step_FinalPosY);
	mychain->SetBranchAddress("Step_FinalPosZ", &step_FinalPosZ);

	string CdTe_1_str("CdTePixel_1");
	string CdTe_2_str("CdTePixel_2");
	string CdTe_3_str("CdTePixel_3");
	string CdTe_4_str("CdTePixel_4");

	TH2D* hi_radius_1 = new TH2D("hi_radius_1", "InitTouchPos for CdTe_pixel_1", 600, -300, 300, 600, -300, 300);
	TH2D* hi_radius_2 = new TH2D("hi_radius_2", "InitTouchPos for CdTe_pixel_2", 600, -300, 300, 600, -300, 300);
	TH2D* hi_radius_3 = new TH2D("hi_radius_3", "InitTouchPos for CdTe_pixel_3", 600, -300, 300, 600, -300, 300);
	TH2D* hi_radius_4 = new TH2D("hi_radius_4", "InitTouchPos for CdTe_pixel_4", 600, -300, 300, 600, -300, 300);

	TH2D* hf_radius_1 = new TH2D("hf_radius_1", "FinalPos for CdTe_pixel_1", 600, -300, 300, 600, -300, 300);
	TH2D* hf_radius_2 = new TH2D("hf_radius_2", "FinalPos for CdTe_pixel_2", 600, -300, 300, 600, -300, 300);
	TH2D* hf_radius_3 = new TH2D("hf_radius_3", "FinalPos for CdTe_pixel_3", 600, -300, 300, 600, -300, 300);
	TH2D* hf_radius_4 = new TH2D("hf_radius_4", "FinalPos for CdTe_pixel_4", 600, -300, 300, 600, -300, 300);

	// the loop.
	// for (Int_t iloop=0; iloop < 12 /*nentries*/ ; iloop++) 
	for (Int_t iloop=0; iloop < nentries ; iloop++) 
	{
		mychain->GetEntry(iloop);
	
		int nsteps = step_InitialLogicalVolume->size();
		if (iloop%10000 == 0)
			cout << iloop << " nsteps: " << nsteps << endl;

		for (int istep = 0; istep < nsteps; istep++)
		{
			string logicalVol = (*step_InitialLogicalVolume)[istep];
			double Xi = (*step_InitialTouchablePosX)[istep];
			double Yi = (*step_InitialTouchablePosY)[istep];
			double Xf = (*step_FinalPosX)[istep];
			double Yf = (*step_FinalPosY)[istep];
			if (logicalVol == CdTe_1_str)
			{
				hi_radius_1->Fill( Xi, Yi );
				hf_radius_1->Fill( Xf, Yf );
			} 
			else if (logicalVol == CdTe_2_str)
			{
				hi_radius_2->Fill( Xi, Yi );
				hf_radius_2->Fill( Xf, Yf );
			} 
			else if (logicalVol == CdTe_3_str)
			{
				hi_radius_3->Fill( Xi, Yi );
				hf_radius_3->Fill( Xf, Yf );
			} 
			else if (logicalVol == CdTe_4_str)
			{
				hi_radius_4->Fill( Xi, Yi );
				hf_radius_4->Fill( Xf, Yf );
			} 
		}
	}
	
	TCanvas* m_c1 = new TCanvas("m_c1", " ", 1200, 1200);
	m_c1->Divide(2, 2);
	gStyle->SetOptFit(1);

	m_c1->cd(1);
	hf_radius_1->Draw();
	m_c1->cd(2);
	hf_radius_2->Draw();
	m_c1->cd(3);
	hf_radius_3->Draw();
	m_c1->cd(4);
	hf_radius_4->Draw();

	m_c1->Print("plot_CdTePixelFinalPositions.gif");

	TCanvas* m_c2 = new TCanvas("m_c2", " ", 1200, 1200);
	m_c2->Divide(2, 2);

	m_c2->cd(1);
	hi_radius_1->Draw();
	m_c2->cd(2);
	hi_radius_2->Draw();
	m_c2->cd(3);
	hi_radius_3->Draw();
	m_c2->cd(4);
	hi_radius_4->Draw();

	m_c2->Print("plot_CdTePixelInitTouchablePositions.gif");
}


