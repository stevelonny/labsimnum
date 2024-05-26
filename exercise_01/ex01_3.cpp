#include <iostream>
#include <cmath>
#include <vector>
#include <library01.h>

using namespace std;
using namespace filesystem;

int main(){

    /* setting # of thtrows */
    int n_throws = 1E7;
    int n_blocks{100};

    /* init random */
    Random rnd;
    initRandom(rnd, paths::path_SEED);

    /* file output */
    ofstream out;
    out.open(paths::path_DATA/"ex01_3.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex01_3.dat");
        return -1;
    }

    /* average pi with buffon method and blocking avg */
    fmt::print("Calculating buffon's needle with {0} throws through {1} blocks\n", n_throws, n_blocks);    
    BlockingBuffon buffone(0.8, 1.);
    buffone.Calculate(n_throws, n_blocks, rnd, out);
    fmt::print("Output written in {0}\n", paths::path_DATA/"ex01_3.dat");

    return 0;
}