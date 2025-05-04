
// /home/mkolstein/VIP/trunk/ListModeDataProcessing/src/PET_CoincidenceClassification.cc:			
// randomflag RANDOM FLAG CLASSIFICATION 1 = RANDOM 
// randomflag RANDOM FLAG CLASSIFICATION 2 = SCATTERED
// randomflag RANDOM FLAG CLASSIFICATION 0 = TRUE

int Main()
{
	std::string fname;
	cout << "filename?" << endl;
	cin >> fname;

	std::ifstream ffile( fname.c_str() );

	std::ofstream truefile( "PROMPTS_true.dat_NEW" );
	std::ofstream scatfile( "PROMPTS_scattered.dat_NEW" );
	std::ofstream randomfile( "PROMPTS_random.dat_NEW" );

	double z1, y1, x1, e1, z2, y2, x2, e2;
	int randomflag;
	// 0 = true, 1 = random, 2 = scat
	while ( !ffile.eof() )
	{
		ffile >> z1 >> y1 >> x1 >> e1 >> z2 >> y2 >> x2 >> e2;
		ffile >> randomflag;
		ffile.ignore(1024, '\n');

		if ( !ffile.eof() )
		{
			if (randomflag == 0)
				truefile << z1 << " " << y1 << " " << x1 << " " << e1 << "  " 
                         << z2 << " " << y2 << " " << x2 << " " << e2 << "  "
						 << randomflag << endl;
			else if (randomflag == 1)
				randomfile << z1 << " " << y1 << " " << x1 << " " << e1 << "  " 
                         << z2 << " " << y2 << " " << x2 << " " << e2 << "  "
						 << randomflag << endl;
			else if (randomflag == 2)
				scatfile << z1 << " " << y1 << " " << x1 << " " << e1 << "  " 
                         << z2 << " " << y2 << " " << x2 << " " << e2 << "  "
						 << randomflag << endl;
		}
	}

	return 0;
}

