#include "webpage.h"
#include "setutility.h"
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>



using namespace std;




vector<string> pageRank(set<string>& cset, int steps, double restartprob, map<string, 
	set<string> >& outgoingmap, map<string, set<string> >& incomingmap)
{

	//Refine incoming/outgoing nodes of webpages to just those in candidate set
	map<string, set<string> > refinedoutgoing;
	map<string, set<string> > refinedincoming;

	//Fill the refined outgoing map
	for(set<string>::iterator candit = cset.begin(); candit != cset.end(); 
		candit++)
	{
		//Finds the correct file in outgoing map
		map<string, set<string> >::iterator oit = outgoingmap.find(*candit);

		//Creates a pairing in the refined outgoing map
		set<string> tempset;
		tempset.insert(*candit); //Inserts the file for self loop
		refinedoutgoing.insert(make_pair(*candit, tempset));

		//Makes sure file was found in outgoing
		if(oit != outgoingmap.end())
		{
			//Iterates through and checks all outgoing elements to see if 
			//In candidate set
			for(set<string>::iterator checkit = oit->second.begin();
			checkit != oit->second.end(); checkit++)
			{
				//Checks if current outgoing file is in cset
				if(cset.find(*checkit) != cset.end())
				{
					//Find correct place of current file in map 
					//Add outgoing file to the refined list
					set<string>& set_ref = refinedoutgoing[*candit];
					set_ref.insert(*checkit);
				}
			}
		}
	}

	//Fill the refined incoming map
	for(set<string>::iterator candit = cset.begin(); candit != cset.end(); 
		candit++)
	{
		//Finds the correct file in outgoing map
		map<string, set<string> >::iterator oit = incomingmap.find(*candit);

		//Creates a pairing in the refined outgoing map
		set<string> tempset;
		tempset.insert(*candit); //Inserts the file for self loop
		refinedincoming.insert(make_pair(*candit, tempset));

		//Makes sure file was found in outgoing
		if(oit != incomingmap.end())
		{
			//Iterates through and checks all outgoing elements to see if 
			//In candidate set
			for(set<string>::iterator checkit = oit->second.begin();
			checkit != oit->second.end(); checkit++)
			{
				//Checks if current outgoing file is in cset
				if(cset.find(*checkit) != cset.end())
				{
					//Find correct place of current file in map 
					//Add outgoing file to the refined list
					set<string>& set_ref = refinedincoming[*candit];
					set_ref.insert(*checkit);
				}
			}
		}
	}


	//Always add 1 to refined outgoing set size to account for self loop

	map<string, vector<double> > probability;
	for(set<string>::iterator mainit = cset.begin(); 
			mainit != cset.end(); mainit++)
	{
		//Initializes the vector in the map w/ 1/n as probability for
		//both old and new probability
		vector<double> tempvec;
		tempvec.push_back((double)(1.0/cset.size()));
		tempvec.push_back((double)(1.0/cset.size()));
		//Pairs each webpage with a initialized vector
		probability.insert(make_pair(*mainit, tempvec));

	}



	//Probability for-loop
	//Starts at 1 because the first step was done above (t-1 iterations)
	for(int i = 0; i < steps; i++)
	{

		//Iterate through and make all newprobabilities old
		for(map<string, vector<double> >::iterator probiter = 
			probability.begin(); probiter != probability.end();
			probiter++)
		{
			probiter->second.at(0) = probiter->second.at(1);
		}

		//For each step, needs to iterate through each page
		for(set<string>::iterator mainit = cset.begin(); 
			mainit != cset.end(); mainit++)
		{
			//Finds the current webpage in probability map
			map<string, vector<double> >::iterator currprobmap = 
			probability.find(*mainit);

			//Finds the current webpage's incoming set
			map<string, set<string> >::iterator currimap = 
			refinedincoming.find(*mainit);

			double summation = 0;


			//Gets the incoming webpage summation
			//Iterates through current webpages incoming set
			for(set<string>::iterator tempwp = currimap->second.begin();
				tempwp != currimap->second.end(); tempwp++)
			{
				//Finds current incoming webpage probability 
				map<string, vector<double> >::iterator tempprobmap = 
				probability.find(*tempwp);

				//Finds current incoming webpage outgoing set (for size)
				map<string, set<string> >::iterator tempomap = 
				refinedoutgoing.find(*tempwp);

				//Current webpage candidate output size (+ 1 to account for 
				//self loop)
				double temposize = (tempomap->second.size()); 


				//Double check if its 1(new) or 0(old)- think it's 0 
				//because we want it from old iteration
				double tempprobability = tempprobmap->second.at(0);

				//Adds each webpages old probability * webpage outputsize
				summation += (double)((tempprobability)*(1.0/temposize)); 
			}

			double newprobability = (double)(((1.0-restartprob)*summation) + 
				(restartprob*(1.0/cset.size())));

			double oldprobability = currprobmap->second.at(1);

			//Updates the new and old probability of the current webpage
			currprobmap->second.at(1) = newprobability;
			currprobmap->second.at(0) = oldprobability;

		}

	}

	set<string> csetcopy = cset;


	vector<string> pagerankordering;

	//Goes until no elements remain in the candidate set
	while(csetcopy.size() > 0)
	{
		double maxprobability = 0;
		string maxprobabilityfilename = "";
		//Sorts by probability
		for(set<string>::iterator orderit = csetcopy.begin();
			orderit != csetcopy.end(); orderit++)
		{
			map<string, vector<double> >::iterator currprob = 
			probability.find(*orderit);
			if(maxprobability < currprob->second.at(1))
			{
				maxprobability = currprob->second.at(1);
				maxprobabilityfilename = *orderit;
			}
		}

		//Adds the largest probability vector to the vector
		pagerankordering.push_back(maxprobabilityfilename);
		csetcopy.erase(maxprobabilityfilename);


	}



	return pagerankordering;



}


