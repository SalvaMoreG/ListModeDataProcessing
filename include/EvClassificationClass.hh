#ifndef EvClassificationClass_H__
#define EvClassificationClass_H__

#include "globalheaders.hh"
#include "TreeClass.hh"
#include "EventClass.hh"
#include "Utilities.hh"

#include "Scanner.hh"
#include "HistoClass.hh"

class EvClassificationClass
{
public:
  						EvClassificationClass();
	virtual				~EvClassificationClass();


	virtual int 		InitializeCounters() = 0;
	virtual int 		InitializeGlobalCounters() = 0;
	virtual int 		UpdateCounters(TreeClass* aTree, int StepIndex) = 0;
	virtual int 		ClassifyEvent(TreeClass *aTree, std::map<int,EventClass> &theEventClassificationMap) = 0;
	virtual int 		PrintClassification() const = 0;

	inline void			SetHistograms( HistoClass* in_histograms )
						{
							theHistograms = in_histograms;
						}
	inline void			SetScanner( const Scanner* in_scanner )
						{
							m_scanner = in_scanner;
						}

	const char*			GetClassificationDescription() { return " "; }

protected:
	void				PrintSomeInfo( const TreeClass* aTree ) const;

	HistoClass*			theHistograms;
	const Scanner*		m_scanner;

private:
};


#endif // #ifdef EvClassificationClass_H__
