#include "PET_EvClassification.hh"

using namespace std;

PET_EvClassification::~PET_EvClassification()
{
}

PET_EvClassification::PET_EvClassification()
{
//	_mindiff = 0.001;
	InitializeCounters();
	InitializeGlobalCounters();
}

int PET_EvClassification::InitializeGlobalCounters()
{
	NE_tot = 0;
	NE_scat = 0;
	NE_scat_PEPE = 0;
	NE_scat_PECOPE = 0;
	NE_scat_COPECOPE = 0;
	NE_scat_PEinco = 0;
	NE_scat_incoinco = 0;
	NE_PEPE = 0;
	NE_PECOPE = 0;
	NE_COPECOPE = 0;
	NE_PEinco = 0;
	NE_incoinco = 0;
	return 0;
}

int PET_EvClassification::InitializeCounters()
{
	m_eventClassification = UNDEFINED; //initializing the classification variable
	ID1=-1;
	ID2=-1;
	scat1=false;
	scat2=false; //boolean to determine a scattered gamma1 or gamma2
	Rayl1=false;
	Rayl2=false; //boolean to determine a Rayleigh gamma1 or gamma2
	NPE1=0;
	NPE2=0; //counters for PhotoElectric interactions
	NCO1=0;
	NCO2=0; //counters for Compton interactions
	Vx = -999;
	Vy = -999;
	Vz = -999;
	Annihil_Vx = -999;
	Annihil_Vy = -999;
	Annihil_Vz = -999;
	
	return 0;
}

int PET_EvClassification::PrintClassification() const
{
	cout<<"***********************************************************"<<endl;
	cout<<"*** PET_EvClassification::PrintClassification ***"<<endl;
	cout<<"***********************************************************"<<endl;
	cout<<"* NE_tot = "<<NE_tot<<endl;
	cout<<"* NE_scat = "<<NE_scat<<endl;
	cout<<"* NE_scat_PEPE = "<<NE_scat_PEPE<<endl;
	cout<<"* NE_scat_PECOPE = "<<NE_scat_PECOPE<<endl;
	cout<<"* NE_scat_COPECOPE = "<<NE_scat_COPECOPE<<endl;
	cout<<"* NE_scat_PEinco = "<<NE_scat_PEinco<<endl;
	cout<<"* NE_scat_incoinco = "<<NE_scat_incoinco<<endl;
	cout<<"* NE_PEPE = "<<NE_PEPE<<endl;
	cout<<"* NE_PECOPE = "<<NE_PECOPE<<endl;
	cout<<"* NE_COPECOPE = "<<NE_COPECOPE<<endl;
	cout<<"* NE_PEinco = "<<NE_PEinco<<endl;
	cout<<"* NE_incoinco = "<<NE_incoinco<<endl;
	cout<<"***********************************************************"<<endl;
	
	return 0;
}

// ===========================================================================