//Adds the word to the wordmap
void addWord(map<string,WebPage*>& wpmap,
		map<string,set<string> >& wordmap, const string& filename, string& word)
{

	for(int i = 0;i< (int)word.length();i++) //Make the word lowercase
	{
		word[i] = tolower(word[i]);
	}


	//Insert word to map with file
	if(wordmap.find(word) != wordmap.end())
	{
		//Possible issue - 
		//filename can be in there twice (need to double check when searching for words)
		set<string>& set_ref = wordmap[word];
		//Makes sure no duplicate files
		if(set_ref.find(filename) == set_ref.end()) {
			set_ref.insert(filename);
		}
	}
	else
	{
		set<string> tempset;
		tempset.insert(filename);
		wordmap.insert(make_pair(word,tempset));
	}

	//Insert word into webpage word set
	map<string,WebPage*>::iterator it = wpmap.find(filename);
	//Check to make sure correct filename
	if(it != wpmap.end()){
		it->second->insertWord(word);
	}




	word = "";
}

//Add outgoing links to specified filename
void addOutgoing(map<string,WebPage*>& wpmap,
		map<string,set<string> >& outgoingmap, const string& filename, 
		string& outgoing)
	{
		//Ensure no nonexistent links are added
		ifstream testfile(outgoing.c_str());
		if(testfile) 
		{
			testfile.close();
		//Insert outgoing to map with file
		if(outgoingmap.find(filename) != outgoingmap.end())
		{
			set<string>& set_ref = outgoingmap[filename];
			set_ref.insert(outgoing); //Not sure if I need to check for duplicates
		}
		else
		{
			set<string> tempset;
			tempset.insert(outgoing);
			outgoingmap.insert(make_pair(filename,tempset));
		}

		map<string,WebPage*>::iterator it = wpmap.find(filename);
		//Check to make sure correct filename
		if(it != wpmap.end()){
			it->second->insertOutgoing(outgoing);
		}
		}

		outgoing = "";
	}

void addIncoming(map<string,WebPage*>& wpmap,
		map<string,set<string> >& incomingmap, const string& filename, 
		string& incoming)
	{

		//Makes sure the file actually exists
		ifstream testfile(incoming.c_str());
		if(testfile)
		{
			testfile.close();

		if(incomingmap.find(incoming) != incomingmap.end())
		{
			set<string>& set_ref = incomingmap[incoming];
			set_ref.insert(filename);
		}
		else
		{
			set<string> tempset;
			tempset.insert(filename);
			incomingmap.insert(make_pair(incoming,tempset));
		}

		map<string,WebPage*>::iterator it = wpmap.find(incoming);

		
		//Check to make sure correct filename
		if(it != wpmap.end()){
			it->second->insertIncoming(filename);
		}
		}

		incoming = "";


	}

