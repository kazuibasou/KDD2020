#ifndef GRAPH_H
#define GRAPH_H

class SampledData{
	public:
		int index;
		std::vector<int> ndata;
		double d;
		double pubd;
		SampledData(const int index);
		~SampledData();
};

class Graph{
	public:
		int N; //number of nodes
		int M; //number of edges
		int maxd; //maximum degree
		std::vector<std::vector<int> > nlist;
		std::vector<std::vector<int> > pub_nlist;
		std::vector<int> privacy;
		Graph();
		~Graph();
		int readgraph(const char *graphdatafile,const char *readgraph);
		int readpokec(const char *graphdatafile,const char *readgraph);
		int set_privacy_labels(const double p);
		SampledData Query(std::string model, const int v);
};

class PublicCluster{
	public:
		int N;
		int M;
		std::vector<int> V;
		std::vector<std::vector<int> > nlist;
		PublicCluster(const Graph &G);
		~PublicCluster();
};

int generate_rand(const int N);

double generate_uniform_rand();

double generate_uniform_rand_with_range(const double a,const double b);

double calc_NRMSE(const std::vector<double> &estimations,const double exact);

PublicCluster return_LPC(Graph &G);

int select_seed(PublicCluster LPC);

#endif