#ifndef GRAPHT_H_HHWU
#define GRAPHT_H_HHWU

#include "graph.h"
#include "Timer.h"

#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;
const int infinity = 1e9;

class GraphT
{
public:
    GraphT(){}
    GraphT(const Graph& g);
    void add_edge(int u, int v);
    void make_unique(vector<int> & data);
    int getVertexID(int u, int offset, int startOrArrival);
    
    void initial_query(const char* filePath); // query file
    void initial_query();
    void initial_ds();
    void initial_ds_f();
    
    void run_earliest_arrival(); 
    void earliest_arrival(int source);
    void run_fastest(); 
    void fastest(int source);
    
    // for testing the correctness
    void run_earliest_arrival(const char* filePath); // output the result
    void earliest_arrival(int source, FILE * file); 
    void run_fastest(const char* filePath);
    void fastest(int source, FILE * file);
    
    void print_result(const int source, const vector<int>& t_time, FILE * file);
    void print_avg_time();
    void print_avg_time(const char* filePath1, const char* filePath2);
    void show_statistics(const char* filePath1, const char* filePath2);
    
    
    bool reachability(int from, int to, int t1, int t2);
    int getFrom(int from, int t1, int t2);
    int getTo(int to, int t1, int t2);
    int output(string s);
public:
    int V, E, gv;
    vector< vector< int > > adj;
    vector< vector<int> > startT, arrivalT;
    vector< int > curSize, sumSize;
    vector< int > timeId, originalId;
    vector< int > sources; 
    int t_start, t_end;
    double time_sum;
    vector <int> a_time, f_time;
    queue <int> myqueue;
    vector <bool> mark_in_Q; // whether the vertex is in queue
    vector <bool> visited; // Whether the vertex has been visited 
    
    int originalV;
};


int GraphT::output(string s)
{


	fstream out;
	string sdata = s + "_scc.txt";
	string sinfo = s + "_info.txt";
	cout << "writing to " << sdata << " ..." << endl;
	out.open(sdata.c_str(), ios::out);
	
	// for TF
	
	out << V << "\t" << E << endl;
	for (int i = 0; i < V; ++ i)
	{
		out << i << " " << adj[i].size() << " ";
		for (int j = 0; j < adj[i].size(); ++ j)
		{
			out << adj[i][j] << " ";	
		}
		out << endl;
	}
	out.close();
	
	// for IP 
	/*
	out << "graph_for_greach" << endl;
	out << V << endl;
	for (int i = 0; i < V; ++ i)
	{
		out << i <<": ";
		for (int j = 0; j < adj[i].size(); ++ j)
		{
			out << adj[i][j] << " ";
		}
		out <<"#" << endl;
	}
	out.close();
	*/
	
	
	cout << "writing to " << sinfo << " ..." << endl;
	out.open(sinfo.c_str(),ios::out);
	out << originalV << endl;
	for (int i = 0; i < originalV; ++ i)
	{
		out << i << " " << arrivalT[i].size()<< " ";	
		for (int j = 0; j < arrivalT[i].size(); ++ j) out << arrivalT[i][j] << " ";
		out << endl;
	}
	
	for (int i = 0; i < originalV; ++ i)
	{
		out << i <<  " " << startT[i].size()<< " ";
		for (int j = 0; j < startT[i].size(); ++ j) out << startT[i][j] << " ";
		out << endl;
	}
	
	out.close();
	
}

void GraphT::make_unique(vector<int> & data)
{
    sort(data.begin(),data.end());
    data.erase( unique(data.begin(),data.end()),data.end());
}


int GraphT::getVertexID(int u, int offset, int startOrArrival)
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

void GraphT::add_edge(int u,int v)
{
    E ++;
    adj[u].push_back(v);
}

