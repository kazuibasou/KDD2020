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
We estimate the NRMSEs of estimates of the following size estimators for a specified sample size and specified values of p.
estimation0: naive weighting
estimation1: proposed weighting
*/

int several_p_exp(Graph &G,std::string model,const std::vector<double> &ps,const double samplerate,const double ratio_m_r,const int exptime,std::vector<std::vector<double> > &NRMSEs){
	int i,j,k,seed, samplesize;
	double exact_value,p,NRMSE;
	std::vector<double> estimations;
	std::vector<std::vector<double> > estimations_results;
	NRMSEs.clear();
	exact_value = double(G.N);
	std::vector<SampledData> samplinglist;
	samplesize = round(G.N*samplerate);

	for(i=0;i<int(ps.size());++i){
		p = ps[i];
		estimations_results = std::vector<std::vector<double> >();
		
		for(j=0;j<exptime;++j){
			G.set_privacy_labels(p);
			PublicCluster LPC = return_LPC(G);
			seed = select_seed(LPC);
			samplinglist.clear();
			RW(G, model, samplesize, seed, samplinglist);
			estimations.clear();
			Size_estimations(samplinglist,ratio_m_r,estimations);
			estimations_results.resize(estimations.size());
			for(k=0;k<int(estimations.size());++k){
				estimations_results[k].push_back(estimations[k]);
			}
		}

		NRMSEs.resize(estimations_results.size());
		for(k=0;k<int(estimations_results.size());++k){
			NRMSE = calc_NRMSE(estimations_results[k],exact_value);
			NRMSEs[k].push_back(NRMSE);
		}
	}

	return 0;
}

int main(int argc,char *argv[]){
	if(argc != 3){
		printf("Error: Please input following: ./asm_aved_estimation (target dataset: youtube, orkut, facebook, or livejournal) (access model: ideal or hidden)\n");
		exit(0);
	}
	//read graph data.
	const char graphdata[] = "graphdata.txt"; //name of file contains data (number of nodes, number of edges and so on) of each graph.
	const char *readgraph = argv[1]; //name of file we read.
	const char *model = argv[2]; //The model of available neighbor data. We specify "ideal" or "hidden".

	Graph G;
	G.readgraph(graphdata,readgraph);

	//main process
	srand((unsigned) time(NULL));
	Settings settings;
	std::vector<std::vector<double> > NRMSEs;
	several_p_exp(G,model, settings.ps, settings.asm_samplerate, settings.ratio_m_r, settings.numrun, NRMSEs);

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + std::string(readgraph) + "_size_estimation_" + model + ".txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"The NRMSEs of estimates of each size estimator for a specified sample size and specified probabilities of p.\n");
	fprintf(ef,"Given dataset: %s\n",readgraph);
	fprintf(ef,"Probabilities of p:");
	for(double p:settings.ps){
		fprintf(ef,"%lf, ",p);
	}
	fprintf(ef,"\n");
	fprintf(ef,"sample rate: %lf\n", settings.asm_samplerate);
	fprintf(ef,"Ratio of a threshold m to sample size: %lf\n",settings.ratio_m_r);
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