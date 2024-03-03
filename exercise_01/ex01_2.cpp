#include <iostream>
#include <cmath>
#include <vector>
#include "library.h"

using namespace std;

int main(){

    vector<int> N {1, 2, 10, 100};
    int n_real = 1e4;
    double lambda{1.};
    double center{0.};
    double gamma{1.};
    
    double sum_unif;
    double sum_expo;
    double sum_cauchy;
    double current;


    Random rnd;
    initRandom(rnd, string(ROOT_PATH) + "/random/seed.in");

/*
    ofstream out_unif, out_expo, out_cauchy;

    string file_unif = fmt::format("{0}/data/ex_01.2unif.dat", string(ROOT_PATH));
    string file_expo = fmt::format("{0}/data/ex_01.2expo.dat", string(ROOT_PATH));
    string file_cauchy = fmt::format("{0}/data/ex_01.2cauchy.dat", string(ROOT_PATH));
    out_unif.open(file_unif);
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", file_unif);
        return -1;
    }
    out_expo.open(file_expo);
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", file_expo);
        return -1;
    }
    out_cauchy.open(file_cauchy);
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", file_cauchy);
        return -1;
    }
*/
/*
    FILE** out = new FILE*[N.size()]; 
    for(int i{0}; i<N.size(); i++){
        out[i] = fopen(string(fmt::format("{0}/data/ex_01.2s_{1}.dat", string(ROOT_PATH), N[i])).c_str(), "w+");
    }
*/

    //ofstream* out = new ofstream[N.size()];
    /* lets use some smart pointer and ofstream, we're writing in c++ anyway */
    unique_ptr<ofstream[]> out(new ofstream[N.size()]);
    for(int i{0}; i<N.size(); i++){
        string filename = fmt::format("{0}/data/ex_01.2s_{1}.dat", string(ROOT_PATH), N[i]);
        out[i].open(filename);
        if(!out[i].is_open()){
            fmt::print(cerr, "Error: unable to open {0}\n", filename);
            return -1;
        }
    }

    for(int i{0}; i<n_real; i++){
        sum_unif    = 0.;
        sum_expo    = 0.;
        sum_cauchy  = 0.;
        for(int s{0}; s<N.size(); s++){
            for(int d{0}; d<N[s]-((s<1)?0:N[s-1]) /* reduces the # of generations by reusing the previous one */; d++){
                current = rnd.Rannyu();
                sum_unif += current;
                sum_expo += expo(current, lambda);
                sum_cauchy += cauchy(current, center, gamma);
            }
        fmt::print(out[s], "{0}\t{1}\t{2}\n", sum_unif/(double)N[s], sum_expo/(double)N[s], sum_cauchy/(double)N[s]);
        }
    }

    //do i need to close files? should be automatically closed by the destructors...

    //delete[] out; /* no need to free mem from heap with smrt pointers! */
/*
    for(int s{0}; s<N; d++){
        for(int i{0}; i<n_real; i++){    
            for(int k{0}; k<N[s]-((s<0)?0:N[s-1]); k++){
                current = rnd.Rannyu();
                sum_unif += current;
                sum_expo += expo(current, lambda);
                sum_cauchy += cauchy(current, center, gamma);
            }
            fmt::print(out, "{0}\t{1}\t{2}\n", sum_unif/(double)N[s], sum_expo/(double)N[s], sum_cauchy/(double)N[s]);
        }
        out.close();
        out.clear();
    }
*/

/*     for(int s{0}; s < N.size(); s++){
        
        sum_unif = 0;
        sum_expo = 0;
        sum_cauchy = 0;
            
        for(int i{0}; i<n_real; i++){
            
            current = rnd.Rannyu();
            sum_unif += current;
            sum_expo += expo(current, lambda);
            sum_cauchy += cauchy(current, center, gamma);   
        }
        fmt::print(out, "{0}\t{1}\t{2}\n", sum_unif/(double)N[s], sum_expo/(double)N[s], sum_cauchy/(double)N[s]);

        out.close();
        out.clear();
    }*/

    return 0;
}