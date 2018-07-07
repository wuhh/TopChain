#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <queue>
#include <string>
#include <cstring>
#include <assert.h>
#include <set>
#include <stack>
#include "Timer.h"
using namespace std;
vector<vector<int> > v;
vector<vector<int> > vr;
int n, m;
vector<int> ind;
vector<int> outd;
vector<int> Lup;
vector<int> Ldown;

class ReachIndex{
public:
	//uint16_t layerup;
	//uint16_t layerdown;
	uint8_t OutLimit;
	uint8_t InLimit;
	uint8_t HLimit;
	pair<int, int> * Label;
	uint16_t Lup;
	uint16_t Ldown;
	int flabel1;
	int flabel2;
	
	ReachIndex(){
		Label=NULL;
	}

	~ReachIndex(){
		if(Label!=NULL)
			delete[] Label;
	}
};


struct DEG
{
	int d;
	int index;
};
vector<DEG> deg;
bool cmp(DEG d1, DEG d2)
{
	return d1.d > d2.d;
}

vector<vector<pair<int, int> > > Lin;
vector<vector<pair<int, int> > > Lout;


int k = 5;

Timer timer;

vector<int> rank_order;
vector<int> reverserank_order;

void readGraph(string s)
{
	fstream in;
	in.open(s.c_str(), ios::in);
	in >> n >> m;
	v.resize(n);
	vr.resize(n);
	ind.resize(n);
	outd.resize(n);
	
	int u;int size;int to;
	while(in >> u)
	{
		in >> size;
		for (int i = 0; i < size; ++ i)
		{
			in >> to;
			v[u].push_back(to);
			vr[to].push_back(u);
			outd[u]++;
			ind[to]++;
		} 
	}
	in.close();
	
	cout << "load done." << endl;
}

int chainNum;
vector<int> vis;
vector<int> toChain;
vector<int> toPos;
vector<vector<int> > toV;
vector< vector<int> > startT, arrivalT;
vector< int > curSize, sumSize;
vector< int > timeId, originalId;

int getVertexID(int u, int offset, int startOrArrival)
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

