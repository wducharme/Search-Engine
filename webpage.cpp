#include "webpage.h"


	WebPage::WebPage(){

	}

	WebPage::WebPage(std::string n){
		name = n;
	}
	

	WebPage::~WebPage(){

	}


	std::vector<std::string> WebPage::getPrint(){
		return print;
	}

	std::set<std::string> WebPage::getWords(){
		return words;
	}

	std::vector<std::string> WebPage::getAnchorWords(){
		return anchorwords;
	}

	std::set<std::string> WebPage::getIncoming(){
		return incoming;
	}

	std::set<std::string> WebPage::getOutgoing(){
		return outgoing;
	}

	std::string WebPage::getName()
	{
		return name;
	}

	void WebPage::pushPrint(std::string s){
		print.push_back(s);
	}

	std::string WebPage::getPrintI(int index){
		return print[index];
	}

	int WebPage::getPrintSize(){
		return print.size();
	}

	void WebPage::pushAnchor(std::string s){
		anchorwords.push_back(s);
	}

	std::string WebPage::getAnchorI(int index){
		return anchorwords[index];
	}

	int WebPage::getAnchorSize(){
		return anchorwords.size();
	}

	void WebPage::insertIncoming(std::string s){
		incoming.insert(s);
	}

	void WebPage::insertOutgoing(std::string s){
		outgoing.insert(s);
	}

	std::set<std::string>::iterator WebPage::outgoingBegin(){
		return outgoing.begin();
	}

	std::set<std::string>::iterator WebPage::outgoingEnd(){
		return outgoing.end();
	}

	std::set<std::string>::iterator WebPage::incomingBegin(){
		return incoming.begin();
	}

	std::set<std::string>::iterator WebPage::incomingEnd(){
		return incoming.end();
	}

	void WebPage::insertWord(std::string s){
		words.insert(s);
	}

	std::set<std::string>::iterator WebPage::wordBegin(){
		return words.begin();
	}

	std::set<std::string>::iterator WebPage::wordEnd(){
		return words.end();
	}

	std::set<std::string>::iterator WebPage::findWord(std::string s){
		return words.find(s);
	}

	void WebPage::setCandidate(std::set<std::string> s){
		candidate = s;
	}

	std::set<std::string> WebPage::getCandidate(){
		return candidate;
	}


