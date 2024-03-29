//
// Created by shahnoor on 12/19/18.
//

#include "test_network_percolation.h"
#include "../nets/network.h"
#include "../util/time_tracking.h"
#include "../percolation/network_percolation.h"
#include "../percolation/reverse/network_percolation_reverse.h"
#include "../percolation/network_percolation_template.h"
#include "../percolation/network_percolation_v3.h"
#include "../percolation/explosive/network_percolation_explosive.h"
#include "../percolation/network_percolation_v5.h"
#include "../percolation/explosive/network_percolation_explosive_v5.h"
#include <chrono>
#include <fstream>
#include <thread>

using namespace std;

/**
 * Date : 2019.05.17
 * @param argc
 * @param argv
 */
void BA_self_jump(int argc, char* argv[]){
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);

//    NetworkBApercolationExplosive_v3 net(m*2, m, N, M);
    NetworkBApercolation_v3 net(m*2, m, N);
    string signature = net.get_signature();
    string filename = signature + "jump-" + currentTime() + ".txt";



//    double t;
    double P{}, P_old{}, dP;
    bool c{false};


    vector<double> dP_list(net.linkCount()), color_list(net.linkCount());

    size_t i{1};

    net.occupyLink();
    P_old = net.largestClusterSize();
//    t = net.occupationProbability();
//    fout << t << '\t' << 0 << '\t' << c << endl;

    while (net.occupyLink()){
//        cout << " ************* **************** *************" << endl;

//        t = net.relativeLinkDensity();
        P = net.largestClusterSize();

        dP = abs(P- P_old);

        P_old = P;
        c = net.isSelfClusterJump();

        dP_list[i] = dP;
        color_list[i] = c;
        ++i;

//        fout << t << '\t' << P << '\t' << c << endl; // takes ~80% of the total time
//        net.viewClusterExtended();
//        net.viewCluster();
    }

    ofstream fout(filename);
    stringstream ss;
    ss   << "{\"signature\":" << "\"" << signature << "\""
         << ",\"m\":" << m << ",\"network_size\":" << N
         << ",\"M\":" << M  // in case of explosive percolation
         << ",\"number_of_links\":" << net.linkCount()
         << ",\"number_of_nodes\":" << net.nodeCount()
         << ",\"cols\":" << "[\"t\", \"dP\", \"color\"]" << "}";

    fout << "#" << ss.str() << endl;
    fout << "#n = number of occupied links" << endl;
    fout << "#t = n / N" << endl;
    fout << "#t = relative occupation probability" << endl;
    fout << "#cluster size = number of nodes in the cluster" << endl;
    fout << "#P = order parameter = largest cluster size / N" << endl;
    fout << "#N = network size = number of nodes in it" << endl;
    fout << "#S_max = largest cluster size" << endl;
    fout << "#dS = largest clusteter jump" << endl;
    fout << "#c = 0 or 1 (different cluster jump or self cluster jump)"<< endl;

    size_t sz = net.linkCount();
    size_t Norm = net.nodeCount();
    for(size_t i{}; i < sz; ++i){
        if (dP_list[i] != 0) {
            fout << (i+1)/double(Norm) << '\t' << dP_list[i] << '\t' << color_list[i] << endl;
        }
    }

    fout.close();
}


/**
 * average of 100 ensemble includeing colors
 * Date : 2019.05.22
 * @param argc
 * @param argv
 */
void BA_self_jump_avg(int argc, char* argv[]){
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);

    NetworkBApercolationExplosive_v3 net(m, m, N, M);
