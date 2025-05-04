
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <math.h>

enum VIP_COINC_CLASSIFICATION
{
    CLASSIFICATION_TRUE = 0, 
    CLASSIFICATION_RANDOM = 1,
    CLASSIFICATION_SCATTER = 2
};

using namespace std;

double GetCountRate_cps(int in_generated, const double& in_activity_MBq, int in_coinc, double& out_error);

void CalcCRandWriteOut(ofstream& outfile, string& in_string, int in_coinc, int in_rejected, 
				const int in_generated, const double& in_activity_MBq, 
				double& out_rate_cps, double& out_rate_acc_cps, 
				double& out_err, double& out_err_acc);

ofstream outfile_countrate("");

// ======================================================================

int Main()
{
    double z1, y1, x1, E1; // mm
    double z2, y2, x2, E2; // mm
    int classificationFlag;
 
    double R = 100.;       // mm

    double a = 0.;
    double b = 0.;
    double c = 0.;

    double p = 0.;
    double S = 0.;
    double L = 0.;

    fstream fdata;
    string fname;
    cout << "Give PROMPTS filename" << endl;    
    cin >> fname;
    fdata.open( fname.c_str(), ios::in );
    if ( !fdata.is_open() )
    {
        cout << "ERROR! File does not exist: " << fname << endl;
        return 1;
    }

    bool useClassificationFlag( true );

    int i = 0;

    ofstream outfile("accepted_PET_img.out_NEW");

    cout << "Give Radius of phantom" << endl;
    cin >> R;

    cout << "Give #generated" << endl;
    int numgen; cin >> numgen;
	
	cout << "Give Activity (in MBq)" << endl;
	double activity_MBq; cin >> activity_MBq;

    TString outfn("accepted_R");
    outfn += (int)(R);
    outfn += ".dat";
    ofstream outtxtfile(outfn);

	TString outfn_cr("countrates_R");
    outfn_cr += (int)(R);
    outfn_cr += ".dat";
	outfile_countrate.open(outfn_cr);

    outtxtfile << "Using Radius: " << R << endl;
    outtxtfile << "generated: " << numgen << endl;
    outtxtfile << "Activity: " << activity_MBq << endl;
    
    if ( R <= 0 ) return -1;

    int N_totalOriginal( 0 );
    int N_rejected( 0 );

    int N_orig_randoms(0), N_rejected_randoms( 0 );
    int N_orig_trues(0), N_rejected_trues( 0 );
    int N_orig_scattered(0), N_rejected_scattered( 0 ); 
    int N_orig_other(0), N_rejected_other( 0 );
        
    while ( !fdata.eof() )
    {
        fdata >> z1 >> y1 >> x1 >> E1 >> z2 >> y2 >> x2 >> E2;
        if (useClassificationFlag)
            fdata >> classificationFlag;

        fdata.ignore(2048, '\n');
    
        if (!fdata.eof())
        { 
            N_totalOriginal++;
            if ( (classificationFlag == CLASSIFICATION_TRUE || classificationFlag == 10) ) N_orig_trues++;
            else if ( classificationFlag == CLASSIFICATION_RANDOM ) N_orig_randoms++; 
            else if ( classificationFlag == CLASSIFICATION_SCATTER ) N_orig_scattered++; 
            else 
			{	
    			cout << "'other', linenr: " << N_totalOriginal 
					 << " classification: " << classificationFlag << endl;
				N_orig_other++;
			}

            a = sqrt(x1*x1 + y1*y1);    // distance hit1 to src
            b = sqrt(x2*x2 + y2*y2);    // distance hit2 to src
            c = sqrt(pow((x1-x2),2) + pow((y1-y2),2));    // distance hit2 to hit1
    
            p = (a+b+c)/2.;                        // semi-perimeter
            S = sqrt(p*(p-a)*(p-b)*(p-c));        // Area of triangle
            L = 2.*S/c;                            // Distance from source to line c 

            // TRIGONOMETRY (See notebook IX)
            // Heron's formula
            // Semiperimeter p = 0.5 * (a + b + c)
            // Area of triangle (a,b,c), S = sqrt[p * ( p - a)(p - b)(p - c)]
            // Also: area of triangle = base * height / 2, So: height = 2 * Area/base
            //         height = L
            //         base = c
            //         area = S
            // L = 2 * S/c
    
            if (useClassificationFlag)
            {
                if (L > R)
                {
                    if ( (classificationFlag == 0 || classificationFlag == 10) ) N_rejected_trues++;
                    else if ( classificationFlag == 1 ) N_rejected_randoms++; 
                    else if ( classificationFlag == 2) N_rejected_scattered++; 
                    else N_rejected_other++;
                }
                else
                {
                    outfile << z1 << " " << y1 << " " << x1 << " " << E1 << "    " 
                            << z2 << " " << y2 << " " << x2 << " " << E2;
                    if (useClassificationFlag)
                        outfile << "   " << classificationFlag;
                    outfile << endl;
                }
            }

            if (L > R) N_rejected++; 

            i++;
        }
    } // while
    
    outfile_countrate << activity_MBq << " MBq" << endl;

	double countrate(0.0), countrate_accepted(0.0);
	string sigh("#total: ");
	double cr_tot, cr_tot_acc;
	double cr_err, cr_err_acc;
	double cr_true_err, cr_true_err_acc;
	CalcCRandWriteOut(outtxtfile, sigh, N_totalOriginal, N_rejected, numgen, activity_MBq
			, cr_tot, cr_tot_acc
			, cr_err, cr_err_acc);

    if (useClassificationFlag)
    {
		sigh = "#trues: ";
		double cr_true, cr_true_acc;
		CalcCRandWriteOut(outtxtfile, sigh, N_orig_trues, N_rejected_trues, numgen, activity_MBq
			, cr_true, cr_true_acc
			, cr_true_err, cr_true_err_acc);

		sigh = "#randoms: ";
		double cr_random, cr_random_acc;
		CalcCRandWriteOut(outtxtfile, sigh, N_orig_randoms, N_rejected_randoms, numgen, activity_MBq
			, cr_random, cr_random_acc
			, cr_err, cr_err_acc);

		sigh = "#scattered: ";
		double cr_scat, cr_scat_acc;
		CalcCRandWriteOut(outtxtfile, sigh, N_orig_scattered, N_rejected_scattered, numgen, activity_MBq
			, cr_scat, cr_scat_acc
			, cr_err, cr_err_acc);

        /*
		sigh = "#other: ";
		double cr_other, cr_other_acc;
		CalcCRandWriteOut(outtxtfile, sigh, N_orig_other, N_rejected_other, numgen, activity_MBq
			, cr_other, cr_other_acc
			, cr_err, cr_err_acc);
        */

		// Calculation of NEC
		//
		double nec_factor;
		nec_factor = cr_true/(cr_true + cr_random + cr_scat);
		double nec = nec_factor * cr_true;

		nec_factor = cr_true_acc/(cr_true_acc + cr_random_acc + cr_scat_acc);
		double nec_acc = nec_factor * cr_true_acc;

		cout << " NECR: " << nec/1e3 << " +- " << cr_true_err/1e3 << " kcps; " 
		 	 << " accepted: " << nec_acc/1e3 << " +- " << cr_true_err_acc/1e3 << " kcps"
			 << endl;
		outtxtfile << " NECR: " << nec/1e3 << " +- " << cr_true_err/1e3 << " kcps; " 
		 	 << " accepted: " << nec_acc/1e3 << " +- " << cr_true_err_acc/1e3 << " kcps"
			 << endl;

		outfile_countrate << "NECR:" << " " 
			 << nec/1e3 << " +- " << cr_true_err/1e3 << " kcps; "
			 << nec_acc/1e3 << " +- " << cr_true_err_acc/1e3 << " kcps"
			 << endl;
	}

    return 0;
}

