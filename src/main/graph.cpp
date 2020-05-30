#include <iostream>
#include <vector>
#include <deque>
#include <numeric>
#include <random>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include "graph.h"

SampledData::SampledData(const int v){
	index = v;
	ndata = std::vector<int>();
	pubd = 0.0;
	d = 0.0;
}

SampledData::~SampledData(){
	
	std::vector<int>().swap(ndata);
}

SampledData Graph::Query(std::string model, const int v){
	SampledData data(v);
	if(privacy[v] == 0){
		data.ndata = nlist[v];
		data.d = double(nlist[v].size());
	}
	else{
		data.ndata = std::vector<int>();
		data.d = 0.0;
	}

	if(model == "ideal"){
		data.pubd = double(pub_nlist[v].size());
	}
	else if(model == "hidden"){
		data.pubd = 0;
	}
	else{
		printf("Error: Given model named %s is not defined.\n", model.c_str());
		exit(0);
	}

	return data;
}

Graph::Graph(){
	N = 0;
	M = 0;
	nlist = std::vector<std::vector<int> >();
	pub_nlist = std::vector<std::vector<int> >();
	privacy = std::vector<int>();
}

Graph::~Graph(){
	std::vector<std::vector<int> >().swap(nlist);
	std::vector<std::vector<int> >().swap(pub_nlist);
	std::vector<int>().swap(privacy);
}

PublicCluster::PublicCluster(const Graph &G){
	N = 0;
	M = 0;
	V = std::vector<int>();
	nlist = std::vector<std::vector<int> >(G.N,std::vector<int>(0));
}

PublicCluster::~PublicCluster(){
	std::vector<int>().swap(V);
	std::vector<std::vector<int> >().swap(nlist);
}
//for dataset based on assumption that each node inndepedently becomes private with probability p.
int Graph::readgraph(const char *graphdatafile,const char *readgraph){
	FILE *gf;
	const char *dir = "../data/dataset/";
	std::string gfpath = std::string(dir) + graphdatafile;
	gf = fopen(gfpath.c_str(), "r");
	if(gf == NULL) {
		printf("Error: Could not open file named %s.\n",graphdatafile);
		exit(0);
	}
	
	char graphname[100];
	int s = sizeof(readgraph)/sizeof(char);
	if(s > 100){
		printf("Error: Length of name of readfile must be less than 100.\n");
		exit(0);
	}

	int i,j;
	int frag = 0;
	std::string readfile = std::string(readgraph);
	while(fscanf(gf, "%s %d %d", graphname,  &i , &j) != EOF) {
		if(strcmp(graphname,readfile.c_str()) == 0){
			N = i;
			M = j;
			frag = 1;
		}
	}
	fclose(gf);
	if(frag == 0){
		printf("Error: Could not find graph file named %s.txt.\n",readgraph);
		exit(0);
	}

	FILE *rf;
	std::string rfpath = std::string(dir) + std::string(readgraph) + ".txt";
	rf = fopen(rfpath.c_str(), "r");
	if(rf == NULL) {
		printf("Error: Could not open file named %s.txt.\n",readgraph);
		exit(0);
	}
	int x,y;
	nlist = std::vector<std::vector<int> >(N,std::vector<int>(0));
	pub_nlist = std::vector<std::vector<int> >(N,std::vector<int>(0));
	while(fscanf(rf, "%d %d", &x , &y) != EOF) {
		nlist[x].push_back(y);
		nlist[y].push_back(x);
	}
	fclose(rf);
	privacy = std::vector<int>(N,0);

	maxd = 0;
	for(i=0;i<N;++i){
		std::sort(nlist[i].begin(),nlist[i].end());
		if(int(nlist[i].size()) > maxd){
			maxd = nlist[i].size();
		}
	}

	return 0;
}
//for pokec dataset
int Graph::readpokec(const char *graphdatafile,const char *readgraph){
	FILE *gf;
	const char *dir = "../data/dataset/";
	std::string gfpath = std::string(dir) + graphdatafile;
	gf = fopen(gfpath.c_str(), "r");
	if(gf == NULL) {
		printf("Error: Could not open file named %s.\n",graphdatafile);
		exit(0);
	}
	
	char graphname[100];
	int s = sizeof(readgraph)/sizeof(char);
	if(s > 100){
		printf("Error: Length of name of readfile must be less than 100.\n");
		exit(0);
	}

	int i,j;
	int frag = 0;
	std::string readfile = std::string(readgraph);
	while(fscanf(gf, "%s %d %d", graphname,  &i , &j) != EOF) {
		if(strcmp(graphname,readfile.c_str()) == 0){
			N = i;
			M = j;
			frag = 1;
		}
	}
	fclose(gf);
	if(frag == 0){
		printf("Error: Could not find graph file named %s.txt.\n",readgraph);
		exit(0);
	}

	FILE *rf;
	std::string rfpath = std::string(dir) + std::string(readgraph) + ".txt";
	rf = fopen(rfpath.c_str(), "r");
	if(rf == NULL) {
		printf("Error: Could not open file named %s.txt.\n",readgraph);
		exit(0);
	}
	int x,y;
	nlist = std::vector<std::vector<int> >(N,std::vector<int>(0));
	pub_nlist = std::vector<std::vector<int> >(N,std::vector<int>(0));
	while(fscanf(rf, "%d %d", &x , &y) != EOF) {
		nlist[x].push_back(y);
		nlist[y].push_back(x);
	}
	fclose(rf);
	maxd = 0;
	for(i=0;i<N;++i){
		std::sort(nlist[i].begin(),nlist[i].end());
		if(int(nlist[i].size()) > maxd){
			maxd = nlist[i].size();
		}
	}

	FILE *pf;
	std::string pfpath = std::string(dir) + std::string(readgraph) + "_privacy.txt";
	pf = fopen(pfpath.c_str(), "r");
	if(pf == NULL) {
		printf("Error: Could not open file named %s_privacy.txt.\n",readgraph);
		exit(0);
	}
	privacy = std::vector<int>(N,0);
	while(fscanf(pf, "%d %d", &x , &y) != EOF) {
		privacy[x] = y;
	}
	fclose(pf);


	return 0;
}

