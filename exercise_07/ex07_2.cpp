#define TAIL_CORRECTION // used to "activate" tail corrections (so old code behaves as it wasnt corrected)

#include <iostream>
#include <system.h>
#include <fmt/ostream.h>
#include <omp.h>

#define N_EQUILIBRATION 30000
#define N_BLOCKS 500000
#define D_SOLID 0.057
#define D_LIQUID 0.115
#define D_GAS 2.675


using namespace std;

void thread_monitoring(vector<int> &b_count, vector<string> &name_count){
    string state;
    for(int t=0; t<=omp_get_thread_num(); t++){
        if(b_count[t] == 0) state = "eq...";
        else state = fmt::format("{0}", b_count[t]);
        fmt::print(cerr, "{1} {0:>7} |", state, name_count[t]);
    }
    fmt::print(cerr, "\r");
    fflush(stdout);
}

int main (int argc, char *argv[]){
    //Setting starting properties: they are the same for all the simulations
    // they are all set to false as we are probing delta values
    system_properties properties;
    properties.measure_penergy = true;
    //Setting the base path for the input and output files
    array<string, 3> names = {"solid", "liquid", "gas"};
    //Thread monitoring
    vector<int> b_count = {0, 0, 0};
    vector<string> name_count = {"solid", "liquid", "gas"};
    //Starting values for the three phases
    array<double, 3> rho = {1.1, 0.8, 0.05};
    array<double, 3> temp = {0.8, 1.1, 1.2};
    array<double, 3> r_cut = {2.2, 2.5, 3.};
    array<int, 3> n_blck = {N_BLOCKS, N_BLOCKS, N_BLOCKS};
    array<int, 3> n_steps = {1, 1, 1};
    array<double, 3> delta = {D_SOLID, D_LIQUID, D_GAS};

    fmt::print("Using max {0} threads, format : <thread>:<phase>_<block>\n", omp_get_max_threads());
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
        string filename_acc = fmt::format("{0}/ex07_istant_acc_{1}.dat", paths::path_DATA.string(), names[l]);
        ofstream fileout(filename.c_str());
        ofstream fileout_acc(filename_acc.c_str());
        //Setting the system
        System SYS(paths::path_ROOT / "exercise_07/INPUT", paths::path_IO / "OUTPUT" /* useless output, lets stash it away */);
        SYS.initialize(input);
        SYS.initialize_properties(properties);
        SYS.block_reset(0);
        b_count[omp_get_thread_num()] = 0;
        //equilibration
        thread_monitoring(b_count, name_count);
        for(int k{0}; k < N_EQUILIBRATION; k++){
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
            //Printing the acceptance rate
            double acc = static_cast<double>(SYS.get_naccepted())/static_cast<double>(SYS.get_nattempts());   
            fmt::print(fileout_acc, "{0:>5.3f}\n",
                                acc);
            //thread monitoring
            b_count[omp_get_thread_num()]++;
            for(int t{0}; t<3; t++) fmt::print(cerr, "\t");
            fmt::print(cerr, "\r");
            if(j%1000 == 0) thread_monitoring(b_count, name_count);
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
        fileout_acc.close();
        fileout_acc.clear();
    }

    #pragma omp taskwait
    fmt::print("\n");
  return 0;
}