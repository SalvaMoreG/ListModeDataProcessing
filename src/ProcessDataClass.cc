
#include "ProcessDataClass.hh"

#include "VIPglobalheaders.hh"
#include "globalheaders.hh"

#include "DoProcess.hh"
#include "InputFlags.hh"

#include "DetHitReconstruction.hh"

#include "CVIPSemiConductorEnergy.h"

using namespace std;

bool debug20181128(false);

ProcessDataClass::ProcessDataClass(  SCAN_TYPE in_scanType, const std::string& in_DetConfFilePath )
	: m_scanType( in_scanType )
	, theFlag(0)
	, theTree(NULL)
	, theRandomGenerator(NULL)
	, theHistograms(NULL)
	, theHitReconstruction(NULL)
	, m_scanner(NULL)
	, m_detectorFactory( NULL )
	, m_promptsAppend( false )
    , mySDTTree(NULL)
{
	InitializeGlobalVariables();
	Initialize( in_DetConfFilePath ); 
    // , in_PixPosFilePath, in_ChShRadFilePath );
}

// ====================================================

ProcessDataClass::~ProcessDataClass()
{
	delete theTree;
	delete theRandomGenerator;
	delete theHitReconstruction;

	delete m_scanner;
	delete m_detectorFactory;

    if (InputFlags::Instance()->DoGmSDTTree() && mySDTTree )
    {
        mySDTTree->Save();
    }

    delete mySDTTree;
}

// ====================================================

int ProcessDataClass::InitializeGlobalVariables()
{
	theEvClassification = 0;
	theCoincidenceFinder = 0;

	theHitTimeMap.clear();
	theLMTimeMap.clear();
	theEventClassificationMap.clear();
	
	return 0;
}

// ====================================================

void
ProcessDataClass::Initialize(const std::string& in_DetConfFilePath)
{
	theRandomGenerator = new TRandom3(time(NULL));

	if (InputFlags::Instance()->GetInputMode() == IN_ROOT)
	{
		theTree = new TreeClass();
	}

	theHitReconstruction = new DetHitReconstruction();

	// Create detectors (just so that the DetParameters file is read at start-up)
	m_detectorFactory = new DetectorFactory(in_DetConfFilePath);
	assert( m_detectorFactory );
	if (m_scanType == SCAN_TYPE_PET || m_scanType == SCAN_TYPE_PEM)
	{
		m_detectorFactory->CreateDetector( DETECTOR_TYPE_ABSORBER );
	}
	else if (m_scanType == SCAN_TYPE_CC || m_scanType == SCAN_TYPE_CCPET)
	{
		m_detectorFactory->CreateDetector( DETECTOR_TYPE_SCATTERER );
		m_detectorFactory->CreateDetector( DETECTOR_TYPE_ABSORBER );
	}

	// Create scanner
	m_scanner = new Scanner( m_scanType, in_DetConfFilePath, *m_detectorFactory );

	// Create SDTTree root file
    if (InputFlags::Instance()->DoGmSDTTree())
    {
        mySDTTree = new MySimpleGmSDTTree();
        std::string zucht("GmSDTTree_LM.root");
        mySDTTree->InitTree(zucht);
    }
}


// ====================================================

int 
ProcessDataClass::DistributeGlobalPointers()
{
	if (theHitReconstruction) 
	{
		theHitReconstruction->SetRandomGenerator(theRandomGenerator);
		theHitReconstruction->SetHistograms(theHistograms);
		theHitReconstruction->SetScanner(m_scanner);
	}
	if (theEvClassification) 
	{
		theEvClassification->SetHistograms(theHistograms);
		theEvClassification->SetScanner(m_scanner);
	}
	if (theCoincidenceFinder) 
	{
		theCoincidenceFinder->SetHistograms(theHistograms);
		theCoincidenceFinder->SetScanner(m_scanner);
	}

	return 0;
}

// ====================================================

int ProcessDataClass::LoadTTree(const string& RootFileName)
{
	assert( theTree != 0);
	return theTree->LoadTTree(RootFileName);
}

// ====================================================

