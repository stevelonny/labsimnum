#include <iostream>
#include <cmath>
#include <vector>
#include <library02.h>

using namespace std;
using namespace filesystem;

int main(){

    /* setting # of throws and blocks */
    int n_throws = 1E4;
    int n_blocks{100};

    /* init rng */
    Random rnd;
    initRandom(rnd, paths::path_SEED);

    /* discrete walker */
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

    /* continuos walker */
    ContinueLattice conlattice;

    out.open(paths::path_DATA/"ex_02.2b.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_02.2b.dat");
        return -1;
    }

    conlattice.Calculate(n_throws, n_blocks, rnd, out);

    out.close();

    return 0;
}