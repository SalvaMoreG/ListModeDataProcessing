#ifndef ProcessDataClass_H__
#define ProcessDataClass_H__

#include "globalheaders.hh"

#include "HitReconstructionClass.hh"

#include "EvClassificationClass.hh"
#include "CoincidenceFinderClass.hh"
#include "CoincidenceClassificationClass.hh"
#include "HitClass.hh"
#include "EventClass.hh"
#include "CoincClass.hh"
#include "HistoClass.hh"
#include "Scanner.hh"

#include "MySimpleGmSDTTree.h"

#include <fstream>

class DoProcess;

class ProcessDataClass 
{
public:

					ProcessDataClass( SCAN_TYPE in_scanType, const std::string& in_DetConfFilePath );
								// , const std::string& in_PixPosFilePath
                                // , const std::string& in_ChShRadFilePath );
	virtual			~ProcessDataClass();

	int 			LoadTTree(const std::string& RootFileName);
	int 			ProcessTree( int minentry, int maxentry );	// returns 1 when not at end of ROOT file, 0 when done

	int 			DistributeGlobalPointers();

	int 			ProcessLM( const std::string& in_outdirName );
	int 			AddHitToLM(unsigned long long int anEventTime);
	void			ClearLMTimeMap();

	void			DoEnergySmearing();

	int 			FlushLastRecHits();

	int 			PrintEventClassification() const;
	int 			PrintCoincClassification() const;

	int 			SaveEventClassification(const std::string& );

	int 			LoadLM(const std::string& filename, int minline, int maxline);
	int 			SaveLM(const std::string& , bool update = 0);
    void            SaveSDTTree();

	int 			CheckFlag(const std::string& ) const;

	inline void		SetHistograms( HistoClass* in_histograms ) { theHistograms = in_histograms; }

	// TODO: member data shouldn't be public...
	EvClassificationClass* 				theEvClassification;
	CoincidenceFinderClass* 			theCoincidenceFinder;
	
	std::map<unsigned long long int, HitClass> 	theHitTimeMap;
	std::map<unsigned long long int, HitClass> 	theLMTimeMap;
	std::map<int,EventClass> 					theEventClassificationMap;

private:
	// methods
	int 						InitializeGlobalVariables();
	void						Initialize(const std::string& in_DetConfFilePath);
								//		, const std::string& in_PixPosFilePath
								//		, const std::string& in_ChShRadFilePath );
	// int 						SavePrompts( const std::string& filename, const std::map<int, CoincClass>& in_coincClassificationMap );

	// data
	SCAN_TYPE					m_scanType;
	int 						theFlag;
	TreeClass* 					theTree;

	TRandom3* 					theRandomGenerator;
	HistoClass* 				theHistograms;
	HitReconstructionClass* 	theHitReconstruction;

	const Scanner*				m_scanner;
	DetectorFactory*	 		m_detectorFactory;
	bool						m_promptsAppend;

    MySimpleGmSDTTree*          mySDTTree;

	// Looping LoadLM
	std::ifstream 				m_inLMfile;
	int							m_inLMlinecount;
};


#endif // #define ProcessDataClass_H__