int ProcessDataClass::ProcessTree(int minentry, int maxentry)
{
	Long64_t nentries = theTree->GetEntriesFast();
	// Loop over Events (EVENTS!!!!!)
	//
    cout << "ProcessDataClass::ProcessTree() nentries: " << nentries << endl;

	// for (Long64_t i=0; i<nentries; i++)
	// instead of looping over all entries, we divide the entries in sections 
	//		(avoids slowing down the code for memory reasons)
	if (maxentry > nentries)
		maxentry = nentries;

    cout << "ProcessDataClass::ProcessTree() minentry: " << minentry 
         << " maxentry: " << maxentry << endl;
         
	for (Long64_t i=minentry; i<maxentry; i++)
	{ 
		Long64_t ientry = theTree->GetEntry(i); //loading tree variables for current event
		if (ientry < 0) break; 	// if any problem in loading then stop looping
		if (i%100000==0) 		// control cout
		{ 
			cout<<"getting entry num "<<i<<endl;
		}

        // WITH GAMOS-5, we write out data per event (instead of per hit, as in GAMOS-3). This means,  
        // that we get a lot of steps and we have to add them together. Here in this code, we do 
        // this by using measuring time (which we need anyway to add different hits together). 
        // DANGER (NOTE): HENCE: if we set the measuring time to 0, each step will be turned
        // into a different hit!!! (This doesn't happen with GAMOS-3, where the data are already 
        // organized in "Hits").
        // For the moment, I always fixed this by setting the measuring time to at least 1 ns (1000 ps). 
        // This is ok for 511 gammas but for 250 MeV protons the time-differences of steps in an event
        // might be larger than 1 ns. So then we would have to set the measuring time larger.
        // A cleaner fix (and I think justifiable in physics) is to add all steps that belong 
        // to the same event, independent of measuring time (when they are in the same detector)
        // See: DetHitReconstruction::ProcessStep (using SumAllStepsPerEvent)
        // Measuring time only would come into play when hits from different events overlap in time. 
        // If measuring_time is zero, this would not happen, but steps from the same event still 
        // would get merged.

		assert(theTree->Event_InitialTime >= 0); 
			// by default Event_IntialTime leaf = -123456. This variable is mandatory to continue.
		unsigned long long int current_EventTime = 
                (unsigned long long int) (theTree->Event_InitialTime*1000.);  
				// event time conversion from ns to ps to increase precision

		// Here, the already reconstructed hits from theHitTimeMap are added to the List Mode (LM) map 
		//			and removed from the hit map:

		AddHitToLM( current_EventTime ); // If theHitTimeMap is empty, nothing is done yet

		int Nsteps = theTree->GetNumOfSteps();
		if (theEvClassification) 
			theEvClassification->InitializeCounters(); //initialize counters for single event

		// Loop through steps
		for (int st=0; st<Nsteps; st++)	// All steps in this event (for various hits)
		{ 
            // Note: DetHitReconstruction is a subclass of HitReconstructionClass
            // Most of the HitReconstructionClass code is defined in DetHitReconstruction
			if (theHitReconstruction) 
			{
				// theHitTimeMap gets filled here: DetHitReconstruction::ProcessStep
				theFlag = theHitReconstruction->ProcessStep(theTree, st, theHitTimeMap);  
					//hits are reconstructed with current event's steps
					// Here:
					// 	 Hits are merged (or ignored) because of measuring time (and dead-time)
					// 	 Also: HitTimeMap gets filled: in_HitTimeMap[centralhit.T] = centralhit;
				CheckFlag("theHitReconstruction->ProcessStep in ProcessDataClass::ProcessTree");
                    // RUBBISH COMPLICATED CODE: 
                    //  int flag = aTree->CheckNeededVariables(in_stepIndex);
                    //  int TreeClass::CheckNeededVariables(const string& mode, int StepIndex)
			}
			if (theEvClassification)
			{
				theFlag = theEvClassification->UpdateCounters(theTree, st);
					//steps are examined one by one to update the event counters
				CheckFlag("theEvClassification->UpdateCounters in ProcessDataClass::ProcessTree");
			}
		}

		if (theEvClassification)
		{
			// 	EvClassificationClass* 				theEvClassification;
			theEvClassification->ClassifyEvent(theTree,theEventClassificationMap); 
                //the current event is classified
		}
	}
	
    if (    InputFlags::Instance()->GetEresMode() == InputFlags::ERES_GERARD
         || InputFlags::Instance()->GetEresMode() == InputFlags::ERES_USER )
	{
		// For the moment, do energy smearing here 
		// (TODO: this is not 100% correct, 
        //      but the rest of the code is too confusing to know where to do it...)
		DoEnergySmearing();		// ENERGY SMEARING!!!
	}
	
	if (maxentry >= nentries)
		return 0;
	else
		return 1;
}

// ====================================================

