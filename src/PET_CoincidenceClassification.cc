#include "PET_CoincidenceClassification.hh"
#include "VIPglobalheaders.hh"
#include "globalheaders.hh"

using namespace std;

PET_CoincidenceClassification::PET_CoincidenceClassification()
{
	InitializeCounters();
}

// ====================================================

PET_CoincidenceClassification::~PET_CoincidenceClassification()
{
}

// ====================================================

int PET_CoincidenceClassification::InitializeCounters()
{
	NC_prompt = 0;
	NC_good = 0;
	NC_scat = 0;
	NC_Rayl = 0;
	NC_rand = 0;
	NC_PEPE = 0;
	NC_PECOPE = 0;
	NC_COPECOPE = 0;
	NC_PEinco = 0;
	NC_incoinco = 0;
	
	NC_good_LOR_Vtx = 0;
	NC_bad_LOR_Vtx = 0;
	NC_good_LOR_annihilVtx = 0;
	NC_bad_LOR_annihilVtx = 0;
	
	return 0;
}

// ====================================================

int PET_CoincidenceClassification::Print() const
{
	cout<<"***********************************************************"<<endl;
	cout<<"*** PET_CoincidenceClassification::PrintClassification ***"<<endl;
	cout<<"***********************************************************"<<endl;
	cout<<"* NC_prompt = "<<NC_prompt<<endl;
	cout<<"* NC_good = "<<NC_good<<endl;
	cout<<"* NC_scat = "<<NC_scat<<endl;
	cout<<"* NC_Rayl = "<<NC_Rayl<<endl;
	cout<<"* NC_rand = "<<NC_rand<<endl;
	cout<<"* NC_PEPE = "<<NC_PEPE<<endl;
	cout<<"* NC_PECOPE = "<<NC_PECOPE<<endl;
	cout<<"* NC_COPECOPE = "<<NC_COPECOPE<<endl;
	cout<<"* NC_PEinco = "<<NC_PEinco<<endl;
	cout<<"* NC_incoinco = "<<NC_incoinco<<endl;
	cout<<"*"<<endl;
	cout<<"* NC_good_LOR_Vtx = "<<NC_good_LOR_Vtx<<endl;
	cout<<"* NC_bad_LOR_Vtx = "<<NC_bad_LOR_Vtx<<endl;
	cout<<"* NC_good_LOR_annihilVtx = "<<NC_good_LOR_annihilVtx<<endl;
	cout<<"* NC_bad_LOR_annihilVtx = "<<NC_bad_LOR_annihilVtx<<endl;
	cout<<"***********************************************************"<<endl;
	return 0;
}

// ====================================================

