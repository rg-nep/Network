//
// Created by shahnoor on 12/19/18.
//

#ifndef NETWORK_TEST_NETWORK_PERCOLATION_H
#define NETWORK_TEST_NETWORK_PERCOLATION_H

#include <iostream>
#include <algorithm>



void network_percolation(int argc, char* argv[]);
void  explosive_percolation_sum(
        uint m, uint network_size, uint M, uint ensemble_size, size_t th_id);
void network_percolation_explosive(int argc, char* argv[]);
void network_percolation_explosive_v2(int argc, char** argv);

void network_percolation_global();

void network_percolationReverse_global();

void BA_order_parameter_jump(int argc, char **argv); // for demarcation line
void BA_entropy_jump_ensemble(int argc, char **argv);

void clusterSizeDistribution(int argc, char **argv);// added 2019.06.22

#endif //NETWORK_NETWORK_PERCOLATION_H
