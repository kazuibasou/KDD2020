#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../main/graph.h"
#include "../exp.h"

/*
We estimate the NRMSEs of the proposed size estimator and the proportions of queried nodes 
with using each exact or proposed weighting method in the hidden privacy model 
for a specified sample size and specified values of p.
*/

//Size estimator with proposed weighting
int Size_estimation(std::vector<SampledData> &samplinglist, const double ratio_m_r, double &estimation){
	int r = samplinglist.size();
	int k,l,v_k,v_l;
	int m = round(r*ratio_m_r);
	double d_k,d_l,pubd_k,pubd_l;

	//estimation: proposed weighting

	double phi = 0.0;double psi = 0.0;

	for(k=0;k<r-m;++k){
		v_k = samplinglist[k].index;
		d_k = double(samplinglist[k].ndata.size());
		pubd_k = double(samplinglist[k].pubd);
		for(l=k+m;l<r;++l){
			v_l = samplinglist[l].index;
			d_l = double(samplinglist[l].ndata.size());
			pubd_l = double(samplinglist[l].pubd);
			if(v_k == v_l){
				phi += double(2);
			}
			psi += (double(d_k)/pubd_l) + (double(d_l)/pubd_k);
		}
	}

	estimation = double(psi)/phi;
	
	return 0;
}

int RW_hidden_comparision(Graph &G, const int samplesize, const int seed, const double ratio_m_r, double &exact_query_prop, double &exact_size_estimation, double &proposed_query_prop, double &proposed_size_estimation){
	int i = 0;
	int v = seed;
	int nextv,index;
	std::vector<int> a(G.N,0),b(G.N,0);
	std::vector<SampledData> samplinglist;
	int flag = 0;
	SampledData data(0),nextdata(0);
	std::vector<int> queried(G.N,0);

	data = G.Query("hidden", v);
	queried[v] = 1;

	for(i=0;i<samplesize;++i){
		flag = 0;
		while(flag == 0){
			index = generate_rand(data.ndata.size());
			nextv = data.ndata[index];
			b[v] += 1;
			nextdata = G.Query("hidden", nextv);
			queried[nextv] = 1;
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

	//Proposed method to approximate the public-degree of each sample
	for(i=0;i<r;++i){
		v = samplinglist[i].index;
		samplinglist[i].pubd = (double(a[v])/b[v])*samplinglist[i].ndata.size();
	}

	proposed_query_prop = double(std::accumulate(queried.begin(), queried.end(), 0))/G.N;
	
	Size_estimation(samplinglist, ratio_m_r, proposed_size_estimation);

	//Exact method to exactly calculate the public-degree of each sample
	SampledData w_data(0);
	for(i=0;i<r;++i){
		samplinglist[i].pubd = 0.0;
		for(int w:samplinglist[i].ndata){
			w_data = G.Query("hidden", w);
			queried[w] = 1;
			if(w_data.ndata.size() != 0){
				samplinglist[i].pubd += 1.0;
			}
		}
	}

	exact_query_prop = double(std::accumulate(queried.begin(), queried.end(), 0))/G.N;
	
	Size_estimation(samplinglist, ratio_m_r, exact_size_estimation);

	return 0;
}

int several_p_exp(Graph &G,const std::vector<double> &ps,const double samplerate,const double ratio_m_r,const int exptime,std::vector<double> &exact_query_props, std::vector<double> &exact_size_NRMSEs, std::vector<double> &proposed_query_props, std::vector<double> &proposed_size_NRMSEs){
	int i, seed, samplesize;
	double exact_query_prop, exact_size_estimation, proposed_query_prop, proposed_size_estimation, average_exact_query_prop, average_proposed_query_prop;
	std::vector<double> exact_size_estimations, proposed_size_estimations;
	samplesize = round(G.N*samplerate);

	for(double p:ps){
		average_exact_query_prop = 0.0;
		average_proposed_query_prop = 0.0;
		exact_size_estimations.clear();
		proposed_size_estimations.clear();

		for(i=0;i<exptime;++i){
			G.set_privacy_labels(p);
			PublicCluster LPC = return_LPC(G);
			seed = select_seed(LPC);
			RW_hidden_comparision(G, samplesize, seed, ratio_m_r, exact_query_prop, exact_size_estimation, proposed_query_prop, proposed_size_estimation);
			average_exact_query_prop += exact_query_prop;
			average_proposed_query_prop += proposed_query_prop;
			exact_size_estimations.push_back(exact_size_estimation);
			proposed_size_estimations.push_back(proposed_size_estimation);
		}

		exact_query_props.push_back(double(average_exact_query_prop)/exptime);
		exact_size_NRMSEs.push_back(calc_NRMSE(exact_size_estimations, double(G.N)));
		proposed_query_props.push_back(double(average_proposed_query_prop)/exptime);
		proposed_size_NRMSEs.push_back(calc_NRMSE(proposed_size_estimations, double(G.N)));
	}

	return 0;
}

int main(int argc,char *argv[]){
	if(argc != 2){
		printf("Error: Please input following: ./asm_hidden_compare (name of target dataset)\n");
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
	std::vector<double> exact_query_props, exact_size_NRMSEs, proposed_query_props, proposed_size_NRMSEs;
	several_p_exp(G,settings.ps,settings.asm_samplerate, settings.ratio_m_r, settings.numrun, exact_query_props, exact_size_NRMSEs, proposed_query_props, proposed_size_NRMSEs);

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + std::string(readgraph) + "_hidden_compare.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"The NRMSEs of the proposed size estimator and the proportions of queried nodes \nwith using each exact or proposed weighting method in the hidden privacy model for a specified sample size and specified probabilities of p.\n");
	fprintf(ef,"Given dataset: %s\n",readgraph);
	fprintf(ef,"Probabilities of p:");
	for(double p:settings.ps){
		fprintf(ef,"%lf, ",p);
	}
	fprintf(ef,"\n");
	fprintf(ef,"sample rate: %lf\n", settings.asm_samplerate);
	fprintf(ef,"Ratio of a threshold m to sample size: %lf\n",settings.ratio_m_r);
	fprintf(ef,"Number of runs: %d\n",settings.numrun);
	fprintf(ef,"Proportion of queried nodes by exact method:\n");
	for(double prop:exact_query_props){
		fprintf(ef,"%lf, ",prop);
	}
	fprintf(ef,"\n");
	fprintf(ef,"NRMSEs of size estimations by exact method:\n");
	for(double NRMSE:exact_size_NRMSEs){
		fprintf(ef,"%lf, ",NRMSE);
	}
	fprintf(ef,"\n");
	fprintf(ef,"Proportion of queried nodes by proposed method:\n");
	for(double prop:proposed_query_props){
		fprintf(ef,"%lf, ",prop);
	}
	fprintf(ef,"\n");
	fprintf(ef,"NRMSEs of size estimations by proposed method:\n");
	for(double NRMSE:proposed_size_NRMSEs){
		fprintf(ef,"%lf, ",NRMSE);
	}
	fprintf(ef,"\n");

	return 0;
}