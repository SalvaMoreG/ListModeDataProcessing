
int Main()
{
	string fname;
	cout << "give filename" << endl;
	cin >> fname;
	ifstream ffile( fname.c_str() );
	if ( !ffile.is_open() )
	{
		cout << "file does not exist: " << fname << endl;
		return 1;
	}
	
	double dummyd;
	int	dummyi, classif;
	int numtrue(0), numscat(0), numrandom(0);
	while ( !ffile.eof() )
	{
		for (int i = 0; i < 8; i++)
			ffile >> dummyd;
		if ( !ffile.eof() )
		{
			ffile >> classif;
			ffile.ignore(1024, '\n');
			if (classif == 0) numtrue++;
			else if (classif == 1) numrandom++;
			else if (classif == 2) numscat++;
		}
	}

	cout << "#ALL: " << numtrue + numrandom + numscat << endl;
	cout << "#TRUE: " << numtrue << endl;
	cout << "#SCATTER: " << numscat << endl;
	cout << "#RANDOM: " << numrandom << endl;

	return 0;
}