vector <int> inOut;
vector <int> pNext;
void readChainInfo(string s)
{
	inOut.resize(n);
	pNext.resize(n);
	
	fstream in;
	in.open(s.c_str(), ios::in);
	in >> chainNum;
	startT = vector< vector<int> >(chainNum, vector<int>() );
    arrivalT = vector< vector<int> >(chainNum, vector<int>());
    curSize = vector<int>(chainNum, 0);
    sumSize = vector<int>(chainNum, 0);
    
    originalId.resize(n);
    
    toV.resize(n);
	toChain.resize(n);
	toPos.resize(n);
    
	int u;
	int size;
	int tmp;
	for (int i = 0; i < chainNum; ++ i)
	{
		in >> u >> size;
		for (int j = 0; j < size; ++ j)
		{
			in >> tmp;
			arrivalT[i].push_back(tmp);
		}
	}
	for (int i = 0; i < chainNum; ++ i)
	{
		in >> u >> size;
		for (int j = 0; j < size; ++ j)
		{
			in >> tmp;
			startT[i].push_back(tmp);
		}
	}
	in.close();
	//cout << "chain info load done" << endl;
}
void findChain()
{	
	for(int i = 0 ; i < chainNum; i ++){
        int cur = startT[i].size() + arrivalT[i].size();
        curSize[i] = cur;
        sumSize[i] = cur + (i > 0 ? sumSize[i-1] : 0);
    }
	
	
    //find chain and pos
    for (int i = 0; i < chainNum; ++ i)
    {
    	int p1 = 0;
    	int p2 = 0;
    	int j = 0;
    	while(j < arrivalT[i].size()+startT[i].size() )
    	{
    		if (p1 == arrivalT[i].size())
    		{
    			int u = getVertexID(i, p2, 1);
    			originalId[u] = i;
    			toPos[u] = startT[i][p2];//toV[i].size();
    			//toV[i].push_back(u);
    			p2++;	
    		}
    		else if (p2 == startT[i].size())
    		{
    			int u = getVertexID(i, p1, 0);
    			originalId[u] = i;
    			toPos[u] = arrivalT[i][p1];//toV[i].size();
    			//toV[i].push_back(u);
    			p1++;
    		}
    		else if (arrivalT[i][p1] <= startT[i][p2])
    		{
    			int u = getVertexID(i, p1, 0);
    			originalId[u] = i;
    			toPos[u] = arrivalT[i][p1];//toV[i].size();
    			//toV[i].push_back(u);
    			p1++;
    		}
    		else 
    		{
    			int u = getVertexID(i, p2, 1);
    			originalId[u] = i;
    			toPos[u] = startT[i][p2]; //toV[i].size();
    			//toV[i].push_back(u);
    			p2++;
    		}
    		
    		j++;
    	}
    	
    }
    
    //set inOut and pNext for jump
    //no jump
    /*
    for (int i = 0; i < chainNum; ++ i)
    {      
        for(int j = int(arrivalT[i].size()) - 1; j >= 0 ; j --)
        {
            int u21 = getVertexID(i, j, 0);
            inOut[u21] = 1;
            vector<int>::iterator it = lower_bound( startT[i].begin(),startT[i].end(), arrivalT[i][j] );
            if( it != startT[i].end() )
            {
                int offset = it - startT[i].begin();
                int u1 = getVertexID(i , offset , 1);
                pNext[u21] = u1;
            }
            else
            {
            	pNext[u21] = -1;
            }
        }
        
        for (int j = 0; j < startT[i].size(); ++ j)
        {
        	int u22 = getVertexID(i,j,1);
        	inOut[u22] = 0;
        }
    }
    */
    
    /*
    for (int i = 0; i < chainNum; ++ i)
    {    
    	int p = startT[i].size()-1;  
        for(int j = int(arrivalT[i].size()) - 1; j >= 0 ; j --)
        {
            int u21 = getVertexID(i, j, 0);
            inOut[u21] = 1;
            
            while(p>0&&startT[i][p-1]>=arrivalT[i][j]) {p--;}
            
            if(p>=0 && startT[i][p] >= arrivalT[i][j])
            {
                int u1 = getVertexID(i , p , 1);
                pNext[u21] = u1;
            }
            else
            {
            	pNext[u21] = -1;
            }
        }
        
        for (int j = 0; j < startT[i].size(); ++ j)
        {
        	int u22 = getVertexID(i,j,1);
        	inOut[u22] = 0;
        }
    }
    */
    
	
	//assign degree
    deg.resize(chainNum);
    for (int i = 0; i < chainNum; ++ i)
    {
    	deg[i].d = startT[i].size() + arrivalT[i].size();
    	deg[i].index = i;
    }
}


//feline label

