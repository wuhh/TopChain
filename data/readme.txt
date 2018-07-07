In this directory, we use some simple programs to transform a temporal graph into scc, together with some infomation and generate query

### example
toy:
5 5 7
0 2 1 2 1 1 2 2 2 1 2 4
1 1 3 1 1 5
2 2 3 1 1 6 4 1 1 7
3 0
4 0

This graph is the same in Figure 2 in "Path Problems in Temporal Graphs, VLDB 2014"

This is a temporal graph, the first line contains three integer: the number of vertices N, the number of static edges E_static, the number of dynamic edges, E_dynamic.
There will be N lines follows. Each line represent a vertice and its out-going edges.
Each line starts with a vertice id, then the number of neighbors it has. For each neighbors it has, it first list it's id, then the dynamic edges number between them, the duration of these edges, and then lists each of these temporal edges.


### transform
We transform the temporal graph into an SCC use the algorithm preseneted in "Path Problems in Temporal Graphs, VLDB 2014".
The output will be two files, toy_scc.txt, toy_info.txt.
toy_scc.txt is the SCC, which is in standard adjcency list format:
toy_scc.txt:
13	15
0 2 1 3 
1 3 2 4 6 
2 1 7 
3 1 4 
4 1 5 
5 1 10 
6 1 7 
7 1 8 
8 2 9 11 
9 1 12 
10 1 11 
11 0 
12 0 
toy_info.txt represents the time information in temporal graph. 
The first line is the number of vertices N in temporal graph, it follows 2*N lines. 
The first N lines represent the in-edge time(arrival time) for each vertices. 
First, each line lists its id, then the number of in-edge, then lists each of the arrival time.
The remainin N lines represent the out-edge time(start time) for each vertices.

toy_info.txt
5
0 0 
1 2 2 3 
2 2 3 5 
3 2 6 7 
4 1 8 
0 3 1 2 4 
1 1 5 
2 2 6 7 
3 0 
4 0 

### generate query
We randomly select two vertices in original graph. Some querys are not legal so we mark them as "-1 -1"

### tools source codes
All the source codes are in data/tools/. you can use the following commands to compile them:
make tran
make info
