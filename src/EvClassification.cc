
#include "EvClassificationClass.hh"

EvClassificationClass::EvClassificationClass()
	: theHistograms(NULL)
	, m_scanner(NULL)
{
}

EvClassificationClass::~EvClassificationClass()
{
};


void 
EvClassificationClass::PrintSomeInfo( const TreeClass* aTree ) const
{
	// cout<<"event "<<aTree->Event_EventID;
	// cout<<" process "<<aTree->Step_FinalProcess->at(StepIndex);
	// cout<<" volume "<<aTree->Step_InitialLogicalVolume->at(StepIndex);
	// cout<<" particle "<<aTree->Step_Particle->at(StepIndex);
	// cout<<" track "<<aTree->Step_TrackID->at(StepIndex);
	// cout<<" parent_track "<<aTree->Step_ParentTrackID->at(StepIndex);
	// cout<<" Edep "<<aTree->Step_AccumulatedEnergyDeposited->at(StepIndex);
	// cout<<" Ein "<<aTree->Step_InitialTotalEnergy->at(StepIndex);
	// cout<<" Etot "<<TotEnergy;
	// //		    cout<<" dev "<<I_x-F_x<<","<<I_y-F_y<<","<<I_z-F_z;
	// cout<<" angle "<<AngleBetween3DVectors(I_x,I_y,I_z,F_x,F_y,F_z);
	// cout<<" compt "<<ComptonAngle(aTree->Step_InitialTotalEnergy->at(StepIndex)
	//					,aTree->Step_FinalTotalEnergy->at(StepIndex));
	// cout<<endl;
}


