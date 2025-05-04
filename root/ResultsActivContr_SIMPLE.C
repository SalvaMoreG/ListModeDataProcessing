
// echo $activity (" " gen ) " " $numAll " " $totGood " " $numRandom " " $totScat

int Main()
{
	cout << "give filename" << endl;
	string fname;
	cin >> fname;
	ifstream fdata;
	fdata.open( fname.c_str() ,ios::in);
	if (!fdata.is_open()) 
	{
		cout << "ERROR: file not open: " << fname << endl;
		return 1;
	}

	const int MAX_IDX = 50;
	double activity, ntot, ngood, nrandom, nscat;
	double m_activity[MAX_IDX];	
	double m_ntot[MAX_IDX], m_ngood[MAX_IDX], m_nrandom[MAX_IDX], m_nscat[MAX_IDX];	
	double m_dntot[MAX_IDX], m_dngood[MAX_IDX], m_dnrandom[MAX_IDX], m_dnscat[MAX_IDX];	
	double m_delx[MAX_IDX]={0};
	double m_nec[MAX_IDX]={0};
	double m_gen[MAX_IDX];
	double gen, genkBq;
	int count(0);
	double m_cr_ntot[MAX_IDX], m_cr_ngood[MAX_IDX], m_cr_nrandom[MAX_IDX], m_cr_nscat[MAX_IDX];	
	double m_cr_dntot[MAX_IDX], m_cr_dngood[MAX_IDX], m_cr_dnrandom[MAX_IDX], m_cr_dnscat[MAX_IDX];	
	double m_cr_nec[MAX_IDX];
	double m_cr_activity[MAX_IDX];
	double m_null[MAX_IDX] = {0.0};
	int count2(0);
	double fck;
	while ( !fdata.eof() )
	{
		fdata >> activity >> gen >> ntot >> ngood >> nrandom >> nscat;
        /*
        // your file should look like this (for example):
        10E6  100000  100   80   0   20
        50E6  100000  400  300  10  290
        ... etc..
        */
        /*
        // if your file only has 3 numbers (activity, #generated, #total_coinc), 
        //  it should look like this (for example):
        10E6  100000  100  
        50E6  100000  400  
        ... etc..
        */
        
		if ( !fdata.eof() )
		{
			// For absolute sensitivities, we have to take the branching ratio 
            // of Na22 or F18 into account
			// double BR = 0.906; // branching ratio for Na22
			double BR = 0.9686; // branching ratio for F18

			GAMOSgen = gen;
			gen = gen * BR;

			genkBq = gen/1000;		// in kBq

			m_activity[count] = activity;
			m_ntot[count] = ntot/genkBq;
				m_dntot[count] = sqrt(ntot)/genkBq;
                
            // if you have #true, #random, #scattered, then do the following too:
                /*
			m_ngood[count] = ngood/genkBq;
				m_dngood[count] = sqrt(ngood)/genkBq;
			m_nrandom[count] = nrandom/genkBq;
				m_dnrandom[count] = sqrt(nrandom)/genkBq;
			m_nscat[count] = nscat/genkBq;
				m_dnscat[count] = sqrt(nscat)/genkBq;

			double factor = (ngood/(ngood+nscat+nrandom));
			m_nec[count] = factor * m_ngood[count];
                */

			cout << "SUMMARY: " << endl;
			cout << "Activity: " << activity << " becquerel" << endl;
			cout << " total generated: " <<  GAMOSgen << endl;
			cout << " total prompts: " << ntot << endl;
			cout << " total true: " << ngood << endl;
			cout << " total randoms: " << nrandom << endl;
			cout << " total scatttered: " << nscat << endl;

			double time = gen/activity;
			cout << "So, total time needed for this run:  " 
                << GAMOSgen << "/(" << activity << ") = " 
                << time << " seconds" << endl;

			if (activity >= 1.0)
			{
				m_cr_activity[count2] = activity;
				m_cr_ntot[count2] = ntot/(1000.0 * time);	// in kcps
				m_cr_ngood[count2] = ngood/(1000.0 * time);
				m_cr_nrandom[count2] = nrandom/(1000.0 * time);
				m_cr_nscat[count2] = nscat/(1000.0 * time);
			
				m_cr_nec[count2] = factor * m_cr_ngood[count];

				m_cr_dntot[count2] = sqrt(ntot)/(1000.0*time);
				m_cr_dngood[count2] = sqrt(ngood)/(1000.0*time);
				m_cr_dnrandom[count2] = sqrt(nrandom)/(1000.0*time);
				m_cr_dnscat[count2] = sqrt(nscat)/(1000.0*time);

				cout << "So, counting rates (taking also into account F18 branching ratio of " << BR << ")" << endl;
				cout << "Total count rate: (" << ntot << "/(" << time << "*" << BR << ") = " 
					 << m_cr_ntot[count2] << " kcps" << endl;
				cout << "True count rate: (" << ngood << "/(" << time << "*" << BR << ") = " 
					 << m_cr_ngood[count2] << " kcps" << endl;
				cout << "Random count rate: (" << nrandom << "/(" << time << "*" << BR << ") = " 
					 << m_cr_nrandom[count2] << " kcps" << endl;

				// increment
				count2++;
			}

			count++;	
		}
	}

	TCanvas* m_cs = new TCanvas("m_cs", "Absolute sensitivity versus activity", 900, 600);
	m_cs->SetFrameFillColor(0);
	m_cs->SetLogx();

	float markersize = 1.2;
	int colortot = kBlack;
	int colorgood = kBlue+1;
	int colorrandom = kOrange+8;
	int colorscat = kRed+1;
	int colornec = kGreen+3;

	TGraphErrors* graph_tot = new TGraphErrors( count, m_activity, m_ntot, m_delx, m_dntot );
	graph_tot->SetMinimum( 0 );
	graph_tot->SetTitle("Absolute sensitivity vs Activity");
	// graph_tot->GetXaxis()->SetTitle("^{22}Na Activity(Bq)");
	graph_tot->GetXaxis()->SetTitle("Activity(Bq)");
	graph_tot->GetXaxis()->SetLabelSize(0.03);
	graph_tot->GetYaxis()->SetTitle("Absolute sensitivity cps/kBq");

	graph_tot->SetMarkerStyle(21);
	graph_tot->SetMarkerSize(markersize);
	graph_tot->SetMarkerColor(colortot);
	graph_tot->SetLineColor(colortot);
	graph_tot->SetLineWidth(2);
	graph_tot->DrawClone("ACP");
    
    // if you have #true, #random, #scattered, then do the following too:
    /*

	TGraphErrors* graph_good = new TGraphErrors( count, m_activity, m_ngood, m_delx, m_dngood );
	graph_good->SetMarkerStyle(21);
	graph_good->SetMarkerSize(markersize);
	graph_good->SetMarkerColor(colorgood);
	graph_good->SetLineColor(colorgood);
	graph_good->SetLineWidth(2);
	graph_good->DrawClone("CP SAME");

	TGraphErrors* graph_random = new TGraphErrors( count, m_activity, m_nrandom, m_delx, m_dnrandom );
	graph_random->SetMarkerStyle(21);
	graph_random->SetMarkerSize(markersize);
	graph_random->SetMarkerColor(colorrandom);
	graph_random->SetLineColor(colorrandom);
	graph_random->SetLineWidth(2);
	graph_random->DrawClone("LP SAME");

	TGraphErrors* graph_scat = new TGraphErrors( count, m_activity, m_nscat, m_delx, m_dnscat );
	graph_scat->SetMarkerStyle(21);
	graph_scat->SetMarkerSize(markersize);
	graph_scat->SetMarkerColor(colorscat);
	graph_scat->SetLineColor(colorscat);
	graph_scat->SetLineWidth(2);
	graph_scat->DrawClone("CP SAME");

	TGraphErrors* graph_nec = new TGraphErrors( count, m_activity, m_nec, m_delx, m_delx );
	graph_nec->SetMarkerStyle(21);
	graph_nec->SetMarkerSize(markersize);
	graph_nec->SetMarkerColor(colornec);
	graph_nec->SetLineColor(colornec);
	graph_nec->SetLineWidth(2);
	graph_nec->DrawClone("CP SAME");
	*/

	TLegend* legend = new TLegend(0.2,0.75,0.45,0.90);
	legend->SetFillColor(0);
	legend->AddEntry(graph_tot, "total", "P");
	legend->AddEntry(graph_good, "true", "P");
	legend->AddEntry(graph_random, "random", "P");
	legend->AddEntry(graph_scat, "scattered", "P");
	legend->AddEntry(graph_nec, "NEC rate", "P");
	legend->Draw();

	// =============================== COUNTING RATES
	TCanvas* m_cs2 = new TCanvas("m_cs2", "Counting rates versus activity", 900, 600);
	m_cs2->SetFrameFillColor(0);

	TGraphErrors* graph_cr_tot = new TGraphErrors( count2, m_cr_activity, m_cr_ntot, m_null, m_cr_dntot );
	graph_cr_tot->SetMinimum( 0 );
	graph_cr_tot->SetTitle("Counting rates vs Activity");
	// graph_cr_tot->GetXaxis()->SetTitle("^{22}Na Activity(Bq)");
	graph_cr_tot->GetXaxis()->SetTitle("Activity(Bq)");
	graph_cr_tot->GetXaxis()->SetLabelSize(0.03);
	graph_cr_tot->GetYaxis()->SetTitle("Count rate kcps");
	graph_cr_tot->SetMarkerStyle(21);
	graph_cr_tot->SetMarkerSize(markersize);
	graph_cr_tot->SetMarkerColor(colortot);
	graph_cr_tot->SetLineColor(colortot);
	graph_cr_tot->SetLineWidth(2);
	graph_cr_tot->DrawClone("ACP");

   // if you have #true, #random, #scattered, then do the following too:
    /*
	TGraphErrors* graph_cr_good = new TGraphErrors( count2, m_cr_activity, m_cr_ngood, m_null, m_cr_dngood );
	graph_cr_good->SetMarkerStyle(21);
	graph_cr_good->SetMarkerSize(markersize);
	graph_cr_good->SetMarkerColor(colorgood);
	graph_cr_good->SetLineColor(colorgood);
	graph_cr_good->SetLineWidth(2);
	graph_cr_good->DrawClone("CP SAME");

	TGraphErrors* graph_cr_random = new TGraphErrors( count2, m_cr_activity, m_cr_nrandom, m_null, m_cr_dnrandom );
	graph_cr_random->SetMarkerStyle(21);
	graph_cr_random->SetMarkerSize(markersize);
	graph_cr_random->SetMarkerColor(colorrandom);
	graph_cr_random->SetLineColor(colorrandom);
	graph_cr_random->SetLineWidth(2);
	graph_cr_random->DrawClone("CP SAME");

	TGraphErrors* graph_cr_scat = new TGraphErrors( count2, m_cr_activity, m_cr_nscat, m_null, m_cr_dnscat );
	graph_cr_scat->SetMarkerStyle(21);
	graph_cr_scat->SetMarkerSize(markersize);
	graph_cr_scat->SetMarkerColor(colorscat);
	graph_cr_scat->SetLineColor(colorscat);
	graph_cr_scat->SetLineWidth(2);
	graph_cr_scat->DrawClone("CP SAME");

	TGraphErrors* graph_cr_nec = new TGraphErrors( count2, m_cr_activity, m_cr_nec, m_null, m_null );
	graph_cr_nec->SetMarkerStyle(21);
	graph_cr_nec->SetMarkerSize(markersize);
	graph_cr_nec->SetMarkerColor(colornec);
	graph_cr_nec->SetLineColor(colornec);
	graph_cr_nec->SetLineWidth(2);
	graph_cr_nec->DrawClone("CP SAME");

	TLegend* legend_cr = new TLegend(0.2,0.75,0.45,0.90);
	legend_cr->SetFillColor(0);
	legend_cr->AddEntry(graph_tot, "total", "P");
	legend_cr->AddEntry(graph_good, "true", "P");
	legend_cr->AddEntry(graph_random, "random", "P");
	legend_cr->AddEntry(graph_scat, "scattered", "P");
	legend_cr->AddEntry(graph_nec, "NEC rate", "P");
	legend_cr->Draw();
	*/

	return 0;
}


