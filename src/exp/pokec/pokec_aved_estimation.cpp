#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../main/graph.h"
#include "../../main/sampling.h"
#include "../../main/estimation.h"
#include "../exp.h"

/*
We estimate the NRMSEs of estimates of the following average degree estimators for specified sample sizes. 
estimation0: naive weighting
estimation1: proposed weighting
*/

int several_samplesize_exp(Graph &G,std::string model,const std::vector<double> samplerates,const int exptime,std::vector<std::vector<double> > &NRMSEs){
	int i,j,k,seed,samplesize;
	double NRMSE;
	double exact_value = double(2*G.M)/G.N;
	std::vector<SampledData> samplinglist;
	std::vector<double> estimations;
	std::vector<std::vector<double> > estimations_results;
	NRMSEs.clear();
	PublicCluster LPC = return_LPC(G);

	for(i=0;i<int(samplerates.size());++i){
		samplesize = round(G.N*samplerates[i]);
		estimations_results = std::vector<std::vector<double> >();
		for(j=0;j<exptime;++j){
			seed = select_seed(LPC);
			samplinglist.clear();
			RW(G, model, samplesize, seed, samplinglist);
			estimations.clear();
			Average_degree_estimations(samplinglist,estimations);
			estimations_results.resize(estimations.size());
			for(k=0;k<int(estimations_results.size());++k){
				estimations_results[k].push_back(estimations[k]);
			}
		}

		NRMSEs.resize(estimations_results.size());
		for(k=0;k<int(NRMSEs.size());++k){
			NRMSE = calc_NRMSE(estimations_results[k],exact_value);
			NRMSEs[k].push_back(NRMSE);
		}
	}
	
	return 0;
}

int main(int argc,char *argv[]){
	if(argc != 2){
		printf("Error: Please input following: ./pokec_aved_estimation (access model: ideal or hidden)\n");
		exit(0);
	}
	//read graph data.
	const char graphdata[] = "graphdata.txt"; //name of file contains data (number of nodes, number of edges and so on) of each graph.
	const char *readgraph = "pokec"; //name of file we read.
	const char *model = argv[1]; //The model of available neighbor data. We specify "ideal" or "hidden".

	Graph G;
	G.readpokec(graphdata,readgraph);

	//main process
	srand((unsigned) time(NULL));
	Settings settings;
	std::vector<std::vector<double> > NRMSEs;
	several_samplesize_exp(G, model, settings.pokec_samplerates, settings.numrun, NRMSEs);

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + std::string(readgraph) + "_aved_" + model + ".txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"The NRMSEs of estimates of each average degree estimator for specified sample rates.\n");
	fprintf(ef,"Sample rates:");
	for(double samplerate:settings.pokec_samplerates){
		fprintf(ef,"%lf, ",samplerate);
	}
	fprintf(ef,"\n");
	fprintf(ef,"Number of runs: %d\n",settings.numrun);
	for(int i=0;i<int(NRMSEs.size());++i){
		fprintf(ef,"NRMSE of estimation%d:\n",i);
		for(double NRMSE:NRMSEs[i]){
			fprintf(ef,"%lf, ",NRMSE);
		}
		fprintf(ef,"\n");
	}
	fclose(ef);

	return 0;
}