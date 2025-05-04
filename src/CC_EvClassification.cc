
#include "CC_EvClassification.hh"

using namespace std;

CC_EvClassification::~CC_EvClassification()
{
}

CC_EvClassification::CC_EvClassification()
{
	InitializeCounters();
	InitializeGlobalCounters();
}

int CC_EvClassification::InitializeGlobalCounters()
{
	NE_PE_null = 0;
	NE_CO_PE = 0;
	NE_nCO_PE = 0;
	NE_CO_COPE = 0;
	NE_nCO_COPE = 0;
	NE_CO_nCOPE = 0;
	NE_nCO_nCOPE = 0;
	NE_inco = 0;
	NE_PE_CO = 0;
	
	NE_scat_PE_null = 0;
	NE_scat_CO_PE = 0;
	NE_scat_nCO_PE = 0;
	NE_scat_CO_COPE = 0;
	NE_scat_nCO_COPE = 0;
	NE_scat_CO_nCOPE = 0;
	NE_scat_nCO_nCOPE = 0;
	NE_scat_inco = 0;
	NE_scat_PE_CO = 0;
	
	NE_multiphot = 0;
	NE_other = 0;
	
	NE_tot = 0;
	return 0;
}

int CC_EvClassification::InitializeCounters()
{
	aClassification = UNDEFINED; //initializing the classification variable
	ID1=-1;
	ID2=-1;
	scat1=false;
	scat2=false; //boolean to determine a scattered gamma1 or gamma2
	NPE1_scatdet=0;
	NPE2_scatdet=0; //counters for PhotoElectric interactions
	NCO1_scatdet=0;
	NCO2_scatdet=0; //counters for Compton interactions
	NOTH1_scatdet=0;
	NOTH2_scatdet=0; //counters for Other interactions
	NPE1_absdet=0;
	NPE2_absdet=0; //counters for PhotoElectric interactions
	NCO1_absdet=0;
	NCO2_absdet=0; //counters for Compton interactions
	NOTH1_absdet=0;
	NOTH2_absdet=0; //counters for Other interactions
	Vx = -999;
	Vy = -999;
	Vz = -999;
	Annihil_Vx = -999;
	Annihil_Vy = -999;
	Annihil_Vz = -999;
	
	return 0;
}

int CC_EvClassification::PrintClassification() const
{
	cout<<"***********************************************************"<<endl;
	cout<<"*** CC_EvClassification::PrintClassification ***"<<endl;
	cout<<"***********************************************************"<<endl;
	cout<<"  NE_tot	     = "<<NE_tot<<endl;
	cout<<"  NE_PE_null	     = "<<NE_PE_null<<endl;
	cout<<"  NE_CO_PE	     = "<<NE_CO_PE<<endl;
	cout<<"  NE_nCO_PE	     = "<<NE_nCO_PE<<endl;
	cout<<"  NE_CO_COPE	     = "<<NE_CO_COPE<<endl;
	cout<<"  NE_nCO_COPE	     = "<<NE_nCO_COPE<<endl;
	cout<<"  NE_CO_nCOPE	     = "<<NE_CO_nCOPE<<endl;
	cout<<"  NE_nCO_nCOPE	     = "<<NE_nCO_nCOPE<<endl;
	cout<<"  NE_inco	     = "<<NE_inco<<endl;
	cout<<"  NE_PE_CO	     = "<<NE_PE_CO<<endl;
	cout<<"  NE_scat_PE_null   = "<<NE_scat_PE_null<<endl;
	cout<<"  NE_scat_CO_PE     = "<<NE_scat_CO_PE<<endl;
	cout<<"  NE_scat_nCO_PE    = "<<NE_scat_nCO_PE<<endl;
	cout<<"  NE_scat_CO_COPE   = "<<NE_scat_CO_COPE<<endl;
	cout<<"  NE_scat_nCO_COPE  = "<<NE_scat_nCO_COPE<<endl;
	cout<<"  NE_scat_CO_nCOPE  = "<<NE_scat_CO_nCOPE<<endl;
	cout<<"  NE_scat_nCO_nCOPE = "<<NE_scat_nCO_nCOPE<<endl;
	cout<<"  NE_scat_inco	     = "<<NE_scat_inco<<endl;
	cout<<"  NE_scat_PE_CO     = "<<NE_scat_PE_CO<<endl;
	cout<<"  NE_multiphot      = "<<NE_multiphot<<endl;
	cout<<"  NE_other          = "<<NE_other<<endl;
	cout<<"***********************************************************"<<endl;
	
	return 0;
}

