
#include "HitClass.hh"

using namespace std;

HitClass::HitClass()
{
	ev = -1;
	E = -1;
	E_central = -1;

	X = -1;
	Y = -1;
	Z = -1;

	locX = -1;
	locY = -1;
	locZ = -1;

	realX = -1;
	realY = -1;
	realZ = -1;

	sourceX = -1;
	sourceY = -1;
	sourceZ = -1;

	T = -1;
	T_trigger = -1;

    /*
	depth = -1;
	thickness = -1;
	Phi = Theta = Psi = -1;
    */
    
	UnitID = "";
	merged = false;
	detectorType = DETECTOR_TYPE_ABSORBER; // DETECTOR_TYPE_NONE;
	volumeName = "N.A.";
	primary_process = "N.A.";
	bypass_trigger = false;
	is_neighbor_hit = false;
}

// ======================================================

int HitClass::PrintHit()
{
	cout << "ev = " << ev << endl;
	cout << "E = " << E << endl;
	cout << "E_central = " << E_central << endl;

	cout << "X = " << X << endl;
	cout << "Y = " << Y << endl;
	cout << "Z = " << Z << endl;

	cout << "locX = " << locX << endl;
	cout << "locY = " << locY << endl;
	cout << "locZ = " << locZ << endl;

	cout << "realX = " << realX << endl;
	cout << "realY = " << realY << endl;
	cout << "realZ = " << realZ << endl;

	cout << "T = " << T << endl;
	cout << "T_trigger = " << T_trigger << endl;
    /*
	cout << "depth = " << depth << endl;
	cout << "thickness = " << thickness << endl;
	cout << "Phi = " << Phi << endl;
	cout << "Psi = " << Psi << endl;
	cout << "Theta = " << Theta << endl;
    */
	cout << "UnitID = " << UnitID << endl;
	cout << "merged = " << merged << endl;
	cout << "detector = " << detectorType << endl;
	cout << "primary process = " << primary_process << endl;
	cout << "bypass_trigger = " << bypass_trigger << endl;
	cout << "is_neighbor_hit = " << is_neighbor_hit << endl;
	return 0;
}

// =============================================================

/*
//int HitClass::RotateAndTranslate(HitClass hit0)
//{
//	// double RM_11 = cos(hit0.Phi)*cos(hit0.Psi);
//	// double RM_12 = -cos(hit0.Phi)*sin(hit0.Psi)+sin(hit0.Phi)*sin(hit0.Theta)*cos(hit0.Psi);
//	// double RM_13 = sin(hit0.Phi)*sin(hit0.Psi)+cos(hit0.Phi)*sin(hit0.Theta)*cos(hit0.Psi);
//	// double RM_21 = cos(hit0.Phi)*sin(hit0.Psi);
//	// double RM_22 = cos(hit0.Phi)*cos(hit0.Psi)+sin(hit0.Phi)*sin(hit0.Theta)*sin(hit0.Psi);
//	// double RM_23 = -sin(hit0.Phi)*cos(hit0.Psi)+cos(hit0.Phi)*sin(hit0.Theta)*sin(hit0.Psi);
//	// double RM_31 = -sin(hit0.Theta);
//	// double RM_32 = sin(hit0.Phi)*cos(hit0.Theta);
//	// double RM_33 = cos(hit0.Phi)*cos(hit0.Theta);
//	
//	double cT = cos(hit0.Theta);
//	double sT = sin(hit0.Theta);
//	double cP = cos(hit0.Psi);
//	double sP = sin(hit0.Psi);
//	double cF = cos(hit0.Phi);
//	double sF = sin(hit0.Phi);
//	
//	// double RM_11 = cP*cF - cT*sF*sP;
//	// double RM_12 = cP*sF + cT*cF*sP;
//	// double RM_13 = sP*sT;
//	// double RM_21 = -sP*cF - cT*sF*cP;
//	// double RM_22 = -sP*sF + cT*cF*cP;
//	// double RM_23 = cP*sT;
//	// double RM_31 = sT*sF;
//	// double RM_32 = -sT*cF;
//	// double RM_33 = cT;
//	
//	//inverse
//	double RM_11 = cP*cF - cT*sF*sP;
//	double RM_12 = -sP*cF - cT*sF*cP;
//	double RM_13 = sT*sF;
//	double RM_21 = cP*sF + cT*cF*sP;
//	double RM_22 = -sP*sF + cT*cF*cP;
//	double RM_23 = -sT*cF;
//	double RM_31 = -sP*sT;
//	double RM_32 = -cP*sT;
//	double RM_33 = cT;
//	
//	//axis switch
//	// double RM_13 = cP*cF - cT*sF*sP;
//	// double RM_12 = cP*sF + cT*cF*sP;
//	// double RM_11 = sP*sT;
//	// double RM_33 = -sP*cF - cT*sF*cP;
//	// double RM_32 = -sP*sF + cT*cF*cP;
//	// double RM_31 = cP*sT;
//	// double RM_23 = sT*sF;
//	// double RM_22 = -sT*cF;
//	// double RM_21 = cT;
//	
//	// double pippo = locZ;
//	// locZ = locY;
//	// locY = pippo;
//	
//	// cout<<endl<<__func__<<endl;
//	// cout<<"local: "<<locX<<","<<locY<<","<<locZ<<","<<endl;
//	// cout<<"local 0: "<<hit0.locX<<","<<hit0.locY<<","<<hit0.locZ<<","<<endl;
//	// cout<<"hit0: "<<hit0.X<<","<<hit0.Y<<","<<hit0.Z<<","<<endl;
//	
//	// X = locX*RM_11+locY*RM_12+locZ*RM_13 + hit0.X;
//	// Y = locX*RM_21+locY*RM_22+locZ*RM_23 + hit0.Y;
//	// Z = locX*RM_31+locY*RM_32+locZ*RM_33 + hit0.Z;
//
//	cout << "HitClass::RotateAndTranslate WARNING!!! Rotating 'local' positions (= real) instead of 'touchable' (pixel-centre)" 
//		 << endl;
//	
//	// cout<<"trans: "<<X<<","<<Y<<","<<Z<<","<<endl;
//	
//	return 0;
//};
*/

// =============================================================0
/*
int HitClass::GetLocalPosition(LocalPixelPositionClass* PixPos)
{
	list<string> DetUnitList = split( UnitID, ptr_fun <int, int> ( ispunct ) );
	list<string>::iterator it = DetUnitList.begin();
	int col = atoi((*it).c_str());
	it++;
	int row = atoi((*it).c_str());
	it++;
	int block = atoi((*it).c_str());
	if (PixPos) 
	{
		this->locX = PixPos->GetX(row,col,block);
		this->locY = PixPos->GetY(row,col,block);
		this->locZ = PixPos->GetZ(row,col,block);
	}
	return 0;
};
*/