vector<int> flabel1;
vector<int> flabel2;
vector<int> topoOrder;
vector<int> rtopoOrder;
int maxDown = 0;
int maxUp = 0;
void feline()
{
	//cout << "feline begin" << endl;
	vector<int> inds1;
	vector<int> inds2;
	stack<int> stack1;
    stack<int> stack2;
    stack<int> stack3;
    inds1.resize(n);
    inds2.resize(n);
    flabel1.resize(n);
    flabel2.resize(n);
    topoOrder.resize(n);
    rtopoOrder.resize(n);
  	Ldown.resize(n);
	Lup.resize(n);
    //copy ind
    for (int i = 0; i < n; ++ i)
    {
		inds1[i]=inds2[i]=ind[i];	
    }
    queue<int>q;
    for (int i = n-1; i >= 0; --i)
    {
    	if (ind[i] == 0) 
    	{	
    		stack1.push(i);
    		Ldown[i] = 0;
    	}
    	
    	if (outd[i] == 0)
    	{
    		stack3.push(i);
    		Lup[i] = 0;
    	}
    	
    	//if (ind[i] == 0) q.push(i);
    }
    
    Timer t1;
    t1.start();
    int count1 = 0;
    while(!stack1.empty())
    {
    	//cout << "!!" << endl;
    	int tmp = stack1.top();
    	stack1.pop();
    	topoOrder[count1] = tmp; 
    	flabel1[tmp] = count1++;
    	for (int i = v[tmp].size()-1; i >= 0; --i)
    	{
    		inds1[v[tmp][i]]--;
    		if (inds1[v[tmp][i]] == 0)
    		{
    			stack1.push(v[tmp][i]);
    			//Ldown[v[tmp][i]] = Ldown[tmp]+1;
    			//maxDown = max(Ldown[v[tmp][i]], maxDown);
    		}
    	}
    }
    for (int i = 0; i < n; ++ i)
    {
    	int tmp = topoOrder[i];
    	if (vr[tmp].size() > 0)
    	{
    		int maxi = Ldown[vr[tmp][0]];
    		for (int j = 0; j < vr[tmp].size(); ++j)
    		{
    			if (Ldown[vr[tmp][j]] > maxi) maxi = Ldown[vr[tmp][j]];
    			
    		}
    		Ldown[tmp] = maxi+1	;
    		maxDown = max(maxDown, Ldown[tmp]);
    	}
    }
    
    //find reverse topo
    
    count1 = 0;
    while(!stack3.empty())
    {
    	int tmp = stack3.top();stack3.pop();
    	rtopoOrder[count1++] = tmp;
  		for (int i = 0; i < vr[tmp].size(); ++ i)
  		{
  			outd[vr[tmp][i]]--;
  			if (outd[vr[tmp][i]] == 0)
  			{
  				stack3.push(vr[tmp][i]);
  			}
  		}
    }
    for (int i = 0; i < n; ++ i)
    {
    	int tmp = rtopoOrder[i];
    	if(v[tmp].size() > 0)
    	{
    		int maxi = Lup	[v[tmp][0]];
    		for (int j = 0; j < v[tmp].size(); ++ j)
    		{
    			if (Lup[v[tmp][j]] > maxi) maxi = Lup[v[tmp][j]];
    		}
    		Lup[tmp] = maxi+1;
    		maxUp = max(maxUp, Lup[tmp]);
    	}
    }
    
    
    //assert(count1 == n);
    t1.stop();
    //cout << t1.GetRuntime()*1000 << endl;
    
    //--------------------
    /*
    Timer t1;
    t1.start();
    int count1 = 0;
    while(!q.empty())
    {
    	//cout << "!!" << endl;
    	int tmp = q.front();
    	q.pop();
    	flabel1[tmp] = count1++;
    	for (int i = v[tmp].size()-1; i >= 0; --i)
    	{
    		inds1[v[tmp][i]]--;
    		if (inds1[v[tmp][i]] == 0)
    		q.push(v[tmp][i]);
    	}
    }
    assert(count1 == n);
    t1.stop();
    cout << t1.GetRuntime()*1000 << endl;
    */
    //-------------------
    
    count1 = 0;
    for (int i = 0; i < n; ++ i)
    {
    	if (ind[i] == 0) stack2.push(i);
    }
    while(!stack2.empty())
    {
    	int tmp = stack2.top(); stack2.pop();
    	flabel2[tmp] = count1++;
    	for(int i = 0; i < v[tmp].size(); ++ i)
    	{
    		inds2[v[tmp][i]]--;
    		if (inds2[v[tmp][i]] == 0) stack2.push(v[tmp][i]);
    	}
    }
    //assert(count1 == n);
    //cout << "feline finished" << endl;
}