int CC_EvClassification::UpdateCounters(TreeClass* aTree, int StepIndex)
{
	if(! aTree->Step_InitialTotalEnergy) return -1;
	if(! aTree->Step_FinalTotalEnergy) return -2;
	if(! aTree->Step_InitialDirX) return -3;
	if(! aTree->Step_InitialDirY) return -4;
	if(! aTree->Step_InitialDirZ) return -5;
	if(! aTree->Step_FinalDirX) return -6;
	if(! aTree->Step_FinalDirY) return -7;
	if(! aTree->Step_FinalDirZ) return -8;
	if(! aTree->Step_ParentTrackID) return -9;
	if(! aTree->Step_CreatorProcess) return -10;
	if(! aTree->Step_Particle) return -11;
	if(! aTree->Step_TrackID) return -12;
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
	if(aTree->Step_InitialDirX->size() <= StepIndex) return -17;
	if(aTree->Step_InitialDirY->size() <= StepIndex) return -18;
	if(aTree->Step_InitialDirZ->size() <= StepIndex) return -19;
	if(aTree->Step_FinalDirX->size() <= StepIndex) return -20;
	if(aTree->Step_FinalDirY->size() <= StepIndex) return -21;
	if(aTree->Step_FinalDirZ->size() <= StepIndex) return -22;
	if(aTree->Step_ParentTrackID->size() <= StepIndex) return -23;
	if(aTree->Step_CreatorProcess->size() <= StepIndex) return -24;
	if(aTree->Step_Particle->size() <= StepIndex) return -25;
	if(aTree->Step_TrackID->size() <= StepIndex) return -26;
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

	double TotEnergy = aTree->Step_InitialTotalEnergy->at(StepIndex)-aTree->Step_FinalTotalEnergy->at(StepIndex); 
				//calculation of step Total Energy only for event classification
	double InitialDirModule =
				sqrt(aTree->Step_InitialDirX->at(StepIndex) * aTree->Step_InitialDirX->at(StepIndex)+aTree->Step_InitialDirY->at(StepIndex)
						* aTree->Step_InitialDirY->at(StepIndex) + aTree->Step_InitialDirZ->at(StepIndex) * aTree->Step_InitialDirZ->at(StepIndex));
	double FinalDirModule =
				sqrt(aTree->Step_FinalDirX->at(StepIndex) * aTree->Step_FinalDirX->at(StepIndex)+aTree->Step_FinalDirY->at(StepIndex)
						* aTree->Step_FinalDirY->at(StepIndex) + aTree->Step_FinalDirZ->at(StepIndex) * aTree->Step_FinalDirZ->at(StepIndex));
	
	//calculating direction normalized vectors for current step
	double I_x = aTree->Step_InitialDirX->at(StepIndex)/InitialDirModule;
	double I_y = aTree->Step_InitialDirY->at(StepIndex)/InitialDirModule;
	double I_z = aTree->Step_InitialDirZ->at(StepIndex)/InitialDirModule;
	double F_x = aTree->Step_FinalDirX->at(StepIndex)/FinalDirModule;
	double F_y = aTree->Step_FinalDirY->at(StepIndex)/FinalDirModule;
	double F_z = aTree->Step_FinalDirZ->at(StepIndex)/FinalDirModule;

	//finding verteces
	if(aTree->Step_CreatorProcess->at(StepIndex)=="UNDEFINED" && aTree->Step_Particle->at(StepIndex)=="e+" && aTree->Step_ParentTrackID->at(StepIndex)==0) 
	//if(aTree->Step_CreatorProcess->at(StepIndex)=="UNDEFINED") 
	{ //this step belongs to a primary particle
		if(aTree->Step_InitialProcess->at(StepIndex)=="UNDEFINED")
		{ //this step is the primary vertex
			Vx = aTree->Step_InitialPosX->at(StepIndex);
			Vy = aTree->Step_InitialPosY->at(StepIndex);
			Vz = aTree->Step_InitialPosZ->at(StepIndex);
		}
		if(aTree->Step_FinalProcess->at(StepIndex)=="annihil")
		{ //this step is the annihilation vertex
			Annihil_Vx = aTree->Step_FinalPosX->at(StepIndex);
			Annihil_Vy = aTree->Step_FinalPosY->at(StepIndex);
			Annihil_Vz = aTree->Step_FinalPosZ->at(StepIndex);
		}
	}

	if(aTree->Step_ParentTrackID->at(StepIndex)<3 && aTree->Step_CreatorProcess->at(StepIndex)=="annihil" && aTree->Step_Particle->at(StepIndex)=="gamma") 
	{
		//this step belongs to one of the two original gamma from positron annihilation
		if(ID1==-1) 
		{
			ID1 = aTree->Step_TrackID->at(StepIndex); //saving ID of gamma 1
		}
		else if(ID2==-1 && aTree->Step_TrackID->at(StepIndex) != ID1) 
		{
			ID2 = aTree->Step_TrackID->at(StepIndex); //saving ID of gamma 2
		}

		int currentTrackId = aTree->Step_TrackID->at(StepIndex);

		if( myDetector ) //this gamma interacted in the sensitive volumes
		{ 
			if( myDetector->GetType() == DETECTOR_TYPE_SCATTERER ) //scatterer interaction
			{
				if(aTree->Step_FinalProcess->at(StepIndex)=="compt") 
				{
					//compton process
					if (currentTrackId = ID1)
						NCO1_scatdet++; 
					else if (currentTrackId = ID2)
						NCO2_scatdet++; 
				}
				else if(aTree->Step_FinalProcess->at(StepIndex)=="phot") 
				{
					//photoelectric process
					if (currentTrackId = ID1)
						NPE1_scatdet++; 
					else if (currentTrackId = ID2)
						NPE2_scatdet++; 
				}
				else if(TotEnergy>0)
				{
					if (currentTrackId = ID1)
						NOTH1_scatdet++;
					else if (currentTrackId = ID2)
						NOTH2_scatdet++;
				}
			}
			else if ( myDetector->GetType() == DETECTOR_TYPE_ABSORBER ) //absorber interaction
			{
				if(aTree->Step_FinalProcess->at(StepIndex)=="compt") 
				{
					//compton process
					if (currentTrackId = ID1)
						NCO1_absdet++; 
					else if (currentTrackId = ID2)
						NCO2_absdet++; 
				}
				else if(aTree->Step_FinalProcess->at(StepIndex)=="phot") 
				{
					//photoelectric process
					if (currentTrackId = ID1)
						NPE1_absdet++; 
					else if (currentTrackId = ID2)
						NPE2_absdet++; 
				}
				else if(TotEnergy>0)
				{
					if (currentTrackId = ID1)
						NOTH1_absdet++;
					else if (currentTrackId = ID2)
						NOTH2_absdet++;
				}
			}
		}

		if ((I_x!=F_x || I_y!=F_y || I_z!=F_z) && (!myDetector || TotEnergy==0)) //direction has changed: scattered event
		{ 
			if (currentTrackId = ID1)
				scat1=true;
			else if (currentTrackId = ID2)
				scat2=true;
		}
	}	// if (aTree->Step_ParentTrackID->at(StepIndex)==1 && aTree->Step_CreatorProcess->at(StepIndex)=="annihil" 
		//		&& aTree->Step_Particle->at(StepIndex)=="gamma")

	return 0;
}

