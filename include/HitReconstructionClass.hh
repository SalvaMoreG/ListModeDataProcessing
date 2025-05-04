#ifndef HitReconstructionClass_H__
#define HitReconstructionClass_H__

#include "globalheaders.hh"
#include "HitClass.hh"
#include "TreeClass.hh"

#include "Utilities.hh"
//	#include "ChargeSharingClass.hh"

#include "HistoClass.hh"
#include "Scanner.hh"

class HitReconstructionClass
{
public:
  							HitReconstructionClass();
  	virtual					~HitReconstructionClass();		// must be virtual!!!!

	virtual double 			StatisticalEnergySmearing(const double& in_E, const double& in_Fano, const double& in_ioni_E) const;
	virtual double 			ElectronicNoiseEnergySmearing(const double& in_E, const double& in_sigma_noise) const;

	virtual int 			MergeStep( std::map<unsigned long long int,HitClass>::iterator it
										, const double& E, const double& X, const double&  Y, const double& Z)
	{
		//calculating the position (X,Y,Z) after merging as the energy weighted centroid
		it->second.X = (it->second.X*it->second.E + X*E)/(it->second.E+E);
		it->second.Y = (it->second.Y*it->second.E + Y*E)/(it->second.E+E);
		it->second.Z = (it->second.Z*it->second.E + Z*E)/(it->second.E+E);
		it->second.E += E; //summing energies
		it->second.merged = true;
		return 0;
	};

	virtual int 			MergeStep( std::map<unsigned long long int,HitClass>::iterator it, const HitClass& myhit)
	{
		//calculating the position (X,Y,Z) after merging as the energy weighted centroid
		it->second.X = (it->second.X*it->second.E + myhit.X*myhit.E)/(it->second.E+myhit.E);
		it->second.Y = (it->second.Y*it->second.E + myhit.Y*myhit.E)/(it->second.E+myhit.E);
		it->second.Z = (it->second.Z*it->second.E + myhit.Z*myhit.E)/(it->second.E+myhit.E);
		it->second.E += myhit.E; 		//summing energies
		it->second.merged = true;
		if(it->second.T_trigger > myhit.T_trigger) 
		//    if(myhit.is_neighbor_hit==0) 
		{
			it->second.T = it->second.T_trigger = myhit.T_trigger;
				// Machiel, 2015-05-25: Note: the time can now be a duplicate time of another hit in the LMTime map....
			it->second.bypass_trigger = myhit.bypass_trigger;
			it->second.is_neighbor_hit = myhit.is_neighbor_hit;
			it->second.E_central = myhit.E_central;
		}
		return 0;
	};

	virtual int 			ProcessStep(TreeClass* aTree, int StepIndex, 
									std::map<unsigned long long int, HitClass> &HitTimeMap) = 0;
    /*
	void					CreateChargeShare( const std::string& theChShRadFilePath )
							{
								theChargeShare = ChargeSharingClass::GetInstance(theChShRadFilePath);
							}
    */

	inline void				SetHistograms( HistoClass* in_histograms )
							{
								theHistograms = in_histograms;
							}
	inline void				SetScanner( const Scanner* in_scanner )
							{
								m_scanner = in_scanner;
							}
	inline void				SetRandomGenerator( TRandom3* inRandomGenerator )
							{
								theRandomGenerator = inRandomGenerator;
							}

protected:
	// ChargeSharingClass* 	theChargeShare;
	
	TRandom3* 				theRandomGenerator;

	HistoClass*				theHistograms;
	const Scanner*			m_scanner;

private: 
};



#endif // #ifdef HitReconstructionClass_H__
