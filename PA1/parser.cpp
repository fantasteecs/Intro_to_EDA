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

using namespace std; 

void Parser_C::readbench(char* Argv1)
{  
    char *Sect;
    char buffer[256];
    Gate_C *pGate;

    ifstream bench;
    bench.open(Argv1,ios::in);
    
    bench.getline(buffer,256);
	while(strstr(buffer,"INPUT")==NULL){bench.getline(buffer,256);}
    char s[10];
  //**********************************************************
    int InputNum=0;
    while(strstr(buffer,"INPUT")!=NULL)
    {    InputNum++;
         cout<<buffer<<endl;
        Sect = strtok(buffer," ( ) ");
        Sect = strtok(NULL,"()");
//        cout<<"INname:"<<nName<<endl;

        pGate = new Gate_C;
		sprintf(s,"%d",InputNum);
		pGate->gateName = Sect;
		pGate->gateType = "INPUT";             //type  
		pGate->level = 0;                      //level
		pGate->arrivalT = 0;
		vGate.push_back(pGate);
		vInput.push_back(pGate);                //arrt
        bench.getline(buffer,256);

    }
    while(strlen(buffer)==0){
        bench.getline(buffer,256);
    }
 ///////////////////////////////////////////////////////////
  while(strstr(buffer,"OUTPUT")!=NULL)
    {   cout<<buffer<<endl;
        Sect = strtok(buffer,"()");
        Sect = strtok(NULL,"()");
//        cout<<"OUTname:"<<nName<<endl;
        output.push_back(Sect);


        bench.getline(buffer,256);
    }
    while(strlen(buffer)==0){
       bench.getline(buffer,256);
    }
 //////////////////////////////////////////////////////////
 //************************************************************
    int gateNum=0;
    while(!bench.eof() && buffer[0]!='\n' && buffer[0]!='\0')
    {   cout<<buffer<<endl;
        Sect = strtok(buffer,"= ");
        pGate = new Gate_C;                         
		pGate->gateName = Sect;
        cout<<Sect<<endl;
		
        Sect = strtok(NULL,"() =");
        pGate->gateType = Sect;                  //type
        cout<<Sect<<endl;

        Sect = strtok(NULL,"() =,");  
       
		while(Sect!=NULL)
        {   
            pGate->FaninName.push_back(Sect);   //fanin
            cout<<Sect<<endl;
            Sect = strtok(NULL,"( ) = , ");

        }


        vGate.push_back(pGate);
        bench.getline(buffer,256);

        gateNum++;
    }
    bench.close();

    cout<<"[ReadBench Done] input:"<<InputNum<<" gate:"<<gateNum<<endl;
    return;
 //************************************************************

}
//------------------------------------------------------------------------
void Parser_C::netlisting()
{
	Gate_C *pGate;
	for(int gateIter=0;gateIter<vGate.size();gateIter++)
    {
		pGate = vGate[gateIter];
		for(int Iter=0;Iter<pGate->FaninName.size();Iter++)
    	{
			for(int GinIter=0;GinIter<vGate.size();GinIter++)
    		{
				if(vGate[GinIter]->gateName == pGate->FaninName[Iter])
				{  // cout<<vGate[GinIter]->gateName<<endl;
			       // cout<<pGate->FaninName[Iter]<<endl;
					pGate->Fanin.push_back(vGate[GinIter]);
					vGate[GinIter]->Fanout.push_back(pGate);
				}
			}
		}
	}
    cout<<"[netlistingCkt Done]"<<endl;
    return;
}
//------------------------------------------------------------------------
void Parser_C::readlib(char* Argv2)
{
    char *Sect;
    char buffer[256],s[256];
    string name,G = "G";
    Lib_C *pLib;
    Gate_C *pGate;

    ifstream lib;
    lib.open(Argv2,ios::in);

    if(!lib)
    {
        printf("[Abort-Parser #1] - Lib File Can Not Be Opened\n");
        assert(0);
    }

	lib.getline(buffer,256);

	while(!lib.eof())
	{
		pLib = new Lib_C;
		Sect = strtok(buffer," \r\n\0");
		pLib->cellType = Sect;                 //celltype
		//cout<<Sect<<endl;
		lib.getline(buffer,256);
		pLib->cellDelay = atoi(buffer);       //celldelay
		//cout<<pLib->cellDelay<<endl;

		lib.getline(buffer,256);


			/////////////////////////////////////////////////////////

    while(!lib.eof() && buffer[0]!='\n' && buffer[0]!='\0')
    {  // cout<<buffer<<endl;
        Sect = strtok(buffer,"= ");
        pGate = new Gate_C;
        name.clear();
		name = G;
		name.append(Sect);                         //name=G+s
		pGate->gateName = Sect;
		//cout<<Sect<<endl;

        Sect = strtok(NULL,"() =");
        pGate->gateType = Sect;                  //type
//cout<<Sect<<endl;

        Sect = strtok(NULL,"() =,");
		//int i=0;
        while(Sect!=NULL)
        {
            pGate->FaninName.push_back(Sect);   //fanin
            //cout<<Sect<<endl;
            Sect = strtok(NULL,"( ) = , ");
           //i++;
        }

       // cout<<i;
        pLib->cell.push_back(pGate);
        lib.getline(buffer,256);


    }
    vLib.push_back(pLib);
    lib.getline(buffer,256);
	}
	lib.close();
    return;
	cout<<"[ReadLib Done] library:"<<vLib.size()<<endl;
	}
	
