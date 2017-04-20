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
		ready = 0;
	};
	~Gate_C(){;}
	string gateName;
	string gate_Name;
	string gateType;
	vector<string> FaninName;
	vector<Gate_C*> Fanin;
	vector<Gate_C*> Fanout;
	Gate_C* maxfanin;
	double arrivalT;
	int level;
	bool logic;
	int ready;
	vector<Gate_C*> true_path;
	int fm_delay;   //floating mode delay
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
	void parse_ckt(char*);
	void netlisting();
	void readlib(char*);
	void netlistingLib();
	void levelization();
	Gate_C* compare(Gate_C*,Gate_C*,double);
	void run();
	void dump(char*);
	void true_path(bool);
	void find_true_path();
	void child_path(vector<Gate_C*> &,int);
	void other_path(vector<int> &/*bool,int*/);
	void find_other_path();
	int take_inputnum();


	vector<Gate_C*> vGate;
	vector<Gate_C*> vInput;
	vector<Gate_C*> vOutput;
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
	//vector<string> output;
	vector<Gate_C*> outputcollect;
	vector<string> mapcell;
	vector<vector<Gate_C*> > outlist;
	vector<Gate_C*> out;
	vector<bool> logic_list;  //用來刪重複
	vector<vector<bool> > inputlist;  //記input logic  用來輸出
	//vector<vector<string> > input_namelist;
	vector<bool> input;
	vector<string> input_name;  //每條path的input
	int hard_slack_constraint;
	int timing_constraint;
	char* benchmark;
};

#endif
