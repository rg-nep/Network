#include <iostream>
#include <chrono>
#include "util/time_tracking.h"
#include "nets/network.h"
#include "test/network_percolation.h"
#include "test/network_build.h"
#include <algorithm>
#include <fstream>
#include <thread>


using namespace std;





void network_percolation_to_file(int argc, char* argv[]){
    uint m = atoi(argv[1]);
    uint network_size = atoi(argv[2]);
    uint ensemble_size = atoi(argv[3]);

    NetworkBApercolation net(3, m, network_size);
    net.reset();
    net.viewNodes();
    net.viewLinks();
    net.viewCluster();
    size_t i{};
    while (net.occupyLink()){
//        cout << " ************* **************** *************" << endl;
//        cout << i << "-th link " << _network_frame.lastLink() << endl;
//        cout << "p = " << _network_frame.occupationProbability() << endl;
        ++i;
        net.viewClusterExtended();
    }

}


void run_in_main(int argc, char* argv[]){
    test_network();
//    degree_distribution_BA(argc, argv);
//    network_percolation_to_file(argc, argv);
    //    degree_distribution_BA(argc, argv);
//      network_percolation(argc, argv);
//    network_percolation_explosive(argc, argv);

//    cout << "cores " << thread::hardware_concurrency() << endl;
//    explosive_percolation_sum(3, 5000, 5, 100, 0); // testing only

}

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    std::cout << "Network" << std::endl;
    cout << currentTime() << endl;
    cout << "Compiled on " << __DATE__ << "\t at " << __TIME__ << endl;
    auto t_start = std::chrono::system_clock::now();
    if(sizeof(uint) < 4){
        cout << "size of unsigned int is less than 4 byte : line " <<__LINE__ << endl;
        return 0;
    }
//    time_t seed = time(nullptr);
//    srand(seed);    // seeding


    run_in_main(argc, argv);


    auto t_end= std::chrono::system_clock::now();
    std::chrono::duration<double> drtion = t_end - t_start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(t_end);
    cout << "Program finished at " << std::ctime(&end_time) << endl;
    std::cout << "Time elapsed "   << getFormattedTime(drtion.count()) << std::endl;
    return 0;

}