int 
PET_EvClassification::UpdateCounters(TreeClass* aTree, int StepIndex)
{
	if(! aTree->Step_InitialTotalEnergy) return -1;
	if(! aTree->Step_FinalTotalEnergy) return -2;
//		if(! aTree->Step_InitialDirX) return -3;
//		if(! aTree->Step_InitialDirY) return -4;
//		if(! aTree->Step_InitialDirZ) return -5;
//		if(! aTree->Step_FinalDirX) return -6;
//		if(! aTree->Step_FinalDirY) return -7;
//		if(! aTree->Step_FinalDirZ) return -8;
	if(! aTree->Step_ParentTrackID) return -9;
	if(! aTree->Step_CreatorProcess) return -10;
	if(! aTree->Step_Particle) return -11;
	if(! aTree->Step_TrackID) return -12;
	if(! aTree->Step_InitialProcess) return -15;
	if(! aTree->Step_FinalProcess) return -13;
	if(! aTree->Step_InitialLogicalVolume) return -14;
	if(! aTree->Step_FinalPosX) return -141;
	if(! aTree->Step_FinalPosY) return -142;
	if(! aTree->Step_FinalPosZ) return -143;
	if(! aTree->Step_InitialPosX) return -144;
	if(! aTree->Step_InitialPosY) return -145;
	if(! aTree->Step_InitialPosZ) return -146;
	
	if(aTree->Step_InitialTotalEnergy->size() <= StepIndex) return -15;
	if(aTree->Step_FinalTotalEnergy->size() <= StepIndex) return -16;
//		if(aTree->Step_InitialDirX->size() <= StepIndex) return -17;
//		if(aTree->Step_InitialDirY->size() <= StepIndex) return -18;
//		if(aTree->Step_InitialDirZ->size() <= StepIndex) return -19;
//		if(aTree->Step_FinalDirX->size() <= StepIndex) return -20;
//		if(aTree->Step_FinalDirY->size() <= StepIndex) return -21;
//		if(aTree->Step_FinalDirZ->size() <= StepIndex) return -22;
	if(aTree->Step_ParentTrackID->size() <= StepIndex) return -23;
	if(aTree->Step_CreatorProcess->size() <= StepIndex) return -24;
	if(aTree->Step_Particle->size() <= StepIndex) return -25;
	if(aTree->Step_TrackID->size() <= StepIndex) return -26;
	if(aTree->Step_InitialProcess->size() <= StepIndex) return -34;
	if(aTree->Step_FinalProcess->size() <= StepIndex) return -27;
	if(aTree->Step_FinalPosX->size() <= StepIndex) return -28;
	if(aTree->Step_FinalPosY->size() <= StepIndex) return -29;
	if(aTree->Step_FinalPosZ->size() <= StepIndex) return -30;
	if(aTree->Step_InitialPosX->size() <= StepIndex) return -31;
	if(aTree->Step_InitialPosY->size() <= StepIndex) return -32;
	if(aTree->Step_InitialPosZ->size() <= StepIndex) return -33;

	// Get Detector
	const Detector* myDetector = m_scanner ? m_scanner->GetDetectorFromVolume( aTree->Step_InitialLogicalVolume->at(StepIndex) ) : NULL;
		// If myDetector == NULL, the hit was outside the detector (i.e. some different sensitive volume)

//		double InitialDirModule =
//	 			sqrt(aTree->Step_InitialDirX->at(StepIndex)*aTree->Step_InitialDirX->at(StepIndex)
//					+ aTree->Step_InitialDirY->at(StepIndex)*aTree->Step_InitialDirY->at(StepIndex)
//					+ aTree->Step_InitialDirZ->at(StepIndex)*aTree->Step_InitialDirZ->at(StepIndex));
//		double FinalDirModule =
//	 			sqrt(aTree->Step_FinalDirX->at(StepIndex)*aTree->Step_FinalDirX->at(StepIndex)
//					+ aTree->Step_FinalDirY->at(StepIndex)*aTree->Step_FinalDirY->at(StepIndex)
//					+ aTree->Step_FinalDirZ->at(StepIndex)*aTree->Step_FinalDirZ->at(StepIndex));
//	
//		//calculating direction normalized vectors for current step
//		double I_x = aTree->Step_InitialDirX->at(StepIndex)/InitialDirModule;
//		double I_y = aTree->Step_InitialDirY->at(StepIndex)/InitialDirModule;
//		double I_z = aTree->Step_InitialDirZ->at(StepIndex)/InitialDirModule;
//		double F_x = aTree->Step_FinalDirX->at(StepIndex)/FinalDirModule;
//		double F_y = aTree->Step_FinalDirY->at(StepIndex)/FinalDirModule;
//		double F_z = aTree->Step_FinalDirZ->at(StepIndex)/FinalDirModule;

	//finding verteces
	//  if(aTree->Step_CreatorProcess->at(StepIndex)=="UNDEFINED" 
    //          && aTree->Step_Particle->at(StepIndex)=="e+" 
	//			&& aTree->Step_ParentTrackID->at(StepIndex)==0) 
	if (aTree->Step_CreatorProcess->at(StepIndex) == "UNDEFINED") 
	{ //this step belongs to a positron
		if (aTree->Step_InitialProcess->at(StepIndex) == "UNDEFINED")
		{ //this step is the positron vertex
			Vx = aTree->Step_InitialPosX->at(StepIndex);
			Vy = aTree->Step_InitialPosY->at(StepIndex);
			Vz = aTree->Step_InitialPosZ->at(StepIndex);
		}

		if (aTree->Step_FinalProcess->at(StepIndex) == "annihil") //this step is the annihilation vertex
		{ 
			Annihil_Vx = aTree->Step_FinalPosX->at(StepIndex);
			Annihil_Vy = aTree->Step_FinalPosY->at(StepIndex);
			Annihil_Vz = aTree->Step_FinalPosZ->at(StepIndex);
		}
	}

	// We only care about scattering of an original gammas 
	//     (not about e+ scattering or about other weird stuff)
	if(	aTree->Step_ParentTrackID->at(StepIndex) < 3 
		&& aTree->Step_CreatorProcess->at(StepIndex) == "annihil" 
		&& aTree->Step_Particle->at(StepIndex) == "gamma") 
		//this step belongs to one of the two original gammas from positron annihilation
	{	
		// - isThisStepAnOriginalGamma: We only care about scattering of an original gammas 
		//			(not about e+ scattering or about other weird stuff)

		if(ID1 == -1) 
		{
			ID1 = aTree->Step_TrackID->at(StepIndex); //saving ID of gamma 1
		}
      	else if(ID2 == -1 && aTree->Step_TrackID->at(StepIndex) != ID1) 
		{
			ID2 = aTree->Step_TrackID->at(StepIndex); //saving ID of gamma 2
		}

		int currentTrackId = aTree->Step_TrackID->at(StepIndex);
		if( myDetector ) //this gamma interacted in the sensitive volumes
		{ 
            // std::vector<std::string>* Step_FinalProcess;
			if(aTree->Step_FinalProcess->at(StepIndex) == "compt") 
			{
				//compton process
				if (currentTrackId == ID1)
					NCO1++; 
				else if (currentTrackId == ID2)
					NCO2++; 
			}
			else if(aTree->Step_FinalProcess->at(StepIndex) == "phot") 
			{
				//photoelectric process
				if (currentTrackId == ID1)
					NPE1++; 
				else if (currentTrackId == ID2)
					NPE2++;
			}
		}

        // OLD: 
		// if(fabs(I_x-F_x)>_mindiff ||  fabs(I_y-F_y)>_mindiff || fabs(I_z-F_z)>_mindiff) //direction has changed: scattered event
		// NEW: 2020-02-12
		// - if myDetector == NULL: this means that the LogicalVolume for this step was not inside the Detector 
		//			(so it was in the source or in some passive material)
		// If the volume is not one of the Absorber volumes, 
		//    then it must be one of the scattering volumes
		//    this can be either the source itself, or some passive material
		//
		if ( !myDetector )  
		{ 
			if ( aTree->Step_FinalProcess->at(StepIndex) == "compt" ) 
			{
				if (currentTrackId == ID1)
					scat1 = true;
				else if (currentTrackId == ID2)
					scat2 = true;
			}
			if (aTree->Step_FinalProcess->at(StepIndex)=="Rayl")
			{
				if (currentTrackId == ID1)
					Rayl1 = true;
				else if (currentTrackId == ID2)
					Rayl2 = true;
			}
		}

	}
	return 0;
}

