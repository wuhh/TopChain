#ifndef _INFO
#define _INFO

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

class Query
{
	public:
	int u, v;
};

class Info
{
	public:
	int getVertexID(int u, int offset, int startOrArrival);
	int getFrom(int from, int t1, int t2);
    int getTo(int to, int t1, int t2);
   	int getFrom2(int from, int t1, int t2);
    int getTo2(int to, int t1, int t2);
    
    int getFrom3(int from);
    int getTo3(int to);
    
    Info(string s);
    Query query(int from, int to, int t1, int t2);
    void t();
    void createQuery(string s, int query_number);
    
    private:
    int v;
    vector< vector<int> > startT, arrivalT;
    vector< int > curSize, sumSize;
    vector< int > timeId, originalId;
};


#endif
