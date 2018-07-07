#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "graph.h"
#include "graphT.h"
using namespace std;

int main(int argc, char* argv[]){

	
	//char* option = argv[1];
	
	string s1 = argv[1];
	Graph g(argv[1]);
	GraphT gT(g);


//	gT.initial_query(argv[3]);
//	gT.initial_query();
//	gT.run_earliest_arrival();
//	gT.run_fastest(argv[2]);
//	gT.run_earliest_arrival(argv[2]);
	

	//cout << gT.reachability(0, 3, 5, 9) << endl;
	
	string s2 = argv[2];
	s2 += s1;
	gT.output(s2);

/*
	if(!strcmp(option,"earliest"))
    {
        gT.run_earliest_arrival();
    }
    else if(!strcmp(option,"fastest"))
    {
        gT.run_fastest();
    }
		
	string tmp= argv[2];
//	tmp=tmp.substr(tmp.find_last_of('/'),tmp.size());
//	gT.print_avg_time(tmp.c_str(), argv[3]);
	//gT.print_avg_time(tmp.c_str(), argv[4]);


	Graph g(argv[1]);
	GraphT gT(g);
	gT.show_statistics(argv[1],argv[2]);
*/	




	//cout<<"hello world!!!" <<endl;
	cout << "transform done! " << endl;
	
	return 0;
}



