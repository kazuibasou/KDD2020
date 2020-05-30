#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "../../main/graph.h"
#include "../exp.h"

//We calculate the coefficients alpha_p of given graph for specified values of p.

int calc_alpha_p(Graph &G,std::vector<double> &ps,std::vector<double> &alpha_ps){
	int i;
	double sum_d = 0.0;
	double sum_square_d = 0.0;
	double d,alpha_p;

	for(i=0;i<G.N;++i){
		d = double(G.nlist[i].size());
		sum_d += d;
		sum_square_d += d*d;
	}

	alpha_ps.clear();
	for(double p:ps){
		alpha_p = double((1-p)*sum_square_d)/((1-p)*sum_square_d + p*sum_d);
		alpha_ps.push_back(alpha_p);
	}

	return 0;
}

int main(int argc,char *argv[]){
	if(argc != 2){
		printf("Error: Please input following: ./calc_alpha_p (name of target dataset)\n");
		exit(0);
	}
	
	//read graph data.
	const char graphdata[] = "graphdata.txt"; //name of file contains data (number of nodes, number of edges and so on) of each graph.
	const char *readgraph = argv[1]; //name of file we read.

	Graph G;
	G.readgraph(graphdata,readgraph);

	//main process
	Settings settings;
	std::vector<double> alpha_ps;
	calc_alpha_p(G,settings.ps,alpha_ps);

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + std::string(readgraph) + "_alpha_p.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"The coefficients alpha_p for specified values of p.\n");
	fprintf(ef,"Given dataset: %s\n",readgraph);
	fprintf(ef,"Values of p:");
	for(double p:settings.ps){
		fprintf(ef,"%lf, ",p);
	}
	fprintf(ef,"\n");
	fprintf(ef,"Coefficients alpha_p:");
	for(double alpha_p:alpha_ps){
		fprintf(ef,"%lf, ",alpha_p);
	}
	fprintf(ef,"\n");
	fclose(ef);

	return 0;
}