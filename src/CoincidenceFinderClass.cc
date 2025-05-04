
#include "CoincidenceFinderClass.hh"
#include "InputFlags.hh"

using namespace std;

CoincidenceFinderClass::CoincidenceFinderClass()
	: m_histograms(NULL)
	, m_scanner(NULL)
{
}

CoincidenceFinderClass::~CoincidenceFinderClass()
{
}

// =======================================================================

HitClass 
CoincidenceFinderClass::FindBestComptonAngle(const HitClass& hit1, const HitClass& hit2, const HitClass& hit3, bool verbose) const
{
	HitClass COhit;
	double x1=0,y1=0,z1=0,e1=0;
	double x2=0,y2=0,z2=0,e2=0;
	double x3=0,y3=0,z3=0,e3=0;
	
	x1 = hit1.X; y1 = hit1.Y; z1 = hit1.Z; e1 = hit1.E;
	x2 = hit2.X; y2 = hit2.Y; z2 = hit2.Z; e2 = hit2.E;
	x3 = hit3.X; y3 = hit3.Y; z3 = hit3.Z; e3 = hit3.E;
	
	double A1x = x2-x1;
	double A1y = y2-y1;
	double A1z = z2-z1;

	double A2x = x3-x2;
	double A2y = y3-y2;
	double A2z = z3-z2;

	double B1x = x3-x1;
	double B1y = y3-y1;
	double B1z = z3-z1;

	double B2x = x2-x3;
	double B2y = y2-y3;
	double B2z = z2-z3;

	double modA1 = sqrt(pow(A1x,2)+pow(A1y,2)+pow(A1z,2));
	double modA2 = sqrt(pow(A2x,2)+pow(A2y,2)+pow(A2z,2));
	double modB1 = sqrt(pow(B1x,2)+pow(B1y,2)+pow(B1z,2));
	double modB2 = sqrt(pow(B2x,2)+pow(B2y,2)+pow(B2z,2));
	double cosA = (A1x*A2x+A1y*A2y+A1z*A2z)/(modA1*modA2);
	double cosB = (B1x*B2x+B1y*B2y+B1z*B2z)/(modB1*modB2);
	double E0 = 0.511;
	double EgA = e3;
	double EeA = e2;
	double EgB = e2;
	double EeB = e3;
	double cosA_e = 1-E0/EgA+E0/(EgA+EeA);
	double cosB_e = 1-E0/EgB+E0/(EgB+EeB);
	
	if (verbose)
	{
		cout<<"cosA = "<<cosA<<endl;
		cout<<"cosA_e = "<<cosA_e<<endl;
		cout<<"cosB = "<<cosB<<endl;
		cout<<"cosB_e = "<<cosB_e<<endl;
	}
	
	// differences between geometric angle and energy-calculated angle
	double DA = fabs(cosA - cosA_e);
	double DB = fabs(cosB - cosB_e);
	
	DA < DB ? COhit = hit2 : COhit = hit3;
	
	COhit.E = hit2.E + hit3.E;
	
	if (hit2.ev != hit3.ev) 
	{
		COhit.ev = -1;
	}
	
	return COhit;
}

// =======================================================================

// Doing Compton Recovery for coincidences of 3 hits
int 
CoincidenceFinderClass::ComptonRecovery_3Hits(map<unsigned long long int, HitClass>& io_hitMap
						, COINCIDENCE_TYPE in_coincidenceType) const
{
	if ( !m_scanner ) return -1;
	if ( io_hitMap.size() != 3) return -1;

	map<unsigned long long int,HitClass>::iterator coinc_it = io_hitMap.begin();
	HitClass hit1 = coinc_it->second; //saving first hit
	coinc_it++;
	HitClass hit2 = coinc_it->second; //saving second hit
	coinc_it++;
	HitClass hit3 = coinc_it->second; //saving third  hit

	HitClass coinc_hit1;
	HitClass coinc_hit2;

	// Telling the PE single hit (coinc_hit1) from the CC+PE pair (coinc_hit2)
	// |hit.E - source_E| closest to 0 --> hit = PE single hit 
	if (   fabs(hit1.E - m_scanner->GetSourceE(in_coincidenceType)) 
				<= fabs(hit2.E - m_scanner->GetSourceE(in_coincidenceType)) 
		&& fabs(hit1.E - m_scanner->GetSourceE(in_coincidenceType)) 
				<= fabs(hit3.E - m_scanner->GetSourceE(in_coincidenceType))) 
	{
		coinc_hit1 = hit1; 	// single PE hit
		coinc_hit2 = FindBestComptonAngle(hit1,hit2,hit3);
	}
	else if (   fabs(hit2.E - m_scanner->GetSourceE(in_coincidenceType)) 
					<= fabs(hit1.E - m_scanner->GetSourceE(in_coincidenceType)) 
			 && fabs(hit2.E - m_scanner->GetSourceE(in_coincidenceType)) 
					<= fabs(hit3.E - m_scanner->GetSourceE(in_coincidenceType))) 
	{
		coinc_hit1 = hit2;	// single PE hit
		coinc_hit2 = FindBestComptonAngle(hit2,hit1,hit3);
	}
	else if (   fabs(hit3.E - m_scanner->GetSourceE(in_coincidenceType)) 
					<= fabs(hit2.E - m_scanner->GetSourceE(in_coincidenceType)) 
			 && fabs(hit3.E - m_scanner->GetSourceE(in_coincidenceType)) 
					<= fabs(hit1.E - m_scanner->GetSourceE(in_coincidenceType))) 
	{
		coinc_hit1 = hit3;	// single PE hit
		coinc_hit2 = FindBestComptonAngle(hit3,hit2,hit1);
	}

	io_hitMap.clear();
	io_hitMap[coinc_hit1.T] = coinc_hit1;

	if (coinc_hit2.T == coinc_hit1.T) coinc_hit2.T++;		
		// because of merging, a step time can be set to an already used time...

	io_hitMap[coinc_hit2.T] = coinc_hit2;

	return 0;
}

