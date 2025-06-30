
#include "DoProcess.hh"
#include "ProcessDataClass.hh"

#include "TStyle.h"
#include "TH1.h"
#include "TCanvas.h"

#include <iostream>
#include <sys/stat.h>

using namespace std;

void test_EnergySmearing();

int main()
{
    test_EnergySmearing();
	return 0;
}


void test_EnergySmearing()
{
    TRandom3* theRandomGenerator = new TRandom3(time(NULL));
    const int NUM_EVENTS(10000);

    double percFWHM_scint = 0.1;  // 10% FWHM
    double sigma_userERes_scint_perc = percFWHM_scint/2.355;

    double percFWHM_semic = 0.017;  // 1.7% FWHM
    double sigma_userERes_semic_perc = percFWHM_semic/2.355;
    double E_10keV(10.0), E_511keV(511.0), E(0.0);

    // histoos
    TH1D* h_10_scint_1 = new TH1D("h_10_scint_1", "10 keV 10% FWHM method 1", 100, 0.0, 20.0);
    TH1D* h_10_scint_2 = new TH1D("h_10_scint_2", "10 keV 10% FWHM method 2", 100, 0.0, 20.0);

    TH1D* h_10_semic_1 = new TH1D("h_10_semic_1", "10 keV 1.7% FWHM method 1", 100, 0.0, 20.0);
    TH1D* h_10_semic_2 = new TH1D("h_10_semic_2", "10 keV 1.7% FWHM method 2", 100, 0.0, 20.0);

    TH1D* h_511_scint_1 = new TH1D("h_511_scint_1", "511 keV 10% FWHM method 1", 200, 400.0, 600.0);
    TH1D* h_511_scint_2 = new TH1D("h_511_scint_2", "511 keV 10% FWHM method 2", 200, 400.0, 600.0);

    TH1D* h_511_semic_1 = new TH1D("h_511_semic_1", "511 keV 1.7% FWHM method 1", 200, 400.0, 600.0);
    TH1D* h_511_semic_2 = new TH1D("h_511_semic_2", "511 keV 1.7% FWHM method 2", 200, 400.0, 600.0);

    for (int i = 0; i < NUM_EVENTS; i++)
    {
        // 10 keV  <<<<<<<<<
        // 10 %, method 2
        double factor = theRandomGenerator->Gaus(1.0, sigma_userERes_scint_perc);
        E = E_10keV * factor;
        h_10_scint_2->Fill(E);

        // 10 %, method 1
        double sigma = percFWHM_scint*E_10keV/2.355;
        E = theRandomGenerator->Gaus(E_10keV, sigma);
        h_10_scint_1->Fill(E);

        // 1.5 %, method 2
        factor = theRandomGenerator->Gaus(1.0, sigma_userERes_semic_perc);
        E = E_10keV * factor;
        h_10_semic_2->Fill(E);

        // 1.5 %, method 1
        sigma = percFWHM_semic*E_10keV/2.355;
        E = theRandomGenerator->Gaus(E_10keV, sigma);
        h_10_semic_1->Fill(E);
        // >>>

        // 511 keV  <<<<<<<<<
        // 10 %, method 2
        factor = theRandomGenerator->Gaus(1.0, sigma_userERes_scint_perc);
        E = E_511keV * factor;
        h_511_scint_2->Fill(E);

        // 10 %, method 1
        sigma = percFWHM_scint*E_511keV/2.355;
        E = theRandomGenerator->Gaus(E_511keV, sigma);
        h_511_scint_1->Fill(E);

        // 1.5 %, method 2
        factor = theRandomGenerator->Gaus(1.0, sigma_userERes_semic_perc);
        E = E_511keV * factor;
        h_511_semic_2->Fill(E);

        // 1.5 %, method 1
        sigma = percFWHM_semic*E_511keV/2.355;
        E = theRandomGenerator->Gaus(E_511keV, sigma);
        h_511_semic_1->Fill(E);
        // >>>
    }

    gStyle->SetOptFit(1);   // obviously, you moron
    TCanvas* m_1 = new TCanvas("m_1", "10 keV", 700, 700);
    m_1->Divide(2, 2);
    m_1->cd(1); h_10_scint_1->Draw(); h_10_scint_1->Fit("gaus");
    m_1->cd(2); h_10_scint_2->Draw(); h_10_scint_2->Fit("gaus");
    m_1->cd(3); h_10_semic_1->Draw(); h_10_semic_1->Fit("gaus");
    m_1->cd(4); h_10_semic_2->Draw(); h_10_semic_2->Fit("gaus");

    TCanvas* m_2 = new TCanvas("m_2", "511 keV", 700, 700);
    m_2->Divide(2, 2);
    m_2->cd(1); h_511_scint_1->Draw(); h_511_scint_1->Fit("gaus");
    m_2->cd(2); h_511_scint_2->Draw(); h_511_scint_2->Fit("gaus");
    m_2->cd(3); h_511_semic_1->Draw(); h_511_semic_1->Fit("gaus");
    m_2->cd(4); h_511_semic_2->Draw(); h_511_semic_2->Fit("gaus");

    m_1->Print("test_EnergySmearing_10keV.png", "png");
    m_2->Print("test_EnergySmearing_511keV.png", "png");
}