void handle()
{


	Lin.resize(n);
	Lout.resize(n);
	queue<int> q;
	queue<int> q1;
	int count1 = 0;

	
	//find chain
	
	for (int i = 0; i < n; ++ i) toChain[i]=originalId[i];
	cout <<"chain Num : "<< chainNum << endl;
	
	/*
	int countt = 0;
	for (int i = 0; i < chainNum; ++ i)
	{
		countt += toV[i].size();
	}
	assert(countt == n);
	*/
	
	//-------------------------------------------------------------
	//assign rank_order
	
	rank_order.resize(chainNum);

	//rank by degree
	sort(deg.begin(), deg.end(), cmp);
	for (int i = 0; i < chainNum; ++ i)
	{
		//order by degree
		rank_order[deg[i].index] = i;
		
		//order randomly
	//	rank_order[i] = i;
	}
	/*
	for (int i = chainNum-1; i > 0; --i)
	{
		int tmp = rand()%(i+1);
		swap(rank_order[i], rank_order[tmp]);
	}
	*/
	
	//------------------
	
	//topological sort and find Lin
	
	timer.stop();
	//cout << "!!!begin time : " << timer.GetRuntime()*1000 << endl;
	
	
	
	
	for (int i = 0; i < n; ++ i)
	{
		int tmp = topoOrder[i];
		//IP construct
		vector<pair<int,int> > mid[2];
		Lin[tmp].push_back(make_pair(rank_order[toChain[tmp]], toPos[tmp]));
		
		mid[0].resize(k);
		mid[1].resize(k);
		int cur = 0;
		int curSize = 0;
		int linTmp = 1;
		int linTmpSize = Lin[tmp].size();
		mid[linTmp] = Lin[tmp];
		mid[linTmp].resize(k);
		for (int j = 0; j < vr[tmp].size(); ++ j)
		{	
			int nin = vr[tmp][j];
			//merge tmp nin
			int p1 = 0; int p2 = 0;
			curSize = 0;
			//mid[cur].resize(k);
			while(p1 < linTmpSize || p2 < Lin[nin].size())
			{
				//cout << "p1 p2: " << p1 << " " << p2 << endl;
				if (curSize == k) break;
				
				if (p1 == linTmpSize) {
					mid[cur][curSize++] = Lin[nin][p2];
					p2++;
					continue;
					
				}
				if (p2 == Lin[nin].size()) {
					mid[cur][curSize++] = mid[linTmp][p1];
					p1++;
					continue; 
					
				}
				
				if (mid[linTmp][p1].first < Lin[nin][p2].first) {mid[cur][curSize++] = mid[linTmp][p1]; p1++;}
				else if (mid[linTmp][p1].first > Lin[nin][p2].first ) {mid[cur][curSize++] = Lin[nin][p2]; p2++;}
				else {mid[cur][curSize++] = max(mid[linTmp][p1], Lin[nin][p2]); p1++;p2++;}
			}
			//Lin[tmp] = mid;
			cur = 1-cur;
			linTmp = 1-linTmp;
			//mid[linTmp].resize(curSize);
			linTmpSize = curSize;
			
		}
		Lin[tmp] = mid[linTmp];	
		Lin[tmp].resize(linTmpSize);
	}
	
	/*
	for (int i = 0; i < n; ++ i)
	{
		int tmp = topoOrder[i];
		//IP construct
		vector<pair<int,int> > mid[2];
		Lin[tmp].push_back(make_pair(rank_order[toChain[tmp]], toPos[tmp]));
		
		mid[0].clear();
		mid[1].clear();
		int pm = 0;
		int pmr = 1;
		mid[pmr] = Lin[tmp];
		for (int j = 0; j < vr[tmp].size(); ++ j)
		{
			//vector<pair<int,int> >().swap(mid[0]);
			//vector<pair<int,int> >().swap(mid[1]);
			int nin = vr[tmp][j];
			//merge tmp nin
			int p1 = 0; int p2 = 0;
			//vector<pair<int,int> >().swap(mid[pm]);
			mid[pm].resize(0);
			while(p1 < mid[pmr].size() || p2 < Lin[nin].size())
			{
				if (mid[pm].size() == k) break;
				
				if (p1 == mid[pmr].size()) {
					mid[pm].push_back(Lin[nin][p2]);p2++;
					continue;
					
					while(p2 < Lin[nin].size() && mid[pm].size() < k)
					{mid[pm].push_back(Lin[nin][p2]);p2++;}
					continue;
				}
				if (p2 == Lin[nin].size()) {
					mid[pm].push_back(mid[pmr][p1]);p1++;
					continue;
					
					while(p1 < mid[pmr].size() && mid[pm].size() < k)
					{mid[pm].push_back(mid[pmr][p1]);p1++;}
					continue;
				}
				
				if (mid[pmr][p1].first < Lin[nin][p2].first) {mid[pm].push_back(mid[pmr][p1]); p1++;}
				else if (mid[pmr][p1].first > Lin[nin][p2].first ) {mid[pm].push_back(Lin[nin][p2]); p2++;}
				else {mid[pm].push_back( max(mid[pmr][p1], Lin[nin][p2]) );p1++;p2++;}
			}
			//Lin[tmp] = mid;
			pm = 1-pm;
			pmr = 1-pmr;
		}
		Lin[tmp] = mid[pmr];	
	}
	*/
	//test==========================
	/*
	for (int i = 0; i < n; ++ i)
	{
		int tmp = topoOrder[i];
		//IP construct
		
		
		Lin[tmp].push_back(make_pair(rank_order[toChain[tmp]], toPos[tmp]));
		vector<pair<int,int> > mid;
		mid.clear();
		for (int j = 0; j < vr[tmp].size(); ++ j)
		{
			vector<pair<int,int> >().swap(mid);
			int nin = vr[tmp][j];
			//merge tmp nin
			int p1 = 0; int p2 = 0;
			while(p1 < Lin[tmp].size() || p2 < Lin[nin].size())
			{
				if (mid.size() == k) break;
				
				if (p1 == Lin[tmp].size()) {mid.push_back(Lin[nin][p2]);p2++;continue;}
				if (p2 == Lin[nin].size()) {mid.push_back(Lin[tmp][p1]);p1++;continue;}
				
				if (Lin[tmp][p1].first < Lin[nin][p2].first) {mid.push_back(Lin[tmp][p1]); p1++;}
				else if (Lin[tmp][p1].first > Lin[nin][p2].first ) {mid.push_back(Lin[nin][p2]); p2++;}
				else {mid.push_back( max(Lin[tmp][p1], Lin[nin][p2]) );p1++;p2++;}
			}
			Lin[tmp] = mid;
		}
	}
	*/
	//test==========================
	timer.stop();
	//cout << "!!!half finish time : " << timer.GetRuntime()*1000 << endl;
	//reverse topological sort and find Lout
	for (int i = 0; i < n; ++ i)
	{
		int tmp = rtopoOrder[i];
		//IP construct
		vector<pair<int,int> > mid[2];
		Lout[tmp].push_back(make_pair(rank_order[toChain[tmp]], toPos[tmp]));
		
		mid[0].resize(k);
		mid[1].resize(k);
		int cur = 0;
		int curSize = 0;
		int loutTmp = 1;
		int loutTmpSize = Lout[tmp].size();
		mid[loutTmp] = Lout[tmp];
		mid[loutTmp].resize(k);
		for (int j = 0; j < v[tmp].size(); ++ j)
		{	
			int nin = v[tmp][j];
			//merge tmp nin
			int p1 = 0; int p2 = 0;
			curSize = 0;
			//mid[cur].resize(k);
			while(p1 < loutTmpSize || p2 < Lout[nin].size())
			{
				//cout << "p1 p2: " << p1 << " " << p2 << endl;
				if (curSize == k) break;
				
				if (p1 == loutTmpSize) {
					mid[cur][curSize++] = Lout[nin][p2];
					p2++;
					continue;
				}
				if (p2 == Lout[nin].size()) {
					mid[cur][curSize++] = mid[loutTmp][p1];
					p1++;
					continue; 					
				}
				
				if (mid[loutTmp][p1].first < Lout[nin][p2].first) {mid[cur][curSize++] = mid[loutTmp][p1]; p1++;}
				else if (mid[loutTmp][p1].first > Lout[nin][p2].first ) {mid[cur][curSize++] = Lout[nin][p2]; p2++;}
				else {mid[cur][curSize++] = min(mid[loutTmp][p1], Lout[nin][p2]); p1++;p2++;}
			}
			//Lout[tmp] = mid;
			cur = 1-cur;
			loutTmp = 1-loutTmp;
			//mid[loutTmp].resize(curSize);
			loutTmpSize = curSize;
			
		}
		Lout[tmp] = mid[loutTmp];	
		Lout[tmp].resize(loutTmpSize);
	}
	
	
	/*
	for (int i = 0; i < n; ++ i)
	{
		int tmp = rtopoOrder[i];	
		//IP construct
		
		Lout[tmp].push_back(make_pair(rank_order[toChain[tmp]], toPos[tmp]));
		vector<pair<int,int> > mid;
		mid.clear();
		for (int j = 0; j < v[tmp].size(); ++ j)
		{
			//vector<pair<int,int> >().swap(mid);
			mid.resize(0);
			int nin = v[tmp][j];
			//merge tmp nin
			int p1 = 0; int p2 = 0;
			while(p1 < Lout[tmp].size() || p2 < Lout[nin].size())
			{
				if (mid.size() == k) break;
				
				if (p1 == Lout[tmp].size()) {mid.push_back(Lout[nin][p2]);p2++;continue;}
				if (p2 == Lout[nin].size()) {mid.push_back(Lout[tmp][p1]);p1++;continue;}
				
				if (Lout[tmp][p1].first < Lout[nin][p2].first) {mid.push_back(Lout[tmp][p1]); p1++;}
				else if (Lout[tmp][p1].first > Lout[nin][p2].first ) {mid.push_back(Lout[nin][p2]); p2++;}
				else {mid.push_back( min(Lout[tmp][p1], Lout[nin][p2]) );p1++;p2++;}
			}
			Lout[tmp] = mid;
		}
		
		
	}
	*/
	timer.stop();
	//cout << "!!!ip finish time : " << timer.GetRuntime()*1000 << endl;
	
	cout << "label done. " << endl;
	//assert(count1 == n);
	
	//cout << "maxUp : " << maxUp << endl;
	//cout << "maxDown : " << maxDown << endl;



	vector<int>().swap(ind);
	vector<int>().swap(outd);

}

