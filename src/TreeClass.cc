#include "TreeClass.hh"
#include "InputFlags.hh"

using namespace std;

TreeClass::~TreeClass()
{
	delete theTTree;
	delete theRootFile;
}

TreeClass::TreeClass()
	: m_isfirstcheck(true)
{
  theTTree = 0; //need to initialize to zero or it would get deleted in TreeClass::LoadTTree
  theRootFile = 0; //need to initialize to zero or it would get deleted in TreeClass::LoadTTree

  InitializeLeaves();
}

int TreeClass::CheckNeededVariables(int StepIndex)
{
	// TODO: check whether we need all this for image...
	{
		// if (! this->Step_EventID) return -100;
        if ( !this->Step_EventID && m_isfirstcheck )
        {
            cout << "WARNING!!!!! Step_EventID is not present in the data root file!" << endl;
            cout << "For most purposes this is not necessary, but for some later analysis it might be needed" << endl;
            cout << "(For example: 3 Compton cones analysis)" << endl;
        }
		
		if (! this->Step_InitialTime) return -1;
		if (! this->Step_AccumulatedEnergyDeposited) return -2;
		if (! this->Step_InitialLogicalVolume) return -3;
		if (! this->Step_InitialUnitID) return -4;
		if (! this->Step_InitialTouchablePosX) return -5;
		if (! this->Step_InitialTouchablePosY) return -6;
		if (! this->Step_InitialTouchablePosZ) return -7;

		if (InputFlags::Instance()->DoClassification())
		{
			if (! this->Step_InitialLocalPosX) return -8;
			if (! this->Step_InitialLocalPosY) return -9;
			if (! this->Step_InitialLocalPosZ) return -10;

			if (this->Step_InitialLocalPosX->size() <= StepIndex) return -18;
			if (this->Step_InitialLocalPosY->size() <= StepIndex) return -19;
			if (this->Step_InitialLocalPosZ->size() <= StepIndex) return -20;
		}
		else if (   !this->Step_InitialLocalPosX || !this->Step_InitialLocalPosY
			 || !this->Step_InitialLocalPosZ ) 
		{
			if ( m_isfirstcheck )
			{
				cout << "WARNING!!!! TreeClass::CheckNeededVariables" << endl;
				cout << "Missing Step_InitialLocalPosX, Y, Z" << endl;
			}
		}
	
		if (this->Step_InitialTime->size() <= StepIndex) return -11;
		if (this->Step_AccumulatedEnergyDeposited->size() <= StepIndex) return -12;
		if (this->Step_InitialLogicalVolume->size() <= StepIndex) return -13;
		if (this->Step_InitialUnitID->size() <= StepIndex) return -14;
		if (this->Step_InitialTouchablePosX->size() <= StepIndex) return -15;
		if (this->Step_InitialTouchablePosY->size() <= StepIndex) return -16;
		if (this->Step_InitialTouchablePosZ->size() <= StepIndex) return -17;
	}
	
	if ( m_isfirstcheck )
        m_isfirstcheck = false;

	// 2015 05 06
	//   Not used (in principle meant for the "-realgeomExtendedOutput" flag, but that flag is not working yet!
	// 2018-11-29, see DetHitReconstruction::CreateNewHit(...)
	//   We can now calculate the real position of the hits, but we still don't need anything of the following!
	//	 (And the flag is actually called "-realHitPosOutput")
	/*
	if (InputFlags::Instance()->doRealHitPosOutput)
	{
		if ( !this->Step_TrackID ) return -101;

		if ( !this->Track_EventID ) return -102;
		if ( !this->Track_TrackID ) return -103;

		if ( !this->Track_FinalPosX ) return -104;
		if ( !this->Track_FinalPosY ) return -105;
		if ( !this->Track_FinalPosZ ) return -106;

		if ( !this->Track_InitialPosX ) return -107;
		if ( !this->Track_InitialPosY ) return -108;
		if ( !this->Track_InitialPosZ ) return -109;
	}
	*/

	return 0;
}

int TreeClass::LoadTTree(const string& RootFileName)
{
	cout<<"loading tree from <"<<RootFileName<<"> file."<<endl;
	if(theTTree) 
	{
		delete theTTree;
	}
	if(theRootFile)
	{
		delete theRootFile;
	}
	theRootFile = new TFile(RootFileName.c_str());  
	if(theRootFile->IsOpen())
	{
		TIter* nextkey = new TIter(theRootFile->GetListOfKeys());
		TKey* key = 0;
		while(key = (TKey*)nextkey->Next())
		{
			TString classname = key->GetClassName();
			if(classname == "TTree")
			{
				theTTree = (TTree*)theRootFile->Get(key->GetName());
				break;
			}
		}
		if(theTTree) 
		{
			this->LinkAvailableBranches();
		}
	}
	return 0;
}