//    NetworkBApercolation_v3 net(m, m, N);


    double t;
    double P{}, P_old{}, dP;
    bool c{false};

    vector<double> dP_list(net.linkCount()), color_list(net.linkCount());

    size_t En = 100;
    size_t i{};
    for(size_t en{}; en < En; ++en) {
        net.reset(en % 10 == 0); // reset network in every 10 interval

        net.occupyLink();

        P_old = net.largestClusterSize();
        t = net.occupationProbability();

        i = 0;
        while (net.occupyLink()) {
//        cout << " ************* **************** *************" << endl;

            t = net.occupationProbability();
            P = net.largestClusterSize();

            dP = abs(P - P_old);

            P_old = P;
            c = net.isSelfClusterJump();

            dP_list[i] += dP;
            color_list[i] += c;

//        net.viewClusterExtended();
//        net.viewCluster();
        }


    }

    string signature = net.get_signature();
    string filename = signature + "jump_avg-" + currentTime() + ".txt";

    ofstream fout(filename);
    stringstream ss;
    ss   << "{\"signature\":" << "\"" << signature << "\""
         << ",\"m\":" << m << ",\"network_size\":" << N
         << ",\"M\":" << M  // in case of explosive percolation
         << ",\"number_of_links\":" << net.linkCount()
         << ",\"number_of_nodes\":" << net.nodeCount()
         << ",\"cols\":" << "[\"t\", \"dP\", \"color\"]" << "}";

    fout << "#" << ss.str() << endl;
    fout << "#n = number of occupied links" << endl;
    fout << "#t = n / N" << endl;
    fout << "#cluster size = number of nodes in the cluster" << endl;
    fout << "#P = order parameter = largest cluster size / N" << endl;
    fout << "#N = network size = number of nodes in it" << endl;
    fout << "#S_max = largest cluster size" << endl;
    fout << "#dS = largest clusteter jump" << endl;
    fout << "#c = 0 or 1 (different cluster jump or self cluster jump)"<< endl;


    double sz = dP_list.size();
    for(i=0; i < dP_list.size(); ++i){
        fout << (i+1)/sz << '\t' << dP_list[i]/En << "\t" << color_list[i]/En << endl;
    }

    fout.close();
}

void network_percolation(int argc, char* argv[]){

//    size_t m = 2;
//    size_t N = 10;
//    size_t M = 2;

    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);


    NetworkBApercolation_v3 net(m, m, N);
//    NetworkBApercolationExplosive_v3 net(m, m, N, M);
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();
//    net.reset(1);
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();

    size_t i{};
//    net.view_randomized_indices();
//    net.viewLinks();
    net.sizeSummary_in_MB();
    while (net.occupyLink()){
//        cout << " ************* **************** *************" << endl;
//        cout << i << "-th link " << net.lastLink() << endl;
//        cout << "relativeLinkDensity, t = " << net.relativeLinkDensity() << endl;
//        cout << "largestClusterSize = " << net.largestClusterSize() << endl;
//        cout << "largestClusterID " << net.largestClusterID() << endl;
//        cout << "is self jump = " << net.isSelfClusterJump() << endl;
//        cout << "number of clusters " << net.clusterCount() << endl;
        ++i;
//        net.viewClusterExtended();
//        net.viewCluster();
//        net.view_randomized_indices();
    }
    net.sizeSummary_in_MB();
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();
//    net.reset();
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();
//    net.time_summary();
    cin.get(); // pausing program
}



