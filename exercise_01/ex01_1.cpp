#include <iostream>
#include <cmath>
#include <vector>
#include <library01.h>

using namespace std;
using namespace filesystem;

int main(int argc, const char** argv){

    /* setting the rnd generator */
    Random rnd;
    initRandom(rnd, paths::path_SEED);

    /* setting # of throws and blocks */
    int n_throws = 1E6;
    int n_blocks{100};

    /* average */
    BlockingAvg avg;

    ofstream out;
    out.open(paths::path_DATA/"ex01_1a.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex01_1a.dat");
        return -1;
    }

    fmt::print("Calculating average of rannyu distribution with {0} throws through {1} blocks\n", n_throws, n_blocks);
    avg.Calculate(n_throws, n_blocks, rnd, out);
    fmt::print("Output written in {0}\n", paths::path_DATA/"ex01_1a.dat");
    out.close();

    /* variance */
    BlockingVar var;

    out.clear();
    out.open(paths::path_DATA/"ex01_1b.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"/ex01_1b.dat");
        return -1;
    }

    fmt::print("Calculating variance of rannyu distribution with {0} throws through {1} blocks\n", n_throws, n_blocks);
    var.Calculate(n_throws, n_blocks, rnd, out);
    fmt::print("Output written in {0}\n", paths::path_DATA/"ex01_1b.dat");

    /* chi2 */
    out.close();

    out.clear();
    out.open(paths::path_DATA/"ex01_1c.dat");
    if(!out.is_open()){
        fmt::print(cerr, "Error: unable to open {0}\n", paths::path_DATA/"ex01_1c.dat");
        return -1;
    }

    n_throws = 1E4;
    int m_intervals{100};
    vector<int> counter(m_intervals);
    double width = 1./(double)m_intervals;
    double current;
    double expected{(double)n_throws/(double)m_intervals};
    double chi;

    fmt::print("Calculating chi2 estimator for the Rannyu distribution with {0} throws in {1} intervals\n", n_throws, m_intervals);
    for(int k{0}; k<100; k++){
        //reset each cycle counters for throws fallen inside defined intervals    
        counter.assign(counter.size(), 0);
        //reset chi2
        chi = 0;
        for(int i{0}; i<n_throws; i++){
            current = rnd.Rannyu();
            // round to the nearest lower int to get which interval the throw is fallen into
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
    fmt::print("Output written in {0}\n", paths::path_DATA/"ex01_1c.dat");
    out.close();

    return 0;
}