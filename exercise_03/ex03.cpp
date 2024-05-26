#include <iostream>
#include <cmath>
#include <vector>
#include <library03.h>

using namespace std;
using namespace filesystem;

int main(){

    /* setting # of throws and blocks */
    int n_throws = 1E4;
    int n_blocks{100};

    Random rnd;
    initRandom(rnd, paths::path_SEED);

    ofstream out;
    out.open(paths::path_DATA/"ex_03_call_direct.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_03_call_direct.dat");
        return -1;
    }

    BlockingDirectCall directcall;

    directcall.Calculate(n_throws, n_blocks, rnd, out);

    out.close();
    out.clear();

    out.open(paths::path_DATA/"ex_03_call_discrete.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_03_call_discrete.dat");
        return -1;
    }

    BlockingDiscreteCall discretecall;
    discretecall.Calculate(n_throws, n_blocks, rnd, out);

    out.close();
    out.clear();

    out.open(paths::path_DATA/"ex_03_put_direct.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_03_put_direct.dat");
        return -1;
    }

    BlockingDirectPut directput;
    directput.Calculate(n_throws, n_blocks, rnd, out);

    out.close();
    out.clear();

    out.open(paths::path_DATA/"ex_03_put_discrete.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_03_put_discrete.dat");
        return -1;
    }

    BlockingDiscretePut discreteput;
    discreteput.Calculate(n_throws, n_blocks, rnd, out);

    return 0;
}