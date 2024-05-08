#include <iostream>
#include <system.h>
#include <fmt/ostream.h>
#include <omp.h>

#define TAIL_CORRECTION // used to "activate" tail corrections (so old code behaves as it wasnt corrected)

using namespace std;

void thread_monitoring(vector<int> &b_count, vector<char> &name_count, vector<char> &sim_count){
    string state;
    for(int t=0; t<=omp_get_thread_num(); t++){
        if(b_count[t] == 0) state = "eq...";
        else state = fmt::format("{0}", b_count[t]);
        fmt::print(cerr, "{0}:{1}_{2}_{3:<7} | ", t, name_count[t], sim_count[t], state);
    }
    fmt::print(cerr, "\r");
    fflush(stdout);
}

int main (int argc, char *argv[]){
    //Setting starting properties: they are the same for all the simulations
    // they are all set to false as we are probing delta values
    system_properties properties;
    properties.measure_penergy =    true;  // needed for pressure and U/N
    properties.measure_kenergy =    true;  // needed for temp and U/N
    properties.measure_tenergy =    true;
    properties.measure_temp =       true;  // needed for pressure  
    properties.measure_pressure =   true;
    properties.measure_gofr =       true;
    //Setting the base path for the input and output files
    array<string, 3> names = {"SOLID", "LIQUID", "GAS"};
    //Thread monitoring
    vector<int> b_count(omp_get_max_threads());
    vector<char> name_count(omp_get_max_threads());
    vector<char> sim_count(omp_get_max_threads());
    //Starting values for the three phases
    array<double, 3> rho = {1.1, 0.8, 0.05};
    array<double, 3> nvt_temp = {0.8, 1.1, 1.2};
    array<double, 3> nve_temp = {1.54, 1.98, 0.95};
    array<double, 3> r_cut = {2.2, 2.5, 5.0};
    array<int, 3> n_blck = {50000, 50000, 50000};
    array<int, 3> nve_equil = {50000, 50000, 500000};
    array<int, 3> nvt_equil = {10000, 10000, 10000};
    array<int, 3> n_steps = {2000, 2000, 2000};
    double nve_delta = 0.0005;
    array<double, 3> delta = {0.11, 0.06, 3.};


    fmt::print("Using max {0} threads, format : <thread>:<phase>_<block>\n", omp_get_max_threads());
    #pragma omp parallel for
    for(int l=0; l<3; l++){ //loop over phases
        #pragma omp taskloop
        for(int i=0; i<2; i++){
            //Setting input parameters
            name_count[omp_get_thread_num()] = l==0?'s':l==1?'l':'g';
            sim_count[omp_get_thread_num()] = i<1?'E':'T';
            system_input input;
            input.sim_type = i;
            input.rho = rho[l];
            input.r_cut = r_cut[l];
            input.temp = (i<1)?nve_temp[l]:nvt_temp[l];
            input.delta = (i<1)?nve_delta:delta[l];
            input.nblocks = n_blck[l];
            input.nsteps = n_steps[l];
            // Setting filenames for the output files
            string out_dir = fmt::format("{0}/OUTPUT_{0}_{1}", paths::path_DATA.string(), (i<1)?"NVE":"NVT", names[l]);
            //Setting the system
            System SYS(paths::path_ROOT / "exercise_07/INPUT", out_dir);
            SYS.initialize(input);
            SYS.initialize_properties(properties);
            SYS.block_reset(0);
            b_count[omp_get_thread_num()] = 0;
            //equilibration
            for(int k{0}; k < ((i<1)?nve_equil[l]:nvt_equil[l]); k++){
                SYS.step();
                thread_monitoring(b_count, name_count, sim_count);
            }
            SYS.block_reset(0);
            for(int j{0}; j < SYS.get_nbl(); j++){ //loop over blocks   
                //simulation
                for(int k{0}; k < SYS.get_nsteps(); k++){ //loop over steps in a block
                    SYS.step();
                    SYS.measure();
                }
                SYS.averages(j+1);
                b_count[omp_get_thread_num()]++;
                thread_monitoring(b_count, name_count, sim_count);            
                SYS.block_reset(j+1);
            }
            SYS.finalize();
        }
    }
    #pragma omp taskwait
    fmt::print("\n");
  return 0;
}