int TreeClass::GetNumOfSteps()
{
	int NumOfSteps = 0;
	if(this->Step_AccumulatedDose) 
	{
		NumOfSteps = this->Step_AccumulatedDose->size();
		return NumOfSteps;
	}
	else if(this->Step_AccumulatedEnergyDeposited) 
	{
		NumOfSteps = this->Step_AccumulatedEnergyDeposited->size();
		return NumOfSteps;
	}
	else if(this->Step_AccumulatedEnergyLost) 
	{
		NumOfSteps = this->Step_AccumulatedEnergyLost->size();
		return NumOfSteps;
	}
	else if(this->Step_AccumulatedKerma) 
	{
		NumOfSteps = this->Step_AccumulatedKerma->size();
		return NumOfSteps;
	}
	else if(this->Step_AccumulatedLength) 
	{
		NumOfSteps = this->Step_AccumulatedLength->size();
		return NumOfSteps;
	}
	else if(this->Step_AccumulatedNonIonizingEnergyDeposited) 
	{
		NumOfSteps = this->Step_AccumulatedNonIonizingEnergyDeposited->size();
		return NumOfSteps;
	}
	else if(this->Step_AngleChange) 
	{
		NumOfSteps = this->Step_AngleChange->size();
		return NumOfSteps;
	}
	else if(this->Step_DirChangeMag) 
	{
		NumOfSteps = this->Step_DirChangeMag->size();
		return NumOfSteps;
	}
	else if(this->Step_DirChangePerp) 
	{
		NumOfSteps = this->Step_DirChangePerp->size();
		return NumOfSteps;
	}
	else if(this->Step_DirChangePhi) 
	{
		NumOfSteps = this->Step_DirChangePhi->size();
		return NumOfSteps;
	}
	else if(this->Step_DirChangeTheta) 
	{
		NumOfSteps = this->Step_DirChangeTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_DirChangeX) 
	{
		NumOfSteps = this->Step_DirChangeX->size();
		return NumOfSteps;
	}
	else if(this->Step_DirChangeY) 
	{
		NumOfSteps = this->Step_DirChangeY->size();
		return NumOfSteps;
	}
	else if(this->Step_DirChangeZ) 
	{
		NumOfSteps = this->Step_DirChangeZ->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalDEDX) 
	{
		NumOfSteps = this->Step_FinalDEDX->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalDensity) 
	{
		NumOfSteps = this->Step_FinalDensity->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalDirPerp) 
	{
		NumOfSteps = this->Step_FinalDirPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalDirPhi) 
	{
		NumOfSteps = this->Step_FinalDirPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalDirTheta) 
	{
		NumOfSteps = this->Step_FinalDirTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalDirX) 
	{
		NumOfSteps = this->Step_FinalDirX->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalDirY) 
	{
		NumOfSteps = this->Step_FinalDirY->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalDirZ) 
	{
		NumOfSteps = this->Step_FinalDirZ->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalElectronDensity) 
	{
		NumOfSteps = this->Step_FinalElectronDensity->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalGammaEquivalentDose) 
	{
		NumOfSteps = this->Step_FinalGammaEquivalentDose->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalKineticEnergy) 
	{
		NumOfSteps = this->Step_FinalKineticEnergy->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalDirPerp) 
	{
		NumOfSteps = this->Step_FinalLocalDirPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalDirPhi) 
	{
		NumOfSteps = this->Step_FinalLocalDirPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalDirTheta) 
	{
		NumOfSteps = this->Step_FinalLocalDirTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalDirX) 
	{
		NumOfSteps = this->Step_FinalLocalDirX->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalDirY) 
	{
		NumOfSteps = this->Step_FinalLocalDirY->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalDirZ) 
	{
		NumOfSteps = this->Step_FinalLocalDirZ->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPreDirPerp) 
	{
		NumOfSteps = this->Step_FinalLocalInPreDirPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPreDirPhi) 
	{
		NumOfSteps = this->Step_FinalLocalInPreDirPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPreDirTheta) 
	{
		NumOfSteps = this->Step_FinalLocalInPreDirTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPreDirX) 
	{
		NumOfSteps = this->Step_FinalLocalInPreDirX->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPreDirY) 
	{
		NumOfSteps = this->Step_FinalLocalInPreDirY->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPreDirZ) 
	{
		NumOfSteps = this->Step_FinalLocalInPreDirZ->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPrePosMag) 
	{
		NumOfSteps = this->Step_FinalLocalInPrePosMag->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPrePosPerp) 
	{
		NumOfSteps = this->Step_FinalLocalInPrePosPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPrePosPhi) 
	{
		NumOfSteps = this->Step_FinalLocalInPrePosPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPrePosTheta) 
	{
		NumOfSteps = this->Step_FinalLocalInPrePosTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPrePosX) 
	{
		NumOfSteps = this->Step_FinalLocalInPrePosX->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPrePosY) 
	{
		NumOfSteps = this->Step_FinalLocalInPrePosY->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalInPrePosZ) 
	{
		NumOfSteps = this->Step_FinalLocalInPrePosZ->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalPosMag) 
	{
		NumOfSteps = this->Step_FinalLocalPosMag->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalPosPerp) 
	{
		NumOfSteps = this->Step_FinalLocalPosPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalPosPhi) 
	{
		NumOfSteps = this->Step_FinalLocalPosPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalPosTheta) 
	{
		NumOfSteps = this->Step_FinalLocalPosTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalPosX) 
	{
		NumOfSteps = this->Step_FinalLocalPosX->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalPosY) 
	{
		NumOfSteps = this->Step_FinalLocalPosY->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalPosZ) 
	{
		NumOfSteps = this->Step_FinalLocalPosZ->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLocalTime) 
	{
		NumOfSteps = this->Step_FinalLocalTime->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalMomMag) 
	{
		NumOfSteps = this->Step_FinalMomMag->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalMomPerp) 
	{
		NumOfSteps = this->Step_FinalMomPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalMomPhi) 
	{
		NumOfSteps = this->Step_FinalMomPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalMomTheta) 
	{
		NumOfSteps = this->Step_FinalMomTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalMomX) 
	{
		NumOfSteps = this->Step_FinalMomX->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalMomY) 
	{
		NumOfSteps = this->Step_FinalMomY->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalMomZ) 
	{
		NumOfSteps = this->Step_FinalMomZ->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalNeutronEquivalentDose) 
	{
		NumOfSteps = this->Step_FinalNeutronEquivalentDose->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalNuclearInterLength) 
	{
		NumOfSteps = this->Step_FinalNuclearInterLength->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPVCopyNumber) 
	{
		NumOfSteps = this->Step_FinalPVCopyNumber->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalParallelPVCopyNumber) 
	{
		NumOfSteps = this->Step_FinalParallelPVCopyNumber->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPosMag) 
	{
		NumOfSteps = this->Step_FinalPosMag->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPosPerp) 
	{
		NumOfSteps = this->Step_FinalPosPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPosPhi) 
	{
		NumOfSteps = this->Step_FinalPosPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPosTheta) 
	{
		NumOfSteps = this->Step_FinalPosTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPosX) 
	{
		NumOfSteps = this->Step_FinalPosX->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPosY) 
	{
		NumOfSteps = this->Step_FinalPosY->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPosZ) 
	{
		NumOfSteps = this->Step_FinalPosZ->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPressure) 
	{
		NumOfSteps = this->Step_FinalPressure->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalProperTime) 
	{
		NumOfSteps = this->Step_FinalProperTime->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalRadLength) 
	{
		NumOfSteps = this->Step_FinalRadLength->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTemperature) 
	{
		NumOfSteps = this->Step_FinalTemperature->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTime) 
	{
		NumOfSteps = this->Step_FinalTime->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTotalEnergy) 
	{
		NumOfSteps = this->Step_FinalTotalEnergy->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTouchablePosX) 
	{
		NumOfSteps = this->Step_FinalTouchablePosX->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTouchablePosY) 
	{
		NumOfSteps = this->Step_FinalTouchablePosY->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTouchablePosZ) 
	{
		NumOfSteps = this->Step_FinalTouchablePosZ->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTouchableEulerPhi) 
	{
		NumOfSteps = this->Step_FinalTouchableEulerPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTouchableEulerPsi) 
	{
		NumOfSteps = this->Step_FinalTouchableEulerPsi->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTouchableEulerTheta) 
	{
		NumOfSteps = this->Step_FinalTouchableEulerTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalWeight) 
	{
		NumOfSteps = this->Step_FinalWeight->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialDEDX) 
	{
		NumOfSteps = this->Step_InitialDEDX->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialDensity) 
	{
		NumOfSteps = this->Step_InitialDensity->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialDirPerp) 
	{
		NumOfSteps = this->Step_InitialDirPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialDirPhi) 
	{
		NumOfSteps = this->Step_InitialDirPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialDirTheta) 
	{
		NumOfSteps = this->Step_InitialDirTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialDirX) 
	{
		NumOfSteps = this->Step_InitialDirX->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialDirY) 
	{
		NumOfSteps = this->Step_InitialDirY->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialDirZ) 
	{
		NumOfSteps = this->Step_InitialDirZ->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialElectronDensity) 
	{
		NumOfSteps = this->Step_InitialElectronDensity->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialGammaEquivalentDose) 
	{
		NumOfSteps = this->Step_InitialGammaEquivalentDose->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialKineticEnergy) 
	{
		NumOfSteps = this->Step_InitialKineticEnergy->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalDirPerp) 
	{
		NumOfSteps = this->Step_InitialLocalDirPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalDirPhi) 
	{
		NumOfSteps = this->Step_InitialLocalDirPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalDirTheta) 
	{
		NumOfSteps = this->Step_InitialLocalDirTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalDirX) 
	{
		NumOfSteps = this->Step_InitialLocalDirX->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalDirY) 
	{
		NumOfSteps = this->Step_InitialLocalDirY->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalDirZ) 
	{
		NumOfSteps = this->Step_InitialLocalDirZ->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalPosMag) 
	{
		NumOfSteps = this->Step_InitialLocalPosMag->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalPosPerp) 
	{
		NumOfSteps = this->Step_InitialLocalPosPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalPosPhi) 
	{
		NumOfSteps = this->Step_InitialLocalPosPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalPosTheta) 
	{
		NumOfSteps = this->Step_InitialLocalPosTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalPosX) 
	{
		NumOfSteps = this->Step_InitialLocalPosX->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalPosY) 
	{
		NumOfSteps = this->Step_InitialLocalPosY->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalPosZ) 
	{
		NumOfSteps = this->Step_InitialLocalPosZ->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLocalTime) 
	{
		NumOfSteps = this->Step_InitialLocalTime->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialMomMag) 
	{
		NumOfSteps = this->Step_InitialMomMag->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialMomPerp) 
	{
		NumOfSteps = this->Step_InitialMomPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialMomPhi) 
	{
		NumOfSteps = this->Step_InitialMomPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialMomTheta) 
	{
		NumOfSteps = this->Step_InitialMomTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialMomX) 
	{
		NumOfSteps = this->Step_InitialMomX->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialMomY) 
	{
		NumOfSteps = this->Step_InitialMomY->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialMomZ) 
	{
		NumOfSteps = this->Step_InitialMomZ->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialNeutronEquivalentDose) 
	{
		NumOfSteps = this->Step_InitialNeutronEquivalentDose->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialNuclearInterLength) 
	{
		NumOfSteps = this->Step_InitialNuclearInterLength->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPVCopyNumber) 
	{
		NumOfSteps = this->Step_InitialPVCopyNumber->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialParallelPVCopyNumber) 
	{
		NumOfSteps = this->Step_InitialParallelPVCopyNumber->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPosMag) 
	{
		NumOfSteps = this->Step_InitialPosMag->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPosPerp) 
	{
		NumOfSteps = this->Step_InitialPosPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPosPhi) 
	{
		NumOfSteps = this->Step_InitialPosPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPosTheta) 
	{
		NumOfSteps = this->Step_InitialPosTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPosX) 
	{
		NumOfSteps = this->Step_InitialPosX->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPosY) 
	{
		NumOfSteps = this->Step_InitialPosY->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPosZ) 
	{
		NumOfSteps = this->Step_InitialPosZ->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPressure) 
	{
		NumOfSteps = this->Step_InitialPressure->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialProperTime) 
	{
		NumOfSteps = this->Step_InitialProperTime->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialRadLength) 
	{
		NumOfSteps = this->Step_InitialRadLength->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTemperature) 
	{
		NumOfSteps = this->Step_InitialTemperature->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTime) 
	{
		NumOfSteps = this->Step_InitialTime->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTotalEnergy) 
	{
		NumOfSteps = this->Step_InitialTotalEnergy->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTouchablePosX) 
	{
		NumOfSteps = this->Step_InitialTouchablePosX->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTouchablePosY) 
	{
		NumOfSteps = this->Step_InitialTouchablePosY->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTouchablePosZ) 
	{
		NumOfSteps = this->Step_InitialTouchablePosZ->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTouchableEulerPhi) 
	{
		NumOfSteps = this->Step_InitialTouchableEulerPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTouchableEulerPsi) 
	{
		NumOfSteps = this->Step_InitialTouchableEulerPsi->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTouchableEulerTheta) 
	{
		NumOfSteps = this->Step_InitialTouchableEulerTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialWeight) 
	{
		NumOfSteps = this->Step_InitialWeight->size();
		return NumOfSteps;
	}
	else if(this->Step_KineticEnergyChange) 
	{
		NumOfSteps = this->Step_KineticEnergyChange->size();
		return NumOfSteps;
	}
	else if(this->Step_MomChangeMag) 
	{
		NumOfSteps = this->Step_MomChangeMag->size();
		return NumOfSteps;
	}
	else if(this->Step_MomChangePerp) 
	{
		NumOfSteps = this->Step_MomChangePerp->size();
		return NumOfSteps;
	}
	else if(this->Step_MomChangePhi) 
	{
		NumOfSteps = this->Step_MomChangePhi->size();
		return NumOfSteps;
	}
	else if(this->Step_MomChangeTheta) 
	{
		NumOfSteps = this->Step_MomChangeTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_MomChangeX) 
	{
		NumOfSteps = this->Step_MomChangeX->size();
		return NumOfSteps;
	}
	else if(this->Step_MomChangeY) 
	{
		NumOfSteps = this->Step_MomChangeY->size();
		return NumOfSteps;
	}
	else if(this->Step_MomChangeZ) 
	{
		NumOfSteps = this->Step_MomChangeZ->size();
		return NumOfSteps;
	}
	else if(this->Step_NofSecondaries) 
	{
		NumOfSteps = this->Step_NofSecondaries->size();
		return NumOfSteps;
	}
	else if(this->Step_ParentTrackID) 
	{
		NumOfSteps = this->Step_ParentTrackID->size();
		return NumOfSteps;
	}
	else if(this->Step_ParticleCharge) 
	{
		NumOfSteps = this->Step_ParticleCharge->size();
		return NumOfSteps;
	}
	else if(this->Step_ParticleLifeTime) 
	{
		NumOfSteps = this->Step_ParticleLifeTime->size();
		return NumOfSteps;
	}
	else if(this->Step_ParticleMass) 
	{
		NumOfSteps = this->Step_ParticleMass->size();
		return NumOfSteps;
	}
	else if(this->Step_ParticlePDGEncoding) 
	{
		NumOfSteps = this->Step_ParticlePDGEncoding->size();
		return NumOfSteps;
	}
	else if(this->Step_ParticleStable) 
	{
		NumOfSteps = this->Step_ParticleStable->size();
		return NumOfSteps;
	}
	else if(this->Step_ParticleWidth) 
	{
		NumOfSteps = this->Step_ParticleWidth->size();
		return NumOfSteps;
	}
	else if(this->Step_PosChangeMag) 
	{
		NumOfSteps = this->Step_PosChangeMag->size();
		return NumOfSteps;
	}
	else if(this->Step_PosChangePerp) 
	{
		NumOfSteps = this->Step_PosChangePerp->size();
		return NumOfSteps;
	}
	else if(this->Step_PosChangePhi) 
	{
		NumOfSteps = this->Step_PosChangePhi->size();
		return NumOfSteps;
	}
	else if(this->Step_PosChangeTheta) 
	{
		NumOfSteps = this->Step_PosChangeTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_PosChangeX) 
	{
		NumOfSteps = this->Step_PosChangeX->size();
		return NumOfSteps;
	}
	else if(this->Step_PosChangeY) 
	{
		NumOfSteps = this->Step_PosChangeY->size();
		return NumOfSteps;
	}
	else if(this->Step_PosChangeZ) 
	{
		NumOfSteps = this->Step_PosChangeZ->size();
		return NumOfSteps;
	}
	else if(this->Step_PrimaryTrackID) 
	{
		NumOfSteps = this->Step_PrimaryTrackID->size();
		return NumOfSteps;
	}
	else if(this->Step_RandomPosMag) 
	{
		NumOfSteps = this->Step_RandomPosMag->size();
		return NumOfSteps;
	}
	else if(this->Step_RandomPosPerp) 
	{
		NumOfSteps = this->Step_RandomPosPerp->size();
		return NumOfSteps;
	}
	else if(this->Step_RandomPosPhi) 
	{
		NumOfSteps = this->Step_RandomPosPhi->size();
		return NumOfSteps;
	}
	else if(this->Step_RandomPosTheta) 
	{
		NumOfSteps = this->Step_RandomPosTheta->size();
		return NumOfSteps;
	}
	else if(this->Step_RandomPosX) 
	{
		NumOfSteps = this->Step_RandomPosX->size();
		return NumOfSteps;
	}
	else if(this->Step_RandomPosY) 
	{
		NumOfSteps = this->Step_RandomPosY->size();
		return NumOfSteps;
	}
	else if(this->Step_RandomPosZ) 
	{
		NumOfSteps = this->Step_RandomPosZ->size();
		return NumOfSteps;
	}
	else if(this->Step_RunID) 
	{
		NumOfSteps = this->Step_RunID->size();
		return NumOfSteps;
	}
	else if(this->Step_StepNumber) 
	{
		NumOfSteps = this->Step_StepNumber->size();
		return NumOfSteps;
	}
	else if(this->Step_SumSecoKineticEnergy) 
	{
		NumOfSteps = this->Step_SumSecoKineticEnergy->size();
		return NumOfSteps;
	}
	else if(this->Step_TimeChange) 
	{
		NumOfSteps = this->Step_TimeChange->size();
		return NumOfSteps;
	}
	else if(this->Step_TrackID) 
	{
		NumOfSteps = this->Step_TrackID->size();
		return NumOfSteps;
	}
	else if(this->Step_TrackLength) 
	{
		NumOfSteps = this->Step_TrackLength->size();
		return NumOfSteps;
	}
	else if(this->Step_CreatorProcess) 
	{
		NumOfSteps = this->Step_CreatorProcess->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalLogicalVolume) 
	{
		NumOfSteps = this->Step_FinalLogicalVolume->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalMaterial) 
	{
		NumOfSteps = this->Step_FinalMaterial->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalPhysicalVolume) 
	{
		NumOfSteps = this->Step_FinalPhysicalVolume->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalProcess) 
	{
		NumOfSteps = this->Step_FinalProcess->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalRegion) 
	{
		NumOfSteps = this->Step_FinalRegion->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalSolid) 
	{
		NumOfSteps = this->Step_FinalSolid->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalSolidType) 
	{
		NumOfSteps = this->Step_FinalSolidType->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalTouchable) 
	{
		NumOfSteps = this->Step_FinalTouchable->size();
		return NumOfSteps;
	}
	else if(this->Step_FinalUnitID) 
	{
		NumOfSteps = this->Step_FinalUnitID->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialUnitID) 
	{
		NumOfSteps = this->Step_InitialUnitID->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialLogicalVolume) 
	{
		NumOfSteps = this->Step_InitialLogicalVolume->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialMaterial) 
	{
		NumOfSteps = this->Step_InitialMaterial->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialPhysicalVolume) 
	{
		NumOfSteps = this->Step_InitialPhysicalVolume->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialProcess) 
	{
		NumOfSteps = this->Step_InitialProcess->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialRegion) 
	{
		NumOfSteps = this->Step_InitialRegion->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialSolid) 
	{
		NumOfSteps = this->Step_InitialSolid->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialSolidType) 
	{
		NumOfSteps = this->Step_InitialSolidType->size();
		return NumOfSteps;
	}
	else if(this->Step_InitialTouchable) 
	{
		NumOfSteps = this->Step_InitialTouchable->size();
		return NumOfSteps;
	}
	else if(this->Step_Particle) 
	{
		NumOfSteps = this->Step_Particle->size();
		return NumOfSteps;
	}
	else if(this->Step_ParticleSubType) 
	{
		NumOfSteps = this->Step_ParticleSubType->size();
		return NumOfSteps;
	}
	else if(this->Step_ParticleType) 
	{
		NumOfSteps = this->Step_ParticleType->size();
		return NumOfSteps;
	}

	return NumOfSteps;
}