int ProcessDataClass::PrintEventClassification() const
{
	if (theEvClassification) theEvClassification->PrintClassification(); 
		//the summary of the event classification is printed out
	return 0;
}

// ====================================================

int ProcessDataClass::SaveEventClassification(const string& filename)
{
	ofstream outF(filename.c_str());
	if (!(outF.is_open()))
	{
		cout << "ERROR ProcessDataClass::SaveEventClassification()" << endl;
		cout << "file <" << filename << "> not open" << endl;
		return -2;
	}
	for (map<int,EventClass>::iterator it=theEventClassificationMap.begin(); 
        it!=theEventClassificationMap.end(); it++)
	{
		outF << it->first << " " << it->second.classification << endl;
	}
	return 0;
}

// ====================================================

int ProcessDataClass::LoadLM(const string& filename, int minline, int maxline)
{
	theLMTimeMap.clear();

	if( !m_inLMfile.is_open() )
	{
		m_inLMfile.open( filename.c_str() );
		if( !(m_inLMfile.is_open()))
		{
			cout << "ERROR ProcessDataClass::LoadLM()" << endl;
			cout << "file <" << filename << "> not open" << endl;
			return 0;
		}
		m_inLMlinecount = 0;
	}

	// We need "detectorType", which is important for Compton camera data
	// 2014-02-24: more or less fixed, see below...
	HitClass Hit;
	double ttime = 0;
	int ic = 0;

	// 2015-02-24 not sure how we are going to use this in the future, but for now: 
	//				the second field gives the volume name of the hit-location
	// e.g.:          0      CdTePixel_1       1810000 0.288291 -116.872 -184.74 131.06
	// 		while ( inF >> Hit.ev >> Hit.primary_process >> ttime >> Hit.E >> Hit.X >> Hit.Y >> Hit.Z )

	while (m_inLMlinecount < maxline && !m_inLMfile.eof())
	{
		while ( m_inLMfile >> Hit.ev >> Hit.volumeName >> ttime >> Hit.E >> Hit.X >> Hit.Y >> Hit.Z )
		{
            m_inLMfile.ignore(1024, '\n');

			// Get the detector type:
			Hit.detectorType = m_scanner->GetDetectorTypeFromVolume( Hit.volumeName );

			// time stuff
			Hit.T = (unsigned long long int) ttime;
			while(theLMTimeMap.find(Hit.T) != theLMTimeMap.end())
			{
				Hit.T += 1;
			}

			// Add hit to the map
			theLMTimeMap[Hit.T] = Hit;
			ic++;

			m_inLMlinecount++;
		}
	}
	cout << "LM data loaded: " << theLMTimeMap.size() << " entries" << endl;
    
    if (m_inLMfile.eof())
    {
        m_inLMfile.close();
    }
    
	return 0;
}

// ====================================================

int ProcessDataClass::SaveLM(const string& filename, bool update)
{
	ofstream outF;
	ofstream outF_realPos;
	if (update)
	{
		outF.open( filename.c_str(),ios::app);
		if (InputFlags::Instance()->doRealHitPosOutput)
		{
			string fn_realPos( filename );
			fn_realPos += "_realPos";
			outF_realPos.open( fn_realPos.c_str(), ios::app);
		}
	}
	else
	{
		outF.open( filename.c_str(),ios::out);
		if (InputFlags::Instance()->doRealHitPosOutput)
		{
			string fn_realPos( filename );
			fn_realPos += "_realPos";
			outF_realPos.open( fn_realPos.c_str(), ios::out);
		}
	}
	if(!(outF.is_open()))
	{
		cout << "ERROR ProcessDataClass::SaveLM()" << endl;
		cout << "file <" << filename << "> not open" << endl;
		return -1;
	}
	outF << std::setprecision (15);
	outF_realPos << std::setprecision (15);

	// TODO: 
	// What's missing is "detectorType", which is important for Compton camera code
	// Units: 
	// Timestamp = picoseconds (ps)
	// Energy = MeV
	// x, y, z = mm
	//
	for (map<unsigned long long int, HitClass>::iterator it=theLMTimeMap.begin();
            it!=theLMTimeMap.end(); it++)
	{
		outF << it->second.ev << " ";
		outF << it->second.volumeName << " ";

		outF << it->second.T << " ";
		outF << it->second.E << " ";
		outF << it->second.X << " ";
		outF << it->second.Y << " ";
		outF << it->second.Z << " ";

        if ( InputFlags::Instance()->doWriteAddClassificationFlag 
                            && InputFlags::Instance()->doClassification)
        {
            int classification(0);
            map<int,EventClass>::const_iterator event_it = 
                    theEventClassificationMap.find(it->second.ev);
            if (    event_it != theEventClassificationMap.end()
                 && event_it->second.classification == PET_EvClassification::scat )
            {
                classification = CLASSIFICATION_SCATTER; // 2;
            }
            outF << "  " << classification ;
        }
        outF << endl;


 		if (InputFlags::Instance()->DoRealHitPosOutput())
 		{
 			outF_realPos << it->second.ev << " ";
 			outF_realPos << it->second.volumeName << " ";

 			outF_realPos << it->second.T << " ";
 			outF_realPos << it->second.E << " ";
 			outF_realPos << it->second.realX << " ";
 			outF_realPos << it->second.realY << " ";
 			outF_realPos << it->second.realZ << endl;
 		}
	}

	if (InputFlags::Instance()->DoGmSDTTree())
    {
        SaveSDTTree();
    }


	return 0;
}

