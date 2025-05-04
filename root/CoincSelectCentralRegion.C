
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

// int ImplementAcceptanceAngle() 
int Main()
{
    double z1, y1, x1, E1; // mm
    double z2, y2, x2, E2; // mm
    int flag;
 
    cout << "Give PROMPTS filename" << endl;    
    string fname; cin >> fname;
    ifstream fdata( fname.c_str(), ios::in );
    if ( !fdata.is_open() )
    {
        cout << "ERROR! File does not exist: " << fname << endl;
        return 1;
    }

	ofstream outfile("selected_PROMPTS.out_addedFlag");
	cout << "Give size of central region (in mm!)" << endl;
	double regionsize; cin >> regionsize;

	cout << "Give # generated" << endl;
	int NGen; cin >> NGen;

	int N_rejected(0);
	int N_accepted_all(0);
	int N_accepted_true(0);
	double k_branching_ratio = 0.9060;

    while ( !fdata.eof())
    {
    	if ( !fdata.eof())
    	{
            fdata >> z1 >> y1 >> x1 >> E1 >> z2 >> y2 >> x2 >> E2;
            fdata >> flag;
			fdata.ignore(2048, '\n');
			// fdata.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
			if (fabs(z1) < 0.5 * regionsize && fabs(z2) < 0.5 * regionsize)
			{
            	outfile << z1 << " " << y1 << " " << x1 << " " << E1 << " " 
            	        << z2 << " " << y2 << " " << x2 << " " << E2 << " "
						<< flag << endl;
				if (flag == 0 || flag == 10)
					N_accepted_true++;
				N_accepted_all++;
			}
			else
				N_rejected++;
				
        }
    } 

	cout << "ALL coincidences all regions: " << endl;
	cout << "#coinc: " << N_accepted_all + N_rejected << endl;
	double s = (double) (N_accepted_all + N_rejected) / (double) NGen;
	double s_abs = 100.0 * s/k_branching_ratio;
	cout << "sensitivity: " << s << " cps/Bq" << " absolute sensitivity: " << s_abs << " %" << endl;
	cout << endl;	

	cout << "ALL coincidences in " << regionsize << " mm of detector " << endl;
	cout << "#accepted: " << N_accepted_all << " #rejected:" << N_rejected << endl;
	s = (double) N_accepted_all / (double) NGen;
	s_abs = 100.0 * s/k_branching_ratio;
	cout << "sensitivity: " << s << " cps/Bq" << " absolute sensitivity: " << s_abs << " %" << endl;
	cout << endl;	

	cout << "TRUE coincidences in " << regionsize << " mm of detector " << endl;
	cout << "#accepted: " << N_accepted_true << endl;
	s = (double) N_accepted_true / (double) NGen;
	s_abs = 100.0 * s/k_branching_ratio;
	cout << "sensitivity: " << s << " cps/Bq" << " absolute sensitivity: " << s_abs << " %" << endl;

    return 0;
}



