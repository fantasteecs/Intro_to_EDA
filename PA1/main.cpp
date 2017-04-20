#include <cstdlib>
#include <iostream>
#include "parser.h"
#include<ctime>

using namespace std;

int main(int argc, char *argv[])
{
	clock_t start,end;
    start = clock();
    Parser_C *pParser;
	pParser = new Parser_C();
	pParser->readbench(argv[1]);
	pParser->netlisting();
	pParser->readlib(argv[2]);
	pParser->netlistingLib();
	pParser->levelization();
	pParser->run();
	pParser->dump(argv[3]);
    end = clock();
    cout<<"[Total CPU time] "<<double(end-start)<<endl;



    return 0;
}