void addAnchorWord(map<string,WebPage*>& wpmap,
		map<string,set<string> >& wordmap, const string& filename, string& aword)
{

	for(int i = 0;i< (int) aword.length();i++) //Make the word lowercase
	{
		aword[i] = tolower(aword[i]);
	}


	//Insert word to map with file
	if(wordmap.find(aword) != wordmap.end())
	{
		//Possible issue - 
		//filename can be in there twice (need to double check when searching for words)
		set<string>& set_ref = wordmap[aword];
		//Makes sure no duplicate files
		if(set_ref.find(filename) == set_ref.end()) {
			set_ref.insert(filename);
		}
	}
	else
	{
		set<string> tempset;
		tempset.insert(filename);
		wordmap.insert(make_pair(aword,tempset));
	}

	//Add aword to webpage anchorwords list
	map<string,WebPage*>::iterator it = wpmap.find(filename);
	it->second->pushAnchor(aword);

	//Insert word into webpage word set
	map<string,WebPage*>::iterator ite = wpmap.find(filename);
	//Check to make sure correct filename
	if(ite != wpmap.end()){
		ite->second->insertWord(aword);
	}

	aword = "";
}


//Parses the files and adds the correct words to the word set and
//and print data member
void parseFile(ifstream& nextfile, map<string,WebPage*>& wpmap,
		map<string,set<string> >& wordmap, 
		map<string,set<string> >& outgoingmap,
		map<string,set<string> >& incomingmap, const string& filename)
{
	string temp, word, print, paren;
	//size_t firstp, secondp, firstb, secondb;
	while(getline(nextfile, temp))
	{
		string::iterator iter = temp.begin();
		print = temp;
		paren = temp;


		while(iter != temp.end())
		{
			char c = (char)*iter;

			if((c>=48 && c<=57) || (c>=65 && c<=90) || (c>=97 && c<=122))
			{
				word+=c; //If letter or number, add to word
			}
			else if(c=='(')
			{

				//Add word
				if(word.size() > 0){
					addWord(wpmap, wordmap, filename, word);
				}

				//To remove from paren guide
				size_t pafirstp = paren.find("(");
				size_t pasecondp = paren.find(")");

				//To remove from print guide
				size_t prfirstp = print.find("(");
				size_t prsecondp = print.find(")");



				string outgoings = paren.substr(pafirstp+1, (pasecondp-pafirstp)-1);
				string incomings = outgoings;


				//Erases (linktofile) contents from print
				print.erase(prfirstp, (prsecondp-prfirstp)+1);

				//Erase (linktofile) from paren, allows us to find next ()
				paren.erase(pafirstp, (pasecondp-pafirstp)+1);



				//Add to outgoing
				if(outgoings.size() > 0){
					addOutgoing(wpmap, outgoingmap, filename, outgoings);
				}

				//Add to incoming
				if(incomings.size() > 0){
					addIncoming(wpmap, incomingmap, filename, incomings);
				}


				//Catch up iterator
				while(*iter != ')')
				{
					iter++;
				}




			}
			else if(c=='[')
			{

				//Add word
				if(word.size() > 0){
					addWord(wpmap, wordmap, filename, word);
				}

				//
				size_t firstb = paren.find("[");
				size_t secondb = paren.find("]");

				//Need to add []'s when adding to print '
				string anchor = paren.substr(firstb, (secondb-firstb)+1);
				//string anchorprint = "["; //Printing format
				string aword = "";


				string::iterator ise = anchor.begin();
				while(ise != anchor.end())
				{
					char g = (char)*ise;
					if((g>=48 && g<=57) || (g>=65 && g<=90) ||
						(g>=97 && g<=122))
					{
						aword+=g;
					}
					else
					{
						if(aword.size() > 0){
							//anchorprint += aword;
							//anchorprint += " "; //Correct printing format
							addAnchorWord(wpmap, wordmap, filename, aword);
						}

					}



					ise++;
				}


				//Erases [] from paren so we can find next one next iter.
				paren.erase(firstb, (secondb-firstb)+1);



				//Catch up iterator
				while(*iter != ']')
				{
					iter++;
				}




			}
			else
			{
				//Insert word to map with file
				if(word.size() > 0){
					addWord(wpmap, wordmap, filename, word);
				}
			}

			iter++;
		}

		if(word.size() > 0){
			//Insert word to map with file
			addWord(wpmap, wordmap, filename, word);
		}


		//Insert print into webpage print - Make sure this works
		map<string,WebPage*>::iterator it = wpmap.find(filename);
		it->second->pushPrint(print);





	}
}


