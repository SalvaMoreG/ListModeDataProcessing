
int Main()
{
	cout << "give results file name" << endl;
	string fname; cin >> fname;
	ifstream in_file( fname.c_str() );
	if ( !in_file.is_open() )
	{
		cout << "File not found: " << fname << endl;
		return 1;
	}

	cout << "Give #generated: " << endl;
	int ngen; cin >> ngen;

	TH1D* h1 = new TH1D("h1", "ALL",  161, -80.5, 80.5);
	TH1D* h2 = new TH1D("h2", "TRUE", 161, -80.5, 80.5);

	// S_A,i = S_i / 0.9060 × 100 
	double k_branching_ratio = 0.9060;
	
	string dir;
	double position;
	int counts_all, counts_true;
	double s_all_sum(0);
	int nbins(0);
	double s, s_abs;
	while ( !in_file.eof() )
	{
		in_file >> dir >> position >> counts_all >> counts_true;
		if ( !in_file.eof() )
		{
			s = (double) (counts_all) / (double) (ngen);
			s_abs = 100 * s/k_branching_ratio;
			h1->Fill(position, s_abs);

			nbins++;
			s_all_sum += s_abs;
			// cout << nbins << " " << position << " " << s_abs << " " << s_all_sum << endl;

			s = (double) (counts_true) / (double) (ngen);
			s_abs = 100 * s/k_branching_ratio;
			h2->Fill(position, s_abs);
		}
	}

	gStyle->SetOptStat(0);

	s_all_sum = s_all_sum/nbins;
	cout << std::setprecision (4);
	cout << "Total absolute sensitivity: " << s_all_sum << " %" << endl;

	TCanvas* m1 = new TCanvas("m1", " ", 700, 600);
	h1->SetMarkerStyle(20);
	h1->GetXaxis()->SetTitle("Z position");
	h1->GetYaxis()->SetTitle("Absolute sensitivity (%)");
	h1->Draw("P HIST");

	TCanvas* m2 = new TCanvas("m2", " ", 700, 600);
	h2->SetMarkerStyle(20);
	h2->GetXaxis()->SetTitle("Z position");
	h2->GetYaxis()->SetTitle("Absolute sensitivity (%)");
	h2->Draw("P HIST");

	return 0;
}

