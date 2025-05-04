#ifndef LocalPixelPositionClass_H__
#define LocalPixelPositionClass_H__

#include "globalheaders.hh"

class LocalPixelPositionClass
{
public:
  				LocalPixelPositionClass(std::string filename);
	virtual 	~LocalPixelPositionClass(){};

	int 		GetNrows(){return N_rows;};
	int 		GetNcolumns(){return N_columns;};
	int 		GetNblocks(){return N_blocks;};
	double 		GetX(int r, int c, int b){return (N_columns*N_rows*b+N_rows*c+r < X.size()) ? X.at(N_columns*N_rows*b+N_rows*c+r) : -123456;};
	double 		GetY(int r, int c, int b){return (N_columns*N_rows*b+N_rows*c+r < Y.size()) ? Y.at(N_columns*N_rows*b+N_rows*c+r) : -123456;};
	double 		GetZ(int r, int c, int b){return (N_columns*N_rows*b+N_rows*c+r < Z.size()) ? Z.at(N_columns*N_rows*b+N_rows*c+r) : -123456;};

protected:
  int 			N_rows;
  int 			N_columns;
  int 			N_blocks;
  std::vector<double> X;
  std::vector<double> Y;
  std::vector<double> Z;
};


#endif // #define LocalPixelPositionClass_H__
