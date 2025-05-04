
struct vec3FWHM
{
	double FWHM_X;
	double FWHM_Y;
	double FWHM_Z;
};

int GetFWHMs( const string& fname, vec3FWHM& out_FWHMs );

int Main()
{
	string fname;
	cout << "give filename listing all RESULTS.txt files" << endl;
	cin >> fname;
	ifstream listfile( fname.c_str() );
	if ( !listfile.is_open() )
	{
		cout << "file does not exist: " << fname << endl;
		return 1;
	}

	const int MAX_POS(7);
	double radialpos[MAX_POS] = { 5, 10, 15, 25, 50, 75, 100 };
	double data_R[MAX_POS] = {0};
	double data_T[MAX_POS] = {0};
	double data_A[MAX_POS] = {0};
	
	int listindex(0);
	vec3FWHM FWHMs;
	double maxVal(0.0);
	while ( !listfile.eof() )
	{
		listfile >> fname;
		if ( !listfile.eof() )
		{
			// cout << "Opening file: " << fname << endl;
			int error = GetFWHMs( fname, FWHMs );
			cout << radialpos[listindex] << ": "
					<< FWHMs.FWHM_X << " " 
			        << FWHMs.FWHM_Y << " " 
			        << FWHMs.FWHM_Z << endl;

			data_R[listindex] = FWHMs.FWHM_X;
			data_T[listindex] = FWHMs.FWHM_Y;
			data_A[listindex] = FWHMs.FWHM_Z;

			if (FWHMs.FWHM_Z > maxVal)
				maxVal = FWHMs.FWHM_Z;

			listindex++;
			// int dummy; cin >> dummy;
		}
	}

	TGraph* graph_R = new TGraph(MAX_POS, radialpos, data_R );
	TGraph* graph_T = new TGraph(MAX_POS, radialpos, data_T );
	TGraph* graph_A = new TGraph(MAX_POS, radialpos, data_A );

	graph_R->SetMarkerColor(kBlue); 
	graph_R->SetLineColor(kBlue); 
	graph_R->SetLineWidth(2);
	graph_R->SetMarkerStyle(21); 
	graph_R->SetMarkerSize(1.5); 

	graph_R->GetXaxis()->SetTitle("Radial offset [mm]");
	graph_R->GetYaxis()->SetTitle("FWHM [mm]");
   	graph_R->SetMinimum(0);
   	graph_R->SetMaximum(maxVal*1.1);
	graph_R->Draw("A PL");

	graph_T->SetMarkerColor(kRed); 
	graph_T->SetLineColor(kRed); 
	graph_T->SetLineWidth(2);
	graph_T->SetMarkerStyle(21); 
	graph_T->SetMarkerSize(1.5); 
	graph_T->Draw("S PL");

	graph_A->SetMarkerColor(kBlack); 
	graph_A->SetLineColor(kBlack); 
	graph_A->SetLineWidth(2);
	graph_A->SetMarkerStyle(21); 
	graph_A->SetMarkerSize(1.5); 
	graph_A->Draw("S PL");

	TLegend* legend = new TLegend(0.9,0.15,0.98,0.45);
	legend->SetFillColor(0);
	legend->AddEntry(graph_R, "Radial", "P");
	legend->AddEntry(graph_T, "Tangential", "P");
	legend->AddEntry(graph_A, "Axial", "P");
	legend->Draw();

	cout << "Is this with Z = center (0) or with Z = 1/4 FOV (1)?" << endl;
	int iscenterZ(0); cin >> iscenterZ;
	string whereZ("Center of axial FOV");
	if (iscenterZ != 0)
		whereZ = "At 1/4 of axial FOV";

	TText* text = new TText(50, 1.2, whereZ.c_str());
	text->SetTextFont(40);
	text->SetTextSize(0.045);
	text->Draw("SAME");

	return 0;
}

int GetFWHMs( const string& fname, vec3FWHM& out_FWHMs )
{
	ifstream datafile( fname.c_str() );
	if ( !datafile.is_open() )
	{
		cout << "file does not exist: " << fname << endl;
		return 1;
	}
	string dummy;
	double sigma;
	double FWHM;
	while ( !datafile.eof() )
	{
		datafile >> dummy >> dummy >> sigma >> dummy >> FWHM;
		if ( !datafile.eof() )
		{
			// cout << "FWHM(1): " << FWHM << " ";
			out_FWHMs.FWHM_X = FWHM;
			datafile >> dummy >> dummy >> sigma >> dummy >> FWHM;
			// cout << "FWHM(2): " << FWHM << " ";
		}
		if ( !datafile.eof() )
		{
			out_FWHMs.FWHM_Y = FWHM;
			datafile >> dummy >> dummy >> sigma >> dummy >> FWHM;
			// cout << "FWHM(3): " << FWHM << " ";
		}
		if ( !datafile.eof() )
			out_FWHMs.FWHM_Z = FWHM;

		// cout << endl;
	}
	datafile.close();
	return 0;
}



