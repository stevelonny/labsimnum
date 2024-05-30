#include <iostream>
#include <random.h>
#include <fmt/ostream.h>
#include <cmath>
#include <vector>
#include "version_config.h"
#include "library.h"

using namespace std;


int main (int argc, char *argv[]){
    
    Random rnd;
    initRandom(rnd, paths::path_SEED);

    int n_throws = 1e6;
    int n_blocks{100};

    double un_delta{1.2};
    double gauss_delta{0.75};
    double un_delta_f{3.};
    double gauss_delta_f{1.8};

    ofstream out;
    out.open(paths::path_DATA/"ex_05_ground_unif.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_05_ground_unif.dat");
        return -1;
    }

    Metro ground(0., 0., 1.5/2., un_delta);

    ground.Calculate(n_throws, n_blocks, rnd, out);

    out.close();
    out.clear();

    out.open(paths::path_DATA/"ex_05_ground_gauss.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_05_ground_gauss.dat");
        return -1;
    }

    std::shared_ptr<UniformStep> gauss = make_shared<GaussStep>(gauss_delta);
    Metro gaussground(0., 0., 1.5/2., gauss);

    gaussground.Calculate(n_throws, n_blocks, rnd, out);

    out.close();
    out.clear();

    out.open(paths::path_DATA/"ex_05_first_unif.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_05_first_unif.dat");
        return -1;
    }

    std::shared_ptr<Acceptance> acc = make_shared<FirstState>();
    Metro first(0., 0., 1.5/2., un_delta_f, acc);

    first.Calculate(n_throws, n_blocks, rnd, out);

    out.close();
    out.clear();

    out.open(paths::path_DATA/"ex_05_first_gauss.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_05_first_gauss.dat");
        return -1;
    }

    std::shared_ptr<UniformStep> gauss1 = make_shared<GaussStep>(gauss_delta_f);
    Metro firstgauss(0., 0., 1.5/2., acc, gauss1);

    firstgauss.Calculate(n_throws, n_blocks, rnd, out);

    out.close();
    
    return 0;
}
