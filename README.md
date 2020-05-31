# Estimating Properties of Social Networks via Random Walk considering Private Nodes

Here we describe how to reproduce the experimental results shown in the paper, “Estimating Properties of Social Networks via Random Walk considering Private Nodes”, accepted to *KDD2020 (full paper)*. 
This repository contains the source codes that were used to generate the experimental results in the paper. 
All the source codes are implemented in C++. 

## Requirements

Require gcc Version 4.2.1 (or later).

We have confirmed that our source codes work on the following environments.

- Mac OS X 10.13.6
- Ubuntu 16.04.3 LTS
- Windows 10 Version 1903

## Build
**(i)** download this repository and then execute the following three commands at `KDD2020` in order.

	mkdir data
	mkdir data/exp
	mkdir bin

**(ii)** download [graph](https://www.dropbox.com/s/ttn1otryrt4rgku/graph.zip?dl=0) and unzip at `KDD2020/data`. 

**(iii)** execute the `make` command at `KDD2020/src` to make **14** executable files in `KDD2020/bin`. 

Finally, the structure of the directory is as follows:

	KDD2020
	├ src
	  ├ main
	  ├ exp
	    ├ statistics
	    ├ asm
	    ├ pokec
	    ├ real_facebook
	├ data
	  ├ graph
	  ├ exp
	├ bin

## Usage
Execute the corresponding command at  `KDD2020/bin` to run the desired experiment. An output file of each experimental result is made at `KDD2020/data/exp`. The command to run, output, arguments, command arguments, experimental settings of each experiment are as follows (14 experiments in total).

### `./calc_alpha_p <graph>`
Return the coefficients *alpha_p* of a target graph for specified values of *p*. The coefficient *alpha_p* is defined in Theorem 4.6 in the paper. 

- **Arguments:**

	- *graph* - youtube, orkut, facebook, livejournal, or pokec 

- **Experimental settings:** 
	
	- The values of *p* are from 0.0 to 0.3 in increments of 0.03.

- **Example:**

		./calc_alpha_p youtube

### `./calc_degree_dist <graph>`
Return the cumulative degree distribution of a target graph.

- **Arguments:**

	- *graph* - youtube, orkut, facebook, livejournal, or pokec 

- **Experimental settings:** 
	
	- Nothing

- **Example:**

		./calc_degree_dist youtube

### `./asm_size_estimation <graph> <access model>`
Return the NRMSEs of each size estimator for specified values of *p* in a specified access model on a target graph. 

- **Arguments:**

	- *graph* - youtube, orkut, facebook, or livejournal
	- *access model* – ideal or hidden

- **Experimental settings:** 
	
	- The sample size is 1% of the total number of nodes. 
	- The probabilities of *p* are from 0.0 to 0.3 in increments of 0.03.
	- The threshold in size estimator is 2.5% of the sample size.
	- The number of runs is 1000.

- **Example:**

		./asm_size_estimation youtube ideal

### `./asm_size_convergence <graph>`
Return the NRMSEs of the convergence value of each size estimator for specified values of *p* on a target graph.

- **Arguments:**

	- *graph* - youtube, orkut, facebook, or livejournal

- **Experimental settings:** 
	
	- The probabilities of *p* are from 0.0 to 0.3 in increments of 0.03.
	- The number of runs is 1000.

- **Example:**

		./asm_size_convergence youtube

### `./asm_aved_estimation <graph> <access model>`
Return the NRMSEs of each average degree estimator for specified values of $$p$$ in a specified access model on a target graph. 

- **Arguments:**

	- *graph* - youtube, orkut, facebook, or livejournal
	- *access model* – ideal or hidden

- **Experimental settings:** 
	
	- The sample size is 1% of the total number of nodes. 
	- The probabilities of *p* are from 0.0 to 0.3 in increments of 0.03.
	- The number of runs is 1000.

- **Example:**

		./asm_aved_estimation youtube ideal

### `./asm_aved_convergence  <graph>`
Return the NRMSEs of the convergence value of each average degree estimator for specified values of *p* on a target graph.

- **Arguments:**

	- *graph* - youtube, orkut, facebook, or livejournal

- **Experimental settings:** 
	
	- The probabilities of *p* are from 0.0 to 0.3 in increments of 0.03.
	- The number of runs is 1000.

- **Example:**

		./asm_aved_convergence youtube

### `./asm_hidden_compare <graph>`
Return the NRMSEs of the proposed size estimator and the proportions of queried nodes using each exact and proposed weighting method for specified values of *p* in the hidden privacy model on a target graph.

- **Arguments:**

	- *graph* - youtube, orkut, facebook, or livejournal

- **Experimental settings:** 
	
	- The sample size is 1% of the total number of nodes. 
	- The probabilities of *p* are from 0.0 to 0.3 in increments of 0.03.
	- The threshold in size estimator is 2.5% of the sample size.
	- The number of runs is 1000.

- **Example:**

		./asm_hidden_compare youtube

### `./asm_public_clusters_size <graph>`
Return the relative sizes of the largest public-cluster and the average absolute sizes of isolated public-clusters of a target graph for specified values of *p*.

- **Arguments:**

	- *graph* - youtube, orkut, facebook, or livejournal

- **Experimental settings:** 
	
	- The probabilities of *p* are from 0.0 to 0.3 in increments of 0.03.
	- The number of runs is 1000.

- **Example:**

		./asm_public_clusters_size youtube

### `./pokec_size_estimation <access model>`
Return the NRMSEs of estimates of each size estimator for specified sample sizes on the Pokec graph.

- **Arguments:**

	- *access model* – ideal or hidden

- **Experimental settings:** 
	
	- The sample sizes from 0.5% to 5% in increments of 0.5% of the total number of nodes. 
	- The threshold in size estimator is 2.5% of the sample size.
	- The number of runs is 1000.

- **Example:**

		./pokec_size_estimation ideal

### `./pokec_size_convergence`
Return the relative error of the convergence value of each size estimator on the Pokec graph. 

- **Experimental settings:** 
	
	- Nothing

### `./pokec_aved_estimation <access model>`
Return the NRMSEs of estimates of each average degree estimator for specified sample sizes on the Pokec graph.

- **Arguments:**

	- *access model* – ideal or hidden

- **Experimental settings:** 
	
	- The sample sizes from 0.5% to 5% in increments of 0.5% of the total number of nodes.
	- The number of runs is 1000.

- **Example:**
	
		./pokec_aved_estimation ideal

### `./pokec_aved_convergence`
Return the relative error of the convergence value of each average degree estimator on the Pokec graph. 

- **Experimental settings:** 
	
	- Nothing

### `./pokec_public_clusters_size`
Return the relative size of the largest public-cluster and the average absolute size of the isolated public-clusters of the Pokec graph.

- **Experimental settings:** 
	
	- Nothing

### `./real_facebook_estimation`
Return the estimates of the size and average degree of Facebook obtained from the samples of real public Facebook users via random walk.

- **Experimental settings:** 
	
	- The threshold in size estimator is 2.5% of the sample size.

## Notes
- All the experiments in the paper were conducted on a Linux server with Intel Xeon E5-2698 (2.20 GHz) CPU and 503 GB of main memory.
- Note that it took approximately two weeks to complete all the experiments with the original experimental settings: `asm_public_clusters_size` took approximately two weeks, and other experiments were completed within several days at most.
- We can try various experimental settings by changing the corresponding parameters in `KDD2020/src/exp/exp.h`. However, we should be careful when changing the experimental settings, as the source codes are not implemented with exhaustive exception handling (e.g., assume that the value of *p* is implicitly between 0 and 1.).


## Reference

Kazuki Nakajima, Kazuyuki Shudo. “Estimating Properties of Social Networks via Random Walk considering Private Nodes”, *The 26th ACM SIGKDD International Conference on Knowledge Discovery & Data Mining ([KDD 2020](https://www.kdd.org/kdd2020/))*, August 2020.


## License

This source code is released under the MIT License, see LICENSE.txt.


## Contact
- Kazuki Nakajima (https://kazuibasou.github.io/index_en.html)
- nakajima.k.an[at]m.titech.ac.jp

(Last update: 2020/05/30)










