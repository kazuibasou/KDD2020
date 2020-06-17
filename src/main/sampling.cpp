#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "sampling.h"

int RW(Graph &G, std::string model,const int samplesize,const int startnode,std::vector<SampledData> &samplinglist){
	
	//Random walk for the ideal model.
	if(model == "ideal"){
		int i = 0;
		int v = startnode;
		int nextv,index;
		samplinglist.clear();
		SampledData data(0);

		for(i=0;i<samplesize;++i){
			data = G.Query(model,v);
			samplinglist.push_back(data);
			index = generate_rand(G.pub_nlist[v].size());
			nextv = G.pub_nlist[v][index];
			v = nextv;
		}
	}

	//Random walk with the proposed public-degree calculation for the hidden privacy model.
	else if(model == "hidden"){
		int i = 0;
		int v = startnode;
		int nextv,index;
		std::vector<int> a(G.N,0),b(G.N,0);
		samplinglist.clear();
		int flag = 0;
		SampledData data(0),nextdata(0);

		data = G.Query(model,v);

		for(i=0;i<samplesize;++i){
			flag = 0;
			while(flag == 0){
				index = generate_rand(data.ndata.size());
				nextv = data.ndata[index];
				b[v] += 1;
				nextdata = G.Query(model,nextv);
				if(nextdata.ndata.size() != 0){
					a[v] += 1;
					flag = 1;
					v = nextv;
				}
			}
			samplinglist.push_back(data);
			data = nextdata;
		}

		int r = samplinglist.size();
		for(i=0;i<r;++i){
			v = samplinglist[i].index;
			samplinglist[i].pubd = (double(a[v])/b[v])*samplinglist[i].ndata.size();
		}
	}

	else{
		printf("Given model named %s is not defined.\n", model.c_str());
		exit(0);
	}

	return 0;
}