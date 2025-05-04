

int Main()
{
	gStyle->SetOptFit();

	string fname;
	cout << "give list mode (LM) hits filename" << endl;
	cin >> fname;

	ifstream ffile( fname.c_str() );
	if ( !ffile.is_open() )
	{
		cout << "file does not exist, " << fname << endl;
		exit(1);
	}

	TCanvas* m_1 = new TCanvas("m_1", "m_1", 600, 600);
	TCanvas* m_4 = new TCanvas("m_4", "m_4", 600, 600);
	TCanvas* m_14 = new TCanvas("m_14", "m_14", 600, 600);
	TCanvas* m_z = new TCanvas("m_z", "m_z", 600, 600);

	TH1D* h1 = new TH1D("h1", "E", 1400, 0.0, 0.0);
	h1->SetCanExtend(TH1::kAllAxes);

	int isbig = 1;
	cout << "is PET geometry big (1) or small (0)?" << endl;
	cin >> isbig;

	double xmin = -300.0;
	double xmax =  300.0;
	int nbins = 601;
	if ( !isbig )
	{
		cout << "Give min x and max x (mm)" << endl;
		cin >> xmin;
		cin >> xmax;
		int nbins = 300;
	}

	cout << "give min z and max z (mm): " << endl;
	double zmin = -20;
	double zmax =  20;
	cin >> zmin;
	cin >> zmax;
	int znbins = int(zmax - zmin) + 11;

	TH1D*  hz = new TH1D("hz", "Z", znbins, 1., 0.);
	hz->SetCanExtend(TH1::kAllAxes);
	TH2D*  h4 = new TH2D("h4",  "hitsXY", nbins, 0., 0., nbins, 1., 0.);
	h4->SetCanExtend(TH1::kAllAxes);
	TH1D* h14 = new TH1D("h14", "radius", (int) xmax, 1., 0.);
	h14->SetCanExtend(TH1::kAllAxes);

	string dummy;
	double E, x, y, z, radius;
	unsigned long long int time_ps;
	unsigned int count(0);
	while ( !ffile.eof() ) // && count < 100000 )
	{
		ffile >> dummy >> dummy >> time_ps >> E >> x >> y >> z;
		ffile.ignore(1024, '\n');
		if ( !ffile.eof() )
		{
			if (count == 0) cout << count << ", First time: " << time_ps << endl;
			h1->Fill(E);
			h4->Fill(x, y);
			
			radius = sqrt(x*x + y*y);
			h14->Fill(radius);

			hz->Fill(z);
			count++;
		}
	}
	cout << count << ", Last time: " << time_ps << endl;
	
	m_1->cd();
	h1->GetXaxis()->SetTitle("Energy [MeV]");
	h1->GetXaxis()->SetTitleSize(0.04);
	h1->GetXaxis()->SetLabelSize(0.04);
	h1->GetYaxis()->SetLabelSize(0.04);
	h1->Draw();	

	m_4->cd();
	// h4->Rebin2D(4);
	h4->GetXaxis()->SetTitle("x [mm]");
	h4->GetXaxis()->SetTitleSize(0.04);
	h4->GetYaxis()->SetTitle("y [mm]");
	h4->GetYaxis()->SetTitleSize(0.04);
	h4->GetXaxis()->SetLabelSize(0.04);
	h4->GetYaxis()->SetLabelSize(0.04);
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.20);
	h4->Draw("COLZ");	

	m_14->cd();
	h14->Draw();	

	m_z->cd();
	hz->Draw();	

	return 0;
}

