#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../main/graph.h"
#include "../../main/convergence.h"
#include "../exp.h"

/*
We calculate the relative error of convergence values of the following average degree estimators. 
estimation0: naive weighting
estimation1: proposed weighting
*/

int main(int argc, char *argv[]){
	if(argc != 1){
		printf("Error: Please input following: ./pokec_aved_convergence\n");
		exit(0);
	}
	//read graph data.
	const char graphdata[] = "graphdata.txt"; //name of file contains data (number of nodes, number of edges and so on) of each graph.
	const char *readgraph = "pokec"; //name of file we read.
	(void)argv;

	Graph G;
	G.readpokec(graphdata,readgraph);

	//main process
	PublicCluster LPC = return_LPC(G);
	double exact_value = double(2*G.M)/G.N;
	std::vector<double> convergences;
	average_degree_convergences(G,LPC,convergences);
	std::vector<double> relative_errors;
	for(int i=0;i<int(convergences.size());++i){
		relative_errors.push_back(double(fabs(convergences[i]-exact_value))/exact_value);
	}

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + std::string(readgraph) + "_aved_convergence.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"Convergence values and the relative errors of each average degree estimator\n");
	for(int i=0;i<int(convergences.size());++i){
		fprintf(ef,"Estimation%d: %lf,%lf\n",i,convergences[i],relative_errors[i]);
	}
	fclose(ef);

	return 0;
}