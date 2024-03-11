#include <iostream>
#include <cmath>
#include <vector>
#include "library.h"

using namespace std;
using namespace filesystem;

int main(){

    /* setting # of throws and blocks */
    int n_throws = 1E7;
    int n_blocks{100};

    Random rnd;
    initRandom(rnd, paths::path_SEED);

    Coseno coseno;

    BlockingMonte cosenounif(coseno, 0., 1.);

    ofstream out;
    out.open(paths::path_DATA/"ex_02.1a.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_02.1a.dat");
        return -1;
    }

    cosenounif.Calculate(n_throws, n_blocks, rnd, out);

    out.close();

    //Coseno coseno2;

    //MonteLin cosenolin(coseno, 0., 1.);

    MonteLin cosenolin(cosenounif);

    out.clear();
    out.open(paths::path_DATA/"ex_02.1b.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_02.1b.dat");
        return -1;
    }

    cosenolin.Calculate(n_throws, n_blocks, rnd, out);

    return 0;
}