//copy index
vector<ReachIndex> reachindex;
void copyIndex()
{
	reachindex.resize(n);
	for (int i = 0; i < n; ++ i)
	{
		reachindex[i].Lup = Lup[i];
		reachindex[i].Ldown = Ldown[i];
		reachindex[i].flabel1 = flabel1[i];
		reachindex[i].flabel2 = flabel2[i];
		
		reachindex[i].OutLimit= Lout[i].size();
		reachindex[i].InLimit= Lin[i].size()+reachindex[i].OutLimit;
		//reachindex[i].HLimit=hv.size()+reachindex[i].InLimit;
		reachindex[i].Label = new pair<int, int>[reachindex[i].InLimit];
		
		for (int j = 0; j < Lout[i].size(); ++ j) reachindex[i].Label[j] = Lout[i][j];
		for (int j = 0; j < Lin[i].size(); ++ j) reachindex[i].Label[reachindex[i].OutLimit+j] = Lin[i][j];
	} 
	
}
//--------------------------label------------------------


bool intersect(const int u1, const int u2)
{	
	int p1 = 0;
	int p2 = reachindex[u2].OutLimit;
	while(p1 < reachindex[u1].OutLimit && p2 < reachindex[u2].InLimit)
	{
		if (reachindex[u1].Label[p1].first == reachindex[u2].Label[p2].first && reachindex[u1].Label[p1].second <= reachindex[u2].Label[p2].second) return 1;
		else if (reachindex[u1].Label[p1].first == reachindex[u2].Label[p2].first) {p1++;p2++;}
		else if (reachindex[u1].Label[p1].first < reachindex[u2].Label[p2].first) p1 ++;
		else p2++;
	}
	return 0;
}