GraphT::GraphT(const Graph& g)
{
    startT = vector< vector<int> >(g.V, vector<int>() );
    arrivalT = vector< vector<int> >(g.V, vector<int>());
    curSize = vector<int>(g.V, 0);
    sumSize = vector<int>(g.V, 0);

    for(int i = 0 ;i < g.V ; i ++)
    {
        for(int j = 0 ;j < g.adj[i].size() ; j ++)
        {
            int u = i;
            int v = g.adj[i][j].v;
            int w = g.adj[i][j].w;
            for(int k = 0 ; k < g.adj[i][j].interval.size(); k ++)
            {
                int t = g.adj[i][j].interval[k];
                // u -> v at t

                startT[u].push_back(t);
                arrivalT[v].push_back(t + w);
            }
        }
    }

    for(int i = 0 ; i < g.V ;i ++){
        make_unique(startT[i]);
        make_unique(arrivalT[i]);
        int cur = startT[i].size() + arrivalT[i].size();
        curSize[i] = cur;
        sumSize[i] = cur + (i > 0 ? sumSize[i-1] : 0);
    }
    
    for(int i=0; i< g.V; i++){
		for(int j=0; j<arrivalT[i].size(); j++){
			timeId.push_back(arrivalT[i][j]);
		}
		
		for(int j=0; j<startT[i].size(); j++){
			timeId.push_back(startT[i][j]);
		}
	}    
    
    originalV = g.V;
    V =  sumSize.back();
  	gv = g.V;
  	a_time.resize(gv);
  	f_time.resize(gv);
    adj.resize(V);
    originalId.resize(V);
    mark_in_Q.resize(V);
    visited.resize(V);
    E = 0;
    
    //Mapping the new vertex id to original vertex id
    
    for(int i=0; i< g.V; i++){
    	for(int j = 0 ; j < arrivalT[i].size(); j ++)
        {
            int u = getVertexID(i, j, 0);
            originalId[u]=i;
        }
        
        for(int j = 0 ; j < startT[i].size() ; j ++)
        {
            int u = getVertexID(i, j , 1);
            originalId[u]=i;
        }
    }
    
    // constructing edges;

    
    for(int i = 0 ; i < g.V; i ++)
    {
        // link T1
        for(int j = 1 ; j < arrivalT[i].size(); j ++)
        {
            int u21 = getVertexID(i, j - 1, 0);
            int u22 = getVertexID(i, j, 0);

            add_edge(u21,u22);
        }
        
		// link T2
        for(int j = 1 ; j < startT[i].size() ; j ++)
        {
            int u11 = getVertexID(i, j-1, 1);
            int u12 = getVertexID(i, j , 1);

            add_edge(u11,u12);
        }

        //link T1 to T2;
      
        int last = -1;        
        for(int j = int(arrivalT[i].size()) - 1; j >= 0 ; j --)
        {
            int u21 = getVertexID(i, j, 0);
            vector<int>::iterator it = lower_bound( startT[i].begin(),startT[i].end(), arrivalT[i][j] );
            if( it != startT[i].end() )
            {
                int offset = it - startT[i].begin();
                if(offset == last)
                    continue;
                last = offset;
                int u1 = getVertexID(i , offset , 1);
                add_edge(u21, u1);
            }
        }
        
        // add edge from temporal graph
        for(int j = 0 ;j < g.adj[i].size() ; j ++)
        {
            for(int k = 0 ;k < g.adj[i][j].interval.size() ; k ++)
            {
                int u = i, v = g.adj[i][j].v, w = g.adj[i][j].w, t = g.adj[i][j].interval[k];

                int offset_u = lower_bound( startT[u].begin(), startT[u].end(), t  ) - startT[u].begin();
                int offset_v = lower_bound( arrivalT[v].begin(), arrivalT[v].end(), t + w ) - arrivalT[v].begin();
                int uT = getVertexID(u, offset_u, 1);
                int vT = getVertexID(v, offset_v, 0);

                add_edge(uT,vT);

            }
        }
    }

}

void GraphT::initial_query()
{
	t_start = 0;
	t_end = infinity;
	
	int s;
	for(int i = 0 ;i < 100 ;i ++)
    {
    	s=rand()%gv;
        sources.push_back(s);
    }

}


void GraphT::initial_query(const char* filePath)
{
	t_start = 0;
	t_end = infinity;
	
	FILE* file = fopen(filePath,"r");
	int s, x;
	for(int i = 0 ;i < 10 ;i ++)
    {
    //	x=fscanf(file,"%d",&s);
    	int y;
    	x=fscanf(file,"%d%d",&s, &y);
        sources.push_back(s);
    }

}