int generate_rand(const int N){
	if(N == 0){
		printf("Error: Given integer is zero.\n");
		exit(0);
	}
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randN(0, N-1);

    return randN(mt);
}

double generate_uniform_rand(){
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> uniformrand(0.0,1.0);

    return uniformrand(mt);
}

double generate_uniform_rand_with_range(const double a,const double b){
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> uniformrand(a,b);

    return uniformrand(mt);
}

double calc_NRMSE(const std::vector<double> &estimations,const double exact){
	double nrmse = 0.0;
	int n = estimations.size();
	double error;

	for(double estimation:estimations){
		error = 1.0 - double(estimation)/exact;
		nrmse += double(error*error);
	}

	nrmse = double(nrmse)/n;
	nrmse = sqrt(nrmse);

	return nrmse;
}

int Graph::set_privacy_labels(const double p){
	privacy = std::vector<int>(N,0);
	double r;
	int i;
	for(i=0;i<N;++i){
		r = generate_uniform_rand();
		if(r <= p){
			privacy[i] = 1;
		}
	}

	return 0;
}

PublicCluster return_LPC(Graph &G){
	int i = 0;
	int j;
	int m = 0;
	int s = 0;
	int n = G.N;
	std::vector<int> search = G.privacy;
	PublicCluster LPC(G);

	while(std::accumulate(search.begin(), search.end(), 0) < n - m){
		if(search[i] == 0){
			PublicCluster PC(G);
			std::deque<int> Q;
			std::vector<int> visit(G.N,0);
			visit[i] = 1;
			Q.push_back(i);

			while(Q.empty() == false){
				j = Q.front();
				Q.pop_front();
				PC.V.push_back(j);
				search[j] = 1;
				for(int k:G.nlist[j]){
					if(G.privacy[k] == 0){
						PC.nlist[j].push_back(k);
						if(visit[k] == 0){
							visit[k] = 1;
							Q.push_back(k);
						}
					}
				}
			}

			s = PC.V.size();
			if(s > m){
				m = s;
				LPC.V = std::vector<int>(PC.V);
				LPC.nlist = std::vector<std::vector<int>>(PC.nlist);
			}
		}
		i += 1;
	}

	LPC.M = 0;
	for(int v:LPC.V){
		LPC.M += LPC.nlist[v].size();
		G.pub_nlist[v] = LPC.nlist[v];
	}
	LPC.N = LPC.V.size();
	LPC.M = int(LPC.M)/2;

	return LPC;
}

int select_seed(PublicCluster LPC){
	int index = generate_rand(LPC.N);
	
	return LPC.V[index];
}
