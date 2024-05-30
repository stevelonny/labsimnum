#include <iostream>
#include <random.h>
#include <fmt/ostream.h>
#include <cmath>
#include <vector>
#include "version_config.h"
#include "library08.h"

using namespace std;

int main (int argc, char *argv[]){
    
    Random rnd;
    initRandom(rnd, paths::path_SEED);

    int n_throws = 1e6;
    int n_blocks{100};

    double delta{2.5};

    ofstream block_out;
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

    auto step = std::make_shared<UniformStep>(delta);
    auto acc = std::make_shared<DoubleGaussian>();
    acc->SetParameters(0.811, 0.619);

    Metro polis(0., n_throws, n_blocks, acc, step);

    // equilibrate
    polis.Reset();
    int n_eq = 1e6;
    for(int i{0}; i<n_eq; i++){
        fmt::print("Equilibrating # {0}/{1}\r", i+1, n_eq);
        polis.Funny(rnd);
    }
    fmt::print("\n");
    std::fflush(stdout);

    // actual simualtion
    polis.Reset();
    for(int i{0}; i<polis.getNBlocks(); i++){
        fmt::print("Running block # {0}/{1}\r", i+1, n_blocks);
        for(int j{0}; j<polis.getLBlock(); j++){
            polis.Funny(rnd);
            fmt::print(coord_out, "{0}\n", polis.getActual());
        }
        polis.Average(rnd);
        //write out to file
        polis.Print(block_out, i);
    }
    fmt::print("\n");
    std::fflush(stdout);

    block_out.close(); 
    
    return 0;
}