void Parser_C::netlistingLib()
{
	Gate_C *pGate;
	Lib_C *pLib;

	for(unsigned int libIter=0;libIter<vLib.size();libIter++)
	{
	    for(unsigned int gateIter=0;gateIter<vLib[libIter]->cell.size();gateIter++)
	    {
		pGate = vLib[libIter]->cell[gateIter];
		for(unsigned int Iter=0;Iter<pGate->FaninName.size();Iter++)
	    	{
			for(unsigned int GinIter=0;GinIter<vLib[libIter]->cell.size();GinIter++)
	    	{   //cout<<libIter<<"  ";
				//cout<<vLib[libIter]->cell[GinIter]->gateName<<" "<<pGate->FaninName[Iter]<<endl;
				if(vLib[libIter]->cell[GinIter]->gateName==pGate->FaninName[Iter])
				{  // cout<<vLib[libIter]->cell[GinIter]->gateName<<" "<<pGate->FaninName[Iter]<<endl;
					pGate->Fanin.push_back(vLib[libIter]->cell[GinIter]);
					vLib[libIter]->cell[GinIter]->Fanout.push_back(pGate);
				}
			}
		}
	    }
	}

    cout<<"[netlistingLib Done] library:"<<vLib.size()<<endl;
    return;
}
//------------------------------------------------------------------------
void Parser_C::levelization()
{
	int nowlevel = 0;
	Gate_C * pGate;
	rowlevel.push_back(level);
	 for(int gateIter=0;gateIter<vInput.size();gateIter++)
    { //cout<<"a";
      //cout<<vInput[gateIter]->Fanout.size();
		rowlevel[0].push_back(vInput[gateIter]);
		for(int GoutIter=0;GoutIter<vInput[gateIter]->Fanout.size();GoutIter++)
		{ //cout<<"a";
			Q.push(vInput[gateIter]->Fanout[GoutIter]);
		}
	}
  
	while(!Q.empty())
	{
		pGate = Q.front();
		if(pGate->level>0)
		{
			Q.pop();
			continue;
		}

		int maxFaninLevel = 0;
        bool unset = false;
		for(int GinIter=0;GinIter<pGate->Fanin.size();GinIter++)
        {
			if(pGate->Fanin[GinIter]->level<0)
			{  //cout<<"a";
				unset=true;
                Q.push(pGate);
                Q.pop();
                break;
			}
			else
			{
				maxFaninLevel = max(maxFaninLevel,pGate->Fanin[GinIter]->level);
			}
		}

		if(!unset)
        {   //cout<<"a";
			int newlevel = maxFaninLevel+1;
			pGate->level = newlevel;
			if(newlevel>nowlevel)
			{
				rowlevel.push_back(level);
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

    cout<<"[Levelization Done]"<<endl;  
    return ;

}
//------------------------------------------------------------------------
Gate_C* Parser_C::compare(Gate_C *pGate,Gate_C *qGate,double maxdelay)
{
	Gate_C *mGate,*nGate;
	double delay = maxdelay;

	mGate = new Gate_C;
	if(pGate->gateType != qGate->gateType)
	{   if((pGate->gateType=="INPUT"&&qGate->gateType=="NOT")&&(qGate->Fanout.size()!=0&&qGate->Fanin.size() == 0))
	    {//cout<<pGate->gateName<<" "<<pGate->gateType<<endl;
	        
	    if(1>delay)delay = 1;
        mGate->FaninName.push_back("INPUT");//pGate的所有fanin
		mGate->arrivalT = delay;                     //fanin中最大arrt當delay
		return mGate;
	    
	    }
		else 
        {
            mGate->arrivalT = -1;
            return mGate;
		}
	}
	else if(qGate->Fanin.size() == 0)
	{
		for(int i=0;i<pGate->Fanin.size();i++)
        {
            if(pGate->Fanin[i]->arrivalT>delay)
            {
                delay = pGate->Fanin[i]->arrivalT;
            }
            mGate->FaninName.push_back(pGate->Fanin[i]->gateName);
		}
		mGate->arrivalT = delay;
		return mGate;
	}
	else if(qGate->gateType == "NOT")
	{   //cout<<"a"<<endl;
		if(pGate->Fanin[0]->Fanout.size()>1)
		{
			mGate->arrivalT = -1;
		}
		else
		{
			mGate = compare(pGate->Fanin[0],qGate->Fanin[0],delay);
		}
		return mGate;
	}
	else if(qGate->Fanin.size() == 1)
	{
		if((compare(pGate->Fanin[0],qGate->Fanin[0],delay)->arrivalT>=0) && (pGate->Fanin[0]->Fanout.size()==1))
        {
			if(pGate->Fanin[1]->arrivalT>delay)
			{
				delay = pGate->Fanin[1]->arrivalT;
			}
			mGate = compare(pGate->Fanin[0],qGate->Fanin[0],delay);
			mGate->FaninName.push_back(pGate->Fanin[1]->gateName);

			if((compare(pGate->Fanin[1],qGate->Fanin[0],delay)->arrivalT>=0) && (pGate->Fanin[1]->Fanout.size()==1))
            {
				delay = maxdelay;
				if(pGate->Fanin[0]->arrivalT>delay)
				{
					delay = pGate->Fanin[0]->arrivalT;
				}
				nGate = compare(pGate->Fanin[1],qGate->Fanin[0],delay);
				nGate->FaninName.push_back(pGate->Fanin[0]->gateName);
				if(nGate->arrivalT > mGate->arrivalT)
				{
					mGate = nGate;
				}
			}
			return mGate;
		}
		else if((compare(pGate->Fanin[1],qGate->Fanin[0],delay)->arrivalT>=0) && (pGate->Fanin[1]->Fanout.size()==1))
		{
			if(pGate->Fanin[0]->arrivalT>delay)
			{
				delay = pGate->Fanin[0]->arrivalT;
			}
			mGate = compare(pGate->Fanin[1],qGate->Fanin[0],delay);
			mGate->FaninName.push_back(pGate->Fanin[0]->gateName);
			return mGate;
		}
		else
		{
			mGate->arrivalT = -1;
			return mGate;
		}
	}
	else
	{
		if(pGate->Fanin[0]->Fanout.size()>1 || pGate->Fanin[1]->Fanout.size()>1)
		{
			mGate->arrivalT = -1;
			return mGate;
		}
		else if((compare(pGate->Fanin[0],qGate->Fanin[0],delay)->arrivalT>=0) && (compare(pGate->Fanin[1],qGate->Fanin[1],delay)->arrivalT>=0))
		{
			mGate = compare(pGate->Fanin[0],qGate->Fanin[0],delay);
			nGate = compare(pGate->Fanin[1],qGate->Fanin[1],delay);
			for(int i=0;i<nGate->FaninName.size();i++)
			{
				mGate->FaninName.push_back(nGate->FaninName[i]);
			}
			mGate->arrivalT = max(mGate->arrivalT,nGate->arrivalT);
			return mGate;
		}
		else if((compare(pGate->Fanin[1],qGate->Fanin[0],delay)->arrivalT>=0) && (compare(pGate->Fanin[0],qGate->Fanin[1],delay)->arrivalT>=0))
		{
			mGate = compare(pGate->Fanin[1],qGate->Fanin[0],delay);
			nGate = compare(pGate->Fanin[0],qGate->Fanin[1],delay);
			for(int i=0;i<nGate->FaninName.size();i++)
			{
				mGate->FaninName.push_back(nGate->FaninName[i]);
			}
			mGate->arrivalT = max(mGate->arrivalT,nGate->arrivalT);
			return mGate;
		}
		else
		{
			mGate->arrivalT = -1;
			return mGate;
		}

	}

}
//------------------------------------------------------------------------
void Parser_C::run()
{
	Gate_C *pGate,*qGate,*mGateell,*minDelayCell;
	Lib_C *pLib;
	double outDelay = 0;

	for(int rowIter=1;rowIter<rowlevel.size();rowIter++)
	{
		for(int levIter=0;levIter<rowlevel[rowIter].size();levIter++)
		{
			pGate = rowlevel[rowIter][levIter];
			double arrivalT,minDelay=5000;
			for(int libIter=0;libIter<vLib.size();libIter++)
			{
				pLib = vLib[libIter];
				qGate = pLib->cell[pLib->cell.size()-1];
				//cout<<"pLib->cellType:"<<pLib->cellType<<endl;
				mGateell = compare(pGate,qGate,0);
				if(mGateell->arrivalT>=0)
				{
					arrivalT = mGateell->arrivalT + pLib->cellDelay;
					//cout<<"arrivalT:"<<arrivalT<<endl; 
					if(arrivalT<minDelay)
					{
						minDelay=arrivalT;
						minDelayCell=mGateell;
						minDelayCell->gateType = pLib->cellType;
					}
				}
			}

			pGate->arrivalT = minDelay;
			minDelayCell->gateName = pGate->gateName;
			minDelayCell->arrivalT = minDelay;
			//cout<<"pGate:"<<pGate->gateType<<" "<<pGate->gateName<<" -> ";
			//cout<<"minDelayCell:"<<minDelayCell->gateType<<" "<<minDelayCell->gateName<<" "<<minDelayCell->arrivalT<<endl;
			if(pGate->Fanout.size() == 0)
			{  // cout<<minDelayCell->gateType<<endl<<endl;
				vPath.push_back(minDelayCell);
				if(minDelay>outDelay)
				{
					outDelay = minDelay;
					critiOutput = minDelayCell;
				}
			}
			else
            {
				vCell.push_back(minDelayCell);
			}
		}
	}
	crioutDelay = outDelay;

	for(int pathIter=0;pathIter<vPath.size();pathIter++)
	{
		pGate = vPath[pathIter];
		for(int GinIter=0;GinIter<pGate->FaninName.size();GinIter++)
		{
			for(int cellIter=0;cellIter<vCell.size();cellIter++)
			{
				if(pGate->FaninName[GinIter] == vCell[cellIter]->gateName)
				{
					pGate->Fanin.push_back(vCell[cellIter]);
					int inpath=0;
					for(int i=0;i<vPath.size();i++)
					{    
						if(vPath[i]->gateName == vCell[cellIter]->gateName)
						inpath = 1;
					    //else cout<<vPath[i]->gateName<<" "<<vCell[cellIter]->gateName<<endl;
					}
					if(inpath == 0)
					{
						vPath.push_back(vCell[cellIter]);
					}
				}
				//if(pGate->FaninName[GinIter] != vCell[cellIter]->gateName)
					//cout<<pGate->FaninName[GinIter]<<" "<<vCell[cellIter]->gateName<<endl;
			}
		}
	}
	/*for(int i=0;i<vPath.size();i++){
		cout<<vPath[i]->gateType<<endl;
	}*/
	
	
    cout<<crioutDelay<<endl;
    cout<<"[RunDagon Done]"<<endl;
}
//------------------------------------------------------------------------
void Parser_C::dump(char* Argv3)
{

    ofstream out;
    out.open(Argv3);
    Gate_C *pGate,*qGate,*mGate;
    string temp;

	for(int i=0;i<vPath.size();i++)
	{
		pGate=vPath[i];
		for(int j=i+1;j<vPath.size();j++)
		{
			qGate=vPath[j];
			if(qGate->gateName.length()>pGate->gateName.length())
			{
				mGate=pGate;
				pGate=qGate;
				vPath[j]=mGate;
			}
			else if((qGate->gateName.length() == pGate->gateName.length())&&
			        (qGate->gateName > pGate->gateName))
			{
				mGate=pGate;
				pGate=qGate;
				vPath[j]=mGate;
			}
	    }
	    vPath[i]=pGate;
	}
 /*   for (int i=0;i<vPath.size();i++)
	{
		vvPath.push_back(vPath[i]);
	}*/
	char s[vPath.size()]; string name,n = "n"; int j=output.size()+(vInput.size()+1);int change[vPath.size()];
	for (int i=0;i<vPath.size();i++){
		change[i]=1;
	}
	for(int pathIter=vPath.size()-1;pathIter>=0;pathIter--)
	{   for (int outIter=output.size()-1;outIter>=0;outIter--){
		if(output[outIter]==vPath[pathIter]->gateName) change[pathIter]=0;
	    }
	}
  for (int i=vPath.size()-1;i>=0;i--){
	  if(change[i]==1){
		name.clear();
		name = n;
		sprintf(s,"%d",j);
		name.append(s);
		vPath[i]->gateName = name;//cout<<vPath[pathIter]->gateName;
		}
		j++;
	  
  }
	int i1,i2; string s1,s2;
	for(int pathIter=0;pathIter<vPath.size();pathIter++)
    {

		pGate = vPath[pathIter];
		for(int GinIter1=0;GinIter1<vPath[pathIter]->FaninName.size();GinIter1++)
		{   for(int inIter1=0;inIter1<vInput.size();inIter1++){
			if(vPath[pathIter]->FaninName[GinIter1]==vInput[inIter1]->gateName)
			{
				for(int GinIter2=GinIter1+1;GinIter2<vPath[pathIter]->FaninName.size();GinIter2++)
				{
					s1=pGate->FaninName[GinIter1]; s1.erase(s1.begin()); i1=atoi( s1.c_str() );
					s2=pGate->FaninName[GinIter2]; s2.erase(s2.begin()); i2=atoi( s2.c_str() );
					if(i2<i1)
					{
						temp = pGate->FaninName[GinIter2];
						pGate->FaninName[GinIter2] = pGate->FaninName[GinIter1];
						pGate->FaninName[GinIter1] = temp;
					}
				}
		}}
		}
	}

	for(int pathIter=0;pathIter<vPath.size();pathIter++)
	{
		pGate = vPath[pathIter];
		for(int GinIter1=0;GinIter1<vPath[pathIter]->Fanin.size();GinIter1++)
		{
			if(vPath[pathIter]->FaninName[GinIter1][0]=='n')
			{
				for(int GinIter2=GinIter1+1;GinIter2<vPath[pathIter]->Fanin.size();GinIter2++)
				{
					s1=pGate->Fanin[GinIter1]->gateName; s1.erase(s1.begin()); i1=atoi( s1.c_str() );
					s2=pGate->Fanin[GinIter2]->gateName; s2.erase(s2.begin()); i2=atoi( s2.c_str() );
					if(i2<i1)
					{
						mGate = pGate->Fanin[GinIter2];//cout<<"change "<<pGate->Fanin[GinIter1]->gateName<<" "<<pGate->Fanin[GinIter2]->gateName<<endl;
						pGate->Fanin[GinIter2] = pGate->Fanin[GinIter1];
						pGate->Fanin[GinIter1] = mGate;
					}
				}
			}
		}
	}

	for(int pathIter=vPath.size()-1;pathIter>=0;pathIter--)
	{
		double de=-1;
		for(int t=0;t<vPath[pathIter]->Fanin.size();t++)
		{
			if(vPath[pathIter]->Fanin[t]->arrivalT>de)
            {
				de=vPath[pathIter]->Fanin[t]->arrivalT;
                vPath[pathIter]->maxfanin = vPath[pathIter]->Fanin[t];
            }
		}
	}

	out<<crioutDelay<<endl<<endl;
	cout<<"[dump] critical delay:"<<crioutDelay<<endl;

	for (int i=0;i<vInput.size();i++){
		out<<"INPUT("<<vInput[i]->gateName<<")"<<endl;
	}
	
	out<<endl;
	for (int i=0;i<output.size();i++){
		out<<"OUTPUT("<<output[i]<<")"<<endl;
	}
	out<<endl;
    for(int pathIter=vPath.size()-1;pathIter>=0;pathIter--)
	{   out<<vPath[pathIter]->gateName<<" = ";
		out<<vPath[pathIter]->gateType<<"(";
		for(int GinIter=0;GinIter<vPath[pathIter]->Fanin.size();GinIter++)
		{
			mapcell.push_back(vPath[pathIter]->Fanin[GinIter]->gateName);
			//out<<vPath[pathIter]->Fanin[GinIter]->gateName<<" ";
		}
		for(int GinIter=0;GinIter<vPath[pathIter]->FaninName.size();GinIter++)
		{
			for(int inIter1=0;inIter1<vInput.size();inIter1++)
		 {
			if(vPath[pathIter]->FaninName[GinIter]==vInput[inIter1]->gateName)
		mapcell.push_back(vPath[pathIter]->FaninName[GinIter]);
	    // out<<vPath[pathIter]->FaninName[GinIter]<<" ";
		 }
		}
		vector<string>::iterator i;
		out<<*(mapcell.begin());
		for(i=mapcell.begin()+1;i!=mapcell.end();i++) out<<", "<<*i;  
		mapcell.clear();
		
		out<<")"<<endl;
	}
	out<<endl;


    vector<string> criPath;
    pGate = critiOutput;
    while(1)
    {   
		criPath.push_back(pGate->gateName);
		if(pGate->maxfanin!=NULL)
		pGate = pGate->maxfanin;
		else
		break;
	}
	int qq=0;
	for(int Iter=0;Iter<pGate->FaninName.size();Iter++)
	{for(int inIter1=0;inIter1<vInput.size();inIter1++){
		if(pGate->FaninName[Iter]==vInput[inIter1]->gateName)
		{
			criPath.push_back(pGate->FaninName[Iter]);
			qq=1;
			break;
	    }
	}
	if(qq==1)break;}
	for(int i=criPath.size()-1;i>=0;i--)
	{
		out<<criPath[i]<<" ";
	}

    return;
}