int TreeClass::LinkAvailableBranches()
{
	TObjArray* mylist = theTTree->GetListOfBranches();
	if(mylist->FindObject("Event_EventID")) 
	{
		theTTree->SetBranchAddress("Event_EventID",&Event_EventID);
	}
	if(mylist->FindObject("Event_FinalTouchablePosX")) 
	{
		theTTree->SetBranchAddress("Event_FinalTouchablePosX",&Event_FinalTouchablePosX);
	}
	if(mylist->FindObject("Event_FinalTouchablePosY")) 
	{
		theTTree->SetBranchAddress("Event_FinalTouchablePosY",&Event_FinalTouchablePosY);
	}
	if(mylist->FindObject("Event_FinalTouchablePosZ")) 
	{
		theTTree->SetBranchAddress("Event_FinalTouchablePosZ",&Event_FinalTouchablePosZ);
	}
	if(mylist->FindObject("Event_FinalTouchableEulerPhi")) 
	{
		theTTree->SetBranchAddress("Event_FinalTouchableEulerPhi",&Event_FinalTouchableEulerPhi);
	}
	if(mylist->FindObject("Event_FinalTouchableEulerTheta")) 
	{
		theTTree->SetBranchAddress("Event_FinalTouchableEulerTheta",&Event_FinalTouchableEulerTheta);
	}
	if(mylist->FindObject("Event_FinalTouchableEulerPsi")) 
	{
		theTTree->SetBranchAddress("Event_FinalTouchableEulerPsi",&Event_FinalTouchableEulerPsi);
	}
	if(mylist->FindObject("Event_InitialDensity")) 
	{
		theTTree->SetBranchAddress("Event_InitialDensity",&Event_InitialDensity);
	}
	if(mylist->FindObject("Event_InitialDirPerp")) 
	{
		theTTree->SetBranchAddress("Event_InitialDirPerp",&Event_InitialDirPerp);
	}
	if(mylist->FindObject("Event_InitialDirPhi")) 
	{
		theTTree->SetBranchAddress("Event_InitialDirPhi",&Event_InitialDirPhi);
	}
	if(mylist->FindObject("Event_InitialDirTheta")) 
	{
		theTTree->SetBranchAddress("Event_InitialDirTheta",&Event_InitialDirTheta);
	}
	if(mylist->FindObject("Event_InitialDirX")) 
	{
		theTTree->SetBranchAddress("Event_InitialDirX",&Event_InitialDirX);
	}
	if(mylist->FindObject("Event_InitialDirY")) 
	{
		theTTree->SetBranchAddress("Event_InitialDirY",&Event_InitialDirY);
	}
	if(mylist->FindObject("Event_InitialDirZ")) 
	{
		theTTree->SetBranchAddress("Event_InitialDirZ",&Event_InitialDirZ);
	}
	if(mylist->FindObject("Event_InitialElectronDensity")) 
	{
		theTTree->SetBranchAddress("Event_InitialElectronDensity",&Event_InitialElectronDensity);
	}
	if(mylist->FindObject("Event_InitialKineticEnergy")) 
	{
		theTTree->SetBranchAddress("Event_InitialKineticEnergy",&Event_InitialKineticEnergy);
	}
	if(mylist->FindObject("Event_InitialLocalDirPerp")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalDirPerp",&Event_InitialLocalDirPerp);
	}
	if(mylist->FindObject("Event_InitialLocalDirPhi")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalDirPhi",&Event_InitialLocalDirPhi);
	}
	if(mylist->FindObject("Event_InitialLocalDirTheta")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalDirTheta",&Event_InitialLocalDirTheta);
	}
	if(mylist->FindObject("Event_InitialLocalDirX")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalDirX",&Event_InitialLocalDirX);
	}
	if(mylist->FindObject("Event_InitialLocalDirY")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalDirY",&Event_InitialLocalDirY);
	}
	if(mylist->FindObject("Event_InitialLocalDirZ")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalDirZ",&Event_InitialLocalDirZ);
	}
	if(mylist->FindObject("Event_InitialLocalPosMag")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalPosMag",&Event_InitialLocalPosMag);
	}
	if(mylist->FindObject("Event_InitialLocalPosPerp")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalPosPerp",&Event_InitialLocalPosPerp);
	}
	if(mylist->FindObject("Event_InitialLocalPosPhi")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalPosPhi",&Event_InitialLocalPosPhi);
	}
	if(mylist->FindObject("Event_InitialLocalPosTheta")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalPosTheta",&Event_InitialLocalPosTheta);
	}
	if(mylist->FindObject("Event_InitialLocalPosX")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalPosX",&Event_InitialLocalPosX);
	}
	if(mylist->FindObject("Event_InitialLocalPosY")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalPosY",&Event_InitialLocalPosY);
	}
	if(mylist->FindObject("Event_InitialLocalPosZ")) 
	{
		theTTree->SetBranchAddress("Event_InitialLocalPosZ",&Event_InitialLocalPosZ);
	}
	if(mylist->FindObject("Event_InitialMomMag")) 
	{
		theTTree->SetBranchAddress("Event_InitialMomMag",&Event_InitialMomMag);
	}
	if(mylist->FindObject("Event_InitialMomPerp")) 
	{
		theTTree->SetBranchAddress("Event_InitialMomPerp",&Event_InitialMomPerp);
	}
	if(mylist->FindObject("Event_InitialMomPhi")) 
	{
		theTTree->SetBranchAddress("Event_InitialMomPhi",&Event_InitialMomPhi);
	}
	if(mylist->FindObject("Event_InitialMomTheta")) 
	{
		theTTree->SetBranchAddress("Event_InitialMomTheta",&Event_InitialMomTheta);
	}
	if(mylist->FindObject("Event_InitialMomX")) 
	{
		theTTree->SetBranchAddress("Event_InitialMomX",&Event_InitialMomX);
	}
	if(mylist->FindObject("Event_InitialMomY")) 
	{
		theTTree->SetBranchAddress("Event_InitialMomY",&Event_InitialMomY);
	}
	if(mylist->FindObject("Event_InitialMomZ")) 
	{
		theTTree->SetBranchAddress("Event_InitialMomZ",&Event_InitialMomZ);
	}
	if(mylist->FindObject("Event_InitialNuclearInterLength")) 
	{
		theTTree->SetBranchAddress("Event_InitialNuclearInterLength",&Event_InitialNuclearInterLength);
	}
	if(mylist->FindObject("Event_InitialPVCopyNumber")) 
	{
		theTTree->SetBranchAddress("Event_InitialPVCopyNumber",&Event_InitialPVCopyNumber);
	}
	if(mylist->FindObject("Event_InitialParallelPVCopyNumber")) 
	{
		theTTree->SetBranchAddress("Event_InitialParallelPVCopyNumber",&Event_InitialParallelPVCopyNumber);
	}
	if(mylist->FindObject("Event_InitialPosMag")) 
	{
		theTTree->SetBranchAddress("Event_InitialPosMag",&Event_InitialPosMag);
	}
	if(mylist->FindObject("Event_InitialPosPerp")) 
	{
		theTTree->SetBranchAddress("Event_InitialPosPerp",&Event_InitialPosPerp);
	}
	if(mylist->FindObject("Event_InitialPosPhi")) 
	{
		theTTree->SetBranchAddress("Event_InitialPosPhi",&Event_InitialPosPhi);
	}
	if(mylist->FindObject("Event_InitialPosTheta")) 
	{
		theTTree->SetBranchAddress("Event_InitialPosTheta",&Event_InitialPosTheta);
	}
	if(mylist->FindObject("Event_InitialPosX")) 
	{
		theTTree->SetBranchAddress("Event_InitialPosX",&Event_InitialPosX);
	}
	if(mylist->FindObject("Event_InitialPosY")) 
	{
		theTTree->SetBranchAddress("Event_InitialPosY",&Event_InitialPosY);
	}
	if(mylist->FindObject("Event_InitialPosZ")) 
	{
		theTTree->SetBranchAddress("Event_InitialPosZ",&Event_InitialPosZ);
	}
	if(mylist->FindObject("Event_InitialPressure")) 
	{
		theTTree->SetBranchAddress("Event_InitialPressure",&Event_InitialPressure);
	}
	if(mylist->FindObject("Event_InitialRadLength")) 
	{
		theTTree->SetBranchAddress("Event_InitialRadLength",&Event_InitialRadLength);
	}
	if(mylist->FindObject("Event_InitialTemperature")) 
	{
		theTTree->SetBranchAddress("Event_InitialTemperature",&Event_InitialTemperature);
	}
	if(mylist->FindObject("Event_InitialTime")) 
	{
		theTTree->SetBranchAddress("Event_InitialTime",&Event_InitialTime);
	}
	if(mylist->FindObject("Event_InitialTotalEnergy")) 
	{
		theTTree->SetBranchAddress("Event_InitialTotalEnergy",&Event_InitialTotalEnergy);
	}
	if(mylist->FindObject("Event_InitialTouchablePosX")) 
	{
		theTTree->SetBranchAddress("Event_InitialTouchablePosX",&Event_InitialTouchablePosX);
	}
	if(mylist->FindObject("Event_InitialTouchablePosY")) 
	{
		theTTree->SetBranchAddress("Event_InitialTouchablePosY",&Event_InitialTouchablePosY);
	}
	if(mylist->FindObject("Event_InitialTouchablePosZ")) 
	{
		theTTree->SetBranchAddress("Event_InitialTouchablePosZ",&Event_InitialTouchablePosZ);
	}
	if(mylist->FindObject("Event_InitialTouchableEulerPhi")) 
	{
		theTTree->SetBranchAddress("Event_InitialTouchableEulerPhi",&Event_InitialTouchableEulerPhi);
	}
	if(mylist->FindObject("Event_InitialTouchableEulerTheta")) 
	{
		theTTree->SetBranchAddress("Event_InitialTouchableEulerTheta",&Event_InitialTouchableEulerTheta);
	}
	if(mylist->FindObject("Event_InitialTouchableEulerPsi")) 
	{
		theTTree->SetBranchAddress("Event_InitialTouchableEulerPsi",&Event_InitialTouchableEulerPsi);
	}
	if(mylist->FindObject("Event_RunID")) 
	{
		theTTree->SetBranchAddress("Event_RunID",&Event_RunID);
	}
	if (mylist->FindObject("Event_InitialLogicalVolume")) 
	{
		Event_InitialLogicalVolume_ptr = &Event_InitialLogicalVolume;
		theTTree->SetBranchAddress("Event_InitialLogicalVolume", &Event_InitialLogicalVolume_ptr);
	}
	if (mylist->FindObject("Event_InitialMaterial")) 
	{
		Event_InitialMaterial_ptr = &Event_InitialMaterial;
		theTTree->SetBranchAddress("Event_InitialMaterial", &Event_InitialMaterial_ptr);
	}
	if (mylist->FindObject("Event_InitialPhysicalVolume")) 
	{
		Event_InitialPhysicalVolume_ptr = &Event_InitialPhysicalVolume;
		theTTree->SetBranchAddress("Event_InitialPhysicalVolume", &Event_InitialPhysicalVolume_ptr);
	}
	if (mylist->FindObject("Event_InitialRegion")) 
	{
		Event_InitialRegion_ptr = &Event_InitialRegion;
		theTTree->SetBranchAddress("Event_InitialRegion", &Event_InitialRegion_ptr);
	}
	if (mylist->FindObject("Event_InitialSolid")) 
	{
		Event_InitialSolid_ptr = &Event_InitialSolid;
		theTTree->SetBranchAddress("Event_InitialSolid", &Event_InitialSolid_ptr);
	}
	if (mylist->FindObject("Event_InitialSolidType")) 
	{
		Event_InitialSolidType_ptr = &Event_InitialSolidType;
		theTTree->SetBranchAddress("Event_InitialSolidType", &Event_InitialSolidType_ptr);
	}
	if (mylist->FindObject("Event_InitialTouchable")) 
	{
		Event_InitialTouchable_ptr = &Event_InitialTouchable;
		theTTree->SetBranchAddress("Event_InitialTouchable", &Event_InitialTouchable_ptr);
	}
	if(mylist->FindObject("Step_AccumulatedDose")) 
	{
		theTTree->SetBranchAddress("Step_AccumulatedDose",&Step_AccumulatedDose);
	}
	if(mylist->FindObject("Step_AccumulatedEnergyDeposited")) 
	{
		theTTree->SetBranchAddress("Step_AccumulatedEnergyDeposited",&Step_AccumulatedEnergyDeposited);
	}
	if(mylist->FindObject("Step_AccumulatedEnergyLost")) 
	{
		theTTree->SetBranchAddress("Step_AccumulatedEnergyLost",&Step_AccumulatedEnergyLost);
	}
	if(mylist->FindObject("Step_AccumulatedKerma")) 
	{
		theTTree->SetBranchAddress("Step_AccumulatedKerma",&Step_AccumulatedKerma);
	}
	if(mylist->FindObject("Step_AccumulatedLength")) 
	{
		theTTree->SetBranchAddress("Step_AccumulatedLength",&Step_AccumulatedLength);
	}
	if(mylist->FindObject("Step_AccumulatedNonIonizingEnergyDeposited")) 
	{
		theTTree->SetBranchAddress("Step_AccumulatedNonIonizingEnergyDeposited",&Step_AccumulatedNonIonizingEnergyDeposited);
	}
	if(mylist->FindObject("Step_AngleChange")) 
	{
		theTTree->SetBranchAddress("Step_AngleChange",&Step_AngleChange);
	}
	if(mylist->FindObject("Step_DirChangeMag")) 
	{
		theTTree->SetBranchAddress("Step_DirChangeMag",&Step_DirChangeMag);
	}
	if(mylist->FindObject("Step_DirChangePerp")) 
	{
		theTTree->SetBranchAddress("Step_DirChangePerp",&Step_DirChangePerp);
	}
	if(mylist->FindObject("Step_DirChangePhi")) 
	{
		theTTree->SetBranchAddress("Step_DirChangePhi",&Step_DirChangePhi);
	}
	if(mylist->FindObject("Step_DirChangeTheta")) 
	{
		theTTree->SetBranchAddress("Step_DirChangeTheta",&Step_DirChangeTheta);
	}
	if(mylist->FindObject("Step_DirChangeX")) 
	{
		theTTree->SetBranchAddress("Step_DirChangeX",&Step_DirChangeX);
	}
	if(mylist->FindObject("Step_DirChangeY")) 
	{
		theTTree->SetBranchAddress("Step_DirChangeY",&Step_DirChangeY);
	}
	if(mylist->FindObject("Step_DirChangeZ")) 
	{
		theTTree->SetBranchAddress("Step_DirChangeZ",&Step_DirChangeZ);
	}
	if(mylist->FindObject("Step_EventID")) 
	{
		theTTree->SetBranchAddress("Step_EventID",&Step_EventID);
	}
	if(mylist->FindObject("Step_FinalDEDX")) 
	{
		theTTree->SetBranchAddress("Step_FinalDEDX",&Step_FinalDEDX);
	}
	if(mylist->FindObject("Step_FinalDensity")) 
	{
		theTTree->SetBranchAddress("Step_FinalDensity",&Step_FinalDensity);
	}
	if(mylist->FindObject("Step_FinalDirPerp")) 
	{
		theTTree->SetBranchAddress("Step_FinalDirPerp",&Step_FinalDirPerp);
	}
	if(mylist->FindObject("Step_FinalDirPhi")) 
	{
		theTTree->SetBranchAddress("Step_FinalDirPhi",&Step_FinalDirPhi);
	}
	if(mylist->FindObject("Step_FinalDirTheta")) 
	{
		theTTree->SetBranchAddress("Step_FinalDirTheta",&Step_FinalDirTheta);
	}
	if(mylist->FindObject("Step_FinalDirX")) 
	{
		theTTree->SetBranchAddress("Step_FinalDirX",&Step_FinalDirX);
	}
	if(mylist->FindObject("Step_FinalDirY")) 
	{
		theTTree->SetBranchAddress("Step_FinalDirY",&Step_FinalDirY);
	}
	if(mylist->FindObject("Step_FinalDirZ")) 
	{
		theTTree->SetBranchAddress("Step_FinalDirZ",&Step_FinalDirZ);
	}
	if(mylist->FindObject("Step_FinalElectronDensity")) 
	{
		theTTree->SetBranchAddress("Step_FinalElectronDensity",&Step_FinalElectronDensity);
	}
	if(mylist->FindObject("Step_FinalGammaEquivalentDose")) 
	{
		theTTree->SetBranchAddress("Step_FinalGammaEquivalentDose",&Step_FinalGammaEquivalentDose);
	}
	if(mylist->FindObject("Step_FinalKineticEnergy")) 
	{
		theTTree->SetBranchAddress("Step_FinalKineticEnergy",&Step_FinalKineticEnergy);
	}
	if(mylist->FindObject("Step_FinalLocalDirPerp")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalDirPerp",&Step_FinalLocalDirPerp);
	}
	if(mylist->FindObject("Step_FinalLocalDirPhi")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalDirPhi",&Step_FinalLocalDirPhi);
	}
	if(mylist->FindObject("Step_FinalLocalDirTheta")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalDirTheta",&Step_FinalLocalDirTheta);
	}
	if(mylist->FindObject("Step_FinalLocalDirX")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalDirX",&Step_FinalLocalDirX);
	}
	if(mylist->FindObject("Step_FinalLocalDirY")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalDirY",&Step_FinalLocalDirY);
	}
	if(mylist->FindObject("Step_FinalLocalDirZ")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalDirZ",&Step_FinalLocalDirZ);
	}
	if(mylist->FindObject("Step_FinalLocalInPreDirPerp")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPreDirPerp",&Step_FinalLocalInPreDirPerp);
	}
	if(mylist->FindObject("Step_FinalLocalInPreDirPhi")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPreDirPhi",&Step_FinalLocalInPreDirPhi);
	}
	if(mylist->FindObject("Step_FinalLocalInPreDirTheta")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPreDirTheta",&Step_FinalLocalInPreDirTheta);
	}
	if(mylist->FindObject("Step_FinalLocalInPreDirX")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPreDirX",&Step_FinalLocalInPreDirX);
	}
	if(mylist->FindObject("Step_FinalLocalInPreDirY")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPreDirY",&Step_FinalLocalInPreDirY);
	}
	if(mylist->FindObject("Step_FinalLocalInPreDirZ")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPreDirZ",&Step_FinalLocalInPreDirZ);
	}
	if(mylist->FindObject("Step_FinalLocalInPrePosMag")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPrePosMag",&Step_FinalLocalInPrePosMag);
	}
	if(mylist->FindObject("Step_FinalLocalInPrePosPerp")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPrePosPerp",&Step_FinalLocalInPrePosPerp);
	}
	if(mylist->FindObject("Step_FinalLocalInPrePosPhi")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPrePosPhi",&Step_FinalLocalInPrePosPhi);
	}
	if(mylist->FindObject("Step_FinalLocalInPrePosTheta")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPrePosTheta",&Step_FinalLocalInPrePosTheta);
	}
	if(mylist->FindObject("Step_FinalLocalInPrePosX")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPrePosX",&Step_FinalLocalInPrePosX);
	}
	if(mylist->FindObject("Step_FinalLocalInPrePosY")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPrePosY",&Step_FinalLocalInPrePosY);
	}
	if(mylist->FindObject("Step_FinalLocalInPrePosZ")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalInPrePosZ",&Step_FinalLocalInPrePosZ);
	}
	if(mylist->FindObject("Step_FinalLocalPosMag")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalPosMag",&Step_FinalLocalPosMag);
	}
	if(mylist->FindObject("Step_FinalLocalPosPerp")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalPosPerp",&Step_FinalLocalPosPerp);
	}
	if(mylist->FindObject("Step_FinalLocalPosPhi")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalPosPhi",&Step_FinalLocalPosPhi);
	}
	if(mylist->FindObject("Step_FinalLocalPosTheta")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalPosTheta",&Step_FinalLocalPosTheta);
	}
	if(mylist->FindObject("Step_FinalLocalPosX")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalPosX",&Step_FinalLocalPosX);
	}
	if(mylist->FindObject("Step_FinalLocalPosY")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalPosY",&Step_FinalLocalPosY);
	}
	if(mylist->FindObject("Step_FinalLocalPosZ")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalPosZ",&Step_FinalLocalPosZ);
	}
	if(mylist->FindObject("Step_FinalLocalTime")) 
	{
		theTTree->SetBranchAddress("Step_FinalLocalTime",&Step_FinalLocalTime);
	}
	if(mylist->FindObject("Step_FinalMomMag")) 
	{
		theTTree->SetBranchAddress("Step_FinalMomMag",&Step_FinalMomMag);
	}
	if(mylist->FindObject("Step_FinalMomPerp")) 
	{
		theTTree->SetBranchAddress("Step_FinalMomPerp",&Step_FinalMomPerp);
	}
	if(mylist->FindObject("Step_FinalMomPhi")) 
	{
		theTTree->SetBranchAddress("Step_FinalMomPhi",&Step_FinalMomPhi);
	}
	if(mylist->FindObject("Step_FinalMomTheta")) 
	{
		theTTree->SetBranchAddress("Step_FinalMomTheta",&Step_FinalMomTheta);
	}
	if(mylist->FindObject("Step_FinalMomX")) 
	{
		theTTree->SetBranchAddress("Step_FinalMomX",&Step_FinalMomX);
	}
	if(mylist->FindObject("Step_FinalMomY")) 
	{
		theTTree->SetBranchAddress("Step_FinalMomY",&Step_FinalMomY);
	}
	if(mylist->FindObject("Step_FinalMomZ")) 
	{
		theTTree->SetBranchAddress("Step_FinalMomZ",&Step_FinalMomZ);
	}
	if(mylist->FindObject("Step_FinalNeutronEquivalentDose")) 
	{
		theTTree->SetBranchAddress("Step_FinalNeutronEquivalentDose",&Step_FinalNeutronEquivalentDose);
	}
	if(mylist->FindObject("Step_FinalNuclearInterLength")) 
	{
		theTTree->SetBranchAddress("Step_FinalNuclearInterLength",&Step_FinalNuclearInterLength);
	}
	if(mylist->FindObject("Step_FinalPVCopyNumber")) 
	{
		theTTree->SetBranchAddress("Step_FinalPVCopyNumber",&Step_FinalPVCopyNumber);
	}
	if(mylist->FindObject("Step_FinalParallelPVCopyNumber")) 
	{
		theTTree->SetBranchAddress("Step_FinalParallelPVCopyNumber",&Step_FinalParallelPVCopyNumber);
	}
	if(mylist->FindObject("Step_FinalPosMag")) 
	{
		theTTree->SetBranchAddress("Step_FinalPosMag",&Step_FinalPosMag);
	}
	if(mylist->FindObject("Step_FinalPosPerp")) 
	{
		theTTree->SetBranchAddress("Step_FinalPosPerp",&Step_FinalPosPerp);
	}
	if(mylist->FindObject("Step_FinalPosPhi")) 
	{
		theTTree->SetBranchAddress("Step_FinalPosPhi",&Step_FinalPosPhi);
	}
	if(mylist->FindObject("Step_FinalPosTheta")) 
	{
		theTTree->SetBranchAddress("Step_FinalPosTheta",&Step_FinalPosTheta);
	}
	if(mylist->FindObject("Step_FinalPosX")) 
	{
		theTTree->SetBranchAddress("Step_FinalPosX",&Step_FinalPosX);
	}
	if(mylist->FindObject("Step_FinalPosY")) 
	{
		theTTree->SetBranchAddress("Step_FinalPosY",&Step_FinalPosY);
	}
	if(mylist->FindObject("Step_FinalPosZ")) 
	{
		theTTree->SetBranchAddress("Step_FinalPosZ",&Step_FinalPosZ);
	}
	if(mylist->FindObject("Step_FinalPressure")) 
	{
		theTTree->SetBranchAddress("Step_FinalPressure",&Step_FinalPressure);
	}
	if(mylist->FindObject("Step_FinalProperTime")) 
	{
		theTTree->SetBranchAddress("Step_FinalProperTime",&Step_FinalProperTime);
	}
	if(mylist->FindObject("Step_FinalRadLength")) 
	{
		theTTree->SetBranchAddress("Step_FinalRadLength",&Step_FinalRadLength);
	}
	if(mylist->FindObject("Step_FinalTemperature")) 
	{
		theTTree->SetBranchAddress("Step_FinalTemperature",&Step_FinalTemperature);
	}
	if(mylist->FindObject("Step_FinalTime")) 
	{
		theTTree->SetBranchAddress("Step_FinalTime",&Step_FinalTime);
	}
	if(mylist->FindObject("Step_FinalTotalEnergy")) 
	{
		theTTree->SetBranchAddress("Step_FinalTotalEnergy",&Step_FinalTotalEnergy);
	}
	if(mylist->FindObject("Step_FinalTouchablePosX")) 
	{
		theTTree->SetBranchAddress("Step_FinalTouchablePosX",&Step_FinalTouchablePosX);
	}
	if(mylist->FindObject("Step_FinalTouchablePosY")) 
	{
		theTTree->SetBranchAddress("Step_FinalTouchablePosY",&Step_FinalTouchablePosY);
	}
	if(mylist->FindObject("Step_FinalTouchablePosZ")) 
	{
		theTTree->SetBranchAddress("Step_FinalTouchablePosZ",&Step_FinalTouchablePosZ);
	}
	if(mylist->FindObject("Step_FinalTouchableEulerPhi")) 
	{
		theTTree->SetBranchAddress("Step_FinalTouchableEulerPhi",&Step_FinalTouchableEulerPhi);
	}
	if(mylist->FindObject("Step_FinalTouchableEulerTheta")) 
	{
		theTTree->SetBranchAddress("Step_FinalTouchableEulerTheta",&Step_FinalTouchableEulerTheta);
	}
	if(mylist->FindObject("Step_FinalTouchableEulerPsi")) 
	{
		theTTree->SetBranchAddress("Step_FinalTouchableEulerPsi",&Step_FinalTouchableEulerPsi);
	}
	if(mylist->FindObject("Step_FinalWeight")) 
	{
		theTTree->SetBranchAddress("Step_FinalWeight",&Step_FinalWeight);
	}
	if(mylist->FindObject("Step_InitialDEDX")) 
	{
		theTTree->SetBranchAddress("Step_InitialDEDX",&Step_InitialDEDX);
	}
	if(mylist->FindObject("Step_InitialDensity")) 
	{
		theTTree->SetBranchAddress("Step_InitialDensity",&Step_InitialDensity);
	}
	if(mylist->FindObject("Step_InitialDirPerp")) 
	{
		theTTree->SetBranchAddress("Step_InitialDirPerp",&Step_InitialDirPerp);
	}
	if(mylist->FindObject("Step_InitialDirPhi")) 
	{
		theTTree->SetBranchAddress("Step_InitialDirPhi",&Step_InitialDirPhi);
	}
	if(mylist->FindObject("Step_InitialDirTheta")) 
	{
		theTTree->SetBranchAddress("Step_InitialDirTheta",&Step_InitialDirTheta);
	}
	if(mylist->FindObject("Step_InitialDirX")) 
	{
		theTTree->SetBranchAddress("Step_InitialDirX",&Step_InitialDirX);
	}
	if(mylist->FindObject("Step_InitialDirY")) 
	{
		theTTree->SetBranchAddress("Step_InitialDirY",&Step_InitialDirY);
	}
	if(mylist->FindObject("Step_InitialDirZ")) 
	{
		theTTree->SetBranchAddress("Step_InitialDirZ",&Step_InitialDirZ);
	}
	if(mylist->FindObject("Step_InitialElectronDensity")) 
	{
		theTTree->SetBranchAddress("Step_InitialElectronDensity",&Step_InitialElectronDensity);
	}
	if(mylist->FindObject("Step_InitialGammaEquivalentDose")) 
	{
		theTTree->SetBranchAddress("Step_InitialGammaEquivalentDose",&Step_InitialGammaEquivalentDose);
	}
	if(mylist->FindObject("Step_InitialKineticEnergy")) 
	{
		theTTree->SetBranchAddress("Step_InitialKineticEnergy",&Step_InitialKineticEnergy);
	}
	if(mylist->FindObject("Step_InitialLocalDirPerp")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalDirPerp",&Step_InitialLocalDirPerp);
	}
	if(mylist->FindObject("Step_InitialLocalDirPhi")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalDirPhi",&Step_InitialLocalDirPhi);
	}
	if(mylist->FindObject("Step_InitialLocalDirTheta")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalDirTheta",&Step_InitialLocalDirTheta);
	}
	if(mylist->FindObject("Step_InitialLocalDirX")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalDirX",&Step_InitialLocalDirX);
	}
	if(mylist->FindObject("Step_InitialLocalDirY")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalDirY",&Step_InitialLocalDirY);
	}
	if(mylist->FindObject("Step_InitialLocalDirZ")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalDirZ",&Step_InitialLocalDirZ);
	}
	if(mylist->FindObject("Step_InitialLocalPosMag")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalPosMag",&Step_InitialLocalPosMag);
	}
	if(mylist->FindObject("Step_InitialLocalPosPerp")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalPosPerp",&Step_InitialLocalPosPerp);
	}
	if(mylist->FindObject("Step_InitialLocalPosPhi")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalPosPhi",&Step_InitialLocalPosPhi);
	}
	if(mylist->FindObject("Step_InitialLocalPosTheta")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalPosTheta",&Step_InitialLocalPosTheta);
	}
	if(mylist->FindObject("Step_InitialLocalPosX")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalPosX",&Step_InitialLocalPosX);
	}
	if(mylist->FindObject("Step_InitialLocalPosY")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalPosY",&Step_InitialLocalPosY);
	}
	if(mylist->FindObject("Step_InitialLocalPosZ")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalPosZ",&Step_InitialLocalPosZ);
	}
	if(mylist->FindObject("Step_InitialLocalTime")) 
	{
		theTTree->SetBranchAddress("Step_InitialLocalTime",&Step_InitialLocalTime);
	}
	if(mylist->FindObject("Step_InitialMomMag")) 
	{
		theTTree->SetBranchAddress("Step_InitialMomMag",&Step_InitialMomMag);
	}
	if(mylist->FindObject("Step_InitialMomPerp")) 
	{
		theTTree->SetBranchAddress("Step_InitialMomPerp",&Step_InitialMomPerp);
	}
	if(mylist->FindObject("Step_InitialMomPhi")) 
	{
		theTTree->SetBranchAddress("Step_InitialMomPhi",&Step_InitialMomPhi);
	}
	if(mylist->FindObject("Step_InitialMomTheta")) 
	{
		theTTree->SetBranchAddress("Step_InitialMomTheta",&Step_InitialMomTheta);
	}
	if(mylist->FindObject("Step_InitialMomX")) 
	{
		theTTree->SetBranchAddress("Step_InitialMomX",&Step_InitialMomX);
	}
	if(mylist->FindObject("Step_InitialMomY")) 
	{
		theTTree->SetBranchAddress("Step_InitialMomY",&Step_InitialMomY);
	}
	if(mylist->FindObject("Step_InitialMomZ")) 
	{
		theTTree->SetBranchAddress("Step_InitialMomZ",&Step_InitialMomZ);
	}
	if(mylist->FindObject("Step_InitialNeutronEquivalentDose")) 
	{
		theTTree->SetBranchAddress("Step_InitialNeutronEquivalentDose",&Step_InitialNeutronEquivalentDose);
	}
	if(mylist->FindObject("Step_InitialNuclearInterLength")) 
	{
		theTTree->SetBranchAddress("Step_InitialNuclearInterLength",&Step_InitialNuclearInterLength);
	}
	if(mylist->FindObject("Step_InitialPVCopyNumber")) 
	{
		theTTree->SetBranchAddress("Step_InitialPVCopyNumber",&Step_InitialPVCopyNumber);
	}
	if(mylist->FindObject("Step_InitialParallelPVCopyNumber")) 
	{
		theTTree->SetBranchAddress("Step_InitialParallelPVCopyNumber",&Step_InitialParallelPVCopyNumber);
	}
	if(mylist->FindObject("Step_InitialPosMag")) 
	{
		theTTree->SetBranchAddress("Step_InitialPosMag",&Step_InitialPosMag);
	}
	if(mylist->FindObject("Step_InitialPosPerp")) 
	{
		theTTree->SetBranchAddress("Step_InitialPosPerp",&Step_InitialPosPerp);
	}
	if(mylist->FindObject("Step_InitialPosPhi")) 
	{
		theTTree->SetBranchAddress("Step_InitialPosPhi",&Step_InitialPosPhi);
	}
	if(mylist->FindObject("Step_InitialPosTheta")) 
	{
		theTTree->SetBranchAddress("Step_InitialPosTheta",&Step_InitialPosTheta);
	}
	if(mylist->FindObject("Step_InitialPosX")) 
	{
		theTTree->SetBranchAddress("Step_InitialPosX",&Step_InitialPosX);
	}
	if(mylist->FindObject("Step_InitialPosY")) 
	{
		theTTree->SetBranchAddress("Step_InitialPosY",&Step_InitialPosY);
	}
	if(mylist->FindObject("Step_InitialPosZ")) 
	{
		theTTree->SetBranchAddress("Step_InitialPosZ",&Step_InitialPosZ);
	}
	if(mylist->FindObject("Step_InitialPressure")) 
	{
		theTTree->SetBranchAddress("Step_InitialPressure",&Step_InitialPressure);
	}
	if(mylist->FindObject("Step_InitialProperTime")) 
	{
		theTTree->SetBranchAddress("Step_InitialProperTime",&Step_InitialProperTime);
	}
	if(mylist->FindObject("Step_InitialRadLength")) 
	{
		theTTree->SetBranchAddress("Step_InitialRadLength",&Step_InitialRadLength);
	}
	if(mylist->FindObject("Step_InitialTemperature")) 
	{
		theTTree->SetBranchAddress("Step_InitialTemperature",&Step_InitialTemperature);
	}
	if(mylist->FindObject("Step_InitialTime")) 
	{
		theTTree->SetBranchAddress("Step_InitialTime",&Step_InitialTime);
	}
	if(mylist->FindObject("Step_InitialTotalEnergy")) 
	{
		theTTree->SetBranchAddress("Step_InitialTotalEnergy",&Step_InitialTotalEnergy);
	}
	if(mylist->FindObject("Step_InitialTouchablePosX")) 
	{
		theTTree->SetBranchAddress("Step_InitialTouchablePosX",&Step_InitialTouchablePosX);
	}
	if(mylist->FindObject("Step_InitialTouchablePosY")) 
	{
		theTTree->SetBranchAddress("Step_InitialTouchablePosY",&Step_InitialTouchablePosY);
	}
	if(mylist->FindObject("Step_InitialTouchablePosZ")) 
	{
		theTTree->SetBranchAddress("Step_InitialTouchablePosZ",&Step_InitialTouchablePosZ);
	}
	if(mylist->FindObject("Step_InitialTouchableEulerPhi")) 
	{
		theTTree->SetBranchAddress("Step_InitialTouchableEulerPhi",&Step_InitialTouchableEulerPhi);
	}
	if(mylist->FindObject("Step_InitialTouchableEulerTheta")) 
	{
		theTTree->SetBranchAddress("Step_InitialTouchableEulerTheta",&Step_InitialTouchableEulerTheta);
	}
	if(mylist->FindObject("Step_InitialTouchableEulerPsi")) 
	{
		theTTree->SetBranchAddress("Step_InitialTouchableEulerPsi",&Step_InitialTouchableEulerPsi);
	}
	if(mylist->FindObject("Step_InitialWeight")) 
	{
		theTTree->SetBranchAddress("Step_InitialWeight",&Step_InitialWeight);
	}
	if(mylist->FindObject("Step_KineticEnergyChange")) 
	{
		theTTree->SetBranchAddress("Step_KineticEnergyChange",&Step_KineticEnergyChange);
	}
	if(mylist->FindObject("Step_MomChangeMag")) 
	{
		theTTree->SetBranchAddress("Step_MomChangeMag",&Step_MomChangeMag);
	}
	if(mylist->FindObject("Step_MomChangePerp")) 
	{
		theTTree->SetBranchAddress("Step_MomChangePerp",&Step_MomChangePerp);
	}
	if(mylist->FindObject("Step_MomChangePhi")) 
	{
		theTTree->SetBranchAddress("Step_MomChangePhi",&Step_MomChangePhi);
	}
	if(mylist->FindObject("Step_MomChangeTheta")) 
	{
		theTTree->SetBranchAddress("Step_MomChangeTheta",&Step_MomChangeTheta);
	}
	if(mylist->FindObject("Step_MomChangeX")) 
	{
		theTTree->SetBranchAddress("Step_MomChangeX",&Step_MomChangeX);
	}
	if(mylist->FindObject("Step_MomChangeY")) 
	{
		theTTree->SetBranchAddress("Step_MomChangeY",&Step_MomChangeY);
	}
	if(mylist->FindObject("Step_MomChangeZ")) 
	{
		theTTree->SetBranchAddress("Step_MomChangeZ",&Step_MomChangeZ);
	}
	if(mylist->FindObject("Step_NofSecondaries")) 
	{
		theTTree->SetBranchAddress("Step_NofSecondaries",&Step_NofSecondaries);
	}
	if(mylist->FindObject("Step_ParentTrackID")) 
	{
		theTTree->SetBranchAddress("Step_ParentTrackID",&Step_ParentTrackID);
	}
	if(mylist->FindObject("Step_ParticleCharge")) 
	{
		theTTree->SetBranchAddress("Step_ParticleCharge",&Step_ParticleCharge);
	}
	if(mylist->FindObject("Step_ParticleLifeTime")) 
	{
		theTTree->SetBranchAddress("Step_ParticleLifeTime",&Step_ParticleLifeTime);
	}
	if(mylist->FindObject("Step_ParticleMass")) 
	{
		theTTree->SetBranchAddress("Step_ParticleMass",&Step_ParticleMass);
	}
	if(mylist->FindObject("Step_ParticlePDGEncoding")) 
	{
		theTTree->SetBranchAddress("Step_ParticlePDGEncoding",&Step_ParticlePDGEncoding);
	}
	if(mylist->FindObject("Step_ParticleStable")) 
	{
		theTTree->SetBranchAddress("Step_ParticleStable",&Step_ParticleStable);
	}
	if(mylist->FindObject("Step_ParticleWidth")) 
	{
		theTTree->SetBranchAddress("Step_ParticleWidth",&Step_ParticleWidth);
	}
	if(mylist->FindObject("Step_PosChangeMag")) 
	{
		theTTree->SetBranchAddress("Step_PosChangeMag",&Step_PosChangeMag);
	}
	if(mylist->FindObject("Step_PosChangePerp")) 
	{
		theTTree->SetBranchAddress("Step_PosChangePerp",&Step_PosChangePerp);
	}
	if(mylist->FindObject("Step_PosChangePhi")) 
	{
		theTTree->SetBranchAddress("Step_PosChangePhi",&Step_PosChangePhi);
	}
	if(mylist->FindObject("Step_PosChangeTheta")) 
	{
		theTTree->SetBranchAddress("Step_PosChangeTheta",&Step_PosChangeTheta);
	}
	if(mylist->FindObject("Step_PosChangeX")) 
	{
		theTTree->SetBranchAddress("Step_PosChangeX",&Step_PosChangeX);
	}
	if(mylist->FindObject("Step_PosChangeY")) 
	{
		theTTree->SetBranchAddress("Step_PosChangeY",&Step_PosChangeY);
	}
	if(mylist->FindObject("Step_PosChangeZ")) 
	{
		theTTree->SetBranchAddress("Step_PosChangeZ",&Step_PosChangeZ);
	}
	if(mylist->FindObject("Step_PrimaryTrackID")) 
	{
		theTTree->SetBranchAddress("Step_PrimaryTrackID",&Step_PrimaryTrackID);
	}
	if(mylist->FindObject("Step_RandomPosMag")) 
	{
		theTTree->SetBranchAddress("Step_RandomPosMag",&Step_RandomPosMag);
	}
	if(mylist->FindObject("Step_RandomPosPerp")) 
	{
		theTTree->SetBranchAddress("Step_RandomPosPerp",&Step_RandomPosPerp);
	}
	if(mylist->FindObject("Step_RandomPosPhi")) 
	{
		theTTree->SetBranchAddress("Step_RandomPosPhi",&Step_RandomPosPhi);
	}
	if(mylist->FindObject("Step_RandomPosTheta")) 
	{
		theTTree->SetBranchAddress("Step_RandomPosTheta",&Step_RandomPosTheta);
	}
	if(mylist->FindObject("Step_RandomPosX")) 
	{
		theTTree->SetBranchAddress("Step_RandomPosX",&Step_RandomPosX);
	}
	if(mylist->FindObject("Step_RandomPosY")) 
	{
		theTTree->SetBranchAddress("Step_RandomPosY",&Step_RandomPosY);
	}
	if(mylist->FindObject("Step_RandomPosZ")) 
	{
		theTTree->SetBranchAddress("Step_RandomPosZ",&Step_RandomPosZ);
	}
	if(mylist->FindObject("Step_RunID")) 
	{
		theTTree->SetBranchAddress("Step_RunID",&Step_RunID);
	}
	if(mylist->FindObject("Step_StepNumber")) 
	{
		theTTree->SetBranchAddress("Step_StepNumber",&Step_StepNumber);
	}
	if(mylist->FindObject("Step_SumSecoKineticEnergy")) 
	{
		theTTree->SetBranchAddress("Step_SumSecoKineticEnergy",&Step_SumSecoKineticEnergy);
	}
	if(mylist->FindObject("Step_TimeChange")) 
	{
		theTTree->SetBranchAddress("Step_TimeChange",&Step_TimeChange);
	}
	if(mylist->FindObject("Step_TrackID")) 
	{
		theTTree->SetBranchAddress("Step_TrackID",&Step_TrackID);
	}
	if(mylist->FindObject("Step_TrackLength")) 
	{
		theTTree->SetBranchAddress("Step_TrackLength",&Step_TrackLength);
	}
	if(mylist->FindObject("Step_CreatorProcess")) 
	{
		theTTree->SetBranchAddress("Step_CreatorProcess",&Step_CreatorProcess);
	}
	if(mylist->FindObject("Step_FinalLogicalVolume")) 
	{
		theTTree->SetBranchAddress("Step_FinalLogicalVolume",&Step_FinalLogicalVolume);
	}
	if(mylist->FindObject("Step_FinalMaterial")) 
	{
		theTTree->SetBranchAddress("Step_FinalMaterial",&Step_FinalMaterial);
	}
	if(mylist->FindObject("Step_FinalPhysicalVolume")) 
	{
		theTTree->SetBranchAddress("Step_FinalPhysicalVolume",&Step_FinalPhysicalVolume);
	}
	if(mylist->FindObject("Step_FinalProcess")) 
	{
		theTTree->SetBranchAddress("Step_FinalProcess",&Step_FinalProcess);
	}
	if(mylist->FindObject("Step_FinalRegion")) 
	{
		theTTree->SetBranchAddress("Step_FinalRegion",&Step_FinalRegion);
	}
	if(mylist->FindObject("Step_FinalSolid")) 
	{
		theTTree->SetBranchAddress("Step_FinalSolid",&Step_FinalSolid);
	}
	if(mylist->FindObject("Step_FinalSolidType")) 
	{
		theTTree->SetBranchAddress("Step_FinalSolidType",&Step_FinalSolidType);
	}
	if(mylist->FindObject("Step_FinalTouchable")) 
	{
		theTTree->SetBranchAddress("Step_FinalTouchable",&Step_FinalTouchable);
	}
	if(mylist->FindObject("Step_FinalUnitID")) 
	{
		theTTree->SetBranchAddress("Step_FinalUnitID",&Step_FinalUnitID);
	}
	if(mylist->FindObject("Step_InitialUnitID")) 
	{
		theTTree->SetBranchAddress("Step_InitialUnitID",&Step_InitialUnitID);
	}
	if(mylist->FindObject("Step_InitialLogicalVolume")) 
	{
		theTTree->SetBranchAddress("Step_InitialLogicalVolume",&Step_InitialLogicalVolume);
	}
	if(mylist->FindObject("Step_InitialMaterial")) 
	{
		theTTree->SetBranchAddress("Step_InitialMaterial",&Step_InitialMaterial);
	}
	if(mylist->FindObject("Step_InitialPhysicalVolume")) 
	{
		theTTree->SetBranchAddress("Step_InitialPhysicalVolume",&Step_InitialPhysicalVolume);
	}
	if(mylist->FindObject("Step_InitialProcess")) 
	{
		theTTree->SetBranchAddress("Step_InitialProcess",&Step_InitialProcess);
	}
	if(mylist->FindObject("Step_InitialRegion")) 
	{
		theTTree->SetBranchAddress("Step_InitialRegion",&Step_InitialRegion);
	}
	if(mylist->FindObject("Step_InitialSolid")) 
	{
		theTTree->SetBranchAddress("Step_InitialSolid",&Step_InitialSolid);
	}
	if(mylist->FindObject("Step_InitialSolidType")) 
	{
		theTTree->SetBranchAddress("Step_InitialSolidType",&Step_InitialSolidType);
	}
	if(mylist->FindObject("Step_InitialTouchable")) 
	{
		theTTree->SetBranchAddress("Step_InitialTouchable",&Step_InitialTouchable);
	}
	if(mylist->FindObject("Step_Particle")) 
	{
		theTTree->SetBranchAddress("Step_Particle",&Step_Particle);
	}
	if(mylist->FindObject("Step_ParticleSubType")) 
	{
		theTTree->SetBranchAddress("Step_ParticleSubType",&Step_ParticleSubType);
	}
	if(mylist->FindObject("Step_ParticleType")) 
	{
		theTTree->SetBranchAddress("Step_ParticleType",&Step_ParticleType);
	}

	// TRACK
	if (mylist->FindObject("Track_EventID")) 
	{
		theTTree->SetBranchAddress("Track_EventID", &Track_EventID);
	}
	if (mylist->FindObject("Track_TrackID")) 
	{
		theTTree->SetBranchAddress("Track_TrackID", &Track_TrackID);
	}

	if (mylist->FindObject("Track_FinalPosX")) 
	{
		theTTree->SetBranchAddress("Track_FinalPosX", &Track_FinalPosX);
	}
	if (mylist->FindObject("Track_FinalPosY")) 
	{
		theTTree->SetBranchAddress("Track_FinalPosY", &Track_FinalPosY);
	}
	if (mylist->FindObject("Track_FinalPosZ")) 
	{
		theTTree->SetBranchAddress("Track_FinalPosZ", &Track_FinalPosZ);
	}
	if (mylist->FindObject("Track_InitialPosX")) 
	{
		theTTree->SetBranchAddress("Track_InitialPosX", &Track_InitialPosX);
	}
	if (mylist->FindObject("Track_InitialPosY")) 
	{
		theTTree->SetBranchAddress("Track_InitialPosY", &Track_InitialPosY);
	}
	if (mylist->FindObject("Track_InitialPosZ")) 
	{
		theTTree->SetBranchAddress("Track_InitialPosZ", &Track_InitialPosZ);
	}
	
	return 0;
}



