#include <iostream>
#include <system.h>
#include <fmt/ostream.h>
// #include <omp.h>

using namespace std;

int main (int argc, char *argv[]){
    int type = 2;
    double h = 0.0;
    system_properties properties;
    system_input input;
    if(argc < 1 || argc > 3){
        fmt::print("Invalid arguments. Expected <sim_type> or <sim_type> <external_field>\n");
        return 1;
    }
    else if (argc == 2) {
        type = atoi(argv[1])>2?3:2;
        fmt::print("Simulation type: {0}, with h=0.0 \n", type);
        properties.measure_temp = false;
        properties.measure_kenergy = false;
        properties.measure_cv = true;
        properties.measure_chi = true;
        properties.measure_tenergy = true;
        properties.measure_magnet = true;
    } else if (argc == 3) {
        type = atoi(argv[1])>2?3:2;
        h = 0.02;
        fmt::print("Simulation type: {0}, with h={1} \n", type, h);
        properties.measure_temp = false;
        properties.measure_kenergy = false;
        properties.measure_cv = false;
        properties.measure_chi = false;
        properties.measure_tenergy = false;
        properties.measure_magnet = true;
    } else {
        fmt::print("Invalid arguments. Expected <sim_type> or <sim_type> <external_field>\n");
        return 1;
    }
    //Setting starting properties: they are the same for all the simulations
    input.npart = 50;
    input.delta = 0.;
    input.sim_type = type;
    input._J = 1.;
    input._H = h;
    input.restart = 0;
    input.rho = 1.;
    input.r_cut = 0.;
    input.nsteps = 20000;
    input.nblocks = 20;
    /* base_properties.measure_cv = true;
    base_properties.measure_chi = true;
    base_properties.measure_tenergy = true;
    base_properties.measure_magnet = false;
    base_input.npart = 50;
    base_input.delta = 0.;
    base_input.sim_type = type;
    base_input._J = 1.;
    base_input._H = h;
    base_input.restart = 0;
    base_input.rho = 1.;
    base_input.r_cut = 0.;
    base_input.nsteps = 20000;
    base_input.nblocks = 20;
    base_input.temp = 0.5; */
    //Thread monitoring
    // vector<int> b_count(omp_get_max_threads());
    // vector<int> i_count(omp_get_max_threads());

    // fmt::print("Using {0} threads, format : <thread>:<temperature>_<block>\n", omp_get_max_threads());
    // #pragma omp parallel for
    for(int i=0; i<16; i++){ //loop over temperatures
        // i_count[omp_get_thread_num()] = i;
        //Setting input parameters
        input.temp = 0.5 + 0.1*(double)i;
        //Setting filenames for the output files
        string filename = fmt::format("{0}/ex06_{2}_{3}_{1}.dat",
                                        paths::path_DATA.c_str(), i, input.sim_type>2?"gibbs":"metro", argc<3?"h0":"h002");
        ofstream fileout(filename.c_str());
        //Setting the system
        System SYS(paths::path_ROOT / "exercise_06/INPUT", paths::path_IO / "OUTPUT" /* useless output, lets stash it away */);
        SYS.initialize(input);
        SYS.initialize_properties(properties);
        SYS.block_reset(0);
        // b_count[omp_get_thread_num()] = 0;
        //Equilibration
        for(int k{0}; k < 10000; k++){
            SYS.step();
        }
        SYS.block_reset(0);
        //Simulation
        for(int j{0}; j < SYS.get_nbl(); j++){ //loop over blocks
            for(int k{0}; k < SYS.get_nsteps(); k++){ //loop over steps in a block
                SYS.step();
                SYS.measure();
            }
            SYS.averages((bool)true);
            //Printing the results on the output file
            vector<int> index;
            if(argc < 3){
                index.push_back(SYS.get_index_tenergy());
                index.push_back(SYS.get_index_cv());
                index.push_back(SYS.get_index_chi());
            }
            else{
                index.push_back(SYS.get_index_magnet());
            }
            fmt::print(fileout, "{0:<4.1f} {1:<4} ",
                                input.temp, j+1);
            for(int t{0}; t < index.size(); t++){
                double cum_avg = SYS.get_sumaverage(index[t]);
                double cum2_avg = SYS.get_sum2average(index[t]);
                fmt::print(fileout, "{0:>10.5f} {1:>10.5f} ",
                                    cum_avg/(double)(j+1), SYS.error(cum_avg, cum2_avg, j+1));
            }
            fmt::print(fileout, "\n");
            //thread monitoring
            // b_count[omp_get_thread_num()]++;
            /* for(int t{0}; t<(omp_get_thread_num()>0?omp_get_thread_num():1); t++){
                fmt::print("{0}:{1}_{2:<3} | ", t, i_count[t], b_count[t]);
            }
            fmt::print("\r");
            fflush(stdout); */
            //single thread monitoring
            if(/* j%5 == 0) */ true) {
                fmt::print("temp:{0:<10.2f} block:{1:<7}\r",
                            input.temp, j+1);
                fflush(stdout);
            }
            SYS.block_reset(j+1);
        }
        SYS.finalize();
        fileout.close();
        fileout.clear();
    }

    //#pragma omp taskwait
    fmt::print("\n");
    
    return 0;
}