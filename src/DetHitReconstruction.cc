
#include "DetHitReconstruction.hh"
#include "InputFlags.hh"

#include "VIPTimer.hh"

using namespace std;

// ================================================================

int 
DetHitReconstruction::ProcessStep(TreeClass* aTree, int in_stepIndex
	, map<unsigned long long int, HitClass>& in_HitTimeMap )
{
	// check appropriate vectors are filled before proceeding
	int flag = aTree->CheckNeededVariables(in_stepIndex);
	if(flag < 0)
	{
		return flag;
	}

	// Get Detector
	const Detector* myDetector = m_scanner ? 
				m_scanner->GetDetectorFromVolume( aTree->Step_InitialLogicalVolume->at(in_stepIndex) ) : NULL;
	if ( !myDetector )
	{
		return 0;		
        // this means that the hit was not in volume associated with our detector ("PET", "CC", "CCPET")
	}

	HitClass centralhit;
	CreateNewHit( aTree, in_stepIndex, *myDetector, centralhit ) ;  
        // Step is here treated as a separate hit...
        // It is called "centralhit" because the original code did some complicated stuff 
        //  (taking 3x3 cells together around the "central hit")
        // I deleted this all that stuff, but I kept the name. TODO: change name

	// Condition 1: energy deposited in CdTe
	if ( centralhit.E > 0 ) 
    { 
		/// *** reconstruction of hits *** ///
		// NeighborList.size = 1 (unless there is charge sharing involved which is never)
		//
		// 2018-11-29: plan to simplify this code (all 3x3 neighbour nonsense can go away, I think)
		// 2018-12-03: I finally simplified the code and threw all "neighbour 3x3 pixel array code" away
		//

		bool merged = false;
		bool rejected = false;

        // MERGE ALL HITS IN SAME DETECTOR DURING THIS EVENT!!!!
		// Loop over in_HitTimeMap to look for steps in same detector
		// When an iterator "it" (which is already in the Map) is found that "connects" with centralHit,
		// 		then 1) "it" is merging with "centralHit"
		//			 2) "centralHit" is marked as merged (and so, will not be added anymore to in_HitTimeMap
        typedef map<unsigned long long int,HitClass>::iterator HitClassIterator;
		for (HitClassIterator it = in_HitTimeMap.begin();it != in_HitTimeMap.end();it++)
		{
			// If step in already seen detector unit
			// Note (2019-10-10: It's important to include volumeName too, because sometimes 
            // (for some weird reason), the UnitId might identical for different detectors 
            //  (probably only when they are really close to each other)
            //
			// 2019-10-11: WARNING: Theres is a "bug" in GAMOS.
            //  If you put "InitialLogicalVolume" twice in the GAMOS "in file",
			//          it ends up getting the wrong value in the ROOT file.
			//  With a correct "in file", the following if-statement works fine

			if (it->second.UnitID == centralhit.UnitID
                && it->second.volumeName == centralhit.volumeName)
			{
				// Here we compare all hits in "the HitTimeMap" with the current "in_stepIndex"
				long long int Delta_T = centralhit.T_trigger - it->second.T_trigger;
				if (Delta_T < 0) Delta_T *= -1;

                if (InputFlags::Instance()->SumAllStepsPerEvent() && centralhit.ev == it->second.ev)
                //  if (centralhit.ev == it->second.ev)
                    // 2019-05-03: If it's the same event (and same detector) it's the same hit
                {
                    //calculating the position (depth) along E-field as the energy weighted centroid
                    MergeStep(it, centralhit);
                        // "it" is changed, centralhit is merged with "it"
                    merged = true;
                    break;      // Get out of the loop. New "centralhit" is merged with already existing hit
                }
                // Here is the measuring time code
				else if (Delta_T < myDetector->meas_T) // Merge hits within measuring time
				{
					//calculating the position (depth) along E-field as the energy weighted centroid
					MergeStep(it, centralhit);
						// "it" is changed, centralhit is merged with "it"
					merged = true;
					break;		// Get out of the loop. New "centralhit" is merged with already existing hit
				}
				// Here is the dead-time code
				// else if (Delta_T < myDetector->dead_T) // BUG, 2015 05 04
				else if (Delta_T < myDetector->meas_T + myDetector->dead_T)
					// Reject steps within dead time of the same detector unit
				{
					rejected = true;
					break;		// Get out of the loop. New "centralhit" is inside dead time
				}
			}
		} // for(map<unsigned long long int,Hit>::iterator it = in_HitTimeMap.begin();it != in_HitTimeMap.end();it++)

		// Electric noise smearing...
		if ( InputFlags::Instance()->GetEresMode() == InputFlags::ERES_OLD )    // old way of doing it
		{
			centralhit.E = ElectronicNoiseEnergySmearing(centralhit.E, myDetector->electronic_sigma_E);
				// MK: What happens here is that the electronic noise is only added once per merged hit
				// (Note that this "centralHit" only gets added to HitMap when it is not merged.
				// If it's merged, this operation is useless.
		}
		//
		// TODO (2): shouldn't we add some kind of trapping energy resolution (actually the largest component) too???!!!
		//      (3): Instead, for the moment, I am using the VIP Common lib "fit"
		//

		// Here, we only look at centralHit when it is NOT rejected and NOT merged...
		if (!rejected && !merged)
		{
			while(in_HitTimeMap.find(centralhit.T) != in_HitTimeMap.end())
			{
				centralhit.T++;
			}
			centralhit.T_trigger = centralhit.T;
			in_HitTimeMap[centralhit.T] = centralhit;
		}
	}  	// if( centralHit.E>0 )

	return 0;
}

