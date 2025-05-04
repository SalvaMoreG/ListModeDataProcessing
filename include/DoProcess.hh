#ifndef __DoProcess_H__
#define __DoProcess_H__

#include "globalheaders.hh"
#include "InputFlags.hh"

#include "ProcessDataClass.hh"
#include "HistoClass.hh"

#include "Scanner.hh"
#include "DetectorFactory.hh"

#include <string>

#include "CC_EvClassification.hh"
#include "CC_CoincidenceFinder.hh"

#include "PET_EvClassification.hh"
#include "PET_CoincidenceFinder.hh"

#include "CCPET_CoincidenceFinder.hh"

class DoProcess
{
public:
							DoProcess();
	virtual 				~DoProcess();

	// main method
	virtual int				Execute( int argc, char *argv[] );

protected:

	// methods
	virtual int 			ProcessInputs(int argc, char *argv[]);
	virtual void			DoDataLoop();
	virtual	void			CreateGeneralObjects();
	virtual void			DoCreateHistoos() = 0;

	// more methods
	virtual CoincidenceFinderClass*			CreateCoincidenceFinder() = 0;
	virtual EvClassificationClass*			CreateEvClassification() = 0;

//	virtual CoincidenceClassificationClass* CreateCoincidenceClassification() = 0;

	// data	
	SCAN_TYPE				m_scanType;

	std::ifstream 			theListFile;

	std::string 			theOutputsDirName;
	std::string 			theListFilePath;
	std::string 			theDetConfFilePath;
	// std::string 			theChShRadFilePath;
	// std::string 			thePixPosFilePath;
	
	ProcessDataClass* 		theDataProcess;
	HistoClass* 			theGlobalHistograms;
};

// ===================================

class PEM_DoProcess : public DoProcess
{
public:
							PEM_DoProcess();
			 				~PEM_DoProcess();

protected:
	virtual void			DoCreateHistoos();

	virtual CoincidenceFinderClass*			CreateCoincidenceFinder() { return new PET_CoincidenceFinder(); }
	virtual EvClassificationClass*			CreateEvClassification() { return new PET_EvClassification(); }
};

// ===================================

class PET_DoProcess : public DoProcess
{
public:
							PET_DoProcess();
			 				~PET_DoProcess();

protected:
	virtual void			DoCreateHistoos();

	virtual CoincidenceFinderClass*			CreateCoincidenceFinder() { return new PET_CoincidenceFinder(); }
	virtual EvClassificationClass*			CreateEvClassification() { return new PET_EvClassification(); }

};

// ===================================

class CC_DoProcess : public DoProcess
{
public:
							CC_DoProcess();
			 				~CC_DoProcess();

protected:
	virtual void			DoCreateHistoos();

	virtual CoincidenceFinderClass*			CreateCoincidenceFinder() { return new CC_CoincidenceFinder(); }
	virtual EvClassificationClass*			CreateEvClassification() { return new CC_EvClassification(); }
};

// ===================================

class CCPET_DoProcess : public DoProcess
{
public:
							CCPET_DoProcess();
			 				~CCPET_DoProcess();

protected:
	virtual void			DoCreateHistoos();

	virtual CoincidenceFinderClass*			CreateCoincidenceFinder() { return new CCPET_CoincidenceFinder(); }
	virtual EvClassificationClass*			CreateEvClassification()
            { /*return new CCPET_EvClassification();*/ return NULL; }
};

// ===================================

class ConvertROOTtoLM_DoProcess : public DoProcess
{
public:
                            ConvertROOTtoLM_DoProcess();
                            ~ConvertROOTtoLM_DoProcess();

    // main method
//    virtual int             Execute( int argc, char *argv[] );

protected:

    // methods
    virtual int             ProcessInputs(int argc, char *argv[]);

    virtual void            DoCreateHistoos() {}
    virtual CoincidenceFinderClass*         CreateCoincidenceFinder() { return NULL; }
    virtual EvClassificationClass*          CreateEvClassification()  { return new PET_EvClassification(); }
        // { return NULL; }
};


// ===================================

#endif // #define DoProcess_H__
