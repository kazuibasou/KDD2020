#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "../../main/graph.h"

//We calculate the cumulative degree distribution of given graph. 

int calc_degree_dist(Graph &G,std::vector<double> &dist){
	int i,j,d;
	dist = std::vector<double>(G.maxd+1,0.0);
	for(i=0;i<G.N;++i){
		d = G.nlist[i].size();
		for(j=1;j<d+1;++j){
			dist[j] += 1;
		}
	}

	for(i=0;i<G.maxd+1;++i){
		dist[i] = double(dist[i])/G.N;
	}

	return 0;
}

int main(int argc,char *argv[]){
	if(argc != 2){
		printf("Error: Please input following: ./calc_degree_dist (name of target dataset)\n");
		exit(0);
	}
	
	//read graph data.
	const char graphdata[] = "graphdata.txt"; //name of file contains data (number of nodes, number of edges and so on) of each graph.
	const char *readgraph = argv[1]; //name of file we read.

	Graph G;
	G.readgraph(graphdata,readgraph); 

	//main process
	std::vector<double> dist;
	calc_degree_dist(G,dist);

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + std::string(readgraph) + "_degree_dist.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"Given dataset: %s\n",readgraph);
	fprintf(ef,"(degree) (cumulative distribution)\n");
	for(int i=1;i<G.maxd+1;++i){
		fprintf(ef,"%d %.9lf\n",i,dist[i]);
	}
	fclose(ef);

	return 0;
}