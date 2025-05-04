#ifndef PET_HitReconstruction_H__
#define PET_HitReconstruction_H__

#include "globalheaders.hh"
#include "HitReconstructionClass.hh"


class DetHitReconstruction : public HitReconstructionClass
{
public:
				DetHitReconstruction(){};
				~DetHitReconstruction(){};

	// overridden from HitReconstructionClass
	//  double 		StatisticalEnergySmearing(const double& in_E, const double& in_Fano, const double& in_ioni_E) const;
	//  double 		ElectronicNoiseEnergySmearing(const double& in_E, const double& in_sigma_noise) const;

	int 		ProcessStep(TreeClass* aTree, int StepIndex, std::map<unsigned long long int, HitClass> &HitTimeMap );

private:
	void		CreateNewHit(const TreeClass* aTree, int in_stepIndex, const Detector& in_myDetector, HitClass& io_hit ) const;
};


#endif // #ifdef PET_HitReconstruction_H__