// ====================================================

void
ProcessDataClass::SaveSDTTree()
{
    std::map<unsigned long long int, unsigned int> totHitsInEventMap;
    unsigned long long int prevEv = 0;
    int count(0);
    for (map<unsigned long long int, HitClass>::iterator it=theLMTimeMap.begin(); 
            it!=theLMTimeMap.end(); it++)
    {
        if (it->second.ev > prevEv )
        {
            totHitsInEventMap[prevEv] = count;
            count = 0;
            prevEv = it->second.ev;
        }
        count++;
    }
    totHitsInEventMap[prevEv] = count;

    prevEv = 0;
    int hitIndex = 0;
    for (map<unsigned long long int, HitClass>::iterator it=theLMTimeMap.begin(); 
            it!=theLMTimeMap.end(); it++)
    {
        if ( it->second.ev > prevEv )
        {
            prevEv = it->second.ev;
            hitIndex = 0;
        }
        const HitClass& hit = it->second;
        count = totHitsInEventMap[it->second.ev];
        mySDTTree->Fill( hit, hitIndex, count );
        hitIndex++;
    }
}

// ====================================================

int 
ProcessDataClass::ProcessLM( const string& in_outdirName )
{
    cout << "ProcessDataClass::ProcessLM, Calling theCoincidenceFinder->FindCoincidence" 
       << " theHitTimeMap.size: " << theHitTimeMap.size() 
       << " theLMTimeMap.size: " << theLMTimeMap.size() << endl;
    //
	theCoincidenceFinder->FindCoincidence( theLMTimeMap );

	if (InputFlags::Instance()->DoClassification())
	{
		theCoincidenceFinder->Classify(theEventClassificationMap);		
			// the only reason this call is made here, is because of the bloody EventClassificationMap
			// TODO: better would, from theCoincidenceFinder, to do: 
			// 	m_coincidenceClassification->Classify( m_processData->GetEventClassificationMap );
	}

	theCoincidenceFinder->SaveCoincidencesAndClear( in_outdirName );

	ClearLMTimeMap();

	return 0;
}

// ====================================================

void
ProcessDataClass::ClearLMTimeMap()
{
	theLMTimeMap.clear();
}

// ====================================================

int ProcessDataClass::PrintCoincClassification() const
{
	if (theCoincidenceFinder) 
		theCoincidenceFinder->PrintClassification();

	return 0;
}

// ====================================================

