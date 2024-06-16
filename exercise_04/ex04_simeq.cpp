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
    //thread monitoring
    array<int, 3> b_count = {0, 0, 0}; 
    //Starting values for the three phases
    array<int, 3> n_blck = {50, 50, 100}; 
    array<int, 3> n_steps = {2000, 2000, 2000};
    array<int, 3> n_steps_eq = {50000, 50000, 500000};
    array<double, 3> rho = {1.1, 0.8, 0.05};
    array<double, 3> r_cut = {2.2, 2.5, 5.0};
    array<double, 3> temp = {1.54, 1.98, 0.95}; //starting temps should not matter when velocities.in is present or when using verlet (?)
    
    #pragma omp parallel for
    for(int l=0; l<3; l++){ //loop over phases
        //Setting input parameters
        system_input input;
        input.restart = 0;
        input.rho = rho[l];
        input.r_cut = r_cut[l];
        input.temp = temp[l];
        input.nblocks = n_blck[l];
        input.nsteps = n_steps[l];
        int nconf{1};
        //input - ouput files
        //string filein = "ex04/INPUT_" + names[l];
        string fileout = "ex04/OUTPUT_EQ_" + names[l];
        /*
        if(!std::filesystem::exists(paths::path_DATA / filein / "CONFIG/velocities.in")){
            std::filesystem::rename (paths::path_DATA / filein / "CONFIG/velocities.out" , paths::path_DATA / filein / "CONFIG/velocities.in");
        }
        */
        //initialize system
        System SYS(paths::path_ROOT / "exercise_04/INPUT", paths::path_DATA / fileout);
        SYS.initialize(input);
        SYS.initialize_properties(properties);
        SYS.block_reset(0);
        fmt::print("Starting equilibration for {0} phase\n", names[l]);
        for(int i{0}; i<n_steps_eq[l]; i++){ //equilibration steps
            SYS.step();
            //SYS.measure();
        }
        SYS.block_reset(0);
        for(int j{0}; j < SYS.get_nbl(); j++){ //loop over blocks
            for(int k{0}; k < SYS.get_nsteps(); k++){ //loop over steps in a block
                SYS.step();
                SYS.measure();
                if(k%100 == 0){ //write out configuration for animations
                    SYS.write_XYZ(nconf);
                    nconf++;
                }
            }
            b_count[l]++; // thread monitoring
            SYS.averages(j+1);
            SYS.block_reset(j+1);
            fmt::print("SOLID {0:>2}/{1} LIQUID {2:>2}/{3} GAS {4:>2}/{5}\r", b_count[0], n_blck[0], b_count[1], n_blck[1], b_count[2], n_blck[2]);
            fflush(stdout);
        }
        SYS.finalize();
    }

    return 0;
}