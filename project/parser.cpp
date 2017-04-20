#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <omp.h>


using namespace std;


void read_line_as_token(const string &line, vector<string> &tokens)
{
        tokens.clear();
        string token;
        for(int g=0;g<line.size();g++)
        {
                if(line[g]==';' || line[g]=='.' || line[g]=='(' || line[g]==')' || line[g]==',' || line[g]=='=' || line[g]==' ')
                {
                        if(token.size()!=0 &&token[0]!=' ')
                        {
                                tokens.push_back(token);
                                token.clear();
                        }
                }
                else
                {
                        token.push_back(line[g]);
                }
        }
        if(token.size()!=0 && token[0]!=' ') 
        tokens.push_back(token);
}
void Parser_C::parse_ckt(char* Argv1)
{
        benchmark=Argv1;
        ifstream in(Argv1, ios::in);
        Gate_C *pGate;
        while(!in.eof())
        {
                vector<string> tokens;
                string line;
                getline(in,line);
                read_line_as_token(line,tokens);

		if(tokens.size()>0&&tokens[0]=="NOT1"){
                    pGate = new Gate_C;
                    pGate->gate_Name = tokens[1];
		            pGate->gateType = "NOT1";             //type


		  	for(int i=0;i<tokens.size();++i)
			{
				if(tokens[i]=="A")
                			pGate->FaninName.push_back(tokens[i+1]);
				else if(tokens[i]=="Y")
              	 		  	pGate->gateName=tokens[i+1];
			}
				vGate.push_back(pGate);
        }
                if(tokens.size()>0&&tokens[0]=="NAND2"){
                  	 pGate = new Gate_C;
                    pGate->gate_Name = tokens[1];
		            pGate->gateType = "NAND2";             //type
                  	//vGate.push_back(pGate);

               	  	for(int i=0;i<tokens.size();++i)
			{
				if(tokens[i]=="A")
		  			pGate->FaninName.push_back(tokens[i+1]);
		  			//cout<<tokens[i+1]<<endl;
				if(tokens[i]=="B")
                    pGate->FaninName.push_back(tokens[i+1]);
				if(tokens[i]=="Y")
                  			pGate->gateName=tokens[i+1];
			}
			     	vGate.push_back(pGate);
        }
                if(tokens.size()>0&&tokens[0]=="NOR2"){
                  	pGate = new Gate_C;
                    pGate->gate_Name = tokens[1];
		            pGate->gateType = "NOR2";             //type
                  	//vGate.push_back(pGate);

               	  	for(int i=0;i<tokens.size();++i)
				{
				if(tokens[i]=="A")
		  			pGate->FaninName.push_back(tokens[i+1]);
				if(tokens[i]=="B")
                  			pGate->FaninName.push_back(tokens[i+1]);
				if(tokens[i]=="Y")
                  			pGate->gateName=tokens[i+1];
				}
                       	vGate.push_back(pGate);
        }
                if(tokens.size()>0&&tokens[0]=="input"){
                	for(int i=1;i<tokens.size();++i){
                      		pGate = new Gate_C;
                    pGate->gateName = tokens[i];//cout<<tokens[i]<<endl;
                    pGate->gate_Name = tokens[i];//cout<<tokens[i]<<endl;
		            pGate->gateType = "input";             //type
		            pGate->level=0;
                  	vGate.push_back(pGate);
                  	vInput.push_back(pGate);

			}
                	int s=1;
                	while(s){
                		getline(in,line);
                		//  cout<<line;
                	    	tokens.clear();
                	 	read_line_as_token(line,tokens);
                	  	if(tokens.size()>0&&tokens[0]!="output"){
               		   		for(int i=0;i<tokens.size();++i){
 		                      		pGate = new Gate_C;
                                    pGate->gateName = tokens[i];//cout<<tokens[i]<<endl;
                                    pGate->gate_Name = tokens[i];//cout<<tokens[i]<<endl;
		                            pGate->gateType = "input";             //type
		                            pGate->level=0;
                                    vGate.push_back(pGate);
                                    vInput.push_back(pGate);

					}
                	 	}
                  		else {
                  	  		s=0;
                  	  		if(tokens.size()>0&&tokens[0]=="output"){
                  	   			for(int i=1;i<tokens.size();++i){
                  	     				//cout<<tokens[i]<<endl;
                  	     				pGate = new Gate_C;
                                        pGate->gateName = tokens[i];//cout<<tokens[i]<<endl;
                                        pGate->gateType = "output";
                  	     				//output.push_back(tokens[i]);
                  	     				vOutput.push_back(pGate);
						}
                  	  			int ss=1;
                  	  			while(ss){
                  	    				getline(in,line);
               		 			//      cout<<line;
                      					tokens.clear();
                      					read_line_as_token(line,tokens);
                      					if(tokens.size()>0&&tokens[0]!="wire"){
                        					for(int i=0;i<tokens.size();++i){
                                                    pGate = new Gate_C;
                                                  pGate->gateName = tokens[i];//cout<<tokens[i]<<endl;
		                                         pGate->gateType = "output";
                          						//output.push_back(tokens[i]);//cout<<tokens[i]<<endl;
								                vOutput.push_back(pGate);
								}
              					}
                      					else ss=0;
                    				}
                   			}
                  		}
                	}
                 }
             //   cout<<endl;
        }
        in.close();

        for(int j=0;j<vOutput.size();j++)
   {
   for(int i=0;i<vGate.size();i++)
   {
     if(vGate[i]->gateName==vOutput[j]->gateName)
     {//cout<<vGate[i]->gateName<<" "<<vGate[i]->gate_Name<<" "<<i<<" "<<vGate.size()<<endl;
     outputcollect.push_back(vGate[i]);}
   }
   }
}
//------------------------------------------------------------------------
void Parser_C::netlisting()
{
	Gate_C *pGate;
	//#pragma omp parallel for
	for(int gateIter=0;gateIter<vGate.size();gateIter++)
    {
		pGate = vGate[gateIter];
		for(int Iter=0;Iter<pGate->FaninName.size();Iter++)
    	{
			for(int GinIter=0;GinIter<vGate.size();GinIter++)
    		{   //cout<<vGate[GinIter]->gateName<<endl;
				if(vGate[GinIter]->gateName == pGate->FaninName[Iter])
				{   //cout<<vGate[GinIter]->gateName<<endl;
			       // cout<<pGate->FaninName[Iter]<<endl;
			       int i=pGate->Fanin.size();
			       //cout<<i<<endl;
					pGate->Fanin.push_back(vGate[GinIter]);
					vGate[GinIter]->Fanout.push_back(pGate);
					break;
				}
			}
		}
	}
   // cout<<"[netlistingCkt Done]"<<endl;
    return;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void Parser_C::levelization()
{

    /*for(int i=0;i<vInput.size();i++)
    {
        vInput[i]->logic=1;
    }*/
	int nowlevel = 0;
	Gate_C * pGate;
	rowlevel.push_back(level);
	 for(int gateIter=0;gateIter<vInput.size();gateIter++)
    { //cout<<"a";
      //cout<<vInput[gateIter]->Fanout.size();
		rowlevel[0].push_back(vInput[gateIter]);                     //input
		for(int GoutIter=0;GoutIter<vInput[gateIter]->Fanout.size();GoutIter++)
		{ //cout<<"a";
			//vInput[gateIter]->Fanout[GoutIter]->fm_delay=1;
			Q.push(vInput[gateIter]->Fanout[GoutIter]);              //push inputfanput

		}
	}

	while(!Q.empty())
	{   //cout<<"b";
		pGate = Q.front();
		if(pGate->level>0)        //level ∕﹚  铬
		{
			Q.pop();
			continue;
		}

		int maxFaninLevel = 0;
        bool unset = false;
		for(int GinIter=0;GinIter<pGate->Fanin.size();GinIter++)
        {

			if(pGate->Fanin[GinIter]->level<0)       //ㄤいfanin临⊿∕﹚->unset=true   break
			{   //cout<<pGate->gateName<<endl;
				unset=true;
                Q.push(pGate);
                Q.pop();
                //pGate->ready+=1;
               // cout<<"r"<<pGate->ready<<" ";
                break;
			}
			else
			{  // cout<<"c";

				maxFaninLevel = max(maxFaninLevel,pGate->Fanin[GinIter]->level);   //珼fanin level程秨﹍ (0,0)

			}
		}


		if(!unset)       //砞level level рfanoutメ秈queue
        {   //cout<<"c";
			int newlevel = maxFaninLevel+1;     //faninlevel单newlevel  秨﹍ 1
			pGate->level = newlevel;
			if(newlevel>nowlevel)         //穝level>瞷level
			{
				rowlevel.push_back(level);
				//cout<<newlevel<<endl;
				nowlevel=newlevel;
			}
			rowlevel[newlevel].push_back(pGate);
			for(int GoutIter=0;GoutIter<pGate->Fanout.size();GoutIter++)
			{
				Q.push(pGate->Fanout[GoutIter]);
			}
			Q.pop();
		}
	}

   // cout<<"[Levelization Done]"<<endl;

    for(int i=0;i<rowlevel.size();i++)
    {
        for(int j=0;j<rowlevel[i].size();j++)
        {  // cout<<rowlevel[i][j]->level<<"  ";
           // cout<<rowlevel[i][j]->gate_Name<<" "<<rowlevel[i][j]->gateName<<endl;
           /* for(int k=0;k<rowlevel[i][j]->true_path.size();k++)
            cout<<rowlevel[i][j]->true_path[k]->gate_Name<<endl;*/
        }
    }
  //  cout<<(rowlevel[0][0])->level;
    return ;

}
//------------------------------------------------------------------------
void Parser_C::true_path(bool log)
{
     input.clear();
    for(int i=0;i<vInput.size();i++)
    {
        vInput[i]->logic=log;
        vInput[i]->fm_delay=0;
        input.push_back(log);
    }


    Gate_C * pGate;
     for(int i=0;i<rowlevel.size();i++)
    {
        for(int j=0;j<rowlevel[i].size();j++)
        {   //cout<<rowlevel[i][j]->level<<"  ";
          //  cout<<rowlevel[i][j]->gate_Name<<" "<<rowlevel[i][j]->gateName<<endl;
           /* for(int k=0;k<rowlevel[i][j]->true_path.size();k++)
            cout<<rowlevel[i][j]->true_path[k]->gate_Name<<endl;*/
           pGate=rowlevel[i][j];
           rowlevel[i][j]->true_path.clear();
         //cout<<pGate->gateType<<" "<<endl;

     //if(pGate->ready==1)
       // {
        if(pGate->gateType=="NOT1")
        {   //cout<<"a";
            pGate->logic=!(pGate->Fanin[0]->logic);
            pGate->true_path.push_back(pGate->Fanin[0]);
            pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
        }
        //}




       // if(pGate->ready==2)
        //{
       else  if(pGate->gateType=="NAND2")
        {
            pGate->logic=!((pGate->Fanin[0]->logic)&(pGate->Fanin[1]->logic));

           /*  if(pGate->Fanin[0]->gateType=="input"&&pGate->Fanin[1]->gateType=="input")
              {
                  pGate->true_path.push_back(pGate->Fanin[0]);
                  pGate->true_path.push_back(pGate->Fanin[1]);
                  pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
              }*/
           //else
           //{


            if((pGate->Fanin[0]->logic==0)&&(pGate->Fanin[1]->logic==0))
            {
                if(pGate->Fanin[0]->fm_delay<pGate->Fanin[1]->fm_delay)
                    {
                        pGate->true_path.push_back(pGate->Fanin[0]);
                        pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
                    }
                else if(pGate->Fanin[0]->fm_delay<pGate->Fanin[1]->fm_delay)
                    {
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                    }
                else
                {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
            }
            else if (pGate->Fanin[0]->logic==0)
            {
                 pGate->true_path.push_back(pGate->Fanin[0]);
                 pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
            }
            else if(pGate->Fanin[1]->logic==0)
            {
                pGate->true_path.push_back(pGate->Fanin[1]);
                pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
            }
            else
            {
                if(pGate->Fanin[0]->fm_delay==pGate->Fanin[1]->fm_delay)
                {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
                else if(pGate->Fanin[0]->fm_delay>pGate->Fanin[1]->fm_delay)
                   {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
                   }
                else
                {
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
            }
           //}
        }
        else if(pGate->gateType=="NOR2")
        {
            pGate->logic=!((pGate->Fanin[0]->logic)|(pGate->Fanin[1]->logic));

             /*if(pGate->Fanin[0]->gateType=="input"&&pGate->Fanin[1]->gateType=="input")
              {
                  pGate->true_path.push_back(pGate->Fanin[0]);
                  pGate->true_path.push_back(pGate->Fanin[1]);
                  pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
              }*/
          // else
           //{

            if((pGate->Fanin[0]->logic==1)&&(pGate->Fanin[1]->logic==1))
            {
                if(pGate->Fanin[0]->fm_delay<pGate->Fanin[1]->fm_delay)
                   {
                       pGate->true_path.push_back(pGate->Fanin[0]);
                       pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
                   }
                else if(pGate->Fanin[0]->fm_delay<pGate->Fanin[1]->fm_delay)
                   {
                        pGate->true_path.push_back(pGate->Fanin[1]);
                        pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                   }
                else
                {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
            }
            else if (pGate->Fanin[0]->logic==1)
            {
                 pGate->true_path.push_back(pGate->Fanin[0]);
                 pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
            }
            else if(pGate->Fanin[1]->logic==1)
            {
                pGate->true_path.push_back(pGate->Fanin[1]);
                pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
            }
            else
            {
                if(pGate->Fanin[0]->fm_delay==pGate->Fanin[1]->fm_delay)
                {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
                else if(pGate->Fanin[0]->fm_delay>pGate->Fanin[1]->fm_delay)
                    {
                        pGate->true_path.push_back(pGate->Fanin[0]);
                        pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
                    }
                else
                {
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
            }
           //}
        }



        }
    }


  /*for(int i=0;i<rowlevel.size();i++)
    {
        for(int j=0;j<rowlevel[i].size();j++)
        {   cout<<"level:"<<rowlevel[i][j]->level<<"  ";
            cout<<rowlevel[i][j]->gate_Name<<" "<<rowlevel[i][j]->gateType<<" "<<rowlevel[i][j]->gateName<<" fm_delay:"<<rowlevel[i][j]->fm_delay<<" logic:"<<rowlevel[i][j]->logic<<endl;
            for(int k=0;k<rowlevel[i][j]->true_path.size();k++)
            cout<<rowlevel[i][j]->true_path[k]->gate_Name<<endl;
        }
    }*/


        //}
}
void Parser_C::find_true_path()
{
    ////////////////////////////////////////////////////////********
    int begin_index=outlist.size();
   for(int j=0;j<outputcollect.size();j++)
   {
       outlist.push_back(out);
       //input.push_back();
       outlist[outlist.size()-1].push_back(outputcollect[j]);
       inputlist.push_back(input);
       //cout<<"output:"<<outputcollect[j]->gate_Name<<endl;
           child_path(outlist[outlist.size()-1],outlist.size()-1);  //程

           /*if(j==3)
           {
               cout<<outlist[outlist.size()-2][0]->gate_Name<<endl;
           cout<<outlist[outlist.size()-2][1]->gate_Name<<endl;
               cout<<outlist[outlist.size()-1][0]->gate_Name<<endl;
          cout<<outlist[outlist.size()-1][1]->gate_Name<<endl;

           }*/
   }
   for (int i=begin_index;i<outlist.size();i++)
   {      //cout<<"num"<<i<<":"<<endl;
       logic_list.push_back(outlist[i][outlist[i].size()-1]->logic);
       /*for(int j=0;j<outlist[i].size();j++)
       {
           cout<<outlist[i][j]->gate_Name<<endl;

       }*/
   }
}
void Parser_C::child_path(vector<Gate_C*> &aa,int index)
{       vector<Gate_C*> a;
        a.assign(aa.begin(), aa.end());
         if(a[a.size()-1]->true_path.size()==0)
          {
             input_name.push_back(a[a.size()-1]->gateName);
             return;
          }
             //cout<<"child:"<<a[a.size()-1]->gate_Name<<endl;
            if(a[a.size()-1]->true_path.size()==2)
            {//cout<<"a"<<endl;
               // if(a[a.size()-1]->gate_Name=="U16")cout<<"HAHAHAHAHA"<<endl;
            outlist.push_back(out);
            inputlist.push_back(input);
            outlist[outlist.size()-1].assign(a.begin(), a.end());

            outlist[outlist.size()-1].push_back(a[a.size()-1]->true_path[1]);//cout<<"a"<<endl;
            outlist[index].push_back(a[a.size()-1]->true_path[0]);//cout<<"a"<<endl;
              //cout<<"lost:"<<a[a.size()-1]->gate_Name<<endl;
           // cout<<outlist[outlist.size()-1][1]->gate_Name;
            //cout<<a[a.size()-1]->true_path[1]->gate_Name;
            //cout<<a[a.size()-2]->true_path[1]->gate_Name;
            //cout<<a[a.size()-2]->true_path[0]->gate_Name<<endl;


              child_path(outlist[outlist.size()-1],outlist.size()-1);
              child_path(outlist[index],index);
           // cout<<a[a.size()-1]->gate_Name;
            }
            else
            {// cout<<"a";
              outlist[index].push_back(a[a.size()-1]->true_path[0]);
             // cout<<"a";
              // cout<<a[a.size()-2]->true_path[0]->gate_Name<<endl;
                 child_path(outlist[index],index);
              // cout<<"child:"<<a[a.size()-1]->gate_Name<<endl;
              //cout<<"a";
            }


}
int Parser_C::take_inputnum()
{
    return vInput.size();
}



void Parser_C::other_path(vector<int> &s/*bool d,int in_case*/)
{
    input.clear();
    for(int i=0;i<vInput.size();i++)
       {
        vInput[i]->logic=!!(s[i]); //cout<<s[i]<<"_";
        vInput[i]->fm_delay=0;
        input.push_back(!!(s[i]));
       }
 /* if(in_case==0)
  {

  for(int i=0;i<vInput.size();i=i+2)
    {
        vInput[i]->logic=d;
        vInput[i]->fm_delay=0;

    }
  for(int i=1;i<vInput.size();i=i+2)
    {
        vInput[i]->logic=!d;
        vInput[i]->fm_delay=0;

    }

  }
  else if(in_case==1)
  {

    for(int i=0;i<vInput.size()/2;i++)
    {
        vInput[i]->logic=d;
        vInput[i]->fm_delay=0;

    }
     for(int i=vInput.size()/2;i<vInput.size();i++)
    {
        vInput[i]->logic=!d;
        vInput[i]->fm_delay=0;

    }

  }
  else if(in_case==2)
  {
       for(int i=0;i<vInput.size()/3;i++)
    {
        vInput[i]->logic=d;
        vInput[i]->fm_delay=0;

    }
     for(int i=vInput.size()/3;i<vInput.size();i++)
    {
        vInput[i]->logic=!d;
        vInput[i]->fm_delay=0;

    }
  }
   else if(in_case==3)
  {
       for(int i=0;i<vInput.size()/4;i++)
    {
        vInput[i]->logic=d;
        vInput[i]->fm_delay=0;

    }
     for(int i=vInput.size()/4;i<vInput.size();i++)
    {
        vInput[i]->logic=!d;
        vInput[i]->fm_delay=0;

    }
  }*/

    //vInput[d]->logic=0;
    Gate_C * pGate;
     for(int i=0;i<rowlevel.size();i++)
    {
        for(int j=0;j<rowlevel[i].size();j++)
        {   //cout<<rowlevel[i][j]->level<<"  ";
          //  cout<<rowlevel[i][j]->gate_Name<<" "<<rowlevel[i][j]->gateName<<endl;
           /* for(int k=0;k<rowlevel[i][j]->true_path.size();k++)
            cout<<rowlevel[i][j]->true_path[k]->gate_Name<<endl;*/
           pGate=rowlevel[i][j];
           rowlevel[i][j]->true_path.clear();
         //cout<<pGate->gateType<<" "<<endl;

     //if(pGate->ready==1)
       // {
        if(pGate->gateType=="NOT1")
        {   //cout<<"a";
            pGate->logic=!(pGate->Fanin[0]->logic);
            pGate->true_path.push_back(pGate->Fanin[0]);
            pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
        }
        //}




       // if(pGate->ready==2)
        //{
       else  if(pGate->gateType=="NAND2")
        {
            pGate->logic=!((pGate->Fanin[0]->logic)&(pGate->Fanin[1]->logic));
           /* if(pGate->Fanin[0]->gateType=="input"&&pGate->Fanin[1]->gateType=="input")
              {
                  pGate->true_path.push_back(pGate->Fanin[0]);
                  pGate->true_path.push_back(pGate->Fanin[1]);
                  pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
              }*/
           //else
          // {

            if((pGate->Fanin[0]->logic==0)&&(pGate->Fanin[1]->logic==0))
            {
                if(pGate->Fanin[0]->fm_delay<pGate->Fanin[1]->fm_delay)
                    {
                        pGate->true_path.push_back(pGate->Fanin[0]);
                        pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
                    }
                else if(pGate->Fanin[0]->fm_delay>pGate->Fanin[1]->fm_delay)
                    {
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                    }
                else
                {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
            }
            else if (pGate->Fanin[0]->logic==0)
            {
                 pGate->true_path.push_back(pGate->Fanin[0]);
                 pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
            }
            else if(pGate->Fanin[1]->logic==0)
            {
                pGate->true_path.push_back(pGate->Fanin[1]);
                pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
            }
            else
            {
                if(pGate->Fanin[0]->fm_delay==pGate->Fanin[1]->fm_delay)
                {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
                else if(pGate->Fanin[0]->fm_delay>pGate->Fanin[1]->fm_delay)
                   {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
                   }
                else
                {
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
            }

           //}
        }
        else if(pGate->gateType=="NOR2")
        {
            pGate->logic=!((pGate->Fanin[0]->logic)|(pGate->Fanin[1]->logic));
           /* if(pGate->Fanin[0]->gateType=="input"&&pGate->Fanin[1]->gateType=="input")
              {
                  pGate->true_path.push_back(pGate->Fanin[0]);
                  pGate->true_path.push_back(pGate->Fanin[1]);
                  pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
              }*/
          // else
           //{


            if((pGate->Fanin[0]->logic==1)&&(pGate->Fanin[1]->logic==1))
            {
                if(pGate->Fanin[0]->fm_delay<pGate->Fanin[1]->fm_delay)
                   {
                       pGate->true_path.push_back(pGate->Fanin[0]);
                       pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
                   }
                else if(pGate->Fanin[0]->fm_delay<pGate->Fanin[1]->fm_delay)
                   {
                        pGate->true_path.push_back(pGate->Fanin[1]);
                        pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                   }
                else
                {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
            }
            else if (pGate->Fanin[0]->logic==1)
            {
                 pGate->true_path.push_back(pGate->Fanin[0]);
                 pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
            }
            else if(pGate->Fanin[1]->logic==1)
            {
                pGate->true_path.push_back(pGate->Fanin[1]);
                pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
            }
            else
            {
                if(pGate->Fanin[0]->fm_delay==pGate->Fanin[1]->fm_delay)
                {
                    pGate->true_path.push_back(pGate->Fanin[0]);
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
                else if(pGate->Fanin[0]->fm_delay>pGate->Fanin[1]->fm_delay)
                    {
                        pGate->true_path.push_back(pGate->Fanin[0]);
                        pGate->fm_delay=pGate->Fanin[0]->fm_delay+1;
                    }
                else
                {
                    pGate->true_path.push_back(pGate->Fanin[1]);
                    pGate->fm_delay=pGate->Fanin[1]->fm_delay+1;
                }
            }
           //}
        }



        }
    }


/*  for(int i=0;i<rowlevel.size();i++)
    {
        for(int j=0;j<rowlevel[i].size();j++)
        {   cout<<rowlevel[i][j]->level<<"  ";
            cout<<rowlevel[i][j]->gate_Name<<" "<<rowlevel[i][j]->gateType<<" "<<rowlevel[i][j]->gateName<<" "<<rowlevel[i][j]->fm_delay<<endl;
            for(int k=0;k<rowlevel[i][j]->true_path.size();k++)
            cout<<rowlevel[i][j]->true_path[k]->gate_Name<<endl;
        }
    }*/

}

void Parser_C::find_other_path()
{
    ////////////////////////////////////////////////////////********
    int begin_index=outlist.size();
   for(int j=0;j<outputcollect.size();j++)
   {
       outlist.push_back(out);
       inputlist.push_back(input);
       outlist[outlist.size()-1].push_back(outputcollect[j]);
       //cout<<"output:"<<outputcollect[j]->gate_Name<<endl;
           child_path(outlist[outlist.size()-1],outlist.size()-1);  //程

           /*if(j==3)
           {
               cout<<outlist[outlist.size()-2][0]->gate_Name<<endl;
           cout<<outlist[outlist.size()-2][1]->gate_Name<<endl;
               cout<<outlist[outlist.size()-1][0]->gate_Name<<endl;
          cout<<outlist[outlist.size()-1][1]->gate_Name<<endl;

           }*/
   }
 /*  for (int i=0;i<outlist.size();i++)
   {      cout<<"num"<<i<<":"<<endl;
       for(int j=0;j<outlist[i].size();j++)
       {
           cout<<outlist[i][j]->gate_Name<<endl;
       }
   }*/
   for (int i=begin_index;i<outlist.size();i++)
   {      //cout<<"num"<<i<<":"<<endl;
       logic_list.push_back(outlist[i][outlist[i].size()-1]->logic);
       /*for(int j=0;j<outlist[i].size();j++)
       {
           cout<<outlist[i][j]->gate_Name<<endl;

       }*/
   }

   for (int i=begin_index;i<outlist.size();i++)
   {
       for(int j=0;j<begin_index;j++)
       {    int cf_path=0;
           if(outlist[i].size()==outlist[j].size())
           {
               if(logic_list[i]==logic_list[j])
               {
                 if(outlist[i][outlist[i].size()-1]->gate_Name==outlist[j][outlist[j].size()-1]->gate_Name)
                {
                if(outlist[i][0]->gate_Name==outlist[j][0]->gate_Name)
                 { //cout<<"=="<<outlist[i][0]->gate_Name<<" "<<outlist[j][0]->gate_Name<<endl;

                   if(outlist[i].size()==2)
                   {
                       outlist[j].clear();
                      // logic_list[j]="NULL";
                   }
                   else{
                   for(int c=1;c<outlist[i].size()-1;c++)
                   {    //cout<<"a";
                       if(outlist[i][c]->gate_Name!=outlist[j][c]->gate_Name)
                         {break;}
                       if(outlist[i][c]->gate_Name==outlist[j][c]->gate_Name)
                        {
                        cf_path++;
                       if(cf_path==outlist[i].size()-2)
                          {
                           outlist[j].clear();
                         // logic_list[j]=NULL;
                         // cout<<"::::::"<<logic_list[j];
                           break;
                          }
                        }

                    }
                   }

                   }
                  }
                }
           }

       }
   }
}
void Parser_C::dump(char* Argv2)
{

    ofstream out;
    out.open(Argv2);
    Gate_C *pGate,*qGate,*mGate;
    string temp;

out<<"Header  {  A True Path Set  }\nBenchmark  {  "<<benchmark<<"  }";

vector<bool> logic111;
bool logic222;
//cout<<outlist[0][outlist[0].size()-1]->gate_Name;
int null=0,t_p=0,violate=0,indxback=0;



  for (int i=0;i<outlist.size();i++)
   {    indxback=0;
       for(int inindex=0;inindex<vInput.size();inindex++)
             {
              if(vInput[inindex]->gate_Name==input_name[i])
                logic222=logic_list[i];
             }


       for(int logicindx=0;logicindx<outlist[i].size()*2;logicindx++)
      {
       if(logicindx%2==0)
       {
       logic111.push_back(logic222);
       logic111.push_back(logic222);
       }
       else
       {
           logic222=!logic222;
          // if(logic222==0)cout<<"0";
          // if(logic222==1)cout<<"1";
       }

      }



       int k=0;

       if(outlist[i].size()==0||(timing_constraint+1-outlist[i].size())>=hard_slack_constraint)
       {
        null++;
       //out<<"num"<<(i-null)<<":"<<endl;

       }
       else   //才constraint outlistΤ狥﹁
       {

            out<<"\n\nPath { "<<(i-null+1)<<" }\n\nA True Path List\n{\n";
           out<<"-------------------------------------------\n";
           out<<"Pin\t\ttype\t\t\tIncr\tPath delay\n-------------------------------------------\n";

            for(int j=outlist[i].size()-1;j>-1;j--)
         {


          if(j==outlist[i].size()-1)
          {
           out<<outlist[i][j]->gate_Name<<"\t\t(in)\t"<<"\t\t0"<<"\t\t"<<"0 ";

           if(logic111[indxback]==0)
           {
               out<<"f"<<endl;
           }
           else
           {
               out<<"r"<<endl;
           }

           indxback++;

          }
          else
          {

           if ((outlist[i][j]->gateType=="NOT1")||(outlist[i][j+1]->gate_Name==outlist[i][j]->true_path[0]->gate_Name))
           {
            out<<outlist[i][j]->gate_Name<<"/A\t("<<outlist[i][j]->gateType<<")\t\t\t0"<<"\t\t"<<k-1<<" ";
            if(logic111[indxback]==0)
           {
               out<<"f"<<endl;
           }
           else
           {
               out<<"r"<<endl;
           }

           indxback++;
           }
           else
           {
            out<<outlist[i][j]->gate_Name<<"/B\t("<<outlist[i][j]->gateType<<")\t\t\t0"<<"\t\t"<<k-1<<" ";
            if(logic111[indxback]==0)
           {
               out<<"f"<<endl;
           }
           else
           {
               out<<"r"<<endl;
           }

           indxback++;
           }



           out<<outlist[i][j]->gate_Name<<"/Y\t("<<outlist[i][j]->gateType<<")\t\t\t1"<<"\t\t"<<k<<" ";
           if(logic111[indxback]==0)
           {
               out<<"f"<<endl;
           }
           else
           {
               out<<"r"<<endl;
           }

           indxback++;



           if(j==0)
           {
            // out<<"slack="<<(timing_constraint+1-outlist[i].size())<<endl;
             out<<outlist[i][j]->gateName<<"\t\t(out)\t"<<"\t\t0"<<"\t\t"<<k<<" ";
             if(logic111[indxback-1]==0)
           {
               out<<"f"<<endl;
           }
           else
           {
               out<<"r"<<endl;
           }
             out<<"-------------------------------------------\nData Required Time\t"<<timing_constraint<<"\nData Arrival Time\t"<<outlist[i].size()-1<<endl;
             out<<"-------------------------------------------\nSlack\t\t\t\t"<<(timing_constraint+1-outlist[i].size())<<"\n}\n\n";
             out<<"\nInput Vector\n{\n";
             for(int inindex=0;inindex<vInput.size();inindex++)
             {
              if(vInput[inindex]->gate_Name==outlist[i][outlist[i].size()-1]->gate_Name)
              {//cout<<outlist[i][outlist[i].size()-1]->gate_Name;
                out<<vInput[inindex]->gate_Name<<"\t=\t";
                if (inputlist[i][inindex]==0)out<<"f"<<endl;
                else if (inputlist[i][inindex]==1)out<<"r"<<endl;
              }

               else
              {
                out<<vInput[inindex]->gate_Name<<"\t=\t";
                if (inputlist[i][inindex]==0)out<<"0"<<endl;
                else if (inputlist[i][inindex]==1)out<<"1"<<endl;
              }

             }
             out<<"\t}";
           }

          }



          k++;



         }

       }
     logic111.clear();
   }


 //  out<<"true_path_num:"<<t_p<<endl;



}



