#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include "../../main/graph.h"
#include "../exp.h"

//We calculate the relative size of the largest public-cluster and the average absolute sizes of not largest public-clusters. 

int public_clusters_size(Graph &G, double &largest_size, double &isolated_average_size){
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
		isolated_average_size = 0.0;
	}
	else{
		int sum = accumulate(sizes.begin(),sizes.end(),0) - largest_size;
		isolated_average_size = double(sum)/(sizes.size() - 1);
	}

	largest_size = double(largest_size)/G.N;

	return 0;
}

int main(int argc,char *argv[]){
	if(argc != 1){
		printf("Error: Please input following: ./pokec_public_clusters_size \n");
		exit(0);
	}
	//read graph data.
	const char graphdata[] = "graphdata.txt"; //name of file contains data (number of nodes, number of edges and so on) of each graph.
	const char *readgraph = "pokec"; //name of file we read.
	(void)argv;

	Graph G;
	G.readpokec(graphdata,readgraph); 

	//main process
	double largest_size, isolated_average_size;
	public_clusters_size(G, largest_size, isolated_average_size);

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + std::string(readgraph) + "_public_clusters_size.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"The relative sizes of the largest public-cluster and the average absolute sizes of isolated public-cluster\n");
	fprintf(ef,"Proportion of private nodes: %lf\n", double(std::accumulate(G.privacy.begin(), G.privacy.end(), 0))/G.N);
	fprintf(ef,"Relative size of the largest public-cluster: %lf\n", largest_size);
	fprintf(ef,"Average absolute size of isolated public-clusters: %lf\n", isolated_average_size);
	fclose(ef);

	return 0;
}