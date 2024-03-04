#include <iostream>
#include <cmath>
#include <vector>
#include "library.h"

using namespace std;
using namespace filesystem;

int main(){

    int n_throws = 1E7;
    int n_blocks{100};

    Random rnd;
    initRandom(rnd, paths::path_SEED);

    BlockingBuffon buffone(0.8, 1.);
    ofstream out;
    out.open(paths::path_DATA/"ex_01.3.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_01.3.dat");
        return -1;
    }

    buffone.Calculate(n_throws, n_blocks, rnd, out);

    return 0;
}