#include <iostream>
#include <cmath>
#include <vector>
#include "library.h"

using namespace std;
using namespace filesystem;

int main(){

    /* setting # of throws and blocks */
    int n_throws = 1E4;
    int n_blocks{100};

    Random rnd;
    initRandom(rnd, paths::path_SEED);

    BlockingLattice dilattice;

    ofstream out;
    out.open(paths::path_DATA/"ex_02.2a.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_02.2a.dat");
        return -1;
    }

    dilattice.Calculate(n_throws, n_blocks, rnd, out);

    out.close();
    out.clear();

    ContinueLattice conlattice;

    out.open(paths::path_DATA/"ex_02.2b.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_02.2b.dat");
        return -1;
    }

    conlattice.Calculate(n_throws, n_blocks, rnd, out);

    return 0;
}