//----------------------query----------------------------
int mark = 0;
int *flag;

bool IPtest(const int u, const int v)
{
	
	int it1=0, it2=0;
	
	if(reachindex[u].Label[ reachindex[u].OutLimit-1 ].first > reachindex[v].Label[0].first ){
		while(it1!=reachindex[u].OutLimit && it2!=reachindex[v].OutLimit){
			if(reachindex[u].Label[it1].first == reachindex[v].Label[it2].first){
				if (reachindex[u].Label[it1].second > reachindex[v].Label[it2].second) return true;
				it1++;
				it2++;
			}else if(reachindex[u].Label[it1].first < reachindex[v].Label[it2].first){
				it1++;
			}else
				return true;
		}
	}
	
	
	if(reachindex[v].Label[ reachindex[v].InLimit-1 ].first <= reachindex[u].Label[ reachindex[u].OutLimit ].first)
		return false;

	it1=reachindex[v].OutLimit;
	it2=reachindex[u].OutLimit;

	while(it1!= reachindex[v].InLimit && it2!=reachindex[u].InLimit){
		if(reachindex[v].Label[it1].first == reachindex[u].Label[it2].first){
			if (reachindex[v].Label[it1].second < reachindex[u].Label[it2].second) return true;
			it1++;
			it2++;
		}else if(reachindex[v].Label[it1].first < reachindex[u].Label[it2].first){
			it1++;
		}else
			return true;
	}

	return false;
	
}

int insidetest(int from, int to)
{
	//new version
	int l = 0; int r = reachindex[from].OutLimit-1;
	int m;
	while(l <= r)
	{
		m = (l+r)/2;
		if (reachindex[from].Label[m].first == rank_order[toChain[to]]) 
		{
			if(reachindex[from].Label[m].second <= toPos[to]){
				return 1;
			}
			return 2;
		}
		else if (reachindex[from].Label[m].first < rank_order[toChain[to]]) l = m+1;
		else r = m-1;
	}
	return 0;
}

