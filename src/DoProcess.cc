#include "DoProcess.hh"
#include "TString.h"

#include "VIPTimer.hh"

using namespace std;

// ====================================================================================

DoProcess::DoProcess()
	: m_scanType( SCAN_TYPE_NONE )
	, theOutputsDirName("outputs")
	, theListFilePath("")
	, theDetConfFilePath("")
	, theDataProcess(NULL)
	, theGlobalHistograms(NULL)
{
	// Create instance (first time) of InputFlags
	InputFlags::Instance();
}

// ====================================================================================

DoProcess::~DoProcess()
{
	delete theDataProcess;
	delete theGlobalHistograms;

	InputFlags::Destroy();
}

// =======================================================================================

int
DoProcess::Execute( int argc, char *argv[] )
{
    cout << "DoProcess::Execute" << endl;
	// Process input flags
	ProcessInputs(argc, argv);
	
	// Check that "outputs" directory exists
	if ( !fileExists(theOutputsDirName) )
	{
		cout << "WARNING! Directory does not exist: " << theOutputsDirName << endl;
		cout << "Please create it first" << endl;
		return 1;
	}

	// Check that the list-file is there
	theListFile.open(theListFilePath.c_str());
	if ( !theListFile.is_open() )
	{
		cout << "ERROR in " << __func__ << endl;
		cout << "List file <" << theListFilePath << "> not open" << endl;
		return 2;
	}

	// Create the histograms
	if (InputFlags::Instance()->DoHistograms())
	{
		DoCreateHistoos();
	}

	// Create general objects...
	CreateGeneralObjects();

	DoDataLoop();

	return 0;
}

// ======================================================================================

void
DoProcess::DoDataLoop()
{
	std::string theOutLMname( theOutputsDirName + "/LM.out" );
	// std::string theOutPROMPTSname( theOutputsDirName + "/PROMPTS.out" );
	// cout << "just checking, output file names: " << theOutLMname << " " << theOutPROMPTSname << endl;
	std::string theOutEvName( theOutputsDirName + "/EV.out");
	std::string theOutHistoName( theOutputsDirName + "/histos.out" );
    
	if (   InputFlags::Instance()->GetOutputMode() == OUT_LM
		|| InputFlags::Instance()->GetOutputMode() == OUT_LMANDPROMPTS )
	{
		// make sure the LM file is clean
		std::ofstream baggerCode( theOutLMname.c_str() );
		baggerCode << " " << endl;
		baggerCode.close();
	}

	std::string fileName = "";
	// Loop over all files
	bool appendToFile = false;
	while ( theListFile >> fileName ) 
	{
		cout << "New file: " << fileName << endl;
		if (InputFlags::Instance()->GetInputMode() == IN_ROOT)
		{
			theDataProcess->LoadTTree( fileName.c_str() );
		}
		bool doNext(1); // Looping over sections of input ROOT FILE or input LM FILE
		// int maxpersection(100000);
		int maxpersection(10000);
		//	int maxpersection(1000);
		int idxmin(0), idxmax(maxpersection);
		while (doNext)
		{
			if (InputFlags::Instance()->GetInputMode() == IN_ROOT)
			{
                int before = VIP_TIMER::GetTime();
				doNext = theDataProcess->ProcessTree(idxmin, idxmax);
                int after = VIP_TIMER::GetTime();
                cout << "DoProcess::DoDataLoop(), theDataProcess->ProcessTree, TIME: " 
                     << after-before << endl;

				//	cout << "done " << idxmin << " : " << idxmax << endl;
				idxmin = idxmax;
				idxmax += maxpersection;
			}
			else if (InputFlags::Instance()->GetInputMode() == IN_LM)
			{
				// TODO loop over all events in section ... (to speed the thing up)
				// Alternative: SPLIT UP LM files!!!!
				doNext = theDataProcess->LoadLM( fileName.c_str(), idxmin, idxmax );
				idxmin = idxmax;
				idxmax += maxpersection;
			}

			if (   InputFlags::Instance()->GetOutputMode() == OUT_LM
				|| InputFlags::Instance()->GetOutputMode() == OUT_LMANDPROMPTS)
			{
				theDataProcess->SaveLM( theOutLMname, appendToFile );			
                // if first: new file; else append to file
			}

			if ( InputFlags::Instance()->GetOutputMode() != OUT_LM )
            {
                // int before = VIP_TIMER::GetTime();
				theDataProcess->ProcessLM( theOutputsDirName );
				// <-- Calls XXX_CoincidenceFinder::FindCoincidence which cleans the LMTimeMap
				// Also calls SaveCoincidencesAndClear to write PROMPTS to disk
                
                // int after = VIP_TIMER::GetTime();
                // cout << "DoProcess::DoDataLoop(), theDataProcess->ProcessLM, TIME: " 
                //         << after-before << endl;
            }
			else
				theDataProcess->ClearLMTimeMap();

			appendToFile = true;

			//	cout << "in DoProcess loop, theLMTimeMap size: " << theDataProcess->theLMTimeMap.size() << endl;
		}
	} // end of loop over all files

	if (InputFlags::Instance()->DoClassification())
	{
		//    theDataProcess->SaveEventClassification( theOutEvName );
		theDataProcess->PrintEventClassification();
	}		
	cout << "DoProcess before flush last rech hits, theLMTimeMap size: " 
         << theDataProcess->theLMTimeMap.size() << endl;
	if (InputFlags::Instance()->GetInputMode() == IN_ROOT)
	{
		theDataProcess->FlushLastRecHits();				// Put last hits from hit map into LM map (if pass trigger threshold)
	}

	cout << "DoProcess after flush last rech hits, theLMTimeMap size: " 
         << theDataProcess->theLMTimeMap.size() << endl;
	
    if (   InputFlags::Instance()->GetOutputMode() == OUT_LM 
        || InputFlags::Instance()->GetOutputMode() == OUT_LMANDPROMPTS)
	{
		theDataProcess->SaveLM( theOutLMname, appendToFile );
	}

	if ( InputFlags::Instance()->GetOutputMode() != OUT_LM )
	{
		theDataProcess->ProcessLM( theOutputsDirName );
			// <-- Calls XXX_CoincidenceFinder::FindCoincidence which cleans the LMTimeMap
			// Also calls SaveCoincidencesAndClear to write PROMPTS to disk		
		if (InputFlags::Instance()->DoClassification())
		{
			theDataProcess->PrintCoincClassification();
		}
	}
	else
		theDataProcess->ClearLMTimeMap();		
	
	if (theGlobalHistograms)
	{
		theGlobalHistograms->SaveHistos( theOutHistoName.c_str() );
	}
}