// ===============================================================

double GetCountRate_cps(int in_generated, const double& in_activity_MBq, int in_coinc, double& out_error)
{
	double BR = 0.9686; // branching ratio for F18
	double gen = in_generated * BR;
	double factor = in_activity_MBq*1e6 / gen;
	double countrate_cps = in_coinc * factor;

	out_error = sqrt(in_coinc) * factor;

	return countrate_cps;
}

// ===============================================================

void CalcCRandWriteOut(ofstream& outfile, string& in_string, int in_coinc, int in_rejected, 
				const int in_generated, const double& in_activity_MBq, 
				double& out_rate_cps, double& out_rate_acc_cps, 
				double& out_err, double& out_err_acc)
{
	out_rate_cps = GetCountRate_cps(in_generated, in_activity_MBq, in_coinc, out_err);
	out_rate_acc_cps = GetCountRate_cps(in_generated, in_activity_MBq, in_coinc-in_rejected, out_err_acc);

    cout     << in_string << " " << in_coinc
             << " #rejected: " << in_rejected 
             << " #accepted: " << in_coinc - in_rejected << endl;
	cout     << " Count Rate: "  << out_rate_cps/1e3 << " +- " << out_err/1e3 << " kcps; "
				<< " accepted: " << out_rate_acc_cps/1e3 << " +- " << out_err_acc/1e3 << " kcps"
             << endl;

    outfile  << in_string << " " << in_coinc
             << " #rejected: " << in_rejected 
             << " #accepted: " << in_coinc - in_rejected << endl;
	outfile  << " Count Rate: "  << out_rate_cps/1e3 << " +- " << out_err/1e3 << " kcps; "
				<< " accepted: " << out_rate_acc_cps/1e3 << " +- " << out_err_acc/1e3 << " kcps"
             << endl;

	outfile_countrate << in_string << " " 
			 << out_rate_cps/1e3 << " +- " << out_err/1e3 << " kcps; "
			 << out_rate_acc_cps/1e3 << " +- " << out_err_acc/1e3 << " kcps"
			 << endl;
}

// ===============================================================


