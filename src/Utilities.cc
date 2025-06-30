#include "Utilities.hh"
 
#include <sys/stat.h>	 // for "fileExists" function

using namespace std;

double Distance3DVector(double X1, double Y1, double Z1, double X2, double Y2, double Z2)
{
  double dist = 0;
  dist  = pow(X1-X2,2);
  dist += pow(Y1-Y2,2);
  dist += pow(Z1-Z2,2);
  if(dist>0) return sqrt(dist);
  return 0;
}

double AngleBetween3DVectors(double X1, double Y1, double Z1, double X2, double Y2, double Z2)
{
  double mycos = 0;
  double mod1 = sqrt(pow(X1,2)+pow(Y1,2)+pow(Z1,2));
  double mod2 = sqrt(pow(X2,2)+pow(Y2,2)+pow(Z2,2));
  mycos = X1*X2+Y1*Y2+Z1*Z2/(mod1*mod2);
  if(mycos>=-1 && mycos <=1) return acos(mycos);
  return 0;
}

double ComptonAngle(double Ein, double Eout)
{
  double mycos = 0;
  mycos = 1-0.511/Eout+0.511/(Ein);
  if(mycos>=-1 && mycos <=1) return acos(mycos);
  return 0;
}

double DistancePointToLine(double Px, double Py, double Pz, double L1x, double L1y, double L1z, double L2x, double L2y, double L2z)
{
  // cout<<"Px = "<<Px<<endl;
  // cout<<"Py = "<<Px<<endl;
  // cout<<"Pz = "<<Px<<endl;
  // cout<<"L1x = "<<L1x<<endl;
  // cout<<"L1y = "<<L1x<<endl;
  // cout<<"L1z = "<<L1x<<endl;
  // cout<<"L2x = "<<L2x<<endl;
  // cout<<"L2y = "<<L2x<<endl;
  // cout<<"L2z = "<<L2x<<endl;

  double dist = 0;

  double x3 = L2x-L1x;
  double y3 = L2y-L1y;
  double z3 = L2z-L1z;
  double x4 = Px-L1x;
  double y4 = Py-L1y;
  double z4 = Pz-L1z;
  double a = pow(y3*z4-z3*y4,2);
  a += pow(x3*z4-z3*x4,2);
  a += pow(x3*y4-y3*x4,2);
  if(a>=0) a = sqrt(a);
  else a = 0;
  double AB = pow(x3,2)+pow(y3,2)+pow(z3,2);
  if(AB>=0) AB = sqrt(AB);
  else AB = 0;
  if(AB!=0) dist = a/AB;
  else dist = 0;

  return dist;
}

template <typename Predicate>
list <string> split( const string& s, Predicate p )
{
  list <string> result;
  string::const_iterator rtok;
  string::const_iterator ltok = find_if( s.begin(), s.end(), not1( p ) );
  while (ltok != s.end())
    {
      rtok = find_if( ltok, s.end(), p );
      result.push_back( string( ltok, rtok ) );
      ltok = find_if( rtok, s.end(), not1( p ) );
    }
  return result;
}

unsigned CountWords( const string& s )
{
  return split( s, ptr_fun <int, int> ( isspace ) ).size();
}

unsigned CountUnderlines( const string& s )
{
  return split( s, ptr_fun <int, int> ( ispunct ) ).size();
}

int ShowTokens_Punct( const string& s )
{
  list<string> mylist = split( s, ptr_fun <int, int> ( ispunct ) );
  for(list<string>::iterator it=mylist.begin(); it!=mylist.end(); it++)
    {
      cout<<(*it)<<endl;
    }
  return 0;
}

bool IsInRightVolume(string current_volume, vector<string> volumes)
{
//	cout << "IsInRightVolume, current_volume: " << current_volume << endl;
//	cout << "volumes.size: " << volumes.size() << endl;
	for(unsigned int i=0; i<volumes.size(); i++)
	{
//		cout << i << " loop volume: " << volumes.at(i) << endl;
		if(current_volume == volumes.at(i))
		{
			return true;
		}
	}
  return false;
}

double AreaRightTriangle(double Cath1, double Cath2)
{
  return Cath1*Cath2/2;
}

double CalcHypo(double Cath1, double Cath2)
{
  if(pow(Cath1,2)+pow(Cath2,2) >= 0) 
    {
      return sqrt(pow(Cath1,2)+pow(Cath2,2));
    }
  return 0;
}

double CalcCath(double Hypo, double Cath)
{
  if(pow(Hypo,2)-pow(Cath,2) >= 0)
    {
      return sqrt(pow(Hypo,2)-pow(Cath,2));
    }
  return 0;
}

double CircularSegmentArea(double R, double h)
{
  //from http://mathworld.wolfram.com/CircularSegment.html
  double A = 0;
  if(R != 0) 
    {
      if((R-h)/R >= -1 && (R-h)/R <= 1) 
	{
	  A = acos((R-h)/R);
	}
    }
  A *= pow(R,2);
  if(h*(2*R-h) >= 0) {
    A -= (R-h)*sqrt(h*(2*R-h));
    return A;
  }
  return 0;
}


string IntegerToString ( int Number )
{
  stringstream ss;
  ss << Number;
  return ss.str();
}


bool fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

