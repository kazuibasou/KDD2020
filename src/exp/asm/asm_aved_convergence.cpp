#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../main/graph.h"
#include "../../main/convergence.h"
#include "../exp.h"

/*
We estimate the NRMSEs of convergence values of the following average degree estimators for specified values of p.
estimation0: naive weighting
estimation1: proposed weighting
*/

int experiment_convergences(Graph &G,const std::vector<double> &ps,const int exptime,std::vector<std::vector<double> > &NRMSEs){
	int i,j;
	double exact_value = double(2*G.M)/G.N;
	double NRMSE;
	std::vector<double> convergences;
	std::vector<std::vector<double> > convergences_results;
	NRMSEs.clear();

	for(double p:ps){
		convergences_results = std::vector<std::vector<double> >();

		for(i=0;i<exptime;++i){
			G.set_privacy_labels(p);
			PublicCluster LPC = return_LPC(G);
			convergences.clear();
			average_degree_convergences(G,LPC,convergences);
			convergences_results.resize(convergences.size());
			for(j=0;j<int(convergences_results.size());++j){
				convergences_results[j].push_back(convergences[j]);
			}
		}

		NRMSEs.resize(convergences_results.size());
		for(j=0;j<int(convergences_results.size());++j){
			NRMSE = calc_NRMSE(convergences_results[j],exact_value);
			NRMSEs[j].push_back(NRMSE);
		}
	}
	
	return 0;
}

int main(int argc,char *argv[]){
	if(argc != 2){
		printf("Error: Please input following: ./asm_aved_convergence (name of target dataset)\n");
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
	std::vector<std::vector<double> > NRMSEs;
	experiment_convergences(G,settings.ps,settings.numrun,NRMSEs);

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + std::string(readgraph) + "_aved_convergence.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"The NRMSEs of convergence values of each size estimator for specified probabilities of p.\n");
	fprintf(ef,"Given dataset: %s\n",readgraph);
	fprintf(ef,"Probabilities of p:");
	for(double p:settings.ps){
		fprintf(ef,"%lf, ",p);
	}
	fprintf(ef,"\n");
	fprintf(ef,"Number of runs: %d\n",settings.numrun);
	for(int i=0;i<int(NRMSEs.size());++i){
		fprintf(ef,"NRMSEs of estimation%d: \n",i);
		for(double NRMSE:NRMSEs[i]){
			fprintf(ef,"%lf, ",NRMSE);
		}
		fprintf(ef,"\n");
	}
	fclose(ef);

	return 0;
}