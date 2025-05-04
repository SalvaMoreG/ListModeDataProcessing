#ifndef globalheaders_H__
#define globalheaders_H__

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TCollection.h"
#include "TKey.h"
#include "TRandom3.h"

#include "iostream"
#include "iomanip"
#include "fstream"
#include "vector"
#include "map"
#include "string"
#include "cstdlib"
#include "cstdio"
#include "cmath"
#include "cassert"
#include "ctime"

#include "algorithm"
#include "cctype"
#include "functional"
#include "list"
#include "sstream"

// using namespace std; NEVER USE NAMESPACE IN A HEADER

enum axis
  {
    X,
    Y,
    Z
  };

enum DETECTOR_TYPE
{
	DETECTOR_TYPE_NONE = 0,
	DETECTOR_TYPE_ABSORBER, 	// PET or PEM or Compton Camera
	DETECTOR_TYPE_SCATTERER		// Compton camera
};

enum INPUT_MODE
{
	IN_ROOT = 0,	// default
	IN_LM			// List mode (ascii)
};

enum OUTPUT_MODE
{
	OUT_LMANDPROMPTS = 0,		// default
	OUT_LM,						// List mode (ascii) file of hits
	OUT_PROMPTS					// Prompts (coincidences)
};

#endif // #ifndef globalheaders_H__