void GraphT::initial_ds()
{
	while(!myqueue.empty()){
		myqueue.pop();
	}

	for(int i=0; i<gv; i++){
		a_time[i]= infinity;
	}
	
	for(int i=0; i<V; i++){
		visited[i]=false;
		mark_in_Q[i]=false;
	}
}

void GraphT::initial_ds_f()
{
	while(!myqueue.empty()){
		myqueue.pop();
	}
	
	for(int i=0; i<gv; i++){
		a_time[i]= infinity;
		f_time[i] = infinity;
	}
	
	for(int i=0; i<V; i++){
		visited[i]=false;
		mark_in_Q[i]=false;
	}

}

void GraphT::run_earliest_arrival()
{
	time_sum=0;
	
	for(int i = 0 ;i < sources.size() ;i ++)
    { 
    	initial_ds();
    	earliest_arrival(sources[i]);
    }
	
	print_avg_time();
}

int GraphT::getFrom(int from, int t1, int t2)
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
int GraphT::getTo(int to, int t1, int t2)
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
bool GraphT::reachability(int from, int to, int t1, int t2)
{
	initial_ds();

	Timer t;
	t.start();
	
	int u = getFrom(from, t1, t2);
	int v = getTo(to, t1, t2);
	//cout << "u: " << u << " v: " << v << endl;
	//cout << timeId[u] << " " << timeId[v] << endl;
	if (u == -1 || v == -1) return 0;
	queue<int> q;
	q.push(u);
	visited[u] = 1;
	while(!q.empty())
	{
		int tmp = q.front();
		q.pop();
		if (tmp == v)  return 1;
		//cout << tmp << ": \n";
		for (int i = 0; i < adj[tmp].size(); ++ i)
		{
			int next = adj[tmp][i];
			//cout << next << endl;
			if (!visited[next] && timeId[next] >=t1&&timeId[next]<=t2)
			{
				visited[next] = 1;
				q.push(next);
			}
		}
	}
	return 0;
}

void GraphT::earliest_arrival(int source)
{
	Timer t;
	t.start();
		
	a_time[source]=t_start;
	
	int u, v, uid;
	for(int j = 0 ; j < startT[source].size(); j ++) {
        u = getVertexID(source, j, 1);
        
        if(timeId[u]>=t_start && timeId[u]<=t_end)
	        myqueue.push(u);
    }
    
	while(!myqueue.empty()){
		u=myqueue.front();
		myqueue.pop();
		visited[u]=true;
		mark_in_Q[u]=false;

		uid=originalId[u];
		if(timeId[u]<a_time[uid]){
			a_time[uid]=timeId[u];
		}

		for(int i=0; i<adj[u].size(); i++){
			v = adj[u][i];
			if(!visited[v] && !mark_in_Q[v]){
				if (timeId[v]>=t_start && timeId[v]<=t_end){
					myqueue.push(v);
					mark_in_Q[v]=true;
				}
			}
		}
	}
	
	t.stop();
	time_sum += t.GetRuntime();
}

void GraphT::run_earliest_arrival(const char* filePath)
{
	time_sum=0;
	
	FILE* file = fopen(filePath,"w");
	
	for(int i = 0 ;i < sources.size() ;i ++)
    { 
    	initial_ds();
    	earliest_arrival(sources[i], file);
    }
    
    fclose(file);
    
    print_avg_time();

}


void GraphT::earliest_arrival(int source, FILE * file)
{
	Timer t;
	t.start();
		
	a_time[source]=t_start;
	
	int u, v, uid;
	for(int j = 0 ; j < startT[source].size(); j ++) {
        u = getVertexID(source, j, 1);
        
        if(timeId[u]>=t_start && timeId[u]<=t_end)
	        myqueue.push(u);
    }
    
	while(!myqueue.empty()){
		u=myqueue.front();
		myqueue.pop();
		visited[u]=true;
		mark_in_Q[u]=false;

		uid=originalId[u];
		if(timeId[u]<a_time[uid]){
			a_time[uid]=timeId[u];
		}

		for(int i=0; i<adj[u].size(); i++){
			v = adj[u][i];
			if(!visited[v] && !mark_in_Q[v]){
				if (timeId[v]>=t_start && timeId[v]<=t_end){
					myqueue.push(v);
					mark_in_Q[v]=true;
				}
			}
		}
	}
	
	t.stop();
	time_sum += t.GetRuntime();
	
	print_result(source, a_time, file);
}