// ====================================================================

// HERE IS WHERE HitClass is getting filled!!!!!
//
void
DetHitReconstruction::CreateNewHit(const TreeClass* aTree, int in_stepIndex
		, const Detector& in_myDetector, HitClass& io_hit ) const
{
	// Local variables with needed step variables
	double Step_E = aTree->Step_AccumulatedEnergyDeposited->at(in_stepIndex);
	if ( InputFlags::Instance()->GetEresMode() == InputFlags::ERES_OLD )    // old way of doing it
	{
		Step_E = StatisticalEnergySmearing( Step_E, in_myDetector.fano_F, in_myDetector.ioni_E); 
				// Applying the poisson smearing weighted by the fano factor
	}

	io_hit.E = Step_E;

	io_hit.X = aTree->Step_InitialTouchablePosX->at(in_stepIndex);
	io_hit.Y = aTree->Step_InitialTouchablePosY->at(in_stepIndex);
	io_hit.Z = aTree->Step_InitialTouchablePosZ->at(in_stepIndex);


	// io_hit.depth = Step_depth;
	// io_hit.thickness = in_myDetector.pixel_thick;

	//	io_hit.locX = aTree->Step_InitialLocalPosX->at(in_stepIndex);
	//	io_hit.locY = aTree->Step_InitialLocalPosY->at(in_stepIndex);
	//	io_hit.locZ = aTree->Step_InitialLocalPosZ->at(in_stepIndex);

    /* 
	// io_hit.Phi = Step_Phi;
	// io_hit.Psi = Step_Psi;
	// io_hit.Theta = Step_Theta;
    */

	// times in ps
	// 2020-06-24 MACHIEL: 
	// io_hit.T = (long long unsigned int) aTree->Step_InitialTime->at(in_stepIndex)*1000;
	// io_hit.T_trigger = (long long unsigned int) aTree->Step_InitialTime->at(in_stepIndex)*1000;
	io_hit.T = (long long unsigned int) (aTree->Step_InitialTime->at(in_stepIndex)*1000); 	
			//step time conversion from ns to ps
	io_hit.T_trigger = (long long unsigned int) (aTree->Step_InitialTime->at(in_stepIndex)*1000); 
			//step time conversion from ns to ps

	io_hit.ev = aTree->Event_EventID;
	io_hit.UnitID = aTree->Step_InitialUnitID->at(in_stepIndex);

	// type of detector!!!!		
	// volumeName is important because different detectors might have the same UnitID (yeah, I know, it's stupid)!!!
	io_hit.detectorType = in_myDetector.GetType();
	io_hit.volumeName = aTree->Step_InitialLogicalVolume->at(in_stepIndex);

//	if (aTree->Event_EventID == 48874)
//	{
//		cout << "HIT: " << io_hit.X << " " << io_hit.Y << " " << io_hit.Z 
//			 << " io_hit.UnitID: " << io_hit.UnitID
//			 << " io_hit.volumeName:  " << io_hit.volumeName
//		     << endl;
//	
//	}
//	else if (aTree->Event_EventID > 48874)
//	{
//		exit(0);	
//	}

    //if (io_hit.ev == 2417)
    /*
    if (io_hit.ev == 32)
    {
        cout << " Tree: eventID: " << aTree->Event_EventID << " " 
        << "hit XYZ: " << io_hit.X << " " << io_hit.Y << " " << io_hit.Z << " " 
        << "detType: " << io_hit.detectorType << " volname: " << io_hit.volumeName << " " 
        << "UnitID: " << io_hit.UnitID << " " 
        << "E: " << io_hit.E << " " 
        << endl;
    } 
    */
    
	// source positions (if set)
	if ( InputFlags::Instance()->doWriteAddSrcPosition )
	{
		io_hit.sourceX = aTree->Event_InitialPosX;
		io_hit.sourceY = aTree->Event_InitialPosY;
		io_hit.sourceZ = aTree->Event_InitialPosZ;
	}

	// 2018-11-29
	// WELL MAYBE (because this hit is only added to HitTimeMap if it's the first step in this detector
	//
	// So, with trial/error, I figured out that "InitialPos" does not coincide with what SDTTree gives for the real position
	/*
	io_hit.realX = aTree->Step_InitialPosX->at(in_stepIndex);
	io_hit.realY = aTree->Step_InitialPosY->at(in_stepIndex);
	io_hit.realZ = aTree->Step_InitialPosZ->at(in_stepIndex);
	*/
	// So, next, I tried FinalPos, and yeah!!!!!!!!!! It gives the same numbers as SDTTree (in GAMOS-3)
	io_hit.realX = aTree->Step_FinalPosX->at(in_stepIndex);
	io_hit.realY = aTree->Step_FinalPosY->at(in_stepIndex);
	io_hit.realZ = aTree->Step_FinalPosZ->at(in_stepIndex);
		// Note: from the hits, created by CreateNewHit(...), only the first step into a voxel is used 
        //  (the other steps are merged)
		// So, the realX,Y,Z of this hit will point to the step with the smallest in_stepIndex 
        //      for this pixel
		// As it should!!! So, yes, this works...

}







