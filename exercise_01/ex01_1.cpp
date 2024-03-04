#include <iostream>
#include <cmath>
#include <vector>
#include "library.h"

using namespace std;
using namespace filesystem;

int main(int argc, const char** argv){

    /* setting the rnd generator */
    Random rnd;
    initRandom(rnd, paths::path_SEED);

    /* setting # of throws and blocks */
    int n_throws = 1E7;
    int n_blocks{100};

    /* average */
    BlockingAvg avg;

    ofstream out;
    out.open(paths::path_DATA/"ex_01.1a.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_01.1a.dat");
        return -1;
    }

    avg.Calculate(n_throws, n_blocks, rnd, out);

    out.close();

    /* variance */
    BlockingVar var;

    out.clear();
    out.open(paths::path_DATA/"ex_01.1b.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"/ex_01.1b.dat");
        return -1;
    }

    var.Calculate(n_throws, n_blocks, rnd, out);

    /* chi2 */
    out.close();

    out.clear();
    out.open(paths::path_DATA/"ex_01.1c.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex_01.1c.dat");
        return -1;
    }

    n_throws = 1E4;
    int m_intervals{100};
    vector<int> counter(m_intervals);
    double width = 1./(double)m_intervals;
    double current;
    double expected{(double)n_throws/(double)m_intervals};
    double chi;

    for(int k{0}; k<100; k++){
        //reset each cycle counters for throws fallen inside defined intervals    
        //reset chi2
        counter.assign(counter.size(), 0);
        chi = 0;
        for(int i{0}; i<n_throws; i++){
            current = rnd.Rannyu();
            // round to the nearest lower int
            int j{static_cast<int>(current/width)};
            counter[j] ++;
            /* 
            for(int j{0}; j<m_intervals; j++){
                counter[j] += ((current<(j+1)*width)?((current>j*width)?1:0):0); //yuk
            }
            */
        }
        for(int i{0}; i<m_intervals; i++){
            chi += pow(counter[i] - expected, 2)/expected;
        }
        fmt::print(out, "{0}\t{1}\n", k+1, chi);
    }

    out.close();

    return 0;
}