// ================================================================================================

void
DoProcess::CreateGeneralObjects()
{
    cout << "DoProcess::CreateGeneralObjects" << endl;
	// the Data Process
	theDataProcess = new ProcessDataClass( m_scanType, theDetConfFilePath );
        // , thePixPosFilePath, theChShRadFilePath );

	theDataProcess->SetHistograms( theGlobalHistograms );
	theDataProcess->theCoincidenceFinder = CreateCoincidenceFinder();

	// Classification is not always possible
	if (InputFlags::Instance()->DoClassification())
	{
        cout << "DoProcess::CreateGeneralObjects Creating Ev Classification" << endl;
		theDataProcess->theEvClassification = CreateEvClassification();
	}

	theDataProcess->DistributeGlobalPointers();
}

// =================================================================================================

int 
DoProcess::ProcessInputs(int argc, char *argv[])
{
	InputFlags* flags = InputFlags::Instance();
	// TODO: this is all rather general for CC, PEM and PET, so put in DoProcess.cc
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
		/*
		else if(par_name == "-PixPos")
		{
			if(argc>par_i+1) thePixPosFilePath=argv[++par_i];
			else
			{
				cout<<"ERROR in "<<__func__<<endl;
				cout<<"missing par value for -PixPos"<<endl;
				return 1;
			}
		}
		else if(par_name == "-ChShRad")
		{
			if(argc>par_i+1) theChShRadFilePath=argv[++par_i];
			else
			{
				cout<<"ERROR in "<<__func__<<endl;
				cout<<"missing par value for -ChShRad"<<endl;
				return 1;
			}
		}
		*/

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

		else if(par_name == "-noComptonRecovery")
		{
			flags->doComptonRecovery = false;
		}
	
		else if (par_name == "-doClassification")
		{
			flags->doClassification = true;
		}
		else if (par_name == "-doHistograms")
		{
			flags->doHistograms = true;
		}

		else if (par_name == "-input")
		{
			if ( (par_i+1) >= argc)
			{
				cout << "Wrong number of arguments. Usage: " << endl;
				cout << "PET_DoProcess -input <option>" << endl;
				exit(1);
			}
			par_i++;
			par_name = argv[par_i];
			if (par_name == "ROOT")
				flags->theInputMode = IN_ROOT;
			else if (par_name == "LM")
				flags->theInputMode = IN_LM;
			else
			{
				cout << "Wrong argument: " << argv[par_i] << ". Usage: " << endl;
				cout << programName << " -input ROOT (default)" << endl;
				cout << programName << " -input LM" << endl;
				exit(1);
			}
		}
		else if (par_name == "-output")
		{
			if ( (par_i+1) >= argc)
			{
				cout << "Wrong number of arguments. Usage: " << endl;
				cout << programName << " -output <option>" << endl;
				exit(1);
			}
			par_i++;
			par_name = argv[par_i];
			if (par_name == "LM" )
				flags->theOutputMode = OUT_LM;
			else if (par_name == "PROMPTS")
				flags->theOutputMode = OUT_PROMPTS;
			else if (par_name == "LMANDPROMPTS")
				flags->theOutputMode = OUT_LMANDPROMPTS;
			else
			{
				cout << "Wrong argument: " << argv[par_i] << ". Usage: " << endl;
				cout << programName << " -output LM" << endl;
				cout << programName << " -output PROMPTS" << endl;
				cout << programName << " -output LMANDPROMPTS (default)" << endl;
				exit(1);

			}
		}
		
		else if (par_name == "-coincVerbosity")
        {
			if ( (par_i+1) >= argc)
			{
				cout << "Wrong number of arguments. Usage: " << endl;
				cout << programName << " -coincVerbosity <option>" << endl;
				exit(1);
			}
            int coincVerbosity = atof( argv[++par_i] );
            flags->coincidenceVerbosity = coincVerbosity;
        }

		else if (par_name == "-realHitPosOutput")		// To get PROMPTS file with real positions
		{
			flags->doRealHitPosOutput = true;
		}
		else if (par_name == "-writeAddEventID")	// To get eventID1 and eventID2 for each prompt
													// (with this together with DataTTree extra info, 
													//				we can get real src-pos for each prompt)
		{
			flags->doWriteAddEventID = true;
		}
		else if (par_name == "-writeAddSrcPosition")	// To get src positions for each prompt (in case they are identical)
		{
			flags->doWriteAddSrcPosition = true;
		}
        else if (par_name == "-writeAddClassificationFlag")
        {
            flags->doWriteAddClassificationFlag = true;
        }

		else if (par_name == "-sumAllStepsPerEvent")   // all steps of one event are added up
        {
            flags->sumAllStepsPerEvent = true;

        }

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
			//   cout << programName << " -PixPos <value>" << endl;
			//   cout << programName << " -ChShRad <value>" << endl;
			//   cout << programName << " -GBthld <value>" << endl;
			cout << endl;

            cout << programName << " -newEResFit (= default, Gerard's parametrisation)" << endl;
            cout << programName << " -zeroERes (= 0 energy resolution)" << endl;
            cout << programName << " -userERes <value> (= give the energy resolution in % FWHM)" << endl;
			cout << "    e.g.: -userERes 0.15" << endl;
			cout << "WARNING! Energy resolution only works on ROOT input!!! (in LM files, in principle it's already done!!!" << endl;
            
            cout << endl;
			cout << programName << " -GmSDT" << endl;

			cout << programName << " -noComptonRecovery" << endl;
			cout << programName << " -doClassification" << endl;

            cout << programName << " -sumAllStepsPerEvent" << endl;

			cout << endl;
			cout << programName << " -input <option>" << endl;
			cout << "	<option> can be: " << endl;
			cout << "		ROOT (default)" << endl;
			cout << "		LM" << endl;
			cout << programName << " -output <option>" << endl;
			cout << "	<option> can be: " << endl;
			cout << "		LM" << endl;
			cout << "		PROMPTS" << endl;
			cout << "		LMANDPROMPTS (default)" << endl;

            cout << programName << " -coincVerbosity <value>" << endl;
            cout << "(If > 1: a list of all events with coincidence finding verdict)" << endl;
            
			cout << programName << " -realHitPosOutput" << endl;
			cout << "			To get LM file with real positions (TODO: Prompts file too?)" << endl;
//			cout << "			To get PROMPTS file with real positions" << endl;

			cout << programName << " -writeAddEventID" << endl;
			cout << "			To get eventID1 and eventID2 for each prompt" << endl;
			cout << programName << " -writeAddSrcPosition" << endl;
			cout << "           To get src positions for each prompt" << endl;
            cout << programName << " -writeAddClassificationFlag" << endl;
            cout << "           Write out a classification number for each prompt" << endl;
            cout << "           0 = true, 1 = random, 2 = scattered" << endl;

			exit(1);
		}
	}

	if ( flags->doWriteAddEventID && flags->doWriteAddSrcPosition)
	{
		cout << "WARNING!!! CANNOT DO BOTH, doWriteAddSrcPosition overwrites doWriteAddEventID!!!!" << endl;
		flags->doWriteAddEventID = false;
	}

	if ( theDetConfFilePath.size() < 1 )
	{
		cout << "ERROR, you did not give a DetParameters conf file: " << theDetConfFilePath << endl;
		exit(1);
	}

	if ( flags->theInputMode == IN_LM )
	{
		cout << "Input Mode: LM input:" << endl;
		cout << "WARNING: No enery resolution is applied (was already applied when making LM)" << endl;
		cout << "WARNING: Cannot do event or coinc classification (we need GAMOS ROOT information for that)" << endl;

		if ( flags->doClassification )
		{
			cout << "WARNING: IGNORING doClassification (NOT possible with LM input)" << endl;
			flags->doClassification = false;
		}
		if ( flags->m_EresMode == InputFlags::ERES_GERARD || flags->m_EresMode == InputFlags::ERES_USER )
		{
			cout << "WARNING: Energy resolution flag ignored";
			cout << " (LM input already corrected for energy resolution)" << endl;
		}
	}

	return 0;
}