int CC_EvClassification::ClassifyEvent(TreeClass *aTree, map<int,EventClass>& theEventClassificationMap)
{
	if(! aTree->Event_EventID) return -1;
	
	if(aTree->Event_EventID==463742)
	{
		cout<<"NPE1_scatdet  = "<<NPE1_scatdet <<endl;
		cout<<"NCO1_scatdet  = "<<NCO1_scatdet <<endl;
		cout<<"NOTH1_scatdet = "<<NOTH1_scatdet<<endl;
		cout<<"NPE1_absdet   = "<<NPE1_absdet  <<endl;
		cout<<"NCO1_absdet   = "<<NCO1_absdet  <<endl;
		cout<<"NOTH1_absdet  = "<<NOTH1_absdet <<endl;
		cout<<"NPE2_scatdet  = "<<NPE2_scatdet <<endl;
		cout<<"NCO2_scatdet  = "<<NCO2_scatdet <<endl;
		cout<<"NOTH2_scatdet = "<<NOTH2_scatdet<<endl;
		cout<<"NPE2_absdet   = "<<NPE2_absdet  <<endl;
		cout<<"NCO2_absdet   = "<<NCO2_absdet  <<endl;
		cout<<"NOTH2_absdet  = "<<NOTH2_absdet <<endl;
	}

	NE_tot++;
	
	int TOT1 = NPE1_scatdet+NPE1_absdet+NCO1_scatdet+NCO1_absdet+NOTH1_scatdet+NOTH1_absdet;
	int TOT2 = NPE2_scatdet+NPE2_absdet+NCO2_scatdet+NCO2_absdet+NOTH2_scatdet+NOTH2_absdet;

	if(TOT1>0 && TOT2>0)
	{
		aClassification = multiphot;
		NE_multiphot++;
	}
	else if(TOT1>0 || TOT2>0)
	{
		int NPE_scatdet = 0;
		int NCO_scatdet = 0;
		int NOTH_scatdet = 0;
		int NPE_absdet = 0;
		int NCO_absdet = 0;
		int NOTH_absdet = 0;
		bool scat = false;
		
		if(TOT1>0) 
		{
			NPE_scatdet  = NPE1_scatdet ;
			NCO_scatdet  = NCO1_scatdet ;
			NOTH_scatdet = NOTH1_scatdet;
			NPE_absdet   = NPE1_absdet  ;
			NCO_absdet   = NCO1_absdet  ;
			NOTH_absdet  = NOTH1_absdet ;
			scat = scat1;
		}
		else if(TOT2>0) 
		{
			NPE_scatdet  = NPE2_scatdet ;
			NCO_scatdet  = NCO2_scatdet ;
			NOTH_scatdet = NOTH2_scatdet;
			NPE_absdet   = NPE2_absdet  ;
			NCO_absdet   = NCO2_absdet  ;
			NOTH_absdet  = NOTH2_absdet ;
			scat = scat2;
		}
	
		if(NPE_scatdet==1 && (NCO_absdet+NPE_absdet+NOTH_absdet)==0) 
		{ //stopped in the scatterer
			if(scat) 
			{ 
				aClassification = scat_PE_null;
				NE_scat_PE_null++;
			}
			else 
			{ //non scat event
				aClassification = PE_null;
				NE_PE_null++;
			}
		}
	
		else if(NPE_absdet==1 && NCO_scatdet==1 && (NCO_absdet+NOTH_absdet+NOTH_scatdet)==0) 
		{ //clean CO+PE event
			if(scat) 
			{ 
				aClassification = scat_CO_PE;
				NE_scat_CO_PE++;
			}
			else 
			{ //non scat event
				aClassification = CO_PE;
				NE_CO_PE++;
			}
		}
	
		else if(NPE_absdet==1 && NCO_scatdet>1 && (NCO_absdet+NOTH_absdet+NOTH_scatdet)==0) 
		{ // nCO+PE event
			if(scat) 
			{ 
				aClassification = scat_nCO_PE;
				NE_scat_nCO_PE++;
			}
			else 
			{ //non scat event
				aClassification = nCO_PE;
				NE_nCO_PE++;
			}
		}
	
		else if(NPE_absdet==1 && NCO_scatdet==1 && NCO_absdet==1 && (NOTH_absdet+NOTH_scatdet)==0) 
		{ // CO+COPE event
			if(scat) 
			{ 
				aClassification = scat_CO_COPE;
				NE_scat_CO_COPE++;
			}
			else 
			{ //non scat event
				aClassification = CO_COPE;
				NE_CO_COPE++;
			}
		}
	
		else if(NPE_absdet==1 && NCO_scatdet>1 && NCO_absdet==1 && (NOTH_absdet+NOTH_scatdet)==0) 
		{ // nCO+COPE event
			if(scat) 
			{ 
				aClassification = scat_nCO_COPE;
				NE_scat_nCO_COPE++;
			}
			else 
			{ //non scat event
				aClassification = nCO_COPE;
				NE_nCO_COPE++;
			}
		}
		
		else if(NPE_absdet==1 && NCO_scatdet==1 && NCO_absdet>1 && (NOTH_absdet+NOTH_scatdet)==0) 
		{ // CO+nCOPE event
			if(scat) 
			{ 
				aClassification = scat_CO_nCOPE;
				NE_scat_CO_nCOPE++;
			}
			else 
			{ //non scat event
				aClassification = CO_nCOPE;
				NE_CO_nCOPE++;
			}
		}
	
		else if(NPE_absdet==1 && NCO_scatdet>1 && NCO_absdet>1 && (NOTH_absdet+NOTH_scatdet)==0) 
		{ // nCO+nCOPE event
			if(scat) 
			{ 
				aClassification = scat_nCO_nCOPE;
				NE_scat_nCO_nCOPE++;
			}
			else 
			{ //non scat event
				aClassification = nCO_nCOPE;
				NE_nCO_nCOPE++;
			}
		}	
		else if((NPE_scatdet+NPE_absdet)==0)
		{ // incomplete event (no PE)
			if(scat) 
			{ 
				aClassification = scat_inco;
				NE_scat_inco++;
			}
			else 
			{ //non scat event
				aClassification = inco;
				NE_inco++;
			}
		}	
		else if(NPE_scatdet==1 && NCO_absdet>0) 
		{ //back scattering event
			aClassification = PE_CO;
			NE_PE_CO++;
		}	
		else 
		{
			aClassification = other;
			NE_other++;
		}
		
	}   // else if(TOT1>0 || TOT2>0)
	
	EventClass current_event;
	current_event.Vx = Vx;
	current_event.Vy = Vy;
	current_event.Vz = Vz;
	current_event.Annihil_Vx = Annihil_Vx;
	current_event.Annihil_Vy = Annihil_Vy;
	current_event.Annihil_Vz = Annihil_Vz;
	current_event.classification = (int)aClassification;
	theEventClassificationMap[aTree->Event_EventID] = current_event;
	return 0;
}