void printCommand(stringstream& commandline, ofstream& outputfile,
	map<string, WebPage*>& wpmap)
{

	string filename = "";
	commandline >> filename;

	string testword;
	//If more than one word, Invalid query		
	if(commandline >> testword)
	{
		outputfile << "Invalid query" << endl;
		return;
	}

	//Check if file exists
	map<string, WebPage*>::iterator it = wpmap.find(filename);
	if(it != wpmap.end()){
		//Output filename
		outputfile << filename << endl;
		for(int i = 0; i < (int)it->second->getPrintSize(); i++){
			outputfile << it->second->getPrintI(i) << endl;
		}

	}
	else
	{
		outputfile << "Invalid query" << endl;
	}

}

void outgoingCommand(stringstream& commandline, ofstream& outputfile,
	map<string, set<string> >& outgoingmap, map<string, WebPage*>& wpmap)
{
	string filename = "";
	commandline >> filename;

	string testword;
	//If more than one word, Invalid query		
	if(commandline >> testword)
	{
		outputfile << "Invalid query" << endl;
		return;
	}

	//Check if file exists
	map<string, WebPage*>::iterator check = wpmap.find(filename);
	if(check != wpmap.end())
	{

		//If file doesn't have any outgoing links, doesn't print anything else
		map<string, set<string> >::iterator it = outgoingmap.find(filename);
		if(it != outgoingmap.end()){

			outputfile << it->second.size() << endl;


			for(set<string>::iterator iter = it->second.begin();
			iter != it->second.end(); iter++){
				outputfile << *iter << endl; ////HERE
			
			}
			outputfile << endl;

		}
		else
		{
			outputfile << "0" << endl;
		}
	}
	else//File doesn't exist
	{
		outputfile << "Invalid query" << endl;
	}


}

void incomingCommand(stringstream& commandline, ofstream& outputfile,
	map<string, set<string> >& incomingmap, map<string, WebPage*>& wpmap)
{
	string filename = "";
	commandline >> filename;

	string testword;
	//If more than one file, Invalid query		
	if(commandline >> testword)
	{
		outputfile << "Invalid query" << endl;
		return;
	}

	//Check if file exists
	map<string, WebPage*>::iterator check = wpmap.find(filename);
	if(check != wpmap.end())
	{

		//If file doesn't have any incoming links, doesn't print anything else
		map<string, set<string> >::iterator it = incomingmap.find(filename);
		if(it != incomingmap.end()){


			outputfile << it->second.size() << endl;

			for(set<string>::iterator iter = it->second.begin();
			iter != it->second.end(); iter++){
				outputfile << *iter << endl; //HERE
			
			}
			outputfile << endl;

		}
		else
		{
			outputfile << "0" << endl;
		}
	}
	else
	{
		outputfile << "Invalid query: File doesn't exist" << endl;
	}


}

void wordSearch(ofstream& outputfile,
	map<string, set<string> >& wordmap, string& tword, 
	map<string, set<string> >& candidatemap, int steps, double restartprob,
	map<string, set<string> >& outgoingmap, 
	map<string, set<string> >& incomingmap)
{

	map<string, set<string> >::iterator it = wordmap.find(tword);
	if(it != wordmap.end())
	{

		//Get the number of files with the word in it
		set<string> tempset = it->second;

		//Output number of files
		set<string> wordcandidates;

		//insert the original files
		wordcandidates.insert(tempset.begin(), tempset.end());

		//Adds candidate set for each successful webpage
		for(set<string>::iterator iter = tempset.begin();
			iter != tempset.end(); iter++)
		{
			map<string, set<string> >::iterator itera = candidatemap.find(*iter);
			if(itera != candidatemap.end())
			{
				wordcandidates.insert(itera->second.begin(),
					itera->second.end());
			}
		}


		//PAGERANK
		vector<string> ranking = pageRank(wordcandidates, steps, 
			restartprob,outgoingmap, incomingmap);


		//Output the files in ranked order
		outputfile << ranking.size() << endl;
		for(int i = 0; i < (int)ranking.size(); i++)
		{
			outputfile << ranking[i] << endl;
		}


	}
	else
	{
		outputfile << "0" << endl;
	}



}


