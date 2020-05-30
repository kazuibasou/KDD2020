#ifndef ESTIMATION_H
#define ESTIMATION_H

int Size_estimations(std::vector<SampledData> &samplinglist,const double ratio_m_r,std::vector<double> &estimations);

int Average_degree_estimations(std::vector<SampledData> &samplinglist,std::vector<double> &estimations);

#endif