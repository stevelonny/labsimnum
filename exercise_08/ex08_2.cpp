#include <iostream>
#include <random.h>
#include <fmt/ostream.h>
#include <cmath>
#include <vector>
#include "version_config.h"
#include "library08.h"

#define STARTING_TEMP 3.
#define N_THROWS 1e6
#define N_BLOCKS 10
#define DELTA 2.5
#define DELTA_MU 1.
#define DELTA_SIGMA 0.5

using namespace std;

int main (int argc, char *argv[]){
    
    Random rnd;
    initRandom(rnd, paths::path_SEED);

    int n_throws = N_THROWS;
    int n_blocks{N_BLOCKS};

    double delta{DELTA};


    auto step = std::make_shared<UniformStep>(delta);
    // auto d_acc = std::make_shared<BoltzDoubleGauss>();
    // std::shared_ptr<DoubleGaussian> acc(d_acc);
    auto acc = std::make_shared<DoubleGaussian>();
    acc->SetParameters(1., 0.5);

    Metro polis(0., n_throws, n_blocks, acc, step);

    // equilibrate
    fmt::print("Equilibrating...\n");
    polis.Reset();
    int n_eq = 1e6;
    for(int i{0}; i<n_eq; i++){
        // fmt::print("Equilibrating # {0}/{1}\r", i+1, n_eq);
        polis.Funny(rnd);
    }
    // fmt::print("\n");
    // std::fflush(stdout);

    // actual simualtion
    double T{STARTING_TEMP};
    double beta{1./T};

    acc->SetParameters(1., 1.);
    // d_acc->setBeta(beta);
    double delta_mu{DELTA_MU};
    double delta_sigma{DELTA_SIGMA};
    double best_mu{1.};
    double best_sigma{1.};
    double best_energy{0.};
    double current_mu{1.};
    double current_sigma{1.};
    double next_mu{1.};
    double next_sigma{1.};
    double current_energy{0.};
    double next_energy{0.};
    double accepted{0.};
    polis.Reset();
    for(int i{0}; i<polis.getNBlocks(); i++){
        fmt::print("Running block # {0}/{1}\r", i+1, n_blocks);
        for(int j{0}; j<polis.getLBlock(); j++){
            polis.Funny(rnd);
            // fmt::print(coord_out, "{0}\n", polis.getActual());
        }
        polis.Average(rnd);
    }
    // fmt::print("\n");
    std::fflush(stdout);
    accepted = polis.getAccepted();
    current_energy = polis.getAvg();
    
    string filename = fmt::format("{0}/ex08_values.dat", paths::path_DATA.string());
    ofstream fileout(filename.c_str());
    fmt::print(fileout, "Temp\tMu\tSigma\tAcceptance\tEnergy\t\n");
    fmt::print("Temp: {0:<.3f}, Mu: {1:<.3f}, Sigma: {2:<.3f}, ,Acceptance: {3:<.2f}, Energy: {4:<.3f}\n", T, current_mu, current_sigma, accepted/(double)n_throws, current_energy);
    fmt::print(fileout, "{0:<.3f}\t{1:<.3f}\t{2:<.3f}\t{3:<.2f}\t{4:<.3f}\n", T, current_mu, current_sigma, accepted/(double)n_throws, current_energy);

    polis.Reset();
    while(T>0.001){
        // dont know if its better to use the previous "current" parameters or search around the current "best"
        // ive tried also adjusting the way the deltas are updated, with a sqrt(T) factor (remember to normalize)
        // could implement a check for when the deltas goes out of bounds
        delta_mu = delta_mu>current_mu ? current_mu : DELTA_MU; // this doesnt work well if mu or sigma are small values, as the step cannot go very far
        delta_sigma = delta_sigma>current_sigma ? current_sigma : DELTA_SIGMA;
        next_mu = abs(current_mu + rnd.Rannyu(-1.,1.)*delta_mu*((T))/(STARTING_TEMP));
        next_sigma = abs(current_sigma + rnd.Rannyu(-1., 1.)*delta_sigma*(T)/(STARTING_TEMP));
        acc->SetParameters(next_mu, next_sigma);
        // d_acc->setBeta(beta);
        polis.Reset();
        for(int i{0}; i<polis.getNBlocks(); i++){
            // fmt::print("Running block # {0}/{1}\r", i+1, n_blocks);
            for(int j{0}; j<polis.getLBlock(); j++){
                polis.Funny(rnd);
                // fmt::print(coord_out, "{0}\n", polis.getActual());
            }
            polis.Average(rnd);
            //write out to file
            // polis.Print(block_out, i);
        }
        // fmt::print("\n");
        // std::fflush(stdout);
        accepted = polis.getAccepted();
        next_energy = polis.getAvg();
        isnan(next_energy) ? next_energy = 0. : next_energy;
        if(next_energy < best_energy){
            best_mu = next_mu;
            best_sigma = next_sigma;
            best_energy = next_energy;
        }
        double alpha{min(1., exp(-beta*(next_energy-current_energy)))};
        double r{rnd.Rannyu()};
        if(r<alpha){
            current_mu = next_mu;
            current_sigma = next_sigma;
            current_energy = next_energy;

        }
        else{
            next_mu = current_mu;
            next_sigma = current_sigma;
            next_energy = current_energy;
        }

        fmt::print(cerr, "Temp: {0:<.3f}, Mu: {1:<.3f}, Sigma: {2:<.3f}, ,Acceptance: {3:<.2f}, Energy: {4:<.3f}\n",T, next_mu, next_sigma, accepted/(double)n_throws, next_energy);
        fmt::print(fileout, "{0:<.3f}\t{1:<.3f}\t{2:<.3f}\t{3:<.2f}\t{4:<.3f}\n", T, next_mu, next_sigma, accepted/(double)n_throws, next_energy);
        fmt::print("Best parameters: Mu: {0:<.3f}, Sigma: {1:<.3f}, Energy: {2:<.3f}\r", best_mu, best_sigma, best_energy);
        std::fflush(stdout);
        T *= 0.995;
        beta = 1./T;
    }

    fmt::print("Best parameters: Mu: {0:<.3f}, Sigma: {1:<.3f}, Energy: {2:<.3f}\n", best_mu, best_sigma, best_energy);
    fileout.close();
/*     ofstream block_out;
    string filename{paths::path_DATA/"ex08_block.dat"};
    block_out.open(filename);
    if(!block_out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", filename);
        return -1;
    }

    ofstream coord_out;
    filename = paths::path_DATA/"ex08_coord.dat";
    coord_out.open(filename);
    if(!coord_out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", filename);
        return -1;
    }

    block_out.close();  */
    
    return 0;
}
