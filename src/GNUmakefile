CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3

all:main exp clean

main: graph.o sampling.o estimation.o convergence.o

graph.o:./main/graph.cpp ./main/graph.h
	$(CXX) $(CXXFLAGS) -c ./main/graph.cpp
sampling.o:./main/sampling.cpp ./main/graph.h ./main/sampling.h
	$(CXX) $(CXXFLAGS) -c ./main/sampling.cpp
estimation.o:./main/estimation.cpp ./main/graph.h ./main/sampling.h ./main/estimation.h
	$(CXX) $(CXXFLAGS) -c ./main/estimation.cpp
convergence.o:./main/convergence.cpp ./main/graph.h
	$(CXX) $(CXXFLAGS) -c ./main/convergence.cpp

exp:statistics asm pokec real_facebook

statistics:calc_degree_dist calc_alpha_p

calc_degree_dist:graph.o calc_degree_dist.o
	$(CXX) $(CXXFLAGS) -o ../bin/calc_degree_dist graph.o calc_degree_dist.o
calc_degree_dist.o:./exp/statistics/calc_degree_dist.cpp ./main/graph.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/statistics/calc_degree_dist.cpp
calc_alpha_p:graph.o calc_alpha_p.o
	$(CXX) $(CXXFLAGS) -o ../bin/calc_alpha_p graph.o calc_alpha_p.o
calc_alpha_p.o:./exp/statistics/calc_alpha_p.cpp ./main/graph.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/statistics/calc_alpha_p.cpp

asm:asm_size_estimation asm_size_convergence asm_aved_estimation asm_aved_convergence asm_hidden_compare asm_public_clusters_size

asm_size_estimation:graph.o sampling.o estimation.o asm_size_estimation.o
	$(CXX) $(CXXFLAGS) -o ../bin/asm_size_estimation graph.o sampling.o estimation.o asm_size_estimation.o
asm_size_estimation.o:./exp/asm/asm_size_estimation.cpp ./main/graph.h ./main/sampling.h ./main/estimation.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/asm/asm_size_estimation.cpp
asm_size_convergence:graph.o convergence.o asm_size_convergence.o
	$(CXX) $(CXXFLAGS) -o ../bin/asm_size_convergence graph.o convergence.o asm_size_convergence.o
asm_size_convergence.o:./exp/asm/asm_size_convergence.cpp ./main/graph.h ./main/convergence.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/asm/asm_size_convergence.cpp
asm_aved_estimation:graph.o sampling.o estimation.o asm_aved_estimation.o
	$(CXX) $(CXXFLAGS) -o ../bin/asm_aved_estimation graph.o sampling.o estimation.o asm_aved_estimation.o
asm_aved_estimation.o:./exp/asm/asm_aved_estimation.cpp ./main/graph.h ./main/sampling.h ./main/estimation.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/asm/asm_aved_estimation.cpp
asm_aved_convergence:graph.o convergence.o asm_aved_convergence.o
	$(CXX) $(CXXFLAGS) -o ../bin/asm_aved_convergence graph.o convergence.o asm_aved_convergence.o
asm_aved_convergence.o:./exp/asm/asm_aved_convergence.cpp ./main/graph.h ./main/convergence.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/asm/asm_aved_convergence.cpp
asm_hidden_compare:graph.o sampling.o asm_hidden_compare.o
	$(CXX) $(CXXFLAGS) -o ../bin/asm_hidden_compare graph.o sampling.o asm_hidden_compare.o
asm_hidden_compare.o:./exp/asm/asm_hidden_compare.cpp ./main/graph.h ./main/sampling.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/asm/asm_hidden_compare.cpp
asm_public_clusters_size:graph.o asm_public_clusters_size.o
	$(CXX) $(CXXFLAGS) -o ../bin/asm_public_clusters_size graph.o asm_public_clusters_size.o
asm_public_clusters_size.o:./exp/asm/asm_public_clusters_size.cpp ./main/graph.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/asm/asm_public_clusters_size.cpp

pokec:pokec_size_estimation pokec_size_convergence pokec_aved_estimation pokec_aved_convergence pokec_public_clusters_size

pokec_size_estimation:graph.o sampling.o estimation.o pokec_size_estimation.o
	$(CXX) $(CXXFLAGS) -o ../bin/pokec_size_estimation graph.o sampling.o estimation.o pokec_size_estimation.o
pokec_size_estimation.o:./exp/pokec/pokec_size_estimation.cpp ./main/graph.h ./main/sampling.h ./main/estimation.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/pokec/pokec_size_estimation.cpp
pokec_size_convergence:graph.o convergence.o pokec_size_convergence.o
	$(CXX) $(CXXFLAGS) -o ../bin/pokec_size_convergence graph.o convergence.o pokec_size_convergence.o
pokec_size_convergence.o:./exp/pokec/pokec_size_convergence.cpp ./main/graph.h ./main/convergence.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/pokec/pokec_size_convergence.cpp
pokec_aved_estimation:graph.o sampling.o estimation.o pokec_aved_estimation.o
	$(CXX) $(CXXFLAGS) -o ../bin/pokec_aved_estimation graph.o sampling.o estimation.o pokec_aved_estimation.o
pokec_aved_estimation.o:./exp/pokec/pokec_aved_estimation.cpp ./main/graph.h ./main/sampling.h ./main/estimation.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/pokec/pokec_aved_estimation.cpp
pokec_aved_convergence:graph.o convergence.o pokec_aved_convergence.o
	$(CXX) $(CXXFLAGS) -o ../bin/pokec_aved_convergence graph.o convergence.o pokec_aved_convergence.o
pokec_aved_convergence.o:./exp/pokec/pokec_aved_convergence.cpp ./main/graph.h ./main/convergence.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/pokec/pokec_aved_convergence.cpp
pokec_public_clusters_size:graph.o pokec_public_clusters_size.o
	$(CXX) $(CXXFLAGS) -o ../bin/pokec_public_clusters_size graph.o pokec_public_clusters_size.o
pokec_public_clusters_size.o:./exp/pokec/pokec_public_clusters_size.cpp ./main/graph.h ./exp/exp.h
	$(CXX) $(CXXFLAGS) -c ./exp/pokec/pokec_public_clusters_size.cpp

real_facebook: real_facebook_estimation

real_facebook_estimation:real_facebook_estimation.o graph.o estimation.o
	$(CXX) $(CXXFLAGS) -o ../bin/real_facebook_estimation real_facebook_estimation.o graph.o estimation.o
real_facebook_estimation.o:./exp/real_facebook/real_facebook_estimation.cpp ./main/graph.h ./main/estimation.h
	$(CXX) $(CXXFLAGS) -c ./exp/real_facebook/real_facebook_estimation.cpp

clean:
	rm *.o