int PET_CoincidenceClassification::Classify(const map<int,EventClass>& anEventClassificationMap,
	map<int,CoincClass>& aCoincClassificationMap, HistoClass* io_histograms)
{
	for (map<int,CoincClass>::iterator coinc_it = aCoincClassificationMap.begin();
			coinc_it != aCoincClassificationMap.end(); coinc_it++)
	{
		NC_prompt++;
		HitClass hit1 = coinc_it->second.hit1;
		HitClass hit2 = coinc_it->second.hit2;
		map<int,EventClass>::const_iterator ev_it = anEventClassificationMap.find(hit1.ev); 
			//look for correspondent event classification
		
		//filling histograms
		double dist_LOR_Vtx = 
			DistancePointToLine(ev_it->second.Vx,ev_it->second.Vy,ev_it->second.Vz
								,hit1.X,hit1.Y,hit1.Z,hit2.X,hit2.Y,hit2.Z);
		double dist_LOR_Annihil_Vtx = DistancePointToLine(ev_it->second.Annihil_Vx,ev_it->second.Annihil_Vy,
							ev_it->second.Annihil_Vz,hit1.X,hit1.Y,hit1.Z,hit2.X,hit2.Y,hit2.Z);
		//      if(io_histograms && ev_it->second.classification!=scat)
		if (io_histograms)
		{
			io_histograms->Fill1DHisto("Dist_LOR_Vtx",dist_LOR_Vtx);
			io_histograms->Fill1DHisto("Dist_LOR_AnnihilVtx",dist_LOR_Annihil_Vtx);
		}

		if (hit1.ev != hit2.ev) 
		{ //is random
			NC_rand++;
			// randomflag RANDOM FLAG CLASSIFICATION 1 = RANDOM 
            coinc_it->second.m_classification = CLASSIFICATION_RANDOM; //   1;
			if (io_histograms)
			{
				io_histograms->Fill1DHisto("Dist_LOR_Vtx_rand",dist_LOR_Vtx);
				io_histograms->Fill1DHisto("Dist_LOR_AnnihilVtx_rand",dist_LOR_Annihil_Vtx);
			}
		}
      	else 
		{ //is not random
			if (ev_it == anEventClassificationMap.end()) 
			{
				cout << "ERROR in PET_CoincidenceClassification::ClassifyCoincidences"<<endl;
				cout << "event num " << hit1.ev << " not found in anEventClassificationMap."<<endl;
			}
			else 
			{
                // it is a scat event
				if (ev_it->second.classification == PET_EvClassification::scat)
				{
					NC_scat++;

                    coinc_it->second.m_classification = CLASSIFICATION_SCATTER; //  2;
/*
cout << "marking a new scattered event, NC_scat = " << NC_scat
     << " classif: " << coinc_it->second.m_classification << endl;
*/
					if (io_histograms)
					{
						if (hit1.E + hit2.E > 0.511+0.025) 
						{
							io_histograms->Fill1DHisto("E1_scat",hit1.E);
							io_histograms->Fill1DHisto("E2_scat",hit2.E);
							io_histograms->Fill1DHisto("E_scat",hit1.E+hit2.E);
							io_histograms->Fill2DHisto("E1_vs_E2_scat",hit2.E,hit1.E);
						}
						io_histograms->Fill1DHisto("Dist_LOR_Vtx_scat",dist_LOR_Vtx);
						io_histograms->Fill1DHisto("Dist_LOR_AnnihilVtx_scat",dist_LOR_Annihil_Vtx);
					}
				}
	      		else if (ev_it->second.classification == PET_EvClassification::Rayl) 
				{ 
					NC_Rayl++;
					if (io_histograms)
					{
						io_histograms->Fill1DHisto("Dist_LOR_Vtx_Rayl",dist_LOR_Vtx);
						io_histograms->Fill1DHisto("Dist_LOR_AnnihilVtx_Rayl",dist_LOR_Annihil_Vtx);
					}
				}
	      		else 
				{ //it is a good event
					NC_good++;
					// randomflag RANDOM FLAG CLASSIFICATION 0 = TRUE
                    coinc_it->second.m_classification = CLASSIFICATION_TRUE; // 0;

					if (ev_it->second.classification == PET_EvClassification::PEPE) 
					{
						NC_PEPE++;
						if(io_histograms)
						{
							io_histograms->Fill1DHisto("Dist_LOR_Vtx_PEPE",dist_LOR_Vtx);
							io_histograms->Fill1DHisto("Dist_LOR_AnnihilVtx_PEPE",dist_LOR_Annihil_Vtx);
						}
					}
					if (ev_it->second.classification == PET_EvClassification::PECOPE) 
					{
						NC_PECOPE++;
						if(io_histograms)
						{
							io_histograms->Fill1DHisto("Dist_LOR_Vtx_PECOPE",dist_LOR_Vtx);
							io_histograms->Fill1DHisto("Dist_LOR_AnnihilVtx_PECOPE",dist_LOR_Annihil_Vtx);
						}
					}
					if (ev_it->second.classification == PET_EvClassification::COPECOPE) 
					{
						NC_COPECOPE++;
						if(io_histograms)
						{
							io_histograms->Fill1DHisto("Dist_LOR_Vtx_COPECOPE",dist_LOR_Vtx);
							io_histograms->Fill1DHisto("Dist_LOR_AnnihilVtx_COPECOPE",dist_LOR_Annihil_Vtx);
						}
					}
					if (ev_it->second.classification == PET_EvClassification::PEinco) 
					{
						NC_PEinco++;
						if(io_histograms)
						{
							io_histograms->Fill1DHisto("Dist_LOR_Vtx_PEinco",dist_LOR_Vtx);
							io_histograms->Fill1DHisto("Dist_LOR_AnnihilVtx_PEinco",dist_LOR_Annihil_Vtx);
						}
					}
					if (ev_it->second.classification == PET_EvClassification::incoinco) 
					{
						NC_incoinco++;
						if(io_histograms)
						{
							io_histograms->Fill1DHisto("Dist_LOR_Vtx_incoinco",dist_LOR_Vtx);
							io_histograms->Fill1DHisto("Dist_LOR_AnnihilVtx_incoinco",dist_LOR_Annihil_Vtx);
						}
					}
				}
			}
		}
    }
	return 0;
}
