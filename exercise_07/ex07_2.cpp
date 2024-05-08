#include <iostream>
#include <system.h>
#include <fmt/ostream.h>
#include <omp.h>

#define TAIL_CORRECTION // used to "activate" tail corrections (so old code behaves as it wasnt corrected)

using namespace std;

int main (int argc, char *argv[]){
    //Setting starting properties: they are the same for all the simulations
    // they are all set to false as we are probing delta values
    system_properties properties;
    properties.measure_penergy = true;
    //Setting the base path for the input and output files
    array<string, 3> names = {"solid", "liquid", "gas"};
    //Thread monitoring
    array<int, 3> b_count = {0, 0, 0};
    //Starting values for the three phases
    array<double, 3> rho = {1.1, 0.8, 0.05};
    array<double, 3> temp = {0.8, 1.1, 1.2};
    array<double, 3> r_cut = {2.2, 2.5, 5.0};
    array<int, 3> n_blck = {500000, 500000, 500000};
    array<int, 3> n_steps = {1, 1, 1};
    array<double, 3> delta = {0.11, 0.06, 3.};

    fmt::print("Using {0} threads, format : <thread>:<phase>_<block>\n", omp_get_max_threads());
    #pragma omp parallel for
    for(int l=0; l<3; l++){ //loop over phases
        //Setting input parameters
        system_input input;
        input.sim_type = 1;
        input.rho = rho[l];
        input.r_cut = r_cut[l];
        input.temp = temp[l];
        input.delta = delta[l];
        input.nblocks = n_blck[l];
        input.nsteps = n_steps[l];
        // Setting filenames for the output files
        string filename = fmt::format("{0}/ex07_istant_{1}.dat", paths::path_DATA.string(), names[l]);
        ofstream fileout(filename.c_str());
        //Setting the system
        System SYS(paths::path_ROOT / "exercise_07/INPUT", paths::path_IO / "OUTPUT" /* useless output, lets stash it away */);
        SYS.initialize(input);
        SYS.initialize_properties(properties);
        SYS.block_reset(0);
        b_count[omp_get_thread_num()] = 0;
        //equilibration
        for(int k{0}; k < 10000; k++){
            SYS.step();
        }
        SYS.block_reset(0);
        for(int j{0}; j < SYS.get_nbl(); j++){ //loop over blocks   
            //simulation
            SYS.step();
            SYS.measure();
            SYS.averages((bool)true);
            //Printing the results on the output file
            int ind_pot = SYS.get_index_potential();
            double blck_avg = SYS.get_average(ind_pot);
            fmt::print(fileout, "{0:>10.5f}\n",
                                blck_avg);
            //thread monitoring
            b_count[omp_get_thread_num()]++;
            fmt::print("SOLID {0:>7} LIQUID {1:>7} GAS {2:>7}\r", b_count[0], b_count[1], b_count[2]) ;
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

    #pragma omp taskwait
    fmt::print("\n");
  return 0;
}