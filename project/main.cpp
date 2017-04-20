#include <cstdlib>
#include <iostream>
#include "parser.h"
#include<ctime>

using namespace std;


int main(int argc, char * argv[])
{

string input_file;
string output_file;
Parser_C *pParser;
pParser = new Parser_C();


// argv[0] is your program name

output_file =argv[2];
pParser->parse_ckt(argv[1]);
pParser->netlisting();
pParser->levelization();
pParser->true_path(1);
pParser->find_true_path();
pParser->true_path(0);
pParser->find_true_path();
int inputnum=pParser->take_inputnum();
//for (int i=0;i<inputnum;i++)
//{
long int test_time=1;
for(int i=0;i<inputnum;i++)
{
    test_time=test_time*2;
}
vector<int> in;
for(long int i=0;i<test_time;i++)
in.push_back(0);
//int n=16;
//pParser->other_path(in);
//pParser->find_other_path();



for(long int k=1;k<test_time-1;k++)
{
long int j=k;
int i=0; //設i=0
while(j>0)
{
in[i]=j%2; //存的時候利用i先存到陣列中

j=j/2;

i++;
}

pParser->other_path(in);
pParser->find_other_path();

}
/*pParser->other_path(0,0);
pParser->find_other_path(); 
pParser->other_path(1,0);
pParser->find_other_path();

pParser->other_path(0,1);
pParser->find_other_path();
pParser->other_path(1,1);
pParser->find_other_path();

pParser->other_path(0,2);
pParser->find_other_path();
pParser->other_path(1,2);
pParser->find_other_path();

pParser->other_path(0,3);
pParser->find_other_path();
pParser->other_path(1,3);
pParser->find_other_path();*/


pParser->hard_slack_constraint=atoi(argv[4]);
pParser->timing_constraint=atoi(argv[3]);
pParser->dump(argv[2]);
return 0;
}
