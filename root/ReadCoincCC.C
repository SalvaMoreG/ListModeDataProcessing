

int Main()
{
	TCanvas* m_1 = new TCanvas("m_1", "m_1", 600, 600);
	m_1->Update();
	TCanvas* m_2 = new TCanvas("m_2", "m_2", 600, 600);
	m_2->Update();
	TCanvas* m_4 = new TCanvas("m_4", "m_4", 600, 600);
	m_4->Update();
	TCanvas* m_5 = new TCanvas("m_5", "m_5", 600, 600);
	m_5->Update();

	std::string fname;
	cout << "filename?" << endl;
	cin >> fname;

	std::ifstream ffile( fname.c_str() );

	double xmin = -150.0;
	double xmax =  150.0;
	double ymin = -150.0;
	double ymax =  150.0;
	double zmin =    0.0;
	double zmax =  350.0;
	int nbins = 500;

	TH1D* h1 = new TH1D("h1", "energy pair-sum", 1300, 0.0, 1300.0);

	TH3D* h2 = new TH3D("h2", "3D hits", 350, zmin, zmax, 300, xmin, xmax, 300, ymin, ymax);

	TH2D* h4 = new TH2D("h4", "hitsXY", nbins, xmin, xmax, nbins, ymin, ymax);

	TH1D* h5 = new TH1D("h5", "hit Z", 350, 0.0, 350.0);
	
	double z1, y1, x1, e1, z2, y2, x2, e2;
	int dum;
	int linenr(0);
	while ( !ffile.eof() )
	{
		ffile >> z1 >> y1 >> x1 >> e1 >> z2 >> y2 >> x2 >> e2;
		ffile.ignore(1024, '\n');	// ignore any additional stuff
		if ( !ffile.eof() )
		{
			h1->Fill(e1+e2);

			h2->Fill(z1, x1, y1);
			h2->Fill(z2, x2, y2);

			h4->Fill(x1, y1);
			h4->Fill(x2, y2);

			h5->Fill(z1);
			h5->Fill(z2);

			linenr++;
		}
	}

	m_1->cd();
	h1->Draw();
	gStyle->SetOptFit(1);
	h1->Fit("gaus");

	m_2->cd();
	// h2->SetMarkerStyle(21);
	// h2->Draw("P");
	h2->Draw();

	m_4->cd();
	h4->Draw("COLZ");

	m_5->cd();
	h5->Draw();

	return 0;
}