void GraphT::run_fastest()
{
	time_sum=0;
	
	for(int i = 0 ;i < sources.size() ;i ++)
    { 
    	initial_ds_f();
    	fastest(sources[i]);
    }
    
    print_avg_time();

}

void GraphT::fastest(int source)
{
	Timer t;
	t.start();
		
	f_time[source]=0;	
	a_time[source]=t_start;	
	
	int u, v, uid;
	for(int j = startT[source].size()-1 ; j >= 0; j --) {
        int x = getVertexID(source, j, 1);
        
        if(timeId[x]>=t_start && timeId[x]<=t_end){
	        myqueue.push(x);
	        int c_t=timeId[x];
	    
			while(!myqueue.empty()){
				u=myqueue.front();
				myqueue.pop();
				visited[u]=true;
				mark_in_Q[u]=false;

				uid=originalId[u];
				if(timeId[u]<a_time[uid]){
					a_time[uid]=timeId[u];
					
					if(f_time[uid]>a_time[uid]-c_t)
						f_time[uid]=a_time[uid]-c_t;
				}

				for(int i=0; i<adj[u].size(); i++){
					v = adj[u][i];
					if(!visited[v] && !mark_in_Q[v]){
						if (timeId[v]>=t_start && timeId[v]<=t_end){
							myqueue.push(v);
							mark_in_Q[v]=true;
						}
					}
				}
			}
	    
	   	}
    }
	
	t.stop();
	time_sum += t.GetRuntime();
}


void GraphT::run_fastest(const char* filePath)
{
	time_sum=0;
	
	FILE* file = fopen(filePath,"w");
	
	for(int i = 0 ;i < sources.size() ;i ++)
    { 
    	initial_ds_f();
    	fastest(sources[i], file);
    }
    
    fclose(file);
    
    print_avg_time();

}

void GraphT::fastest(int source, FILE * file)
{
	Timer t;
	t.start();
		
	f_time[source]=0;
	a_time[source]=t_start;	
	
	int u, v, uid;
	for(int j = startT[source].size()-1 ; j >= 0; j --) {
        int x = getVertexID(source, j, 1);
        
        if(timeId[x]>=t_start && timeId[x]<=t_end){
	        myqueue.push(x);
	        int c_t=timeId[x];
	    
			while(!myqueue.empty()){
				u=myqueue.front();
				myqueue.pop();
				visited[u]=true;
				mark_in_Q[u]=false;

				uid=originalId[u];
				if(timeId[u]<a_time[uid]){
					a_time[uid]=timeId[u];
					
					if(f_time[uid]>a_time[uid]-c_t)
						f_time[uid]=a_time[uid]-c_t;
				}

				for(int i=0; i<adj[u].size(); i++){
					v = adj[u][i];
					if(!visited[v] && !mark_in_Q[v]){
						if (timeId[v]>=t_start && timeId[v]<=t_end){
							myqueue.push(v);
							mark_in_Q[v]=true;
						}
					}
				}
			}
	    
	   	}
    }
	
	t.stop();
	time_sum += t.GetRuntime();
	
	print_result(source, f_time, file);
}


void GraphT::print_result(const int source, const vector<int>& t_time, FILE * file)
{
	
	for(int i = 0 ;i < gv ;i ++)
    { 
    	if(t_time[i]!=infinity){
    		fprintf(file, "%d %d %d %d %d\n", source, i, t_start, t_end, t_time[i]);
    	}
    }

}

void GraphT::print_avg_time()
{
	cout<<"Average time: " << time_sum/100 <<endl;
}

void GraphT::print_avg_time(const char* filePath1, const char* filePath2)
{
	FILE* file = fopen(filePath2,"a");
	
	fprintf(file, "%s\t%f\n", filePath1, time_sum/10);;
	fclose(file);
}

void GraphT::show_statistics(const char* filePath1, const char* filePath2)
{
    FILE* file = fopen(filePath2,"a");
	
	fprintf(file, "%s\t%d\t%d\n", filePath1, V, E);
	fclose(file);
}

#endif