void andCommand(stringstream& commandline, ofstream& outputfile,
	map<string, set<string> >& wordmap, map<string, WebPage*>& wpmap,
	map<string, set<string> >& candidatemap, int steps, double restartprob,
	map<string, set<string> >& outgoingmap, 
	map<string, set<string> >& incomingmap)
	{

		//Look up words in a file

		string temp = "";
		vector<string> wordlist;
		while(commandline >> temp){
			//Make the word lowercase
			for(int i = 0;i< (int)temp.length();i++) 
			{
				temp[i] = tolower(temp[i]);
			}
			wordlist.push_back(temp);
		}

		vector<string> successwp;


		for(map<string, WebPage*>::iterator it = wpmap.begin();
			it != wpmap.end(); it++)
		{
			bool notfound = false;
			for(int c = 0; c < (int)wordlist.size();c++)
			{
			set<string>::iterator check = it->second->findWord(wordlist[c]);
				if(check == it->second->wordEnd())
				{
					notfound = true;
				}
			}

			if(!notfound)
			{
				successwp.push_back(it->second->getName());
			}
		}


		set<string> andcandidates;

		//Adds candidate set for each successful webpage
		for(int i = 0; i < (int)successwp.size();i++)
		{
			map<string, set<string> >::iterator iter = candidatemap.find(successwp[i]);
			if(iter != candidatemap.end())
			{
				andcandidates.insert(successwp[i]); //Add original file
				andcandidates.insert(iter->second.begin(), 
					iter->second.end()); //Add candidate set of file
			}
		}


		//PAGERANK
		vector<string> ranking = pageRank(andcandidates, steps, 
			restartprob,outgoingmap, incomingmap);

		//Output the files in ranked order
		outputfile << ranking.size() << endl;
		for(int i = 0; i < (int)ranking.size(); i++)
		{
			outputfile << ranking[i] << endl;
		}




	}


	void orCommand(stringstream& commandline, ofstream& outputfile,
	map<string, set<string> >& wordmap, map<string, WebPage*>& wpmap,
	 map<string, set<string> >& candidatemap, int steps, double restartprob,
	map<string, set<string> >& outgoingmap, 
	map<string, set<string> >& incomingmap)
	{

		//Look up words in a file

		string temp = "";
		vector<string> wordlist;
		while(commandline >> temp){
			//Make the word lowercase
			for(int i = 0;i< (int)temp.length();i++) 
			{
				temp[i] = tolower(temp[i]);
			}
			wordlist.push_back(temp);
		}

		vector<string> successwp;

		//Same as AND except if it finds any of the words in a file, it
		// instantly adds it to the list
		for(map<string, WebPage*>::iterator it = wpmap.begin();
			it != wpmap.end(); it++)
		{
			bool found = false;
			for(int c = 0; c < (int)wordlist.size();c++)
			{
			set<string>::iterator check = it->second->findWord(wordlist[c]);
				if(check != it->second->wordEnd())
				{
					found = true;
				}
			}

			if(found)
			{
				successwp.push_back(it->second->getName());
			}
		}


		set<string> orcandidates;

		//Adds candidate set for each successful webpage
		for(int i = 0; i < (int)successwp.size();i++)
		{
			map<string, set<string> >::iterator iter = candidatemap.find(successwp[i]);
			if(iter != candidatemap.end())
			{
				orcandidates.insert(successwp[i]); //Insert original file
				orcandidates.insert(iter->second.begin(), 
					iter->second.end()); //Add candidate set of file
			}
		}

		//PAGERANK
		vector<string> ranking = pageRank(orcandidates, steps, 
			restartprob,outgoingmap, incomingmap);


		//Output the files in ranked order
		outputfile << ranking.size() << endl;
		for(int i = 0; i < (int)ranking.size(); i++)
		{
			outputfile << ranking[i] << endl;
		}



	}