void  explosive_percolation_sum(uint m, uint network_size, uint M, uint ensemble_size, size_t th_id) {
    NetworkBApercolationExplosiveSum net(m, m, network_size, M);

    size_t sz = net.linkCount();
    double node_count = net.nodeCount();
    cout << sz << " " << node_count << endl;
    vector<size_t> largest_cluster_size(sz);
    vector<double> entropy1(sz), entropy2(sz);
    vector<double> entropy_jump(ensemble_size), entropy_jump_pc(ensemble_size);
    for (size_t k{}; k < ensemble_size; ++k) {
        auto t_start= chrono::_V2::system_clock::now();

        net.reset();
//        _network_frame.viewNodes();
//        _network_frame.viewLinks();
//        _network_frame.viewClusterExtended();

        size_t i{};
        while (net.occupyLink()) {
//            cout << " ************* **************** *************" << endl;
//            cout << i << "-th link " << _network_frame.lastLink() << endl;
//            cout << "p = " << _network_frame.occupationProbability() << endl;
//            cout << "P = " << _network_frame.largestClusterSize() << endl;
            largest_cluster_size[i] += net.largestClusterSize();
            entropy1[i] += net.entropy_v1();
            entropy2[i] += net.entropy_v2();
            net.jump();
//            cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
//            cout << entropy1[i] << " ==? " << entropy2[i] << endl;

            ++i;
//            _network_frame.viewClusterExtended();
        }
        entropy_jump[k] = net.largestEntropyJump();
        entropy_jump_pc[k] = net.largestEntropyJump_pc();
//        cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    string signature = net.get_signature();
    string filename = signature + currentTime() + "_t" + to_string(th_id);
    string filename_jump = signature + "_entropy_jump_" + currentTime() + "_t" + to_string(th_id);
    ofstream fout(filename);
    stringstream ss;
    ss   << "{\"signature\":" << "\"" << signature << "\""
         << ",\"m\":" << m << ",\"network_size\":" << network_size
         << ",\"number_of_links\":" << net.linkCount()
         << ",\"number_of_nodes\":" << net.nodeCount()
         << ",\"M\":" << M << ",\"ensemble_size\":" << ensemble_size << "}";

    fout << "#" << ss.str() << endl;
    fout << "#n = number of occupied links" << endl;
    fout << "#t = n / N" << endl;
    fout << "#cluster size = number of nodes in the cluster" << endl;
    fout << "#P = order parameter = largest cluster size / N" << endl;
    fout << "#N = network size = number of nodes in it" << endl;
    fout << "#S_max = largest cluster size" << endl;
    fout << "#H1 = entropy slow method" << endl;
    fout << "#H2 = entropy fast method" << endl;
    fout << "#<n>\t<S_max>\t<H1>\t<H2>"<< endl;
    for(size_t i{}; i < sz ; ++i){
        fout << (i+1)
             << '\t' << largest_cluster_size[i] / double(ensemble_size)
             << '\t' << entropy1[i] / double(ensemble_size)
             << '\t' << entropy2[i] / double(ensemble_size)
             << endl;
    }

    fout.close();

    ofstream fout_jump(filename_jump);
    fout_jump << ss.str() << endl;
    fout_jump << "#<largest entropy jump>\t<p>" << endl;
    for(size_t k{}; k < ensemble_size ; ++k){
        fout_jump << entropy_jump[k] << '\t' << entropy_jump_pc[k] << endl;
    }
    fout_jump.close();
}


void network_percolation_explosive(int argc, char* argv[]) {
    uint m = atoi(argv[1]);
    uint network_size = atoi(argv[2]);
    uint M = atoi(argv[3]);
    uint ensemble_size = atoi(argv[4]);
    cout << "IF ensemble_size == 100 " << " four network each with 25 iteration" << endl;
    cout << "m " << m << endl;
    cout << "N " << network_size << endl;
    cout << "M " << M << endl;
    cout << "ensemble size " << ensemble_size << endl;
    int number_of_thread = std::thread::hardware_concurrency();
    vector<thread> ths(number_of_thread);
    for(size_t i{}; i < number_of_thread ; ++i) {
        ths[i] = thread(
                explosive_percolation_sum,
                m,
                network_size,
                M,
                ensemble_size / number_of_thread,
                i
        );
        cout << "thread " << ths[i].get_id() << " started" << endl;
    }
    for(size_t i{}; i < number_of_thread ; ++i) {
        if(ths[i].joinable()){
            cout << "thread " << ths[i].get_id() << " finished" << endl;
            ths[i].join();

        }
    }


}

/*
 * m0=3
 * m=1
 * N=100000
 * BA took 0.22 sec
 * MDA took 2.42 sec  ???
 *
 */
void network_percolation_global(){
    NetworkPercolation<NetworkMDA> networkPercolation(10, 1, 1000000);
    cout << networkPercolation.get_signature() << endl;
//    networkPercolation.viewNodes();
//    networkPercolation.viewLinks();
//    networkPercolation.viewCluster();
//    networkPercolation.viewClusterExtended();

    int i=0;
    while(networkPercolation.occupyLink()){
//        cout << "iteration " << ++i << "****************************" << endl;
//        cout << "last link " << networkPercolation.lastLink() << endl;
//        networkPercolation.viewNodes();
//        networkPercolation.viewClusterExtended();
    }
    cout << "time " << networkPercolation.time() << " sec" << endl;

}

void network_percolationReverse_global(){
    NetworkPercolationReverse<NetworkBA> networkPercolation(3, 1, 10);
    cout << networkPercolation.get_signature() << endl;
    networkPercolation.viewNodes();
    networkPercolation.viewLinks();
    networkPercolation.viewCluster();
//    networkPercolation.viewClusterExtended();

    int i=0;
    while(networkPercolation.occupyLink()){
//        cout << "iteration " << ++i << "****************************" << endl;
//        cout << "last link " << networkPercolation.lastLink() << endl;
//        networkPercolation.viewNodes();
//        networkPercolation.viewClusterExtended();
    }
    cout << "time " << networkPercolation.time() << " sec" << endl;

}


void test_NetworkBApercolationExplosive_v3(int argc, char **argv){
    if(argc < 5 ){
        cout << "argv[1] == m" << endl;
        cout << "argv[2] == N" << endl;
        cout << "argv[3] == M" << endl;
        cout << "argv[4] == Ensemble" << endl;
        return;
    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);
    int ensemble_size= atoi(argv[4]);
    NetworkBApercolationExplosive_v3 net(m, m, N, M);
    net.setRandomState(0, false);
    net.initialize_network();


    size_t sz = net.linkCount();
    double node_count = net.nodeCount();
    cout << sz << " " << node_count << endl;
    vector<size_t> largest_cluster_size(sz);
    vector<double> entropy1(sz), entropy2(sz);
    vector<double> entropy_jump(ensemble_size), entropy_jump_pc(ensemble_size);
    for (size_t k{}; k < ensemble_size; ++k) {
        auto t_start= chrono::_V2::system_clock::now();

        net.reset();
//        _network_frame.viewNodes();
//        _network_frame.viewLinks();
//        _network_frame.viewClusterExtended();

        size_t i{};
        while (net.occupyLink()) {
//            cout << " ************* **************** *************" << endl;
//            cout << i << "-th link " << _network_frame.lastLink() << endl;
//            cout << "p = " << _network_frame.occupationProbability() << endl;
//            cout << "P = " << _network_frame.largestClusterSize() << endl;
            largest_cluster_size[i] += net.largestClusterSize();
            entropy1[i] += net.entropy_v1();
            entropy2[i] += net.entropy_v2();
            net.jump();
//            cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
//            cout << entropy1[i] << " ==? " << entropy2[i] << endl;

            ++i;
//            _network_frame.viewClusterExtended();
        }
        entropy_jump[k] = net.largestEntropyJump();
        entropy_jump_pc[k] = net.largestEntropyJump_pc();
//        cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    string signature = net.get_signature();
    string filename = signature + currentTime();
    string filename_jump = signature + "_entropy_jump_" + currentTime();
    ofstream fout(filename);
    stringstream ss;
    ss   << "{\"signature\":" << "\"" << signature << "\""
         << ",\"m\":" << m << ",\"network_size\":" << N
         << ",\"number_of_links\":" << net.linkCount()
         << ",\"number_of_nodes\":" << net.nodeCount()
         << ",\"M\":" << M << ",\"ensemble_size\":" << ensemble_size << "}";

    fout << "#" << ss.str() << endl;
    fout << "#n = number of occupied links" << endl;
    fout << "#t = n / N" << endl;
    fout << "#cluster size = number of nodes in the cluster" << endl;
    fout << "#P = order parameter = largest cluster size / N" << endl;
    fout << "#N = network size = number of nodes in it" << endl;
    fout << "#S_max = largest cluster size" << endl;
    fout << "#H1 = entropy slow method" << endl;
    fout << "#H2 = entropy fast method" << endl;
    fout << "#<n>\t<S_max>\t<H1>\t<H2>"<< endl;
    for(size_t i{}; i < sz ; ++i){
        fout << (i+1)
             << '\t' << largest_cluster_size[i] / double(ensemble_size)
             << '\t' << entropy1[i] / double(ensemble_size)
             << '\t' << entropy2[i] / double(ensemble_size)
             << endl;
    }

    fout.close();

    ofstream fout_jump(filename_jump);
    fout_jump << ss.str() << endl;
    fout_jump << "#<largest entropy jump>\t<p>" << endl;
    for(size_t k{}; k < ensemble_size ; ++k){
        fout_jump << entropy_jump[k] << '\t' << entropy_jump_pc[k] << endl;
    }
    fout_jump.close();

}



void test_NetworkBApercolationExplosive_v3_jump(int argc, char **argv){
    if(argc < 5 ){
        cout << "argv[1] == m" << endl;
        cout << "argv[2] == N" << endl;
        cout << "argv[3] == M" << endl;
        cout << "argv[4] == Ensemble" << endl;
        return;
    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);
    int ensemble_size = atoi(argv[4]);
    NetworkBApercolationExplosive_v3 net(m, m, N, M);
    net.setRandomState(0, true);
    net.initializeNetwork();

    cout << net.nodeCount()  << ", " << net.linkCount() << ", " << endl;
    vector<double> entropy_jump(ensemble_size), entropy_jump_pc(ensemble_size);
    for (size_t k{}; k < ensemble_size; ++k) {
        auto t_start= chrono::_V2::system_clock::now();

        net.reset();
//        _network_frame.viewNodes();
//        _network_frame.viewLinks();
//        _network_frame.viewClusterExtended();

        size_t i{};
        while (net.occupyLink()) {
            net.entropy_v2();
            net.jump();
            ++i;
//            _network_frame.viewClusterExtended();
        }
        entropy_jump[k] = net.largestEntropyJump();
        entropy_jump_pc[k] = net.largestEntropyJump_pc();
//        cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    auto tm = currentTime();
    string signature = net.get_signature();
    string filename_jump = signature + "_entropy_jump_" + tm;
    stringstream ss;
    ss << "{"
         << R"*("signature":")*" << signature << "\""
         << R"*(,"m":)*" << m
         << R"*(,"network_size":)*" << N
         << R"*(,"number_of_links":)*" << net.linkCount()
         << R"*(,"number_of_nodes":)*" << net.nodeCount()
         << R"*(,"M":)*" << M
         << R"*(,"ensemble_size":)*" << ensemble_size
         << R"*(,"date":")*" << tm << "\""
         << "}";

    ofstream fout_jump(filename_jump);
    fout_jump << '#' << ss.str() << endl;
    fout_jump << "#<largest entropy jump>\t<p>" << endl;
    for(size_t k{}; k < ensemble_size ; ++k){
        fout_jump << entropy_jump[k] << '\t' << entropy_jump_pc[k] << endl;
    }
    fout_jump.close();

}

// 2019.10.02
void test_NetworkBA_v2(int argc, char **argv){
    if(argc < 3 ){
        cout << "argv[1] == m" << endl;
        cout << "argv[2] == N" << endl;
        return;
    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    NetworkBA_v2 net(m, m);
    net.setRandomState(0, false);
    net.grow(N);
    cout << net.getLinkCount() << endl;
    cout << net.getNodeCount() << endl;
    net.view();
}

void test_v5(int argc, char **argv) {
    if(argc < 5 ){
        cout << "argv[1] == m" << endl;
        cout << "argv[2] == N" << endl;
        cout << "argv[3] == M" << endl;
        cout << "argv[4] == Ensemble" << endl;
        return;
    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);

    int ensemble_size = atoi(argv[4]);


//    NetworkBApercolation_v5 net(m, m, N);
    NetworkBApercolationExplosive_v5 net(m, m, N, M);
    net.setRandomState(0, true);
    net.initializeNetwork();

    size_t linkCount = net.linkCount();
    size_t nodeCount = net.nodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
//    net.viewNetwork();
//    net.viewListOfLinkIndices();
    vector<double> entropy_jump(ensemble_size), entropy_jump_pc(ensemble_size);
    vector<double> entropy(linkCount), order_param(linkCount); // entropy and order parameter
    for (size_t k{0}; k < ensemble_size; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
//        net.viewListOfLinkIndices();
        net.reset(k%25 == 0); // every 25 step. reset the network

        size_t i{};
//        net.viewClusters();
//        net.viewListOfLinkIndices();
//        net.viewNetwork();
//        cout << "entering to while" << endl;
        while (net.occupyLink()) {
//            cout << "i " << i  << endl;
            entropy[i] += net.entropy();
            order_param[i] += net.largestClusterSize();
//            net.viewClusters();
//            cout << net.entropy_v1()  << "\t";
//            cout << net.entropy_v2() << endl;
//            cout << net.largestClusterSize() << endl;
            net.jump();
//            _network_frame.viewClusterExtended();
            ++i;
//            if (i == 7) break;
        }
        entropy_jump[k] = net.largestEntropyJump();
        entropy_jump_pc[k] = net.largestEntropyJump_pc();
//        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    auto tm = currentTime();
    string signature = net.get_signature();
    string filename_jump = signature + "_entropy_jump_" + tm;
    stringstream ss;
    ss << "{"
       << R"*("signature":")*" << signature << "\""
       << R"*(,"class":")*" << net.getClassName() << "\""
       << R"*(,"m":)*" << m
       << R"*(,"network_size":)*" << N
       << R"*(,"number_of_links":)*" << net.linkCount()
       << R"*(,"number_of_nodes":)*" << net.nodeCount()
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << ensemble_size
       << R"*(,"date":")*" << tm << "\""
       << "}";

    ofstream fout_jump(filename_jump);
    fout_jump << '#' << ss.str() << endl;
    fout_jump << "#<largest entropy jump>\t<p>" << endl;
    for(size_t k{}; k < ensemble_size ; ++k){
        fout_jump << entropy_jump[k] << '\t' << entropy_jump_pc[k] << endl;
    }
    fout_jump.close();


    string filename = signature + "_entropy-order_" + tm;
    ofstream fout(filename);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "#<t>\t<H>\t<P>" << endl;
    for(size_t k{}; k < entropy.size() ; ++k){
        fout << (k+1)/double(N)
             << "\t" << entropy[k]/ensemble_size
             << "\t" << order_param[k]/(ensemble_size*double(N)) << endl;
    }
    fout.close();

}