int ProcessDataClass::AddHitToLM( unsigned long long int anEventTime )
{
	if (!m_scanner) return -1;

	// Loop over the HitTimeMap to drop all reconstructed hits 
	map<unsigned long long int,HitClass>::iterator iterator_lastok = theHitTimeMap.begin();
	bool deletehits(false);
    
	for ( map<unsigned long long int,HitClass>::iterator it = theHitTimeMap.begin(); 
          it != theHitTimeMap.end(); it++ ) 
	{ 
		const Detector* myDetector = m_scanner->GetDetector( it->second.detectorType );
        
		if (!myDetector) return -1;

		// Check if the hit in theHitTimeMap can't be merged with steps belonging to current event:
        // If the hit is before current Event.T - (meas.T + dead.T): 
        //   - hit is complete
        //   If hit.E > threshold
        //      - add it to LMTimeMap
        //   Remove it from HitTimeMap
        //
		// With "Exceed...()" we select hits from before eventTime - (meas-time + dead-time)
        //
		if ( myDetector->ExceedMaxOverlapTime(anEventTime, it->second) ) 
		{ 
            // loopHit = it->second
            // So: anEventTime (time of latest hit) > loopHit.T + (meas_T+dead_T));
            //  This means the newest hit "does not belong" to the loopHit
            //  So: loopHit can be deleted from HitTimeMap and added to LMTimeMap.
			//    (only if above the trigger threshold ???)
            
			// if ( myDetector->PassTriggerThreshold(it->second) )  
                    // 2020-09-16: I think this if-statement is wrong here!
			{ 
                if ( myDetector->PassTriggerThreshold(it->second) ) // 2020-09-16: it should be here
                    theLMTimeMap[it->first] = it->second;
                
                // 2022-09-16, 
                // es decir: hits before evt.Time - (meas.T + dead.T) 
                //      should be removed from theHitTimeMap anyway!
				iterator_lastok = it;
				deletehits = true;
			}
		}
		// If the hit is within EventTime - (measuring-time + dead-time):
		// 	 We delete all hits up until here from the HitTimeMap, 
		//        since they have already been added to the LMlist
		//   The hit "it", is not erased yet, because it might have to be merged later
		//				in DetHitReconstruction::ProcessStep before adding to LMlist
		else 
		{
			theHitTimeMap.erase(theHitTimeMap.begin(),it);	// erase does not include "it" itself
			deletehits = false;
			break;	// and then, get the hell out of this loop....
		}
	} //  for map<unsigned long long int,Hit>::iterator it = HitTimeMap.begin(); ...)

	if (deletehits)
	{
		theHitTimeMap.erase(theHitTimeMap.begin(), iterator_lastok);
		theHitTimeMap.erase(iterator_lastok);		// we want to erase iterator_lastok too
	}

	return 0;
}

// ====================================================

int ProcessDataClass::FlushLastRecHits()
{
	if (!m_scanner) return -1;

	for (map<unsigned long long int,HitClass>::iterator it = theHitTimeMap.begin();it != theHitTimeMap.end();it++) 
	{ 
		const Detector* myDetector = m_scanner->GetDetector( it->second.detectorType );
		if(!myDetector) return -1;

		if (myDetector->PassTriggerThreshold(it->second)) 
		{ //adding to LMTimeMap only if above the trigger threshold
			theLMTimeMap[it->first]=it->second;
		}
	} 
	theHitTimeMap.clear();

    if (    InputFlags::Instance()->GetEresMode() == InputFlags::ERES_GERARD
         || InputFlags::Instance()->GetEresMode() == InputFlags::ERES_USER )
	{
		// For the moment, do energy smearing here 
		// (TODO: this is not 100% correct, but the rest of the code is too confusing still to know where to do it...)
		DoEnergySmearing();		// ENERGY SMEARING!!!
	}

	return 0;
}

// ====================================================

int 
ProcessDataClass::CheckFlag(const string& function_name) const
{
	if (theFlag < 0) {
		cout << "ERROR in " << function_name << endl;
		cout << "error code = " << theFlag << endl;
		cout << "aborting session" << endl;
	}
	assert(theFlag>=0);
	return 0;
}

// ====================================================

// This function calls SemiConductor functions "GetSmearedEnergy" (see "CVIPSemiConductorEnergy.h" in VIPCommonLib)
//  and implements Gerard's energy resolution measurement parameters...
//
void
ProcessDataClass::DoEnergySmearing()
{
	VIPCdTe semiconductorCdTe;
	VIPSi semiconductorSi;

	for (map<unsigned long long int, HitClass>::iterator it=theLMTimeMap.begin(); it!=theLMTimeMap.end(); it++)
	{
        if ( InputFlags::Instance()->GetEresMode() == InputFlags::ERES_GERARD )
        {
            // TODO: check that current hit is in CdTe....
            bool isCdTe = (it->second.detectorType == DETECTOR_TYPE_ABSORBER) ? true : false;
            if (isCdTe)
            {
                it->second.E = semiconductorCdTe.GetSmearedEnergy( it->second.E );
            }
            else
            {
                it->second.E = semiconductorSi.GetSmearedEnergy( it->second.E );
            }
        }
        else if ( InputFlags::Instance()->GetEresMode() == InputFlags::ERES_USER )
        {
            double factor = theRandomGenerator->Gaus(1.0, InputFlags::Instance()->GetUserEres());
            it->second.E = it->second.E * factor;
        }
	}
}















