#ifndef _PARSER_H_
#define _PARSER_H_
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
using std::string;
using std::vector;
using std::fstream;
using namespace std;

class Gate_C
{
	public:
	Gate_C()
	{
		gateName = "NULL";
		gateType = "NULL";
		level = -1;
	};
	~Gate_C(){;}
	string gateName;
	string gateType;
	vector<string> FaninName;
	vector<Gate_C*> Fanin;
	vector<Gate_C*> Fanout;
	Gate_C* maxfanin;
	double arrivalT;
	int level;
};

class Lib_C
{
	public:
	Lib_C(){;}
	~Lib_C(){;}
	string cellType;
	double cellDelay;
	vector<Gate_C*> cell;
};

class Parser_C
{
	public:
	//constructor
	Parser_C(){gateNum = 0;
	           InputNum = 0;}
	~Parser_C(){;}
	//function
	void readbench(char*);
	void netlisting();
	void readlib(char*);
	void netlistingLib();
	void levelization();
	Gate_C* compare(Gate_C*,Gate_C*,double);
	void run();
	void dump(char*);

	vector<Gate_C*> vGate;
	vector<Gate_C*> vInput;
	vector<Lib_C*> vLib;
	queue<Gate_C*> Q; 
	vector<Gate_C*> level;
    vector<vector<Gate_C*> > rowlevel;
    vector<Gate_C*> vPath;//vector<Gate_C*> vvPath;
    vector<Gate_C*> vCell;

	int gateNum;
	int InputNum;
	double crioutDelay;
	Gate_C *critiOutput;
	vector<string> output;
	vector<string> mapcell;  
};

#endif