// =======================================================================

int
CoincidenceFinderClass::SaveCoincidences(const string& in_filename, const map<int, CoincClass>& in_coincMap, bool doAppend) const
{
	ofstream outF;
	ofstream outF_realHitPos;
	string fn_realHitPos;
	if (InputFlags::Instance()->doRealHitPosOutput)
	{
		fn_realHitPos = in_filename;
		fn_realHitPos += "_realPos";
	}

	string filename( in_filename );
	if (InputFlags::Instance()->doWriteAddSrcPosition)	// add event ID and src pos (if identical for both) for each prompt
	{
		filename += "_addedSrcPos";
		fn_realHitPos += "_addedSrcPos";
	}
	if ( InputFlags::Instance()->doWriteAddEventID )
	{
		filename += "_addedEvt";
		fn_realHitPos += "_addedEvt";
	}
    if ( InputFlags::Instance()->doWriteAddClassificationFlag )
    {
        filename += "_addedFlag";
        fn_realHitPos += "_addedFlag";
    }

	// open files (either as new, or to append)
	if ( doAppend )
	{
		outF.open( filename.c_str(),ios::app );
		if (InputFlags::Instance()->doRealHitPosOutput)
			outF_realHitPos.open( fn_realHitPos.c_str(),ios::app );
	}
	else
	{
		outF.open( filename.c_str(),ios::out);
		if (InputFlags::Instance()->doRealHitPosOutput)
			outF_realHitPos.open( fn_realHitPos.c_str(),ios::out );
	}

	// check file is open
	if (!(outF.is_open()))
    {
      cout << "ERROR CoincidenceFinderClass::SaveCoincidences()" << endl;
      cout << "file <" << filename << "> not open" << endl;
      return -1;
    }

	double factorToKeV( 1000.0 );

  	for (map<int, CoincClass>::const_iterator it = in_coincMap.begin(); it != in_coincMap.end(); it++)
    {
		// write out PROMPTS with measured hit positions
		outF << it->second.hit1.Z << " ";
		outF << it->second.hit1.Y << " ";
		outF << it->second.hit1.X << " ";
		outF << it->second.hit1.E*factorToKeV << " ";		// Added 2013 May31, Machiel. SOE and LMOSEM work in keV

		outF << it->second.hit2.Z << " ";
		outF << it->second.hit2.Y << " ";
		outF << it->second.hit2.X << " ";
		outF << it->second.hit2.E*factorToKeV << " " ;		// Added 2013 May31, Machiel. SOE and LMOSEM work in keV

		if (InputFlags::Instance()->doWriteAddSrcPosition)
        {
			if (it->second.hit1.ev == it->second.hit2.ev)
			{
				outF << it->second.hit1.ev << "  " 
					 << it->second.hit1.sourceX << " " 
					 << it->second.hit1.sourceY << " " 
					 << it->second.hit1.sourceZ << " " ;
			}
			else
			{
				outF << "-1   -1   -1   -1";
			}
        }
		if ( InputFlags::Instance()->doWriteAddClassificationFlag )
        {
            outF << " " << it->second.m_classification ;
        }
		if ( InputFlags::Instance()->doWriteAddEventID )
		{
			outF << " " << it->second.hit1.ev << "  " << it->second.hit2.ev ;
		}
		outF << endl;

 		if (InputFlags::Instance()->doRealHitPosOutput)
 		{
 			// write out PROMPTS with real hit positions (the real gamma impact points)
 			outF_realHitPos << it->second.hit1.realZ << " ";
 			outF_realHitPos << it->second.hit1.realY << " ";
 			outF_realHitPos << it->second.hit1.realX << " ";

 			outF_realHitPos << it->second.hit1.E*factorToKeV << " ";

 			outF_realHitPos << it->second.hit2.realZ << " ";
 			outF_realHitPos << it->second.hit2.realY << " ";
 			outF_realHitPos << it->second.hit2.realX << " ";

 			outF_realHitPos << it->second.hit2.E*factorToKeV << " " ;

			if ( InputFlags::Instance()->doWriteAddClassificationFlag )
        	{
           		outF_realHitPos << " " << it->second.m_classification ;
        	}
 			if (InputFlags::Instance()->doWriteAddEventID)		// add event IDs for each prompt
 			{
 				outF_realHitPos << " " << it->second.hit1.ev << "  " << it->second.hit2.ev ;
 			}
 			outF_realHitPos << endl;
		 }

    }
	outF.close();

	if (InputFlags::Instance()->doRealHitPosOutput)
		outF_realHitPos.close();

  	return 0;
}
