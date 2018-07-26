#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <set>
#include <vector>
#include <string>



class WebPage{
public:
	WebPage();

	WebPage(std::string n);

	~WebPage();

	std::vector<std::string> getPrint();

	std::set<std::string> getWords();

	std::vector<std::string> getAnchorWords();

	std::set<std::string> getIncoming();

	std::set<std::string> getOutgoing();

	std::string getName();

	void pushPrint(std::string s);

	std::string getPrintI(int index);

	int getPrintSize();

	void pushAnchor(std::string s);

	std::string getAnchorI(int index);

	int getAnchorSize();

	void insertIncoming(std::string s);

	void insertOutgoing(std::string s);

	std::set<std::string>::iterator outgoingBegin();

	std::set<std::string>::iterator outgoingEnd();

	std::set<std::string>::iterator incomingBegin();

	std::set<std::string>::iterator incomingEnd();

	void insertWord(std::string s);

	std::set<std::string>::iterator wordBegin();

	std::set<std::string>::iterator wordEnd();

	std::set<std::string>::iterator findWord(std::string s);

	void setCandidate(std::set<std::string> s);

	std::set<std::string> getCandidate();



private:
	std::vector<std::string> print; //Stores the phrase to be printed
	std::string name; //Stores the webpage name/filename
	std::set<std::string> words; //Stores the words in the webpage
	std::vector<std::string> anchorwords; //Stores the anchorwords in the webpage
	std::set<std::string> incoming; //Stores the incoming files
	std::set<std::string> outgoing; //Store the outgoing files
	std::set<std::string> candidate; //Store the candidate set for the webpage


};
#endif