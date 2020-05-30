#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "convergence.h"

//Convergence values of size
int size_convergences(Graph &G, PublicCluster &LPC, std::vector<double> &convergences){
	double pubd,d;
	double a = 0.0; double b = 0.0;

	for(int v:LPC.V){
		d = double(G.nlist[v].size());
		pubd = double(LPC.nlist[v].size());
		a += double(pubd*d);
		b += double(pubd*pubd);
	}

	convergences.clear();
	convergences.push_back(double(LPC.N));
	convergences.push_back(double(LPC.N*a)/b);

	return 0;
}

int average_degree_convergences(Graph &G, PublicCluster &LPC, std::vector<double> &convergences){
	double a = 0.0;
	for(int v:LPC.V){
		a += double(LPC.nlist[v].size())/G.nlist[v].size();
	}

	convergences.clear();
	convergences.push_back(double(2*LPC.M)/LPC.N);
	convergences.push_back(double(2*LPC.M)/a);

	return 0;
}