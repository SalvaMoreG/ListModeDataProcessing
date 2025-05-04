

int Main()
{
	gStyle->SetOptFit();

	string fname;
	cout << "give filename" << endl;	
	cin >> fname;

	ifstream ffile( fname.c_str() );
	if ( !ffile.is_open() )
	{
		cout << "file does not exist, " << fname << endl;
		exit(1);
	}

	TCanvas* m_1 = new TCanvas("m_1", "m_1", 600, 600);
	TH1D* h1 = new TH1D("h1", "E", 1400, 0.0, 1.400);

	string dummy;
	double time, E, x, y, z;
	while ( !ffile.eof() )
	{
		ffile >> dummy >> dummy >> time >> E >> x >> y >> z;
		if ( !ffile.eof() )
		{
			h1->Fill(E);
		}
	}
	
	m_1->cd();
	h1->Draw();	

	return 0;
}

