#include "CC_CoincidenceClassification.hh"

#include "CC_EvClassification.hh"

using namespace std;

CC_CoincidenceClassification::CC_CoincidenceClassification()
{
	InitializeCounters();
}

CC_CoincidenceClassification::~CC_CoincidenceClassification()
{
}

int CC_CoincidenceClassification::InitializeCounters()
{
	NC_prompt   = 0;
	NC_rand     = 0;
	NC_scat     = 0;
	NC_BackScat = 0;
	NC_others   = 0;
	
	NC_PE_null = 0;
	NC_CO_PE = 0;
	NC_nCO_PE = 0;
	NC_CO_COPE = 0;
	NC_nCO_COPE = 0;
	NC_CO_nCOPE = 0;
	NC_nCO_nCOPE = 0;
	NC_inco = 0;
	NC_PE_CO = 0;
	
	NC_scat_PE_null = 0;
	NC_scat_CO_PE = 0;
	NC_scat_nCO_PE = 0;
	NC_scat_CO_COPE = 0;
	NC_scat_nCO_COPE = 0;
	NC_scat_CO_nCOPE = 0;
	NC_scat_nCO_nCOPE = 0;
	NC_scat_inco = 0;
	NC_scat_PE_CO = 0;
	
	NC_multiphot = 0;
	NC_other = 0;
	
	return 0;
}

int CC_CoincidenceClassification::Print() const
{
	cout<<"***********************************************************"<<endl;
	cout<<"*** CC_CoincidenceClassification::PrintClassification ***"<<endl;
	cout<<"***********************************************************"<<endl;
	cout<<"* NC_prompt   = "<<NC_prompt   <<endl;
	cout<<"* NC_good     = "<<NC_good   <<endl;
	cout<<"* NC_rand     = "<<NC_rand     <<endl;
	cout<<"* NC_scat     = "<<NC_scat     <<endl;
	cout<<"* NC_BackScat = "<<NC_BackScat <<endl;
	cout<<"* NC_others   = "<<NC_others   <<endl;
	cout<<"* NC_PE_null	     = "<<NC_PE_null<<endl;
	cout<<"* NC_CO_PE	     = "<<NC_CO_PE<<endl;
	cout<<"* NC_nCO_PE	     = "<<NC_nCO_PE<<endl;
	cout<<"* NC_CO_COPE	     = "<<NC_CO_COPE<<endl;
	cout<<"* NC_nCO_COPE	     = "<<NC_nCO_COPE<<endl;
	cout<<"* NC_CO_nCOPE	     = "<<NC_CO_nCOPE<<endl;
	cout<<"* NC_nCO_nCOPE	     = "<<NC_nCO_nCOPE<<endl;
	cout<<"* NC_inco	     = "<<NC_inco<<endl;
	cout<<"* NC_PE_CO	     = "<<NC_PE_CO<<endl;
	cout<<"* NC_scat_PE_null   = "<<NC_scat_PE_null<<endl;
	cout<<"* NC_scat_CO_PE     = "<<NC_scat_CO_PE<<endl;
	cout<<"* NC_scat_nCO_PE    = "<<NC_scat_nCO_PE<<endl;
	cout<<"* NC_scat_CO_COPE   = "<<NC_scat_CO_COPE<<endl;
	cout<<"* NC_scat_nCO_COPE  = "<<NC_scat_nCO_COPE<<endl;
	cout<<"* NC_scat_CO_nCOPE  = "<<NC_scat_CO_nCOPE<<endl;
	cout<<"* NC_scat_nCO_nCOPE = "<<NC_scat_nCO_nCOPE<<endl;
	cout<<"* NC_scat_inco	     = "<<NC_scat_inco<<endl;
	cout<<"* NC_scat_PE_CO     = "<<NC_scat_PE_CO<<endl;
	cout<<"* NC_multiphot      = "<<NC_multiphot<<endl;
	cout<<"* NC_other          = "<<NC_other<<endl;
	cout<<"***********************************************************"<<endl;
	return 0;
}

