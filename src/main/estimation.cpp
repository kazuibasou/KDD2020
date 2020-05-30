#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "sampling.h"
#include "estimation.h"

//Size estimator
int Size_estimations(std::vector<SampledData> &samplinglist, const double ratio_m_r, std::vector<double> &estimations){
	int r = samplinglist.size();
	int k,l,v_k,v_l;
	int m = round(r*ratio_m_r);
	double d_k,d_l,pubd_k,pubd_l;

	//estimation0: naive weighting
	//estimation1: proposed weighting

	double phi0 = 0.0;double psi0 = 0.0;
	                  double psi1 = 0.0;

	for(k=0;k<r-m;++k){
		v_k = samplinglist[k].index;
		d_k = double(samplinglist[k].d);
		pubd_k = double(samplinglist[k].pubd);
		for(l=k+m;l<r;++l){
			v_l = samplinglist[l].index;
			d_l = double(samplinglist[l].d);
			pubd_l = double(samplinglist[l].pubd);
			if(v_k == v_l){
				phi0 += double(2);
			}
			psi0 += (double(pubd_k)/pubd_l)+ (double(pubd_l)/pubd_k);
			psi1 += (double(d_k)/pubd_l)+ (double(d_l)/pubd_k);
		}
	}

	estimations.clear();
	estimations.push_back(double(psi0)/phi0);
	estimations.push_back(double(psi1)/phi0);
	
	return 0;
}

//Average degree estimator.
int Average_degree_estimations(std::vector<SampledData> &samplinglist,std::vector<double> &estimations){

	//estimation0: naive weighting
	//estimation1: proposed weighting

	double est0 = 0.0;
	double est1 = 0.0;
	double d,pubd;

	for(SampledData data:samplinglist){
		d = double(data.d);
		pubd = double(data.pubd);
		est0 += double(1)/pubd;
		est1 += double(1)/d;
	}

	estimations.clear();
	estimations.push_back(double(samplinglist.size())/est0);
	estimations.push_back(double(samplinglist.size())/est1);
	
	return 0;
}