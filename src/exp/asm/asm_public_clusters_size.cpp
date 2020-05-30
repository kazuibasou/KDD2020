#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../main/graph.h"
#include "../exp.h"

//We estimate the relative sizes of the largest public-cluster and the average absolute sizes of not largest public-clusters for specified values of p. 

int public_clusters_size(Graph &G, double &largest_size, double &not_largest_average_size){
	int i,j,size;
	std::vector<int> search = G.privacy;
	std::vector<int> sizes;
	sizes.clear();
	largest_size = 0;

	for(i=0;i<G.N;++i){
		if(search[i] == 0){
			std::deque<int> Q;
			std::vector<int> visit(G.N,0);
			visit[i] = 1;
			Q.push_back(i);
			size = 0;

			while(Q.empty() == false){
				j = Q.front();
				Q.pop_front();
				size += 1;
				search[j] = 1;
				for(int k:G.nlist[j]){
					if(G.privacy[k] == 0 && visit[k] == 0){
						visit[k] = 1;
						Q.push_back(k);
					}
				}
			}

			sizes.push_back(size);
			if(size > largest_size){
				largest_size = size;
			}
		}
	}

	if(sizes.size() == 1){
		not_largest_average_size = 0.0;
	}
	else{
		int sum = accumulate(sizes.begin(),sizes.end(),0) - largest_size;
		not_largest_average_size = double(sum)/(sizes.size() - 1);
	}

	largest_size = double(largest_size)/G.N;

	return 0;
}

int several_ps_exp(Graph &G,const std::vector<double> &ps,const int exptime,std::vector<double> &largest_sizes, std::vector<double> &not_largest_average_sizes){
	int i;
	double largest_size, average_largest_size, not_largest_average_size, average_not_largest_average_size;

	largest_sizes.clear();
	not_largest_average_sizes.clear();

	for(double p:ps){
		average_largest_size = 0.0;
		average_not_largest_average_size = 0.0;
		for(i=0;i<exptime;++i){
			G.set_privacy_labels(p);
			public_clusters_size(G, largest_size, not_largest_average_size);
			average_largest_size += largest_size;
			average_not_largest_average_size += not_largest_average_size;
		}
		average_largest_size = double(average_largest_size)/exptime;
		average_not_largest_average_size = double(average_not_largest_average_size)/exptime;
		largest_sizes.push_back(average_largest_size);
		not_largest_average_sizes.push_back(average_not_largest_average_size);
	}
	
	return 0;
}

int main(int argc,char *argv[]){
	if(argc != 2){
		printf("Error: Please input following: ./asm_public_clusters_size (name of target dataset)\n");
		exit(0);
	}
	//read graph data.
	const char graphdata[] = "graphdata.txt"; //name of file contains data (number of nodes, number of edges and so on) of each graph.
	const char *readgraph = argv[1]; //name of file we read.

	Graph G;
	G.readgraph(graphdata,readgraph); 

	//main process
	srand((unsigned) time(NULL));
	Settings settings;
	std::vector<double> largest_sizes, not_largest_average_sizes;
	several_ps_exp(G,settings.ps,settings.numrun,largest_sizes, not_largest_average_sizes);

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + std::string(readgraph) + "_public_clusters_size.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"The relative sizes of the largest public-cluster and the average absolute sizes of not largest public-clusters for specified probabilities of p.\n");
	fprintf(ef,"Given dataset: %s\n",readgraph);
	fprintf(ef,"Probabilities of p:");
	for(double p:settings.ps){
		fprintf(ef,"%lf, ",p);
	}
	fprintf(ef,"\n");
	fprintf(ef,"Number of runs: %d\n",settings.numrun);
	fprintf(ef,"Average relative size of the largest public cluster:");
	for(double largest_size:largest_sizes){
		fprintf(ef,"%lf, ",largest_size);
	}
	fprintf(ef,"\n");
	fprintf(ef,"Average absolute size of not largest public clusters:");
	for(double not_largest_average_size:not_largest_average_sizes){
		fprintf(ef,"%lf, ",not_largest_average_size);
	}
	fprintf(ef,"\n");
	fclose(ef);

	return 0;
}