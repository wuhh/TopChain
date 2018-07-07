#ifndef GRAPH_H_HHWU
#define GRAPH_H_HHWU


#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <set>
#include <queue>
#include <iostream>

using namespace std;

class Edge
{
public:
    void init(int v,int w,int size)
    {
        this->v = v;
        this->w = w;
        this->interval.resize(size);
    }
public:
    int v;
    int w;
    vector<int> interval;
};

class Graph
{
public:
    Graph() {}
    Graph(const char* filePath);

public:
    vector< vector<Edge> > adj;
    int V, static_E, dynamic_E;

};



Graph::Graph(const char* filePath)
{
    FILE* file = fopen(filePath,"r");
    int x;

    x=fscanf(file, "%d %d %d",&V, &static_E, &dynamic_E);

    adj.resize(V);

    for(int i = 0; i < V; i ++)
    {
        int u, num_of_v;
        x=fscanf(file,"%d %d",&u,&num_of_v);
        adj[u].resize(num_of_v);

        for(int j = 0; j < num_of_v ; j ++)
        {
            int v, num_of_t, weight_of_v;
            x=fscanf(file,"%d %d %d",&v,&num_of_t,&weight_of_v);
            adj[u][j].init(v,weight_of_v,num_of_t);
            for(int k = 0 ; k < num_of_t; k ++)
            {
                int t;
                x=fscanf(file, "%d",&t);
                adj[u][j].interval[k] = t;
            }
        }
    }

    fclose(file);
}


#endif