void test_v3(int argc, char **argv) {
    if(argc < 5 ){
        cout << "argv[1] == m" << endl;
        cout << "argv[2] == N" << endl;
        cout << "argv[3] == M" << endl;
        cout << "argv[4] == Ensemble" << endl;
        return;
    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);

    int ensemble_size = atoi(argv[4]);


//    NetworkBApercolation_v3 net(m, m, N);
    NetworkBApercolationExplosive_v3 net(m, m, N, M);
    net.setRandomState(0, true);
    net.initializeNetwork();

    size_t linkCount = net.linkCount();
    size_t nodeCount = net.nodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
//    net.viewNetwork();
//    net.viewListOfLinkIndices();
    vector<double> entropy_jump(ensemble_size), entropy_jump_pc(ensemble_size);
    vector<double> entropy(linkCount), order_param(linkCount); // entropy and order parameter
    for (size_t k{0}; k < ensemble_size; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
//        net.viewListOfLinkIndices();
        net.reset(k%25 == 0); // every 25 step. reset the network

        size_t i{};
//        net.viewClusters();
//        net.viewListOfLinkIndices();
//        net.viewNetwork();
//        cout << "entering to while" << endl;
        while (net.occupyLink()) {
//            cout << "i " << i  << endl;
            entropy[i] += net.entropy();
            order_param[i] += net.largestClusterSize();
//            net.viewClusters();
//            cout << net.entropy_v1()  << "\t";
//            cout << net.entropy_v2() << endl;
//            cout << net.largestClusterSize() << endl;
            net.jump();
//            _network_frame.viewClusterExtended();
            ++i;
//            if (i == 7) break;
        }
        entropy_jump[k] = net.largestEntropyJump();
        entropy_jump_pc[k] = net.largestEntropyJump_pc();
//        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    auto tm = currentTime();
    string signature = net.get_signature();
    string filename_jump = signature + "_entropy_jump_" + tm;
    stringstream ss;
    ss << "{"
       << R"*("signature":")*" << signature << "\""
       << R"*(,"class":")*" << net.getClassName() << "\""
       << R"*(,"m":)*" << m
       << R"*(,"network_size":)*" << N
       << R"*(,"number_of_links":)*" << net.linkCount()
       << R"*(,"number_of_nodes":)*" << net.nodeCount()
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << ensemble_size
       << R"*(,"date":")*" << tm << "\""
       << "}";

    ofstream fout_jump(filename_jump);
    fout_jump << '#' << ss.str() << endl;
    fout_jump << "#<largest entropy jump>\t<p>" << endl;
    for(size_t k{}; k < ensemble_size ; ++k){
        fout_jump << entropy_jump[k] << '\t' << entropy_jump_pc[k] << endl;
    }
    fout_jump.close();


    string filename = signature + "_entropy-order_" + tm;
    ofstream fout(filename);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "#<t>\t<H>\t<P>" << endl;
    for(size_t k{}; k < entropy.size() ; ++k){
        fout << (k+1)/double(N)
             << "\t" << entropy[k]/ensemble_size
             << "\t" << order_param[k]/(ensemble_size*double(N)) << endl;
    }
    fout.close();

}