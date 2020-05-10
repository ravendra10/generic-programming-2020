#include<iostream>
#include<vector>
#include <chrono> 
#include "sparse_matrix.h"
#include <stack>

using namespace std;
using namespace std::chrono; 

#define NODES 500
#define EDGES 500

int main()
{
	
	int graph[NODES][NODES];
	for(int i = 0; i < NODES; ++i)
	{
		for(int j = 0; j < NODES; ++j)
		{
			graph[i][j] = 0;
		}
	}
	
	int e = 0;
	while(e<EDGES)
	{
		int i = rand()%NODES;
		int j = rand()%NODES;

		if(graph[i][j] == 0)
		{
			graph[i][j] = 1 + (rand()%10);
			++e;
		}
	}
	
	sparse_matrix<int,unordered_map<int,int>> G(graph,NODES,NODES,0);
	

	cout<<"DFS on 2d array : \n";

	bool vis[NODES];
	for(int i=0;i<NODES;++i)
		vis[i]=false;

	auto start = high_resolution_clock::now();
	for(int i=0;i<NODES;++i)
	{
		if(vis[i])
			continue;
		stack <int> S;
		S.push(i);
		vis[i] = true;
		while(!S.empty())
		{
			int n = S.top(); 
			S.pop();
			//cout<<S.size()<<"\n";
			
			for(int j = 0; j < NODES; ++j)
			{
				if(graph[n][j] != 0 && !vis[j])
				{
					S.push(j);
					vis[j] = true;
				}
			}
		}
	}
	auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << " microseconds\n" << endl;

	for(int i=0;i<NODES;++i)
		vis[i]=false;

	cout<<"DFS on sparse_matrix : \n";

	start = high_resolution_clock::now();

	for(int i=0;i<NODES;++i)
	{
		if(vis[i])
			continue;
		stack <int> S;
		S.push(i);
		vis[i] = true;
		while(!S.empty())
		{
			int n = S.top(); S.pop();
			

			for(auto j = G[n].begin(); j != G[n].end(); ++j)
			{
				int x = j->first;
				if(!vis[x])
				{
					S.push(x);
					vis[x] = true;
				}
			}
		}
	}
	stop = high_resolution_clock::now(); 
    duration = duration_cast<microseconds>(stop - start); 

	cout<< duration.count() << " microseconds" << endl;
}