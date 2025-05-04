#ifndef TreeClass_H__
#define TreeClass_H__

#include "globalheaders.hh"

#include <vector>

class TreeClass 
{
public:
  int    Event_EventID;
  double Event_FinalTouchablePosX;
  double Event_FinalTouchablePosY;
  double Event_FinalTouchablePosZ;
  double Event_FinalTouchableEulerPhi;
  double Event_FinalTouchableEulerTheta;
  double Event_FinalTouchableEulerPsi;
  double Event_InitialDensity;
  double Event_InitialDirPerp;
  double Event_InitialDirPhi;
  double Event_InitialDirTheta;
  double Event_InitialDirX;
  double Event_InitialDirY;
  double Event_InitialDirZ;
  double Event_InitialElectronDensity;
  double Event_InitialKineticEnergy;
  double Event_InitialLocalDirPerp;
  double Event_InitialLocalDirPhi;
  double Event_InitialLocalDirTheta;
  double Event_InitialLocalDirX;
  double Event_InitialLocalDirY;
  double Event_InitialLocalDirZ;
  double Event_InitialLocalPosMag;
  double Event_InitialLocalPosPerp;
  double Event_InitialLocalPosPhi;
  double Event_InitialLocalPosTheta;
  double Event_InitialLocalPosX;
  double Event_InitialLocalPosY;
  double Event_InitialLocalPosZ;
  double Event_InitialMomMag;
  double Event_InitialMomPerp;
  double Event_InitialMomPhi;
  double Event_InitialMomTheta;
  double Event_InitialMomX;
  double Event_InitialMomY;
  double Event_InitialMomZ;
  double Event_InitialNuclearInterLength;
  double Event_InitialPVCopyNumber;
  double Event_InitialParallelPVCopyNumber;
  double Event_InitialPosMag;
  double Event_InitialPosPerp;
  double Event_InitialPosPhi;
  double Event_InitialPosTheta;
  double Event_InitialPosX;
  double Event_InitialPosY;
  double Event_InitialPosZ;
  double Event_InitialPressure;
  double Event_InitialRadLength;
  double Event_InitialTemperature;
  double Event_InitialTime;
  double Event_InitialTotalEnergy;
  double Event_InitialTouchablePosX;
  double Event_InitialTouchablePosY;
  double Event_InitialTouchablePosZ;
  double Event_InitialTouchableEulerPhi;
  double Event_InitialTouchableEulerTheta;
  double Event_InitialTouchableEulerPsi;

  int Event_RunID;
  std::string Event_InitialLogicalVolume;
  std::string Event_InitialMaterial;
  std::string Event_InitialPhysicalVolume;
  std::string Event_InitialRegion;
  std::string Event_InitialSolid;
  std::string Event_InitialSolidType;
  std::string Event_InitialTouchable;
  std::string* Event_InitialLogicalVolume_ptr;
  std::string* Event_InitialMaterial_ptr;
  std::string* Event_InitialPhysicalVolume_ptr;
  std::string* Event_InitialRegion_ptr;
  std::string* Event_InitialSolid_ptr;
  std::string* Event_InitialSolidType_ptr;
  std::string* Event_InitialTouchable_ptr;

// <<<<<<<<< added to get real hit-position (not voxel center measured hit-position) from G4Track->GetPosition()
// Machiel 2015 05 06
//
	// TRACK
	std::vector<int>* Track_EventID;
	std::vector<int>* Track_TrackID;

	std::vector<double>* Track_FinalPosX;
	std::vector<double>* Track_FinalPosY;
	std::vector<double>* Track_FinalPosZ;

	std::vector<double>* Track_InitialPosX;
	std::vector<double>* Track_InitialPosY;
	std::vector<double>* Track_InitialPosZ;

// >>>>>>>>>>>>

