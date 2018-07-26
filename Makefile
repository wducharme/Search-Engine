CXX = g++
CPPFLAGS = -g -Wall --std=c++11

all: crawler search 

search: webpage.cpp search.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@

crawler: crawler.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@