// ===============================================

int PET_EvClassification::ClassifyEvent(TreeClass *aTree, map<int,EventClass>& theEventClassificationMap)
{
	NE_tot++;
	if (NPE1>0 && theHistograms)  theHistograms->Fill1DHisto("N_ComptonPerAbsPhoton",NCO1);
	if (NPE2>0 && theHistograms)  theHistograms->Fill1DHisto("N_ComptonPerAbsPhoton",NCO2);

    if (scat1 || scat2)
    {
        // cout << "PET_EvClassification::ClassifyEvent, scat1,2: " << scat1 << " " << scat2 << endl;
        // cout << "NPE1,2: " << NPE1 << " " << NPE2 << " NCO1,2: " << NCO1 << " " << NCO2 << endl;
    }
    
	if (NPE1==1 && NPE2==1 && NCO1+NCO2==0)
	{ //both gamma only photoelectric: good event PE+PE
		if (scat1 || scat2)
		{ //either one of the gammas scattered -> scat event
			m_eventClassification = scat; //class 1 = scat event
			if (Rayl1 || Rayl2)
			{
				m_eventClassification = Rayl;
			}
            NE_scat++;
            NE_scat_PEPE++;
		}
		else 
		{ //non scat event
			NE_PEPE++;
			m_eventClassification = PEPE;
		}
	}
	else if (NPE1==1 && NPE2==1 && NCO1+NCO2>0 && NCO1*NCO2==0)
	{ //both gamma photoelectric, but at least one compton for only one of the two: PE+COPE
		if (scat1 || scat2)
		{ //either one of the gammas scattered -> scat event
			m_eventClassification = scat; //class 1 = scat event
			if (Rayl1 || Rayl2)
			{
				m_eventClassification = Rayl;
			}
			NE_scat++;
			NE_scat_PECOPE++;
		}
		else 
		{ //non scat event
			NE_PECOPE++;
			m_eventClassification = PECOPE;
		}
	}
	else if (NPE1==1 && NPE2==1 && NCO1+NCO2>0 && NCO1*NCO2>0) 
	{ //both gamma photoelectric, but both gamma with at least one compton: COPE+COPE
		if (scat1 || scat2)
		{ //either one of the gammas scattered -> scat event
			m_eventClassification = scat; //class 1 = scat event
			if (Rayl1||Rayl2) 
			{
				m_eventClassification = Rayl;
			}
			NE_scat++;
			NE_scat_COPECOPE++;
		}
		else 
		{ //non scat event
			NE_COPECOPE++;
			m_eventClassification = COPECOPE;
		}
	}
	else if (NPE1+NPE2==1) 
	{ //only one photoelectric: PE+inco
		if (scat1 || scat2)
		{ //either one of the gammas scattered -> scat event
			m_eventClassification = scat; //class 1 = scat event
			if (Rayl1||Rayl2) 
			{
				m_eventClassification = Rayl;
			}
			NE_scat++;
			NE_scat_PEinco++;
		}
		else 
		{ //non scat event
			NE_PEinco++;
			m_eventClassification = PEinco;
		}
	}
	else if (NPE1+NPE2==0)
	{ //no photoelectric: inco+inco
		if (scat1 || scat2)
		{ //either one of the gammas scattered -> scat event
			m_eventClassification = scat; //class 1 = scat event
			if ( Rayl1 || Rayl2 ) 
			{
				m_eventClassification = Rayl;
			}
			NE_scat++;
			NE_scat_incoinco++;
		}
		else 
		{ //non scat event
			NE_incoinco++;
			m_eventClassification = incoinco;
		}
	}
	
	EventClass current_event;
	current_event.Vx = Vx;
	current_event.Vy = Vy;
	current_event.Vz = Vz;
	current_event.Annihil_Vx = Annihil_Vx;
	current_event.Annihil_Vy = Annihil_Vy;
	current_event.Annihil_Vz = Annihil_Vz;
	current_event.classification = (int)m_eventClassification;
	theEventClassificationMap[aTree->Event_EventID] = current_event;

	return 0;
}

