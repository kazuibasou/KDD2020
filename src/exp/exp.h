#ifndef EXP_H
#define EXP_H

class Settings{
	public:
		//The ratio of m to r. m is a parameter in size estimator. r is a length of a random walk.
		double ratio_m_r;
		//The sample size of a random walk on the experiments of asm.
		double asm_samplerate;
		//The sequence of sample size of a random walk on the experiments of pokec.
		std::vector<double> pokec_samplerates;
		//The number of runs.
		int numrun;
		//The sequence of probability p.
		std::vector<double> ps;

		//specify experimental settings here.
		Settings(){

			ratio_m_r = 0.025;

			asm_samplerate = 0.01;

			pokec_samplerates.clear();
			for(int i=0;i<10;++i){
				pokec_samplerates.push_back(0.005*(i+1));
			}
			
			numrun = 1000;
			
			ps.clear();
			for(int i=0;i<11;++i){
				ps.push_back(double(0.03*i));
			}
		}

		~Settings(){
			std::vector<double>().swap(ps);
			std::vector<double>().swap(pokec_samplerates);
		}
};

#endif