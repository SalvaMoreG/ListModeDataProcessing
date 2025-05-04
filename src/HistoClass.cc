#include "HistoClass.hh"

using namespace std;

HistoClass::HistoClass()
{
  the1DHistos.clear();
  the2DHistos.clear();
}

HistoClass::~HistoClass()
{
	for(map<TString,TH1F*>::iterator it = the1DHistos.begin();it != the1DHistos.end(); it++)
	{
		TH1F* h1 = (it->second);
		delete h1;
	}

	for(map<TString,TH2F*>::iterator it = the2DHistos.begin();it != the2DHistos.end(); it++)
	{
		TH2F* h2 = (it->second);
		delete h2;
	}
//	cout << "dtor HistoClass::~HistoClass() terminated fine" << endl;
}

int HistoClass::Add1DHisto(TString name, TString title, int Nbins, double min, double max)
{
	TH1F* h = new TH1F(name, title, Nbins, min, max);
	the1DHistos[name] = h;
	return 0;
}

int HistoClass::Add2DHisto(TString name, TString title, int NbinsX, double Xmin, double Xmax, int NbinsY, double Ymin, double Ymax)
{
	TH2F* h = new TH2F(name, title, NbinsX, Xmin, Xmax, NbinsY, Ymin, Ymax);
	the2DHistos[name] = h;
	return 0;
}

int HistoClass::Fill1DHisto(TString name, double var, double weight)
{
	map<TString,TH1F*>::iterator it = the1DHistos.find(name);
	if (it == the1DHistos.end())
	{
		cout << "ERROR in HistoClass::Fill1DHisto" << endl;
		cout << "histogram <" << name << "> not found" << endl;
		return -1;
	}
	else
	{
		it->second->Fill(var, weight);
	}
  return 0;
}

int HistoClass::Fill2DHisto(TString name, double var1, double var2, double weight)
{
	map<TString,TH2F*>::iterator it = the2DHistos.find(name);
	if(it == the2DHistos.end())
	{
		cout<<"ERROR in HistoClass::Fill2DHisto"<<endl;
		cout<<"histogram <"<<name<<"> not found"<<endl;
		return -1;
	}
	else
	{
		it->second->Fill(var1, var2, weight);
	}
	return 0;
}

int HistoClass::SaveAllHistos(TFile* f, bool overwrite)
{
	Int_t option = 0;
	if (overwrite) option = TObject::kOverwrite;
	if(f->IsOpen()) 
	{
		f->cd();
	}
	else
	{
		cout << "ERROR in HistoClass::SaveAllHistos" << endl;
		cout << "file " << f->GetName() << " is not open." << endl;
		return -1;
	}

	for(map<TString,TH1F*>::iterator it = the1DHistos.begin();it != the1DHistos.end(); it++)
	{
		it->second->Write(it->first,option);
	}

	for(map<TString,TH2F*>::iterator it = the2DHistos.begin();it != the2DHistos.end(); it++)
	{
		it->second->Write(it->first,option);
	}

	return 0;
}

int HistoClass::SaveSingleHisto(TFile* f, TString name, bool overwrite)
{
	Int_t option = 0;
	if(overwrite) option = TObject::kOverwrite;
	
	if(f->IsOpen()) 
	{
		f->cd();
	}
	else
	{
		cout<<"ERROR in HistoClass::SaveHisto"<<endl;
		cout<<"file "<<f->GetName()<<" is not open."<<endl;
		return -1;
	}
	map<TString,TH1F*>::iterator it = the1DHistos.find(name);
	if(it != the1DHistos.end())
	{
		it->second->Write(it->first,option);
	}
	map<TString,TH2F*>::iterator it2 = the2DHistos.find(name);
	if(it2 != the2DHistos.end())
	{
		it2->second->Write(it2->first,option);
	}
	return 0;
}

int HistoClass::SaveHistos(TString filename, bool recreate, bool update)
{
// cout << "HistoClass::SaveHistos, filename: " << filename << " recreate: " << recreate << " update: " << update << endl;
	TFile* f = 0;
	if(recreate)
	{
		f = new TFile(filename,"RECREATE");
	}
	else
	{
		f = new TFile(filename,"UPDATE");
	}
	if (!f)
	{
		cout<<"ERROR in HistoClass::SaveHistos"<<endl;
		cout<<"file "<<filename<<" not undefined."<<endl;
		return -1;
	}
	if (!(f->IsOpen()))
	{
		cout<<"ERROR in HistoClass::SaveHistos"<<endl;
		cout<<"file "<<filename<<" not open."<<endl;
		delete f;
		return -2;
	}
	SaveAllHistos(f, update);

	f->Close();	// <------------ IMPORTANT TO call "TFile::Close". If not, we get a ROOT segmentation fault when ending the program
	delete f;
// cout << "End of HistoClass::SaveHistos" << endl;
}
