
int Main()
{
	TCanvas* m_1 = new TCanvas("m_1", "m_1", 600, 600);
	m_1->Update();
	TCanvas* m_2 = new TCanvas("m_2", "m_2", 600, 600);
	m_2->Update();
	TCanvas* m_4 = new TCanvas("m_4", "m_4", 600, 600);
	m_4->Update();

	TCanvas* m_6 = new TCanvas("m_6", "m_6", 1200, 600);
	m_6->Update();

	std::string fname;
	cout << "filename?" << endl;
	cin >> fname;

	std::ifstream ffile( fname.c_str() );

	int isbig = 1;
	cout << "is PET geometry big (1) or small (0)?" << endl;
	cin >> isbig;

	double xmin = -250.0;
	double xmax =  250.0;
	int nbins = 500;
	if ( !isbig )
	{
		xmin = -150.0;
		xmax =  150.0;
		int nbins = 300;
	}

	TH1D* h1 = new TH1D("h1", "energy", 500, 500, 525);
	TH3D* h2 = new TH3D("h2", "hits", 200, xmin, xmax, 200, xmin, xmax, 500, -250, 250);

	TH2D* h4 = new TH2D("h4", "hitsXY", nbins, xmin, xmax, nbins, xmin, xmax);
	TH2D* h5 = new TH2D("h5", "hitsXZ", nbins, xmin, xmax, nbins+200, xmin-100, xmax+100);

	TH1D* h6 = new TH1D("h6", "hits_Z", nbins+200, xmin-100, xmax+100);
	TH2D* h7 = new TH2D("h7", "hits_Z vs hits_Z", nbins+200, xmin-100, xmax+100, nbins+200, xmin-100, xmax+100);
	
	double z1, y1, x1, e1, z2, y2, x2, e2;
	while ( !ffile.eof() )
	{
		ffile >> z1 >> y1 >> x1 >> e1 >> z2 >> y2 >> x2 >> e2;
		ffile.ignore(1024, '\n');

		if ( !ffile.eof() )
		{
// cout << "e1: " << e1 << endl;
// cout << "e2: " << e2 << endl;
			h1->Fill(e1);
			h1->Fill(e2);

			h2->Fill(x1, y1, z1);
			h2->Fill(x2, y2, z2);

			h4->Fill(x1, y1);
			h4->Fill(x2, y2);

			h5->Fill(x1, z1);
			h5->Fill(x2, z2);

			h6->Fill( z1 );
			h6->Fill( z2 );

			h7->Fill( z1, z2 );
		}
	}

	m_1->cd();
	h1->Draw();
	gStyle->SetOptFit(1);
	h1->Fit("gaus");

	m_2->cd();
	// h2->Draw();
	h5->Draw("COLZ");

	m_4->cd();
	h4->Draw("COLZ");

	m_6->Divide(2, 1);
	m_6->cd(1);
	h6->Draw();

	m_6->cd(2);
	h7->SetMarkerStyle(20);
	h7->SetMarkerSize(0.5);
	h7->Draw("P");

	return 0;
}

