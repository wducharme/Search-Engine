#include <set>
#include <fstream>
#include <iostream>
#include <sstream>


using namespace std;

//Parses the files and adds the correct words to the word set and
//and print data member
void findOutgoing(ifstream& nextfile, set<string>& explored, 
	const string& filename, ofstream& crawloutput)
{

	//Since it opened, add the file to the successfull links page
	explored.insert(filename);
	crawloutput << filename << endl;



	string temp, paren;
	while(getline(nextfile, temp))
	{
		string::iterator iter = temp.begin();
		paren = temp;


		while(iter != temp.end())
		{
			char c = (char)*iter;

			//Finds an outgoing link
			if(c=='(')
			{

				//To remove from paren guide
				size_t pafirstp = paren.find("(");
				size_t pasecondp = paren.find(")");

				string outgoing = paren.substr(pafirstp+1, (pasecondp-pafirstp)-1);

				//Erase (linktofile) from paren, allows us to find next ()
				paren.erase(pafirstp, (pasecondp-pafirstp)+1);

				//Catch up iterator
				while(*iter != ')')
				{
					iter++;
				}

				//Makes sure haven't already explored the file
				if(explored.find(outgoing) == explored.end())
				{
					ifstream outgoingfile(outgoing.c_str());

					//Makes sure the file opens properly
					if(outgoingfile)
					{
						findOutgoing(outgoingfile, explored, outgoing, crawloutput);
					}
				}
			}
			else
			{
				iter++;
			}

		}

		
	}

}





int main(int argc, char* argv[]){

//Correctly opening files
	const char* crawlconfigfilename;

	string defaultcrawlconfig = "crawlconfig.txt";

	if(argc < 2){
		crawlconfigfilename = defaultcrawlconfig.c_str();
	}
	else
	{
		crawlconfigfilename = argv[1];
	}

	ifstream crawlconfigfile(crawlconfigfilename);

	if(!crawlconfigfile)
	{
		crawlconfigfile.open("crawlconfig.txt");
	}


	string seedfilename = "";
	string crawloutputfilename = "";
	string crawlconfigline = "";


	//Reads in the config file - gets seed file and output file
	while(getline(crawlconfigfile, crawlconfigline))
	{
		string::iterator cit = crawlconfigline.begin();
		string tempword = "";
		string tempfile = "";
		bool error = false;
		bool done = false;

		while(cit != crawlconfigline.end())
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
				while(*cit != ' ' && *cit != ':' && cit != crawlconfigline.end())
				{
					tempword += *cit;
					cit++;
				}
				if(tempword == "INDEX_FILE")
				{
					while(cit != crawlconfigline.end() && !done)
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
							while(*cit != ' ' && *cit != '#' && cit != crawlconfigline.end())
							{
								tempfile += *cit;
								cit++;
							}
							seedfilename = tempfile;
							done = true;
						}
					}
				}
				else if(tempword == "OUTPUT_FILE")
				{
					while(cit != crawlconfigline.end() && !done)
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
							while(*cit != ' ' && *cit != '#' && cit != crawlconfigline.end())
							{
								tempfile += *cit;
								cit++;
							}
							crawloutputfilename = tempfile;
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
				cit++;
			}


			if(error || done)
			{
				break;
			}
		}
	}


	ifstream seedfile(seedfilename.c_str());
	if(!seedfile)
	{
		seedfile.open("crawlseed.txt");
		if(!seedfile)
		{
			cout << "Please enter a valid seedfile name" << endl;
			return 0;
		}
	}

	ofstream crawloutputfile(crawloutputfilename.c_str());


	string filename = "";
	set<string> explored;

	//Starts iterating through the files in the seed list
	while(getline(seedfile, filename))
	{
		if(explored.find(filename) == explored.end())
		{
			ifstream nextfile(filename.c_str());
			if(nextfile)
			{
				findOutgoing(nextfile,explored,filename, crawloutputfile);
			}
		}
	}



	//return 1;

}





