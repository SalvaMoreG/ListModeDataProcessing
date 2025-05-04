#ifndef Utilities_H__
#define Utilities_H__

#include "globalheaders.hh"

double 			Distance3DVector(double X1, double Y1, double Z1, double X2, double Y2, double Z2);
double 			AngleBetween3DVectors(double X1, double Y1, double Z1, double X2, double Y2, double Z2);
double 			ComptonAngle(double Ein, double Eout);
std::string 	IntegerToString ( int Number );

template <typename Predicate> std::list <std::string> split( const std::string& s, Predicate p );

unsigned 		CountWords( const std::string& s );
unsigned 		CountUnderlines( const std::string& s );

// TODO: why here?
bool 			IsInRightVolume(std::string current_volume, std::vector<std::string> volumes);

double 			DistancePointToLine(double Px, double Py, double Pz, double L1x, double L1y, double L1z, double L2x, double L2y, double L2z);
double 			CircularSegmentArea(double R, double h);
double 			CalcHypo(double Cath1, double Cath2);
double 			CalcCath(double Hypo, double Cath);
double 			AreaRightTriangle(double Cath1, double Cath2);

int 			ShowTokens_Punct( const std::string& s );

bool 			fileExists(const std::string& filename);

#endif
