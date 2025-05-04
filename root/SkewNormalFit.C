#include<TMath.h>

double skewNormal(double *E, double *par);
void FitHistogram(TH1D* h, TF1* fitFunc, double Emin, double Emax);

int Main()
{
	gStyle->SetOptFit();
    
	//Hardcoded now, just for a faster testing
    string fname = "/home/smoreno/VIP_PET/trunk/MyWork_2024/2024_07_VIPPrototype_Experimental/TranslateVIPChipReadoutData_Energy_fase3_example/LM_new_low.out";
    ifstream ffile( fname.c_str() );

	/*cout << "give filename" << endl;	
	cin >> fname;

	if ( !ffile.is_open() )
	{
		cout << "file does not exist, " << fname << endl;
		exit(1);
	}
    */
	

	TH1D* h1 = new TH1D("h1", "E", 1400, 0.45, 0.55);

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

    double Emin(0.492), Emax(0.518);
    TF1* fitFunc = new TF1("fitFunc", skewNormal, Emin, Emax, 4);

    FitHistogram(h1, fitFunc, Emin, Emax);
    
    return 0;
}

void FitHistogram(TH1D* h, TF1* fitFunc, double Emin, double Emax)
{
    TCanvas* m_1 = new TCanvas("m_1", "m_1", 800, 800);
    m_1->cd();
    h->Draw();
    
    fitFunc->SetParNames("xi", "omega", "alpha", "A");
    fitFunc->SetParameters(0.5, 0.03, -7, 200);
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);
        
    TFitResultPtr result = h->Fit(fitFunc, "S", "", Emin, Emax);
    cout << result->Status() << endl; //Status = 0 means that fit converged
}

double skewNormal(double *E, double *par) 
{
    double xi = par[0];     
    double omega = par[1];  
    double alpha = par[2];  
    double A = par[3];
    double z = (E[0] - xi) / omega;
    double phi = TMath::Gaus(z, 0, 1, true); 
    double Phi = 0.5 * (1 + TMath::Erf(alpha * z / TMath::Sqrt(2))); 
    return A * (2 / omega) * phi * Phi;
}
