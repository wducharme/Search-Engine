#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

using namespace std;



int main(int argc, char *argv[]){

	//Correctly opening files
	const char* configfilename;

	string defaultconfig = "config.txt";

	if(argc < 2){
		configfilename = defaultconfig.c_str();
	}
	else
	{
		configfilename = argv[1];
	}

	ifstream configfile(configfilename);

	if(!configfile)
	{
		configfile.open("config.txt");
	}

	string configline;

	string indexfilename;
	string queryfilename;
	string outputfilename;


	while(getline(configfile, configline))
	{
		string::iterator cit = configline.begin();
		string tempword = "";
		string tempfile = "";
		bool error = false;
		bool done = false;

		while(cit != configline.end())
		{
			if(*cit == '#')
			{
				break;
			}
			else if(*cit == ' ')
			{
				cit++;
			}
			else if(*cit >= 65 && *cit <= 122)
			{
				while(*cit != ' ' && *cit != '=' && cit != configline.end())
				{
					tempword += *cit;
					cit++;
				}
				if(tempword == "INDEX_FILE")
				{
					while(cit != configline.end() && !done)
					{
						if(*cit == '=')
						{
							cit++;
						}
						else if(*cit == ' ')
						{
							cit++;
						}
						else
						{
							while(*cit != ' ' && cit != configline.end())
							{
								tempfile += *cit;
								cit++;
							}
							indexfilename = tempfile;
							done = true;
						}
					}
				}
				else if(tempword == "QUERY_FILE")
				{
					while(cit != configline.end() && !done)
					{
						if(*cit == '=')
						{
							cit++;
						}
						else if(*cit == ' ')
						{
							cit++;
						}
						else
						{
							while(*cit != ' ' && cit != configline.end())
							{
								tempfile += *cit;
								cit++;
							}
							queryfilename = tempfile;
							done = true;
						}
					}
				}
				else if(tempword == "OUTPUT_FILE")
				{
					while(cit != configline.end() && !done)
					{
						if(*cit == '=')
						{
							cit++;
						}
						else if(*cit == ' ')
						{
							cit++;
						}
						else
						{
							while(*cit != ' ' && cit != configline.end())
							{
								tempfile += *cit;
								cit++;
							}
							outputfilename = tempfile;
							done = true;
						}
					}
				}
				else{
					error = true;
				}
			}


			if(error || done)
			{
				break;
			}
		}
	}









/*


		cout << configline << endl;

		string::iterator cit = configline.begin();
		string tempword = "";
		string tempfile = "";
		bool equals = false;
		bool error = false;
		bool done = false;

		while(cit != configline.end())
		{
			cout << *cit << endl;
			if(*cit == '#')
			{
				break;
			}
			else if(*cit == ' ')
			{
				//DO NOTHING
				cit++;
			}
			else if(*cit >= 65 && *cit <= 122)
			{
				while(*cit != ' ' && *cit != '=')
				{
					tempword += *cit;
					cit++;
				}
				cout << tempword << "X" << endl;
				if(tempword == "INDEX_FILE")
				{
					cout << "YES" << endl;
					if(*cit == '=')
					{
						equals = true;
					}
					cit++;

					while(cit != configline.end() && !done)
					{
						if(*cit == '=')
						{
							if(equals == false)
							{
								equals = true;
							}
							else{
								error = true;
							}
							cit++;
						}
						else if(*cit == ' ')
						{
							cit++;
						}
						else
						{
							while(*cit != ' ')
							{
								tempfile += *cit;
								cit++;
							}
							cout << tempfile << endl;
							indexfilename = tempfile;
							done = true;

						}
					}
					
				}
				else if(tempword == "QUERY_FILE")
				{
					if(*cit == '=')
					{
						equals = true;
					}
					cit++;

					while(cit != configline.end() && !done)
					{
						if(*cit == '=')
						{
							if(equals == false)
							{
								equals = true;
							}
							else{
								error = true;
							}
							cit++;
						}
						else if(*cit == ' ')
						{
							cit++;
						}
						else
						{
							while(*cit != ' ')
							{
								tempfile += *cit;
								cit++;
							}
							queryfilename = tempfile;
							done = true;

						}
					}

				}
				else if(tempword == "OUTPUT_FILE")
				{
					if(*cit == '=')
					{
						equals = true;
					}
					cit++;

					while(cit != configline.end() && !done)
					{
						if(*cit == '=')
						{
							if(equals == false)
							{
								equals = true;
							}
							else{
								error = true;
							}
							cit++;
						}
						else if(*cit == ' ')
						{
							cit++;
						}
						else
						{
							while(*cit != ' ')
							{
								tempfile += *cit;
								cit++;
								
							}
							outputfilename = tempfile;
							done = true;

						}
				
					}
				}
				else{
					error = true;
				}

				}

			
			else
			{
				error = true;
			}

			if(error || done)
			{
				break;
			}
		
		}


	}*/

	cout << "INDEX:" << indexfilename << endl;
	cout << "QUERY:" << queryfilename << endl;
	cout << "OUTPUT:" << outputfilename << endl;

	return 0;
}