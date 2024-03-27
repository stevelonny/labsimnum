#include <iostream>
#include <system.h>
#include <fmt/ostream.h>
#include <omp.h>

using namespace std;

int main (int argc, char *argv[]){
    //Setting starting properties: they are the same for all the simulations
    system_properties properties;
    properties.measure_temp = true;
    properties.measure_kenergy = true;
    properties.measure_pressure = true;
    properties.measure_penergy = true;
    properties.measure_pressure = true;
    properties.measure_tenergy = true;
    properties.writeout = true;    
    //Setting the base path for the input and output files
    array<string, 3> names = {"SOLID", "LIQUID", "GAS"};
    array<int, 3> b_count = {0, 0, 0};
    //Starting values for the three phases
    array<int, 3> n_blck = {50, 50, 50}; 
    array<int, 3> n_steps = {2000, 2000, 2000};
    array<double, 3> rho = {1.1, 0.8, 0.05};
    array<double, 3> r_cut = {2.2, 2.5, 5.0};
    array<double, 3> temp = {0.8, 1.1, 1.2}; //correct starting temperatures
    
    #pragma omp parallel for
    for(int l=0; l<3; l++){ //loop over phases
        //Setting input parameters
        system_input input;
        input.restart = 1;
        input.rho = rho[l];
        input.r_cut = r_cut[l];
        input.temp = temp[l];
        input.nblocks = n_blck[l];
        input.nsteps = n_steps[l];
        //Setting the system
        string filein = "ex04/INPUT_" + names[l];
        string fileout = "ex04/OUTPUT_" + names[l];
        System SYS(paths::path_DATA / filein, paths::path_DATA / fileout);
        SYS.initialize(input);
        SYS.initialize_properties(properties);
        SYS.block_reset(0);
        for(int j{0}; j < SYS.get_nbl(); j++){ //loop over blocks
            for(int k{0}; k < SYS.get_nsteps(); k++){ //loop over steps in a block
                SYS.step();
                SYS.measure();
            }
            b_count[l]++;
            SYS.averages(j+1);
            SYS.block_reset(j+1);
            fmt::print("SOLID {0:>2}/{1} LIQUID {2:>2}/{3} GAS {4:>2}/{5}\r", b_count[0], n_blck[0], b_count[1], n_blck[1], b_count[2], n_blck[2]);
            fflush(stdout);
        }
        fmt::print("\n");
        SYS.finalize();
    }

    return 0;
}