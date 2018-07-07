#ifndef _INFO_CPP
#define _INFO_CPP

#include "info.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int Info::getVertexID(int u, int offset, int startOrArrival)
{
    //  0 arrival 1 start
    
    int base = u == 0 ? 0 : sumSize[u - 1];
    const vector<int> & arrival = arrivalT[u];
    const vector<int> & start = startT[u];
 
    if(startOrArrival == 0)
    {
        return base + offset;
    }
    else if(startOrArrival == 1)
    {
        return base +  arrival.size() + offset;
    }

}
Info::Info(string s)
{

    //in info
	fstream in;
	in.open(s.c_str(), ios::in);
	in >> v;
	cout << v << endl;
	startT = vector< vector<int> >(v, vector<int>() );
    arrivalT = vector< vector<int> >(v, vector<int>());
    curSize = vector<int>(v, 0);
    sumSize = vector<int>(v, 0);
    
	int u;
	int size;
	int tmp;
	for (int i = 0; i < v; ++ i)
	{
		in >> u >> size;
		for (int j = 0; j < size; ++ j)
		{
			in >> tmp;
			arrivalT[i].push_back(tmp);
		}
	}
	for (int i = 0; i < v; ++ i)
	{
		in >> u >> size;
		for (int j = 0; j < size; ++ j)
		{
			in >> tmp;
			startT[i].push_back(tmp);
		}
	}
	cout << "load done" << endl;
	for(int i = 0 ; i < v;i ++){
        int cur = startT[i].size() + arrivalT[i].size();
        curSize[i] = cur;
        sumSize[i] = cur + (i > 0 ? sumSize[i-1] : 0);
    }
    
    for(int i=0; i< v; i++){
		for(int j=0; j<arrivalT[i].size(); j++){
			timeId.push_back(arrivalT[i][j]);
		}
		
		for(int j=0; j<startT[i].size(); j++){
			timeId.push_back(startT[i][j]);
		}
	}
	in.close();
	//cout << "hi" << endl;
}

int Info::getFrom(int from, int t1, int t2)
{
	int tmp;
	for (int j = 0; j < startT[from].size(); ++ j)
	{
		tmp = getVertexID(from, j, 1);
		if (timeId[tmp]>=t1 && timeId[tmp]<=t2)
		{
			return tmp;	
		}
		else if (timeId[tmp]>t2)
		return -1;
	}
	return -1;
}
int Info::getFrom2(int from, int t1, int t2)
{
	if (startT[from].size() == 0) return -1;
	
	int tmp;
	int l = 0; int r = startT[from].size()-1;
	while(l < r-1)
	{
		int m = (l+r)/2;
		tmp = getVertexID(from, m, 1);
		if (timeId[tmp] >= t1)
		{
			r = m;
		}	
		else
		{
			l = m+1;
		}
	}
	tmp = getVertexID(from, l, 1);
	if (timeId[tmp]>=t1&&timeId[tmp]<=t2) return tmp;
	tmp = getVertexID(from, r, 1);
	if (timeId[tmp]>=t1&&timeId[tmp]<=t2) return tmp;
	
	return -1;
}
int Info::getFrom3(int from)
{
	return getVertexID(from, 0, 1);
}

int Info::getTo(int to, int t1, int t2)
{
	int tmp;
	for (int j = arrivalT[to].size()-1; j >= 0; -- j)
	{
		tmp = getVertexID(to, j, 0);
		
		if (timeId[tmp]>=t1 && timeId[tmp]<=t2)
		{
			return tmp;	
		}
		else if (timeId[tmp]<t1)
		return -1;
	}
	return -1;
}
int Info::getTo2(int to, int t1, int t2)
{
	if (arrivalT[to].size() == 0) return -1;
	
	int tmp;
	int l = 0; int r = arrivalT[to].size()-1;
	while(l < r-1)
	{
		int m = (l+r)/2;
		tmp = getVertexID(to, m, 0);	
		if (tmp<=t2)
		{
			l = m;
		}
		else
		{
			r = m-1;
		}
	}
	tmp = getVertexID(to, r, 0);
	if (timeId[tmp]>=t1&&timeId[tmp]<=t2) return tmp;
	tmp = getVertexID(to, l, 0);
	if (timeId[tmp]>=t1&&timeId[tmp]<=t2) return tmp;		
	
	return -1;
}

int Info::getTo3(int to)
{
	return getVertexID(to, arrivalT[to].size()-1, 0);
}

Query Info::query(int from, int to, int t1, int t2)
{
	
	
	//int u = getFrom(from, t1, t2);
	//int v = getTo(to, t1, t2);
	
	//cout << u << " " << v << endl;
	
	int u1 = getFrom2(from, t1, t2);
	int v1 = getTo2(to, t1, t2);

	
	//if (u1 == -1 || v1 == -1) return 0;
	
	//do some things
	
	//return 1;
	
	Query q;
	q.u = u1;
	q.v = v1;
	
	return q;
}
void Info::createQuery(string s, int query_number)
{
	fstream out;
	//query file
	out.open(s.c_str(), ios::out);
	
	int t1 = 0;
	int t2 = 10000;
	int i = 0;
	while(i < query_number)
	{
		int from = rand()%v;
		int to = rand()%v;
		//int u1 = getFrom2(from, t1, t2);
		//int v1 = getTo2(to, t1, t2);
		int u1 = getFrom3(from);
		int v1 = getTo3(to);
		//if (from == to) continue;
		if (u1 == -1 || v1 == -1 || arrivalT[to].size() == 0 || startT[from].size() == 0 || from == to) {
			out << -1 << " " << -1 << endl;
			i++;
			continue;
		}
		
		out << u1 << " " << v1 << endl;
		i++;
	}
	out.close();
}

int main(int argc, char* argv[])
{
	srand(time(0));
	string infoName = argv[1];
	Info info(infoName);
	string queryFile = argv[2];
	info.createQuery(queryFile, atoi(argv[3]));
	return 0;
}
#endif
