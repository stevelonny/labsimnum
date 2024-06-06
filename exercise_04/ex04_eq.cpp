#include <iostream>
#include <system.h>
#include <fmt/ostream.h>
#include <omp.h>

using namespace std;

int main (int argc, char *argv[]){
    // Setting starting properties: they are the same for all the simulations
    system_properties properties;
    properties.measure_temp = true;
    properties.measure_kenergy = true; // necessary to compute temperature
    // Setting the base path for the input and output files
    array<string, 3> names = {"solid", "liquid", "gas"};
    // Thread monitoring
    vector<int> b_count(omp_get_max_threads());
    vector<int> i_count(omp_get_max_threads());
    vector<char> name_count(omp_get_max_threads());
    // Starting values for the three phases
    array<double, 3> rho = {1.1, 0.8, 0.05};
    array<double, 3> r_cut = {2.2, 2.5, 5.0};
    array<int, 3> n_blck = {100, 100, 500};
    array<int, 3> n_steps = {500, 500, 1000};
    // Starting temps from which the simulations start cycles of 0.05 increments
    array<double, 3> temp = {1.4, 1.7, 0.8};

    fmt::print("Using {0} threads, format : <thread>:<phase>_<temperature>_<block>\n", omp_get_max_threads());
    #pragma omp parallel for
    for(int l=0; l<3; l++){ //loop over phases
        #pragma omp taskloop
        for(int i=0; i<10; i++){ //loop over temperatures
            i_count[omp_get_thread_num()] = i;
            name_count[omp_get_thread_num()] = l==0?'s':l==1?'l':'g';
            // Setting input parameters
            system_input input;
            input.rho = rho[l];
            input.r_cut = r_cut[l];
            input.temp = temp[l] + 0.05*(double)i;
            input.nblocks = n_blck[l];
            input.nsteps = n_steps[l];
            //  Setting filenames for the output files
            string filename = (paths::path_DATA).string() + "/ex04_eq_";
            filename += names[l];
            ofstream fileout((filename + "_" + to_string(i) + ".dat").c_str());
            // Setting the system
            System SYS(paths::path_ROOT / "exercise_04/INPUT", paths::path_IO / "OUTPUT" /* useless output, lets stash it away */);
            SYS.initialize(input);
            SYS.initialize_properties(properties);
            SYS.block_reset(0);
            b_count[omp_get_thread_num()] = 0;
            for(int j{0}; j < SYS.get_nbl(); j++){ //loop over blocks
                for(int k{0}; k < SYS.get_nsteps(); k++){ //loop over steps in a block
                    SYS.step();
                    SYS.measure();
                }
                SYS.averages((bool)true);
                // Printing the results on the output file
                int ind_temp = SYS.get_index_temp();
                double blck_avg = SYS.get_average(ind_temp);
                double cum_avg = SYS.get_sumaverage(ind_temp);
                double cum2_avg = SYS.get_sum2average(ind_temp);
                fmt::print(fileout, "{0:<10.2f} {1:<7} {2:>10.5f} {3:>10.5f} {4:>10.5f}\n",
                                    input.temp, j+1, blck_avg, cum_avg/(double)(j+1), SYS.error(cum_avg, cum2_avg, j+1));
                // thread monitoring
                b_count[omp_get_thread_num()]++;
                for(int t=0; t<omp_get_thread_num(); t++){
                    fmt::print("{0}:{1}_{2}_{3:<3} | ", t, name_count[t], i_count[t], b_count[t]);
                }
                fmt::print("\r");
                fflush(stdout);
                /* if(j%10 == 0){
                    fmt::print("temp:{0:<10.2f} block:{1:<7} blck_temp:{2:>10.5f} cum_temp:{3:>10.5f} error:{4:>10.5f}\r",
                                input.temp, j+1, blck_avg, cum_avg/(double)(j+1), SYS.error(cum_avg, cum2_avg, j+1));
                    fflush(stdout);
                } */
                SYS.block_reset(j+1);
            }
            SYS.finalize();
            fileout.close();
            fileout.clear();
        }
    }

    #pragma omp taskwait
    fmt::print("\n");
  return 0;
}