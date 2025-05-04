
#include "HitReconstructionClass.hh"
#include "InputFlags.hh"


HitReconstructionClass::HitReconstructionClass()
//	: theChargeShare(0)
	: theRandomGenerator(NULL)
	, theHistograms(NULL)
	, m_scanner(NULL)
{
}


HitReconstructionClass::~HitReconstructionClass() 
{
	//     delete theChargeShare;
}

/*
 * The functions below "StatisticalEnergySmearing" and "ElectronicNoiseEnergySmearing" are implemented long ago
 * In principle, we should give the Fano factor and ionisation factor etc...
 * In practice, I hardly ever use it.
 * I either use:
 *     -zeroERes (zero energy resolution)
 *     -newERes  (Gerard's implementation of Fano factor and ionisation etc...
 *     -userERes (just a fixed FWHM percentage --> converted to Gaussian sigma inside the code)
 * See: ProcessDataClass::DoEnergySmearing()
*/

// TODO: Why is the trapping energy missing (ca. 1%)????!
double
HitReconstructionClass::StatisticalEnergySmearing(const double& in_E, const double& in_Fano, const double& in_ioni_E) const
{
    if ( InputFlags::Instance()->GetEresMode() != InputFlags::ERES_OLD )    
        return in_E;

	// old way of doing it
    double var = in_E * in_ioni_E * in_Fano;
    double sigma = sqrt(var);
    double newE_stat = theRandomGenerator->Gaus(in_E, sigma);
    return newE_stat;
}

double
HitReconstructionClass::ElectronicNoiseEnergySmearing(const double& in_E, const double& in_sigma_noise) const
{
    if ( InputFlags::Instance()->GetEresMode() != InputFlags::ERES_OLD )    
        return in_E;
    
	// old way of doing it
    double newE_elec = in_E + theRandomGenerator->Gaus(0, in_sigma_noise);
    return newE_elec;

}