  std::vector<double>* Step_AccumulatedDose;
  std::vector<double>* Step_AccumulatedEnergyDeposited;
  std::vector<double>* Step_AccumulatedEnergyLost;
  std::vector<double>* Step_AccumulatedKerma;
  std::vector<double>* Step_AccumulatedLength;
  std::vector<double>* Step_AccumulatedNonIonizingEnergyDeposited;
  std::vector<double>* Step_AngleChange;
  std::vector<double>* Step_DirChangeMag;
  std::vector<double>* Step_DirChangePerp;
  std::vector<double>* Step_DirChangePhi;
  std::vector<double>* Step_DirChangeTheta;
  std::vector<double>* Step_DirChangeX;
  std::vector<double>* Step_DirChangeY;
  std::vector<double>* Step_DirChangeZ;
  std::vector<int>*    Step_EventID;
  std::vector<double>* Step_FinalDEDX;
  std::vector<double>* Step_FinalDensity;
  std::vector<double>* Step_FinalDirPerp;
  std::vector<double>* Step_FinalDirPhi;
  std::vector<double>* Step_FinalDirTheta;
  std::vector<double>* Step_FinalDirX;
  std::vector<double>* Step_FinalDirY;
  std::vector<double>* Step_FinalDirZ;
  std::vector<double>* Step_FinalElectronDensity;
  std::vector<double>* Step_FinalGammaEquivalentDose;
  std::vector<double>* Step_FinalKineticEnergy;
  std::vector<double>* Step_FinalLocalDirPerp;
  std::vector<double>* Step_FinalLocalDirPhi;
  std::vector<double>* Step_FinalLocalDirTheta;
  std::vector<double>* Step_FinalLocalDirX;
  std::vector<double>* Step_FinalLocalDirY;
  std::vector<double>* Step_FinalLocalDirZ;
  std::vector<double>* Step_FinalLocalInPreDirPerp;
  std::vector<double>* Step_FinalLocalInPreDirPhi;
  std::vector<double>* Step_FinalLocalInPreDirTheta;
  std::vector<double>* Step_FinalLocalInPreDirX;
  std::vector<double>* Step_FinalLocalInPreDirY;
  std::vector<double>* Step_FinalLocalInPreDirZ;
  std::vector<double>* Step_FinalLocalInPrePosMag;
  std::vector<double>* Step_FinalLocalInPrePosPerp;
  std::vector<double>* Step_FinalLocalInPrePosPhi;
  std::vector<double>* Step_FinalLocalInPrePosTheta;
  std::vector<double>* Step_FinalLocalInPrePosX;
  std::vector<double>* Step_FinalLocalInPrePosY;
  std::vector<double>* Step_FinalLocalInPrePosZ;
  std::vector<double>* Step_FinalLocalPosMag;
  std::vector<double>* Step_FinalLocalPosPerp;
  std::vector<double>* Step_FinalLocalPosPhi;
  std::vector<double>* Step_FinalLocalPosTheta;
  std::vector<double>* Step_FinalLocalPosX;
  std::vector<double>* Step_FinalLocalPosY;
  std::vector<double>* Step_FinalLocalPosZ;
  std::vector<double>* Step_FinalLocalTime;
  std::vector<double>* Step_FinalMomMag;
  std::vector<double>* Step_FinalMomPerp;
  std::vector<double>* Step_FinalMomPhi;
  std::vector<double>* Step_FinalMomTheta;
  std::vector<double>* Step_FinalMomX;
  std::vector<double>* Step_FinalMomY;
  std::vector<double>* Step_FinalMomZ;
  std::vector<double>* Step_FinalNeutronEquivalentDose;
  std::vector<double>* Step_FinalNuclearInterLength;
  std::vector<int>*    Step_FinalPVCopyNumber;
  std::vector<int>*	   Step_FinalParallelPVCopyNumber;
  std::vector<double>* Step_FinalPosMag;
  std::vector<double>* Step_FinalPosPerp;
  std::vector<double>* Step_FinalPosPhi;
  std::vector<double>* Step_FinalPosTheta;
  std::vector<double>* Step_FinalPosX;
  std::vector<double>* Step_FinalPosY;
  std::vector<double>* Step_FinalPosZ;
  std::vector<double>* Step_FinalPressure;
  std::vector<double>* Step_FinalProperTime;
  std::vector<double>* Step_FinalRadLength;
  std::vector<double>* Step_FinalTemperature;
  std::vector<double>* Step_FinalTime;
  std::vector<double>* Step_FinalTotalEnergy;
  std::vector<double>* Step_FinalTouchablePosX;
  std::vector<double>* Step_FinalTouchablePosY;
  std::vector<double>* Step_FinalTouchablePosZ;
  std::vector<double>* Step_FinalTouchableEulerPhi;
  std::vector<double>* Step_FinalTouchableEulerTheta;
  std::vector<double>* Step_FinalTouchableEulerPsi;
  std::vector<double>* Step_FinalWeight;
  std::vector<double>* Step_InitialDEDX;
  std::vector<double>* Step_InitialDensity;
  std::vector<double>* Step_InitialDirPerp;
  std::vector<double>* Step_InitialDirPhi;
  std::vector<double>* Step_InitialDirTheta;
  std::vector<double>* Step_InitialDirX;
  std::vector<double>* Step_InitialDirY;
  std::vector<double>* Step_InitialDirZ;
  std::vector<double>* Step_InitialElectronDensity;
  std::vector<double>* Step_InitialGammaEquivalentDose;
  std::vector<double>* Step_InitialKineticEnergy;
  std::vector<double>* Step_InitialLocalDirPerp;
  std::vector<double>* Step_InitialLocalDirPhi;
  std::vector<double>* Step_InitialLocalDirTheta;
  std::vector<double>* Step_InitialLocalDirX;
  std::vector<double>* Step_InitialLocalDirY;
  std::vector<double>* Step_InitialLocalDirZ;
  std::vector<double>* Step_InitialLocalPosMag;
  std::vector<double>* Step_InitialLocalPosPerp;
  std::vector<double>* Step_InitialLocalPosPhi;
  std::vector<double>* Step_InitialLocalPosTheta;
  std::vector<double>* Step_InitialLocalPosX;
  std::vector<double>* Step_InitialLocalPosY;
  std::vector<double>* Step_InitialLocalPosZ;
  std::vector<double>* Step_InitialLocalTime;
  std::vector<double>* Step_InitialMomMag;
  std::vector<double>* Step_InitialMomPerp;
  std::vector<double>* Step_InitialMomPhi;
  std::vector<double>* Step_InitialMomTheta;
  std::vector<double>* Step_InitialMomX;
  std::vector<double>* Step_InitialMomY;
  std::vector<double>* Step_InitialMomZ;
  std::vector<double>* Step_InitialNeutronEquivalentDose;
  std::vector<double>* Step_InitialNuclearInterLength;
  std::vector<int>*    Step_InitialPVCopyNumber;
  std::vector<int>*    Step_InitialParallelPVCopyNumber;
  std::vector<double>* Step_InitialPosMag;
  std::vector<double>* Step_InitialPosPerp;
  std::vector<double>* Step_InitialPosPhi;
  std::vector<double>* Step_InitialPosTheta;
  std::vector<double>* Step_InitialPosX;
  std::vector<double>* Step_InitialPosY;
  std::vector<double>* Step_InitialPosZ;
  std::vector<double>* Step_InitialPressure;
  std::vector<double>* Step_InitialProperTime;
  std::vector<double>* Step_InitialRadLength;
  std::vector<double>* Step_InitialTemperature;
  std::vector<double>* Step_InitialTime;
  std::vector<double>* Step_InitialTotalEnergy;
  std::vector<double>* Step_InitialTouchablePosX;
  std::vector<double>* Step_InitialTouchablePosY;
  std::vector<double>* Step_InitialTouchablePosZ;
  std::vector<double>* Step_InitialTouchableEulerPhi;
  std::vector<double>* Step_InitialTouchableEulerTheta;
  std::vector<double>* Step_InitialTouchableEulerPsi;
  std::vector<double>* Step_InitialWeight;
  std::vector<double>* Step_KineticEnergyChange;
  std::vector<double>* Step_MomChangeMag;
  std::vector<double>* Step_MomChangePerp;
  std::vector<double>* Step_MomChangePhi;
  std::vector<double>* Step_MomChangeTheta;
  std::vector<double>* Step_MomChangeX;
  std::vector<double>* Step_MomChangeY;
  std::vector<double>* Step_MomChangeZ;
  std::vector<int>* Step_NofSecondaries;
  std::vector<int>* Step_ParentTrackID;
  std::vector<double>* Step_ParticleCharge;
  std::vector<double>* Step_ParticleLifeTime;
  std::vector<double>* Step_ParticleMass;
  std::vector<int>* Step_ParticlePDGEncoding;
  std::vector<int>* Step_ParticleStable;
  std::vector<double>* Step_ParticleWidth;
  std::vector<double>* Step_PosChangeMag;
  std::vector<double>* Step_PosChangePerp;
  std::vector<double>* Step_PosChangePhi;
  std::vector<double>* Step_PosChangeTheta;
  std::vector<double>* Step_PosChangeX;
  std::vector<double>* Step_PosChangeY;
  std::vector<double>* Step_PosChangeZ;
  std::vector<int>* Step_PrimaryTrackID;
  std::vector<double>* Step_RandomPosMag;
  std::vector<double>* Step_RandomPosPerp;
  std::vector<double>* Step_RandomPosPhi;
  std::vector<double>* Step_RandomPosTheta;
  std::vector<double>* Step_RandomPosX;
  std::vector<double>* Step_RandomPosY;
  std::vector<double>* Step_RandomPosZ;
  std::vector<int>* Step_RunID;
  std::vector<int>* Step_StepNumber;
  std::vector<double>* Step_SumSecoKineticEnergy;
  std::vector<double>* Step_TimeChange;
  std::vector<int>* Step_TrackID;
  std::vector<double>* Step_TrackLength;
  std::vector<std::string>* Step_CreatorProcess;
  std::vector<std::string>* Step_FinalLogicalVolume;
  std::vector<std::string>* Step_FinalMaterial;
  std::vector<std::string>* Step_FinalPhysicalVolume;
  std::vector<std::string>* Step_FinalProcess;
  std::vector<std::string>* Step_FinalRegion;
  std::vector<std::string>* Step_FinalSolid;
  std::vector<std::string>* Step_FinalSolidType;
  std::vector<std::string>* Step_FinalTouchable;
  std::vector<std::string>* Step_FinalUnitID;
  std::vector<std::string>* Step_InitialUnitID;
  std::vector<std::string>* Step_InitialLogicalVolume;
  std::vector<std::string>* Step_InitialMaterial;
  std::vector<std::string>* Step_InitialPhysicalVolume;
  std::vector<std::string>* Step_InitialProcess;
  std::vector<std::string>* Step_InitialRegion;
  std::vector<std::string>* Step_InitialSolid;
  std::vector<std::string>* Step_InitialSolidType;
  std::vector<std::string>* Step_InitialTouchable;
  std::vector<std::string>* Step_Particle;
  std::vector<std::string>* Step_ParticleSubType;
  std::vector<std::string>* Step_ParticleType;


public:
  					TreeClass();
  					~TreeClass();

  int 				LoadTTree(const std::string& RootFileName);

  TTree* 			theTTree;
  TFile* 			theRootFile;

  Long64_t 			GetEntriesFast(){return theTTree->GetEntriesFast();};
  Long64_t 			GetEntry(Long64_t i){return theTTree->GetEntry(i);};

  int 				GetNumOfSteps();
  int 				CheckNeededVariables(int StepIndex);
  int 				m_isfirstcheck;

protected:
  int 				LinkAvailableBranches();
  int 				InitializeLeaves();  
};


#endif // #ifdef TreeClass_H__
