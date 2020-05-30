#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "../../main/graph.h"
#include "../../main/estimation.h"
#include "../exp.h"

int read_sampling_list(std::vector<SampledData> &samplinglist){
	FILE *rf;
	const char *dir = "../data/dataset/";
	std::string rfpath = std::string(dir) + "facebook_RW_samples.txt";
	rf = fopen(rfpath.c_str(), "r");
	if(rf == NULL) {
		printf("Could not open file named %s.\n","facebook_RW_samples.txt");
		printf("Pleace check if name of file is correct.\n");
		exit(0);
	}

	int v, pubd, d;
	samplinglist.clear();
	while(fscanf(rf, "%d %d %d", &v , &pubd, &d) != EOF) {
		SampledData data(v);
		data.pubd = double(pubd);
		data.d = double(d);
		samplinglist.push_back(data);
	}
	fclose(rf);

	return 0;
}

int main(int argc, char *argv[]){
	if(argc != 1){
		printf("Please input following: ./real_facebook_estimation \n");
		exit(0);
	}

	(void)argv;

	std::vector<SampledData> samplinglist;
	read_sampling_list(samplinglist);

	Settings settings;
	
	//average degree estimation
	std::vector<double> aved_estimations;
	Average_degree_estimations(samplinglist, aved_estimations);

	//size estimation
	std::vector<double> size_estimations;
	Size_estimations(samplinglist, settings.ratio_m_r, size_estimations);

	//write result
	const char *dir = "../data/exp/";
	const std::string expfilepath = std::string(dir) + "real_facebook_estimation.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n",expfilepath.c_str());
		return -1;
	}
	fprintf(ef,"Estimation values for average degree of Facebook.\n");
	for(int i=0;i<int(aved_estimations.size());++i){
		fprintf(ef,"Estimation%d: %lf\n", i, aved_estimations[i]);
	}
	fprintf(ef,"Estimation values for size of Facebook.\n");
	fprintf(ef,"Ratio of a threshold m to sample size: %lf\n",settings.ratio_m_r);
	for(int i=0;i<int(size_estimations.size());++i){
		fprintf(ef,"Estimation%d: %lf\n", i, size_estimations[i]);
	}
	fclose(ef);

	return 0;
}