void createCandidateSets(map<string,WebPage*>& wpmap, 
	map<string,set<string> >& candidatemap)
{
	for(map<string,WebPage*>::iterator it = wpmap.begin(); 
		it != wpmap.end(); it++)
	{
		string tempfilename = it->first;

		//Combine the incoming and outgoing sets of each webpage --NEED TO CHECK THIS DOESN"T SEGFAULT
		//WITH A FILE THAT DOESNT HAVE INCOMING OR OUTGOING
		set<string> tempset = unionSet<string>(it->second->getOutgoing(), 
			it->second->getIncoming());

		//Set the candidate set for the webpage in candidatemap
		candidatemap.insert(make_pair(tempfilename, tempset));

		//Set the candidate set in the actual webpage
		it->second->setCandidate(tempset);


	}
}



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

	double restartprob;
	int steps;


	//Reads in the config file
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
							while(*cit != ' ' && *cit != '#' && cit != configline.end())
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
							while(*cit != ' ' && *cit != '#' &&  cit != configline.end())
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
							while(*cit != ' ' && *cit != '#' && cit != configline.end())
							{
								tempfile += *cit;
								cit++;
							}
							outputfilename = tempfile;
							done = true;
						}
					}
				}
				else if(tempword == "RESTART_PROBABILITY")
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
							while(*cit != ' ' && *cit != '#' && cit != configline.end())
							{
								tempfile += *cit;
								cit++;
							}
							restartprob = stof(tempfile);
							done = true;
						}
					}
				}
				else if(tempword == "STEP_NUMBER")
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
							while(*cit != ' ' && *cit != '#' && cit != configline.end())
							{
								tempfile += *cit;
								cit++;
							}
							steps = stoi(tempfile);
							done = true;
						}
					}
				}
				else{
					error = true;
				}
			}
			else{
				cit++;
			}


			if(error || done)
			{
				break;

			}
		}

	}


	ifstream indexfile(indexfilename.c_str());
	ifstream queryfile(queryfilename.c_str());
	ofstream outputfile(outputfilename.c_str());



	if(!indexfile || !queryfile || !outputfile)
	{
		cout << "Please enter valid filenames" << endl;
		return 0;
	}


	//set<WebPages> wplist;
	map<string, WebPage*> wpmap;
	map<string, set<string> > wordmap;
	map<string, set<string> > outgoingmap;
	map<string, set<string> > incomingmap;
	map<string, set<string> > candidatemap;

	string filename;


	//Initializes the set of webPages
	while(getline(indexfile,filename))
	{
		//Skips empty lines and duplicate files
		if((filename.size() != 0) && (wpmap.find(filename) == wpmap.end())){ 
			wpmap.insert( make_pair(filename, new WebPage(filename)) );
		}
	}


	//Reset indexfile to the top
	indexfile.clear();
	indexfile.seekg(0, ios::beg);

	//Parse the file - add all elements to correct lists
	while(getline(indexfile, filename))
	{
		ifstream nextfile(filename.c_str());

		parseFile(nextfile,wpmap,wordmap,outgoingmap,incomingmap,filename);

	}

	//Create candidate sets for each filename/webpage
	createCandidateSets(wpmap, candidatemap);


	
	//QUERY
	string command;
	while(getline(queryfile, command))
	{
		stringstream commandline(command);
		//cout << command << endl;

		string firstword = "";

		//Need to do a first word check for command
		commandline >> firstword;

		//Check if first word is "and" and the
		//Need to check if 
		if(firstword == "AND" && commandline.str().length() > 4)
		{
			andCommand(commandline, outputfile, wordmap, wpmap, candidatemap,
				steps, restartprob, outgoingmap, incomingmap);

		}
		else if(firstword == "OR" && commandline.str().length() > 3)
		{
			orCommand(commandline, outputfile, wordmap, wpmap, candidatemap,
				steps, restartprob, outgoingmap, incomingmap);
		}
		else if(firstword == "PRINT" && commandline.str().length() > 6)
		{
			printCommand(commandline, outputfile, wpmap);

		}
		else if(firstword == "OUTGOING" && commandline.str().length() > 9)
		{
			outgoingCommand(commandline, outputfile, outgoingmap, wpmap);

		}
		else if(firstword == "INCOMING" && commandline.str().length() > 9)
		{
			incomingCommand(commandline, outputfile, incomingmap, wpmap);
		}
		else if(firstword == "") //Catch empty lines
		{
			outputfile << "Invalid query" << endl;
		}
		else 
		{
			string testword;
			//If more than one word, Invalid query
			if(commandline >> testword)
			{
				outputfile << "Invalid query" << endl;
				continue;
			}
			else
			{
				//Make the word lowercase
				for(int i = 0;i< (int)firstword.length();i++) 
				{
					firstword[i] = tolower(firstword[i]);
				}

				wordSearch(outputfile, wordmap, firstword, candidatemap,
					steps, restartprob, outgoingmap, incomingmap);
			}

		}

	}



	//Deletes all of the dynamically allocated webpages
	for(map<string, WebPage*>::iterator ite = wpmap.begin();
		ite != wpmap.end(); ite++)
	{
		delete ite->second;
	}






}