int countQuery = 0;
bool query(int from, int to)
{
	countQuery ++;
	flag[from] = mark;
	
	if (reachindex[from].flabel1 > reachindex[to].flabel1 || reachindex[from].flabel2 > reachindex[to].flabel2) return 0;
	if (IPtest(from, to)) return 0;
	
	//intersect
	if (intersect(from, to)) return 1;

	for (int i = 0; i < v[from].size(); ++ i)
	{
		int w = v[from][i];
		if (w == to) return 1;
		if (reachindex[w].Ldown<reachindex[to].Ldown && flag[w]!=mark)
	//	if (flag[w]!=mark)
		{
			if (query(w,to)) return 1;
		}
	}
	return 0;
}
bool naiveQuery(int from, int to)
{
	flag[from] = mark;
	if (from == to) return 1;
	for (int i = 0; i < v[from].size(); ++ i)
	{
		int w = v[from][i];
		if (flag[w]!= mark)
		{
			if (naiveQuery(w,to)) return 1;
		}
		
	}
	return 0;
}
void query(string s)
{
	flag = new int[n];
	memset(flag,0,sizeof(int)*n);
	
	//read query
	vector<int> from;
	vector<int> to;
	fstream in;
	in.open(s.c_str(), ios::in);
	int t1, t2;
	while(in >> t1 >> t2)
	{
		from.push_back(t1);
		to.push_back(t2);
	}
	in.close();
	
	cout << "query number: " << from.size() << endl;
	
	timer.start();
	countQuery = 0;
	//begin query
	int queryNum = from.size();
	int reachNum = 0;
	for (int i = 0; i < queryNum; ++ i)
	{
		
		bool arrive;
		if (from[i] == to[i]) arrive = 1;
		//else if (Ldown[from[i]]>=Ldown[to[i]] /*|| Lup[from[i]]<=Lup[to[i]]*/) arrive = 0;
		//else if (flabel1[from[i]]>flabel1[to[i]] || flabel2[from[i]]>flabel2[to[i]]) arrive = 0;
		//else if (flabel1[from[i]]>flabel1[to[i]]) arrive = 0;
		else if (reachindex[from[i]].Ldown >= reachindex[to[i]].Ldown) arrive = 0;
		else if (reachindex[from[i]].flabel1>reachindex[to[i]].flabel1 || reachindex[from[i]].flabel2>reachindex[to[i]].flabel2 ) arrive = 0;
		else if (intersect(from[i], to[i])) arrive = 1;
		else
		{
			mark = i+1;
			arrive = query(from[i],to[i]);
		}
		reachNum += arrive;
		//if (arrive) cout << from[i] << " " << to[i] << endl;
	}
	
	timer.stop();
	cout << "reach number: " << reachNum<< endl;
	cout << endl;
	cout << "countQuery= " << countQuery << endl;
	cout << "QUERY_TIME = " << timer.GetRuntime()*1000 << " ms" << endl;
	
	delete [] flag;
}
//----------------------query----------------------------


int main(int argc, char* argv[])
{
	//srand(time(0));
	string s;
	k = atoi(argv[1]);
	//argv[2] for graph
	s = argv[2];
	string s2;
	//argv[3] for query
	s2 = argv[3];
	//argv[4] for info
	string s3 = argv[4];
	
	//read
	timer.start();
	readGraph(s);
	readChainInfo(s3);
	timer.stop();
	//cout << "read graph time: " << timer.GetRuntime() << endl;
	
	
	//index
	timer.start();
	findChain();
	feline();//feline label
	handle();
	timer.stop();
	//cout << "!!!index time : " << timer.GetRuntime()*1000 << endl;
	double indexTime = timer.GetRuntime()*1000;
	copyIndex();
	
	//query
	query(s2);
	//cout << countQuery << endl;
	cout << "CONSTR_TIME= " << indexTime << " ms" << endl;
	
	long long count1 = 0;
	for (int i = 0; i < n; ++ i) 
	{
		count1 += Lin[i].size();
		count1 += Lout[i].size();
	}
	//n*k*2
	cout << "INDEX_SIZE= " << (count1 + n*1 + n*2)*4.0/1024/1024 << " MB" << endl;
}
