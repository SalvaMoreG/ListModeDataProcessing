
#include "DoProcess.hh"

#include <iostream>
#include <sys/stat.h>

#include "TString.h"

// this is a test

using namespace std;

int main( int argc, char *argv[] )
{
	ConvertROOTtoLM_DoProcess process;
	int err = process.Execute( argc, argv );
	return err;
}

// ************************************************************

ConvertROOTtoLM_DoProcess::ConvertROOTtoLM_DoProcess()
{
	m_scanType = SCAN_TYPE_PET;
}

ConvertROOTtoLM_DoProcess::~ConvertROOTtoLM_DoProcess()
{
}

int
ConvertROOTtoLM_DoProcess::ProcessInputs(int argc, char *argv[])
{
    InputFlags* flags = InputFlags::Instance();

    flags->theInputMode = IN_ROOT;
    flags->theOutputMode = OUT_LM;

    //
    theListFilePath = "";
    theDetConfFilePath = "";

    std::string programName = argv[0];
    bool flagMissing = (argc < 2) ? true : false;

    for(int par_i=1; par_i<argc || flagMissing; par_i++)
    {
        string par_name = flagMissing ? "-help" : argv[par_i];
        if(par_name == "-list")
        {
            if(argc>par_i+1) theListFilePath=argv[++par_i];
            else
            {
                cout<<"ERROR in "<<__func__<<endl;
                cout<<"missing par value for -list"<<endl;
                return 1;
            }
        }
        else if(par_name == "-conf")
        {
            if(argc>par_i+1) theDetConfFilePath=argv[++par_i];
            else
            {
                cout<<"ERROR in "<<__func__<<endl;
                cout<<"missing par value for -conf"<<endl;
                return 1;
            }
        }

        // ENERGY RESOLUTION FLAGS <<<<<<<
        else if(par_name == "-newEResFit")
        {
            flags->m_EresMode = InputFlags::ERES_GERARD;
        }
        else if(par_name == "-zeroERes")
        {
            flags->m_EresMode = InputFlags::ERES_ZERO;
        }
        else if(par_name == "-userERes")
        {
            flags->m_EresMode = InputFlags::ERES_USER;
            if  (argc > par_i+1)
            {
                double percFWHM = atof( argv[++par_i] );
                flags->m_userEres = percFWHM/2.355;
            }
            else
            {
                cout<<"ERROR in "<<__func__<<endl;
                cout<<"missing par value for -conf"<<endl;
                return 1;
            }
        }
        // >>>

        else if(par_name == "-GmSDT")
        {
            flags->doGmSDTTree = true;
        }

        else if (par_name == "-sumAllStepsPerEvent")   // all steps of one event are added up
        {
            flags->sumAllStepsPerEvent = true;

        }
        else if (par_name == "-realHitPosOutput")       // To get PROMPTS file with real positions
        {
            flags->doRealHitPosOutput = true;
        }

		// /*
        else if (par_name == "-doClassification")
        {
            flags->doClassification = true;
        }
        else if (par_name == "-writeAddClassificationFlag")
        {
            flags->doWriteAddClassificationFlag = true;
        }
		// */

        else
        {
            if (par_name != "-help")
            {
                cout << "wrong flag: " << par_name << ". ";
                cout << endl;
            }

            cout << "Possible flags are (flags with (*) are obligatory): " << endl;
            cout << endl;
            cout << programName << " -list <gamos-list file> (*)" << endl;
            cout << programName << " -conf <detector-parameters file> (*)" << endl;
            cout << endl;

            cout << programName << " -newEResFit (= default, Gerard's parametrisation)" << endl;
            cout << programName << " -zeroERes (= 0 energy resolution)" << endl;
            cout << programName << " -userERes <value> (= give the energy resolution in % FWHM)" << endl;
			cout << "    e.g.: -userERes 0.15" << endl;
			cout << "WARNING! Energy resolution only works on ROOT input!!! (in LM files, in principle it's already done!!!" << endl;

            cout << programName << " -GmSDT" << endl;
            cout << programName << " -sumAllStepsPerEvent" << endl;
            cout << programName << " -realHitPosOutput" << endl;

            cout << programName << " -doClassification" << endl;
            cout << programName << " -writeAddClassificationFlag" << endl;

            cout << endl;
            exit(1);
        }
    }

    if ( theDetConfFilePath.size() < 1 )
    {
        cout << "ERROR, you did not give a DetParameters conf file: " << theDetConfFilePath << endl;
        exit(1);
    }

    return 0;
}




















