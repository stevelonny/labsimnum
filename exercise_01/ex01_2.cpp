#include <iostream>
#include <cmath>
#include <vector>
#include <library01.h>

using namespace std;
using namespace filesystem;

int main(){

    /* setting # of realization and dist parameters */
    vector<size_t> N {1, 2, 10, 100};
    size_t n_real = 1e4;
    double lambda{1.};
    double center{0.};
    double gamma{1.};
    /* aux variables */
    double sum_unif;
    double sum_expo;
    double sum_cauchy;
    double current;

    /* init rng */
    Random rnd;
    initRandom(rnd, paths::path_SEED);

    /* file output */
    /* lets use some smart pointer and ofstream, we're writing in c++ anyway */
    unique_ptr<ofstream[]> out(new ofstream[N.size()]);
    for(int i{0}; i<N.size(); i++){
        path filepath = paths::path_DATA / fmt::format(string("ex01_2s_{}.dat"), N[i]);
        out[i].open(filepath);
        if(!out[i].is_open()){
            fmt::print(cerr, "Error: unable to open {0}\n", filepath);
            return -1;
        }
    }

    /* generate distributions */
    fmt::print("Generating distributions with {0} realizations\n", n_real);
    for(size_t i{0}; i<n_real; i++){
        sum_unif    = 0.;
        sum_expo    = 0.;
        sum_cauchy  = 0.;
        for(size_t s{0}; s<N.size(); s++){
            for(size_t d{0}; d<N[s]-((s<1)?0:N[s-1]) /* reduces the # of generations by reusing the previous one */; d++){
                current = rnd.Rannyu();
                sum_unif += current;
                sum_expo += exponential_distribution(current, lambda);
                sum_cauchy += cauchy_distribution(current, center, gamma);
            }
        fmt::print(out[s], "{0}\t{1}\t{2}\n", sum_unif/(double)N[s], sum_expo/(double)N[s], sum_cauchy/(double)N[s]);
        }
    }
    fmt::print("Output written in {0}\n", string(paths::path_DATA.c_str()) + "ex01_2s_*.dat");
    //do i need to close files? should be automatically closed by the destructors...

    //delete[] out; /* no need to free mem from heap with smrt pointers! */

    return 0;
}