int CC_CoincidenceClassification::Classify(const map<int,EventClass>& anEventClassificationMap
			, map<int,CoincClass>& aCoincClassificationMap
			, HistoClass* io_histograms )
{
	for( map<int,CoincClass>::const_iterator coinc_it = aCoincClassificationMap.begin(); coinc_it != aCoincClassificationMap.end(); coinc_it++)
	{
		NC_prompt++;
		HitClass hit1 = coinc_it->second.hit1;
		HitClass hit2 = coinc_it->second.hit2;
		map<int,EventClass>::const_iterator ev_it = anEventClassificationMap.find(hit1.ev); //look for correspondent event classification
		
		if (hit1.ev != hit2.ev) 
		{ //is random
			NC_rand++;
		}
		else 
		{ //is not random
			if(ev_it==anEventClassificationMap.end()) cout<<"ERROR!!!!"<<endl;
			else 
			{
				if(ev_it->second.classification == CC_EvClassification::PE_null        ) 
				{
					NC_PE_null++;
					cout<<"PE_null at event num "<<hit1.ev<<endl;
				}
				else if(ev_it->second.classification == CC_EvClassification::CO_PE	  ) NC_CO_PE++;
				else if(ev_it->second.classification == CC_EvClassification::nCO_PE	  ) NC_nCO_PE++;
				else if(ev_it->second.classification == CC_EvClassification::CO_COPE    ) NC_CO_COPE++;
				else if(ev_it->second.classification == CC_EvClassification::nCO_COPE   ) NC_nCO_COPE++;
				else if(ev_it->second.classification == CC_EvClassification::CO_nCOPE   ) NC_CO_nCOPE++;
				else if(ev_it->second.classification == CC_EvClassification::nCO_nCOPE  ) NC_nCO_nCOPE++;
				else if(ev_it->second.classification == CC_EvClassification::inco	        ) 
				{
					NC_inco++;
					cout<<"incomplete at event num "<<hit1.ev<<endl;
				}
				else if(ev_it->second.classification == CC_EvClassification::PE_CO	        ) NC_PE_CO++;
				else if(ev_it->second.classification == CC_EvClassification::scat_PE_null   ) NC_scat_PE_null++;
				else if(ev_it->second.classification == CC_EvClassification::scat_CO_PE		) NC_scat_CO_PE++;
				else if(ev_it->second.classification == CC_EvClassification::scat_nCO_PE	) NC_scat_nCO_PE++;
				else if(ev_it->second.classification == CC_EvClassification::scat_CO_COPE   ) NC_scat_CO_COPE++;
				else if(ev_it->second.classification == CC_EvClassification::scat_nCO_COPE  ) NC_scat_nCO_COPE++;
				else if(ev_it->second.classification == CC_EvClassification::scat_CO_nCOPE  ) NC_scat_CO_nCOPE++;
				else if(ev_it->second.classification == CC_EvClassification::scat_nCO_nCOPE ) NC_scat_nCO_nCOPE++;
				else if(ev_it->second.classification == CC_EvClassification::scat_inco	) NC_scat_inco++;
				else if(ev_it->second.classification == CC_EvClassification::scat_PE_CO	) NC_scat_PE_CO++;
				else if(ev_it->second.classification == CC_EvClassification::multiphot	) NC_multiphot++;
				else if(ev_it->second.classification == CC_EvClassification::other      ) 
				{
					NC_other++;
					cout<<"other at event num "<<hit1.ev<<endl;
				}
				else 
				{
					cout<<"weird: class = "<<ev_it->second.classification<<" for event "<<hit1.ev<<endl;
				}
			}
		}
    }	// end of for loop
	NC_good = NC_CO_PE+NC_nCO_PE+NC_CO_COPE+NC_nCO_COPE+NC_CO_nCOPE+NC_nCO_nCOPE;
	NC_scat = NC_scat_CO_PE+NC_scat_nCO_PE+NC_scat_CO_COPE+NC_scat_nCO_COPE+NC_scat_CO_nCOPE+NC_scat_nCO_nCOPE+NC_scat_inco;
	NC_BackScat = NC_scat_PE_CO+NC_PE_CO;
	NC_others = NC_other+NC_multiphot+NC_inco+NC_PE_null+NC_scat_PE_null;
	
	return 0;
}
    
    
    