int TreeClass::InitializeLeaves()
{
  Event_EventID = -123456;
  Event_FinalTouchablePosX = -123456;
  Event_FinalTouchablePosY = -123456;
  Event_FinalTouchablePosZ = -123456;
  Event_FinalTouchableEulerPhi = -123456;
  Event_FinalTouchableEulerTheta = -123456;
  Event_FinalTouchableEulerPsi = -123456;
  Event_InitialDensity = -123456;
  Event_InitialDirPerp = -123456;
  Event_InitialDirPhi = -123456;
  Event_InitialDirTheta = -123456;
  Event_InitialDirX = -123456;
  Event_InitialDirY = -123456;
  Event_InitialDirZ = -123456;
  Event_InitialElectronDensity = -123456;
  Event_InitialKineticEnergy = -123456;
  Event_InitialLocalDirPerp = -123456;
  Event_InitialLocalDirPhi = -123456;
  Event_InitialLocalDirTheta = -123456;
  Event_InitialLocalDirX = -123456;
  Event_InitialLocalDirY = -123456;
  Event_InitialLocalDirZ = -123456;
  Event_InitialLocalPosMag = -123456;
  Event_InitialLocalPosPerp = -123456;
  Event_InitialLocalPosPhi = -123456;
  Event_InitialLocalPosTheta = -123456;
  Event_InitialLocalPosX = -123456;
  Event_InitialLocalPosY = -123456;
  Event_InitialLocalPosZ = -123456;
  Event_InitialMomMag = -123456;
  Event_InitialMomPerp = -123456;
  Event_InitialMomPhi = -123456;
  Event_InitialMomTheta = -123456;
  Event_InitialMomX = -123456;
  Event_InitialMomY = -123456;
  Event_InitialMomZ = -123456;
  Event_InitialNuclearInterLength = -123456;
  Event_InitialPVCopyNumber = -123456;
  Event_InitialParallelPVCopyNumber = -123456;
  Event_InitialPosMag = -123456;
  Event_InitialPosPerp = -123456;
  Event_InitialPosPhi = -123456;
  Event_InitialPosTheta = -123456;
  Event_InitialPosX = -123456;
  Event_InitialPosY = -123456;
  Event_InitialPosZ = -123456;
  Event_InitialPressure = -123456;
  Event_InitialRadLength = -123456;
  Event_InitialTemperature = -123456;
  Event_InitialTime = -123456;
  Event_InitialTotalEnergy = -123456;
  Event_InitialTouchablePosX = -123456;
  Event_InitialTouchablePosY = -123456;
  Event_InitialTouchablePosZ = -123456;
  Event_InitialTouchableEulerPhi = -123456;
  Event_InitialTouchableEulerTheta = -123456;
  Event_InitialTouchableEulerPsi = -123456;
  Event_RunID = -123456;
  Event_InitialLogicalVolume = "-123456";
  Event_InitialMaterial = "-123456";
  Event_InitialPhysicalVolume = "-123456";
  Event_InitialRegion = "-123456";
  Event_InitialSolid = "-123456";
  Event_InitialSolidType = "-123456";
  Event_InitialTouchable = "-123456";

  Step_AccumulatedDose = 0;
  Step_AccumulatedEnergyDeposited = 0;
  Step_AccumulatedEnergyLost = 0;
  Step_AccumulatedKerma = 0;
  Step_AccumulatedLength = 0;
  Step_AccumulatedNonIonizingEnergyDeposited = 0;
  Step_AngleChange = 0;
  Step_DirChangeMag = 0;
  Step_DirChangePerp = 0;
  Step_DirChangePhi = 0;
  Step_DirChangeTheta = 0;
  Step_DirChangeX = 0;
  Step_DirChangeY = 0;
  Step_DirChangeZ = 0;
  Step_EventID = 0;
  Step_FinalDEDX = 0;
  Step_FinalDensity = 0;
  Step_FinalDirPerp = 0;
  Step_FinalDirPhi = 0;
  Step_FinalDirTheta = 0;
  Step_FinalDirX = 0;
  Step_FinalDirY = 0;
  Step_FinalDirZ = 0;
  Step_FinalElectronDensity = 0;
  Step_FinalGammaEquivalentDose = 0;
  Step_FinalKineticEnergy = 0;
  Step_FinalLocalDirPerp = 0;
  Step_FinalLocalDirPhi = 0;
  Step_FinalLocalDirTheta = 0;
  Step_FinalLocalDirX = 0;
  Step_FinalLocalDirY = 0;
  Step_FinalLocalDirZ = 0;
  Step_FinalLocalInPreDirPerp = 0;
  Step_FinalLocalInPreDirPhi = 0;
  Step_FinalLocalInPreDirTheta = 0;
  Step_FinalLocalInPreDirX = 0;
  Step_FinalLocalInPreDirY = 0;
  Step_FinalLocalInPreDirZ = 0;
  Step_FinalLocalInPrePosMag = 0;
  Step_FinalLocalInPrePosPerp = 0;
  Step_FinalLocalInPrePosPhi = 0;
  Step_FinalLocalInPrePosTheta = 0;
  Step_FinalLocalInPrePosX = 0;
  Step_FinalLocalInPrePosY = 0;
  Step_FinalLocalInPrePosZ = 0;
  Step_FinalLocalPosMag = 0;
  Step_FinalLocalPosPerp = 0;
  Step_FinalLocalPosPhi = 0;
  Step_FinalLocalPosTheta = 0;
  Step_FinalLocalPosX = 0;
  Step_FinalLocalPosY = 0;
  Step_FinalLocalPosZ = 0;
  Step_FinalLocalTime = 0;
  Step_FinalMomMag = 0;
  Step_FinalMomPerp = 0;
  Step_FinalMomPhi = 0;
  Step_FinalMomTheta = 0;
  Step_FinalMomX = 0;
  Step_FinalMomY = 0;
  Step_FinalMomZ = 0;
  Step_FinalNeutronEquivalentDose = 0;
  Step_FinalNuclearInterLength = 0;
  Step_FinalPVCopyNumber = 0;
  Step_FinalParallelPVCopyNumber = 0;
  Step_FinalPosMag = 0;
  Step_FinalPosPerp = 0;
  Step_FinalPosPhi = 0;
  Step_FinalPosTheta = 0;
  Step_FinalPosX = 0;
  Step_FinalPosY = 0;
  Step_FinalPosZ = 0;
  Step_FinalPressure = 0;
  Step_FinalProperTime = 0;
  Step_FinalRadLength = 0;
  Step_FinalTemperature = 0;
  Step_FinalTime = 0;
  Step_FinalTotalEnergy = 0;
  Step_FinalTouchablePosX = 0;
  Step_FinalTouchablePosY = 0;
  Step_FinalTouchablePosZ = 0;
  Step_FinalTouchableEulerPhi = 0;
  Step_FinalTouchableEulerTheta = 0;
  Step_FinalTouchableEulerPsi = 0;
  Step_FinalWeight = 0;
  Step_InitialDEDX = 0;
  Step_InitialDensity = 0;
  Step_InitialDirPerp = 0;
  Step_InitialDirPhi = 0;
  Step_InitialDirTheta = 0;
  Step_InitialDirX = 0;
  Step_InitialDirY = 0;
  Step_InitialDirZ = 0;
  Step_InitialElectronDensity = 0;
  Step_InitialGammaEquivalentDose = 0;
  Step_InitialKineticEnergy = 0;
  Step_InitialLocalDirPerp = 0;
  Step_InitialLocalDirPhi = 0;
  Step_InitialLocalDirTheta = 0;
  Step_InitialLocalDirX = 0;
  Step_InitialLocalDirY = 0;
  Step_InitialLocalDirZ = 0;
  Step_InitialLocalPosMag = 0;
  Step_InitialLocalPosPerp = 0;
  Step_InitialLocalPosPhi = 0;
  Step_InitialLocalPosTheta = 0;
  Step_InitialLocalPosX = 0;
  Step_InitialLocalPosY = 0;
  Step_InitialLocalPosZ = 0;
  Step_InitialLocalTime = 0;
  Step_InitialMomMag = 0;
  Step_InitialMomPerp = 0;
  Step_InitialMomPhi = 0;
  Step_InitialMomTheta = 0;
  Step_InitialMomX = 0;
  Step_InitialMomY = 0;
  Step_InitialMomZ = 0;
  Step_InitialNeutronEquivalentDose = 0;
  Step_InitialNuclearInterLength = 0;
  Step_InitialPVCopyNumber = 0;
  Step_InitialParallelPVCopyNumber = 0;
  Step_InitialPosMag = 0;
  Step_InitialPosPerp = 0;
  Step_InitialPosPhi = 0;
  Step_InitialPosTheta = 0;
  Step_InitialPosX = 0;
  Step_InitialPosY = 0;
  Step_InitialPosZ = 0;
  Step_InitialPressure = 0;
  Step_InitialProperTime = 0;
  Step_InitialRadLength = 0;
  Step_InitialTemperature = 0;
  Step_InitialTime = 0;
  Step_InitialTotalEnergy = 0;
  Step_InitialTouchablePosX = 0;
  Step_InitialTouchablePosY = 0;
  Step_InitialTouchablePosZ = 0;
  Step_InitialTouchableEulerPhi = 0;
  Step_InitialTouchableEulerTheta = 0;
  Step_InitialTouchableEulerPsi = 0;
  Step_InitialWeight = 0;
  Step_KineticEnergyChange = 0;
  Step_MomChangeMag = 0;
  Step_MomChangePerp = 0;
  Step_MomChangePhi = 0;
  Step_MomChangeTheta = 0;
  Step_MomChangeX = 0;
  Step_MomChangeY = 0;
  Step_MomChangeZ = 0;
  Step_NofSecondaries = 0;
  Step_ParentTrackID = 0;
  Step_ParticleCharge = 0;
  Step_ParticleLifeTime = 0;
  Step_ParticleMass = 0;
  Step_ParticlePDGEncoding = 0;
  Step_ParticleStable = 0;
  Step_ParticleWidth = 0;
  Step_PosChangeMag = 0;
  Step_PosChangePerp = 0;
  Step_PosChangePhi = 0;
  Step_PosChangeTheta = 0;
  Step_PosChangeX = 0;
  Step_PosChangeY = 0;
  Step_PosChangeZ = 0;
  Step_PrimaryTrackID = 0;
  Step_RandomPosMag = 0;
  Step_RandomPosPerp = 0;
  Step_RandomPosPhi = 0;
  Step_RandomPosTheta = 0;
  Step_RandomPosX = 0;
  Step_RandomPosY = 0;
  Step_RandomPosZ = 0;
  Step_RunID = 0;
  Step_StepNumber = 0;
  Step_SumSecoKineticEnergy = 0;
  Step_TimeChange = 0;

	Step_TrackID = 0;	

  Step_TrackLength = 0;
  Step_CreatorProcess = 0;
  Step_FinalLogicalVolume = 0;
  Step_FinalMaterial = 0;
  Step_FinalPhysicalVolume = 0;
  Step_FinalProcess = 0;
  Step_FinalRegion = 0;
  Step_FinalSolid = 0;
  Step_FinalSolidType = 0;
  Step_FinalTouchable = 0;
  Step_FinalUnitID = 0;
  Step_InitialUnitID = 0;
  Step_InitialLogicalVolume = 0;
  Step_InitialMaterial = 0;
  Step_InitialPhysicalVolume = 0;
  Step_InitialProcess = 0;
  Step_InitialRegion = 0;
  Step_InitialSolid = 0;
  Step_InitialSolidType = 0;
  Step_InitialTouchable = 0;
  Step_Particle = 0;
  Step_ParticleSubType = 0;
  Step_ParticleType = 0;     

	// TRACK (pointers)
	Track_EventID = 0;
	Track_TrackID = 0;

	Track_FinalPosX = 0;
	Track_FinalPosY = 0;
	Track_FinalPosZ = 0;

	Track_InitialPosX = 0;
	Track_InitialPosY = 0;
	Track_InitialPosZ = 0;
                                  
  return 0;
}

