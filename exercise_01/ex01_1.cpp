#include <iostream>
#include <cmath>
#include <vector>
#include "library.h"

using namespace std;

int main(int argc, const char** argv){

    Random rnd;
    initRandom(rnd, string(ROOT_PATH) + "/random/seed.in");

    int n_throws = 1E7;
    int n_blocks{100};
    int l_blocks{(int)(n_throws/n_blocks)};
    
    /* 1st moment, average */

    double sum;
    double ave{0.};
    double av2{0.};
    double sum_prog{0.};
    double su2_prog{0.};

    /* 2nd moment, variance */

    double v_sum;
    double v_ave{0.};
    double v_av2{0.};
    double v_sum_prog{0.};
    double v_su2_prog{0.};


    //vector<double> error_prog(n_blocks);
    //vector<double> ave(n_blocks);
    //vector<double> av2(n_blocks);
    ofstream out_a;
    out_a.open(string(ROOT_PATH) + "/data/ex_01.1a.dat");
    if(!out_a.is_open()){
        cerr << "Errore nella scrittura del file di output" << string(ROOT_PATH) + "/data/ex_01.1a.dat" << endl;
        return -1;
    }
    ofstream out_b;
    out_b.open(string(ROOT_PATH) + "/data/ex_01.1b.dat");
    if(!out_b.is_open()){
        cerr << "Errore nella scrittura del file di output" << string(ROOT_PATH) + "/data/ex_01.1b.dat" << endl;
        return -1;
    }
    for(int i{0}; i<n_blocks; i++){
        sum = 0;
        v_sum = 0;
        for(int j{0}; j<l_blocks; j++){
            sum += rnd.Rannyu();
            v_sum += pow(rnd.Rannyu() - 0.5, 2);
        }
        //current averages
        ave = sum / (double)l_blocks;
        av2 = ave*ave;
        v_ave = v_sum / (double)l_blocks;
        v_av2 = v_ave*v_ave;
        
        //cumulative sum: this is the (i+1)-th point
        sum_prog += ave;           
        su2_prog += av2;
        v_sum_prog += v_ave;           
        v_su2_prog += v_av2;
        //write out to file
        out_a << (i+1)*l_blocks << '\t' << sum_prog/(double)(i+1)/* cum_avg */ << '\t';
        out_a << error(sum_prog/(double)(i+1), su2_prog/(double)(i+1)/* cum_av2 */, i) << endl;
        out_b << (i+1)*l_blocks << '\t' << v_sum_prog/(double)(i+1)/* vcum_avg */ << '\t';
        out_b << error(v_sum_prog/(double)(i+1), v_su2_prog/(double)(i+1)/* vcum_av2 */, i) << endl;
    }

    out_a.close();
    out_b.close();

    out_a.clear();
    out_a.open(string(ROOT_PATH) + "/data/ex_01.1c.dat");
    if(!out_a.is_open()){
        cerr << "Errore nella scrittura del file di output" << string(ROOT_PATH) + "/data/ex_01.1c.dat" << endl;
        return -1;
    }
    
    n_throws = 1E4;
    int m_intervals{100};
    vector<int> counter(m_intervals);
    double width = 1./(double)m_intervals;
    double current;
    double expected{(double)n_throws/(double)m_intervals};
    cout << expected << endl;
    double chi;

    for(int k{0}; k<100; k++){
        //reset each cycle counters for throws fallen inside defined intervals    
        //reset chi2
        counter.assign(counter.size(), 0);
        chi = 0;
        for(int i{0}; i<n_throws; i++){
            current = rnd.Rannyu();
            for(int j{0}; j<m_intervals; j++){
                counter[j] += ((current<(j+1)*width)?((current>j*width)?1:0):0);
            }
        }
        for(int i{0}; i<m_intervals; i++){
            chi += pow(counter[i] - expected, 2)/expected;
        }
        out_a << k+1 << '\t'/* e4sesso con steveeeeeeeeeeeeee */ << chi << endl;
    }

    out_a.close();

    return 0;
}