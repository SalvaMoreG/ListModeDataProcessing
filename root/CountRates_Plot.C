
// echo $activity (" " gen ) " " $numAll " " $totGood " " $numRandom " " $totScat

int Main()
{
	cout << "give filename with list of count rate filenames" << endl;
	string fname; cin >> fname;
	ifstream flist( fname.c_str() ,ios::in);
	if (!flist.is_open()) 
	{
		cout << "ERROR: list file not open: " << fname << endl;
		return 1;
	}

	const int MAX_IDX = 50;
	double m_cr_activity[MAX_IDX];    
	double m_cr_ntot[MAX_IDX], m_cr_nT[MAX_IDX], m_cr_nR[MAX_IDX], m_cr_nS[MAX_IDX];	
	double m_cr_dntot[MAX_IDX], m_cr_dnT[MAX_IDX], m_cr_dnR[MAX_IDX], m_cr_dnS[MAX_IDX];	
	double m_cr_nec[MAX_IDX], m_cr_dnec[MAX_IDX];
    
	double m_cr_ntot_acc[MAX_IDX], m_cr_nT_acc[MAX_IDX], m_cr_nR_acc[MAX_IDX], m_cr_nS_acc[MAX_IDX];	
	double m_cr_dntot_acc[MAX_IDX], m_cr_dnT_acc[MAX_IDX], m_cr_dnR_acc[MAX_IDX], m_cr_dnS_acc[MAX_IDX];	
	double m_cr_nec_acc[MAX_IDX], m_cr_dnec_acc[MAX_IDX];
    
	double m_null[MAX_IDX] = {0.0};
    int count(0);
    string dumStr;
    //
	while ( !flist.eof() )
	{
		flist >> fname;
        if ( !flist.eof() )
        {            
            ifstream fdata( fname.c_str() ,ios::in);
            if ( !fdata.is_open() )
            {
                cout << "ERROR: list file not open: " << fname << endl;
                return 1;
            }            
            if ( !fdata.eof() )    
            {
                fdata >> m_cr_activity[count] >> dumStr;    // in MBq
                m_cr_activity[count] = m_cr_activity[count] * 1e6;  // in Bq

                // #total:  383.234 +- 6.29013 kcps; 376.213 +- 6.23225 kcps
                fdata >> dumStr >> m_cr_ntot[count] >> dumStr >> m_cr_dntot[count] 
                      >> dumStr >> m_cr_ntot_acc[count] >> dumStr >> m_cr_dntot_acc[count] >> dumStr;
                fdata >> dumStr >> m_cr_nT[count] >> dumStr >> m_cr_dnT[count] 
                      >> dumStr >> m_cr_nT_acc[count] >> dumStr >> m_cr_dnT_acc[count] >> dumStr;
                fdata >> dumStr >> m_cr_nR[count] >> dumStr >> m_cr_dnR[count] 
                      >> dumStr >> m_cr_nR_acc[count] >> dumStr >> m_cr_dnR_acc[count] >> dumStr;                      
                fdata >> dumStr >> m_cr_nS[count] >> dumStr >> m_cr_dnS[count] 
                      >> dumStr >> m_cr_nS_acc[count] >> dumStr >> m_cr_dnS_acc[count] >> dumStr;

                fdata >> dumStr >> m_cr_nec[count] >> dumStr >> m_cr_dnec[count] 
                      >> dumStr >> m_cr_nec_acc[count] >> dumStr >> m_cr_dnec_acc[count] >> dumStr;
/*
cout << count << " " << m_cr_activity[count] << endl;
cout << " Tacc: " << m_cr_nT_acc[count] 
     << " Racc: " << m_cr_nR_acc[count] << " Sacc: " << m_cr_nS_acc[count] << endl;
*/                      
			}
			count++;
			fdata.close();
		}
	}

	// Graphical stuff
	float markersize = 1.2;
	int colortot = kBlack;
	int colorT = kBlue+1;
	int colorR = kOrange+8;
	int colorS = kRed+1;
	int colornec = kGreen+3;	
    cout << "#points: " << count << endl;
	
	// =============================== COUNTING RATES
	TCanvas* m_cr = new TCanvas("m_cr", "Counting rates versus activity", 900, 600);
	m_cr->SetFrameFillColor(0);

	TGraphErrors* graph_cr_tot = new TGraphErrors( count, m_cr_activity, m_cr_ntot, m_null, m_cr_dntot );
	graph_cr_tot->SetMinimum( 0 );
	graph_cr_tot->SetTitle("Counting rates vs Activity");
	graph_cr_tot->GetXaxis()->SetTitle("Activity(Bq)");
	graph_cr_tot->GetXaxis()->SetLabelSize(0.03);
	graph_cr_tot->GetYaxis()->SetTitle("Count rate kcps");
	graph_cr_tot->SetMarkerStyle(21);
	graph_cr_tot->SetMarkerSize(markersize);
	graph_cr_tot->SetMarkerColor(colortot);
	graph_cr_tot->SetLineColor(colortot);
	graph_cr_tot->SetLineWidth(2);
	graph_cr_tot->DrawClone("ACP");

	TGraphErrors* graph_cr_T = new TGraphErrors( count, m_cr_activity, m_cr_nT, m_null, m_cr_dnT );
	graph_cr_T->SetMarkerStyle(21);
	graph_cr_T->SetMarkerSize(markersize);
	graph_cr_T->SetMarkerColor(colorT);
	graph_cr_T->SetLineColor(colorT);
	graph_cr_T->SetLineWidth(2);
	graph_cr_T->DrawClone("CP SAME");

	TGraphErrors* graph_cr_R = new TGraphErrors( count, m_cr_activity, m_cr_nR, m_null, m_cr_dnR );
	graph_cr_R->SetMarkerStyle(21);
	graph_cr_R->SetMarkerSize(markersize);
	graph_cr_R->SetMarkerColor(colorR);
	graph_cr_R->SetLineColor(colorR);
	graph_cr_R->SetLineWidth(2);
	graph_cr_R->DrawClone("CP SAME");

	TGraphErrors* graph_cr_S = new TGraphErrors( count, m_cr_activity, m_cr_nS, m_null, m_cr_dnS );
	graph_cr_S->SetMarkerStyle(21);
	graph_cr_S->SetMarkerSize(markersize);
	graph_cr_S->SetMarkerColor(colorS);
	graph_cr_S->SetLineColor(colorS);
	graph_cr_S->SetLineWidth(2);
	graph_cr_S->DrawClone("CP SAME");

	TGraphErrors* graph_cr_nec = new TGraphErrors( count, m_cr_activity, m_cr_nec, m_null, m_cr_dnT );
	graph_cr_nec->SetMarkerStyle(21);
	graph_cr_nec->SetMarkerSize(markersize);
	graph_cr_nec->SetMarkerColor(colornec);
	graph_cr_nec->SetLineColor(colornec);
	graph_cr_nec->SetLineWidth(2);
	graph_cr_nec->DrawClone("CP SAME");

	TLegend* legend_cr = new TLegend(0.2,0.75,0.45,0.90);
	legend_cr->SetFillColor(0);
	legend_cr->AddEntry(graph_cr_tot, "total", "P");
	legend_cr->AddEntry(graph_cr_T, "true", "P");
	legend_cr->AddEntry(graph_cr_R, "random", "P");
	legend_cr->AddEntry(graph_cr_S, "scattered", "P");
	legend_cr->AddEntry(graph_cr_nec, "NEC rate", "P");
	legend_cr->Draw();
    
    // ===================================================================================
    // COUNT RATES, ACCEPTANCE CORRECTED
	//
	TCanvas* m_cr_acc = new TCanvas("m_cr_acc", "Counting rates versus activity (acc corr)", 900, 600);
	m_cr_acc->SetFrameFillColor(0);

	TGraphErrors* graph_cr_tot_acc = 
        new TGraphErrors( count, m_cr_activity, m_cr_ntot_acc, m_null, m_cr_dntot_acc );
	graph_cr_tot_acc->SetMinimum( 0 );
	graph_cr_tot_acc->SetTitle("Counting rates vs Activity (Acceptance Corrected)");
	graph_cr_tot_acc->GetXaxis()->SetTitle("Activity(Bq)");
	graph_cr_tot_acc->GetXaxis()->SetLabelSize(0.03);
	graph_cr_tot_acc->GetYaxis()->SetTitle("Count rate kcps");
	graph_cr_tot_acc->SetMarkerStyle(21);
	graph_cr_tot_acc->SetMarkerSize(markersize);
	graph_cr_tot_acc->SetMarkerColor(colortot);
	graph_cr_tot_acc->SetLineColor(colortot);
	graph_cr_tot_acc->SetLineWidth(2);
	graph_cr_tot_acc->DrawClone("ACP");

	TGraphErrors* graph_cr_T_acc = new TGraphErrors( count, m_cr_activity, m_cr_nT_acc, m_null, m_cr_dnT_acc );
	graph_cr_T_acc->SetMarkerStyle(21);
	graph_cr_T_acc->SetMarkerSize(markersize);
	graph_cr_T_acc->SetMarkerColor(colorT);
	graph_cr_T_acc->SetLineColor(colorT);
	graph_cr_T_acc->SetLineWidth(2);
	graph_cr_T_acc->DrawClone("CP SAME");

	TGraphErrors* graph_cr_R_acc = new TGraphErrors( count, m_cr_activity, m_cr_nR_acc, m_null, m_cr_dnR_acc );
	graph_cr_R_acc->SetMarkerStyle(21);
	graph_cr_R_acc->SetMarkerSize(markersize);
	graph_cr_R_acc->SetMarkerColor(colorR);
	graph_cr_R_acc->SetLineColor(colorR);
	graph_cr_R_acc->SetLineWidth(2);
	graph_cr_R_acc->DrawClone("CP SAME");

	TGraphErrors* graph_cr_S_acc = new TGraphErrors( count, m_cr_activity, m_cr_nS_acc, m_null, m_cr_dnS_acc );
	graph_cr_S_acc->SetMarkerStyle(21);
	graph_cr_S_acc->SetMarkerSize(markersize);
	graph_cr_S_acc->SetMarkerColor(colorS);
	graph_cr_S_acc->SetLineColor(colorS);
	graph_cr_S_acc->SetLineWidth(2);
	graph_cr_S_acc->DrawClone("CP SAME");

	TGraphErrors* graph_cr_nec_acc = 
        new TGraphErrors( count, m_cr_activity, m_cr_nec_acc, m_null, m_cr_dnT_acc );
	graph_cr_nec_acc->SetMarkerStyle(21);
	graph_cr_nec_acc->SetMarkerSize(markersize);
	graph_cr_nec_acc->SetMarkerColor(colornec);
	graph_cr_nec_acc->SetLineColor(colornec);
	graph_cr_nec_acc->SetLineWidth(2);
	graph_cr_nec_acc->DrawClone("CP SAME");

	TLegend* legend_cr_acc = new TLegend(0.2,0.75,0.45,0.90);
	legend_cr_acc->SetFillColor(0);
	legend_cr_acc->AddEntry(graph_cr_tot_acc, "total", "P");
	legend_cr_acc->AddEntry(graph_cr_T_acc, "true", "P");
	legend_cr_acc->AddEntry(graph_cr_R_acc, "random", "P");
	legend_cr_acc->AddEntry(graph_cr_S_acc, "scattered", "P");
	legend_cr_acc->AddEntry(graph_cr_nec_acc, "NEC rate", "P");
    legend_cr_acc->Draw();    

	return 0;
}


