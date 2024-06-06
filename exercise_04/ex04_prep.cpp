#include <iostream>
#include <system.h>
#include <fmt/ostream.h>

using namespace std;

int main (int argc, char *argv[]){
    //Setting starting properties: they are the same for all the simulations
    system_properties properties;
    properties.measure_temp = true;
    properties.measure_kenergy = true;
    //Setting the base path for the input and output files
    array<string, 3> names = {"SOLID", "LIQUID", "GAS"};
    //Starting values for the three phases
    array<int, 3> n_blck = {1, 1, 1}; //number of blocks needed to stabilize
    array<int, 3> n_steps = {50000, 50000, 500000};
    array<double, 3> rho = {1.1, 0.8, 0.05};
    array<double, 3> r_cut = {2.2, 2.5, 5.0};
    array<double, 3> temp = {1.54, 1.98, 0.95}; //correct starting temperatures

    #pragma omp parallel for
    for(int l=0; l<3; l++){ //loop over phases
        //Setting input parameters
        system_input input;
        input.rho = rho[l];
        input.r_cut = r_cut[l];
        input.temp = temp[l];
        input.nblocks = n_blck[l];
        input.nsteps = n_steps[l];
        //Setting the system
        string filename = "ex04/INPUT_" + names[l];
        System SYS(paths::path_ROOT / "exercise_04/INPUT", paths::path_DATA / filename);
        SYS.initialize(input);
        SYS.initialize_properties(properties);
        SYS.block_reset(0);
        for(int j{0}; j < SYS.get_nbl(); j++){ //loop over blocks
            for(int k{0}; k < SYS.get_nsteps(); k++){ //loop over steps in a block
                SYS.step();
                SYS.measure();
            }
            SYS.averages(j+1);
            SYS.block_reset(j+1);
            //fflush(stdout);
        }
        //fmt::print("\n");
        SYS.finalize();
    }

    return 0;
}