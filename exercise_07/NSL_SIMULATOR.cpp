/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <system.h>
#include <fmt/ostream.h>

using namespace std;

int main(int argc, char *argv[])
{

    System SYS(paths::path_ROOT / "exercise_07/INPUT", paths::path_ROOT / "exercise_07/OUTPUT");
    system_properties properties;
    system_input input;
    input.rho = 0.05;
    input.r_cut = 5.;
    input.temp = 1.2;
    input.delta = atof(argv[1]);
    input.nblocks = 10;
    input.nsteps = 100;
    input.sim_type = 1;

    SYS.initialize(input);
    SYS.initialize_properties(properties);
    SYS.block_reset(0);

    for (int i = 0; i < SYS.get_nbl(); i++)
    { // loop over blocks
        for (int j = 0; j < SYS.get_nsteps(); j++)
        { // loop over steps in a block
            SYS.step();
            SYS.measure();
        }
        SYS.averages(i + 1);
        SYS.block_reset(i + 1);
    }
    SYS.finalize();

    return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
