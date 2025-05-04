#ifndef HistoClass_H__
#define HistoClass_H__

#include "globalheaders.hh"

class HistoClass
{
public:
					HistoClass();
					~HistoClass();

	int 			Add1DHisto(TString, TString, int, double, double);
	int 			Add2DHisto(TString, TString, int, double, double, int, double, double);
	int 			SaveAllHistos(TFile* f, bool overwrite = 0);
	int 			SaveSingleHisto(TFile* f, TString name, bool overwrite = 0);
	int 			SaveHistos(TString filename, bool recreate = 1, bool update = 0);
	int 			Fill1DHisto(TString name, double var, double weight = 1);
	int 			Fill2DHisto(TString name, double var1, double var2, double weight = 1);
	
	std::map<TString,TH1F*> the1DHistos;
	std::map<TString,TH2F*> the2DHistos;

};


#endif // #define HistoClass_H__
