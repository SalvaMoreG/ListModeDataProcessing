
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

int main()
{
    double z1, y1, x1, E1; // mm
    double z2, y2, x2, E2; // mm
    int random;
 
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

    int i = 0;

	ofstream outfile("accepted_PET_img.out_NEW");

    // while( R > 0 )
    {
        cout << "Give Radius of phantom" << endl;
        cin >> R;
	
		TString outfn("accepted_R");
		outfn += (int)(R);
		outfn += ".dat";
		ofstream outtxtfile(outfn);

        cout << "Using Radius: " << R << endl;
        outtxtfile << "Using Radius: " << R << endl;

        // if ( R <= 0 ) break;

		int N_totalOriginal( 0 );
        int N_rejected( 0 );

        int N_orig_randoms(0), N_rejected_randoms( 0 );
        int N_orig_trues(0), N_rejected_trues( 0 );
        int N_orig_scattered(0), N_rejected_scattered( 0 ); 
        int N_orig_other(0), N_rejected_other( 0 );
		
        while (1)
        {
            fdata >> z1 >> y1 >> x1 >> E1 >> z2 >> y2 >> x2 >> E2;
            // RandomFlag
            fdata >> random;

			fdata.ignore(2048, '\n');
			// fdata.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
            if (fdata.eof()) break;
    
			N_totalOriginal++;
			if ( (random == 0 || random == 10) ) N_orig_trues++;
			else if ( random == 1 ) N_orig_randoms++; 
			else if ( random == 2) N_orig_scattered++; 
			else N_orig_other++;

            a = sqrt(x1*x1 + y1*y1);	// distance hit1 to src
            b = sqrt(x2*x2 + y2*y2);	// distance hit2 to src
            c = sqrt(pow((x1-x2),2) + pow((y1-y2),2));	// distance hit2 to hit1
    
            p = (a+b+c)/2.;						// semi-perimeter
            S = sqrt(p*(p-a)*(p-b)*(p-c));		// Area of triangle
            L = 2.*S/c;							// Distance from source to line c 

			// TRIGONOMETRY (See notebook IX)
			// Heron's formula
			// Semiperimeter p = 0.5 * (a + b + c)
			// Area of triangle (a,b,c), S = sqrt[p * ( p - a)(p - b)(p - c)]
			// Also: area of triangle = base * height / 2, So: height = 2 * Area/base
			// 		height = L
			// 		base = c
			// 		area = S
			// L = 2 * S/c
    
			if (L > R) // outside FOV (XY boundaries)
			{
               	if ( (random == 0 || random == 10) ) N_rejected_trues++;
               	else if ( random == 1 ) N_rejected_randoms++; 
               	else if ( random == 2) N_rejected_scattered++; 
				else N_rejected_other++;
			}
			else
			{
            	outfile << z1 << " " << y1 << " " << x1 << " " << E1 << "    " 
						<< z2 << " " << y2 << " " << x2 << " " << E2;
               		outfile << "   " << random;
				outfile << endl;
			}

            if (L > R) N_rejected++; 

            // if (!(i%100000)) cout<<i<<endl;
            i++;
        }

        cout     << "#original LORs: " << N_totalOriginal
			     << " #rejected: " << N_rejected 
			     << " #accepted: " << N_totalOriginal - N_rejected
			     << endl;
        outtxtfile << "#original LORs: " << N_totalOriginal
			     << " #rejected: " << N_rejected 
			     << " #accepted: " << N_totalOriginal - N_rejected
			     << endl;

        cout << "#trues:         " << N_orig_trues
                 << " #rejected: " << N_rejected_trues 
                 << " #accepted: " << N_orig_trues - N_rejected_trues 
				 << endl;
        cout << "#randoms:       " << N_orig_randoms
                 << " #rejected: " << N_rejected_randoms 
                 << " #accepted: " << N_orig_randoms - N_rejected_randoms 
				 << endl;
        cout << "#scattered:     " << N_orig_scattered
				 << " #rejected: " << N_rejected_scattered 
                 << " #accepted: " << N_orig_scattered - N_rejected_scattered 
				 << endl;
        cout << "#other:         " << N_orig_other 
				 << " #rejected: " << N_rejected_other 
                 << " #accepted: " << N_orig_other - N_rejected_other 
				 << endl;

        outtxtfile << "#trues:         " << N_orig_trues
                 << " #rejected: " << N_rejected_trues 
                 << " #accepted: " << N_orig_trues - N_rejected_trues 
				 << endl;
        outtxtfile << "#randoms:       " << N_orig_randoms
                 << " #rejected: " << N_rejected_randoms 
                 << " #accepted: " << N_orig_randoms - N_rejected_randoms 
				 << endl;
        outtxtfile << "#scattered:     " << N_orig_scattered
				 << " #rejected: " << N_rejected_scattered 
                 << " #accepted: " << N_orig_scattered - N_rejected_scattered 
				 << endl;
        outtxtfile << "#other:         " << N_orig_other 
				 << " #rejected: " << N_rejected_other 
                 << " #accepted: " << N_orig_other - N_rejected_other 
				 << endl;

        fdata.close();
        fdata.open( fname.c_str(), ios::in );
        if ( !fdata.is_open() )
        {
            cout << "ERROR! File does not exist: " << fname << endl;
            return 1;
        }
    } 

    return 0;
}



