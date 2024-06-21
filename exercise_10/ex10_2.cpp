#include <mpi.h>
#include <version_config.h>
#include <iostream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <battle_royale.h>

#define NMIGRATIONS 10

// remember that sword of arma in 64bit systems is int64_t, so 64/8=MPI_INTEGER8 (otherwhise 32bit for 32bit systems)

int main(int argc, char* argv[]){

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size < 3 || size > 21){
        fmt::print("This program must be run with 3 to 11 processes\n");
        MPI_Finalize();
        return 1;
    }

    int* currentgens = new int[size-1];
    int r_gen{0};
    if(rank == 0){
        for(int i{0}; i<size-1; i++){
            currentgens[i] = 0;
        }        
    }
    arma::dmat coords;
    int Nmigr{NMIGRATIONS};
    auto atlas = make_shared<Mapper>();
    if(argc >=2 && strcmp(argv[1], "circle")==0){
        atlas->InitCirlce();
    }
    else if(argc >=2 && strcmp(argv[1], "square")==0){
        atlas->InitSquare();
    }
    else if(argc >= 2 && strcmp(argv[1], "provita")==0){
        coords.set_size(110, 2);
        if(rank == 0){
            ifstream filein(fmt::format("{0}/{1}/cap_prov_ita.dat", paths::path_ROOT, "/exercise_10"));
            if(filein.is_open()){
                int n_cities{110};
                for(int i{0}; i<n_cities; i++){
                    filein >> coords(i, 0) >> coords(i, 1);
                }
            }
            else{
                fmt::print("File not found\n");
                MPI_Finalize();
                return 1;
            }
        }
        MPI_Bcast(coords.memptr(), coords.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        atlas->InitCoords(coords);
    }
    if((argc < 2 || argc >4)  && ((strcmp(argv[1], "circle") != 0 || strcmp(argv[1], "square") != 0) || strcmp(argv[1], "provita") != 0)){
        fmt::print(cerr, "Usage: <mapper> [<seltype>]\nwhere <mapper> is either 'circle' or 'square' or 'provita'\n");
        fmt::print(cerr, "and optionally <seltype> is either 0(lin rank-based) 1(roulette) 2(tournament) 3(stoch-univ-acc)\n");
        MPI_Finalize();
        return 1;
    }
    int seltype = 0;
    if(argc >2){
        seltype = atoi(argv[2]);
        if(seltype < 0 || seltype > 4){
            fmt::print(cerr, "Invalid selection type. Must be 0, 1, 2 or 3\n");
            return 1;
        }
    }
    arma::imat tributes;
    string filename;
    ofstream fileout;
    if(rank == 0){
        filename = fmt::format("{0}/ex10_2_atlas_{1}.dat", paths::path_DATA.string(), argv[1]);
        fileout.open(filename.c_str());
        for(int i{1}; i<atlas->getNCities()+1; i++){
            fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", i, atlas->Position(i)[0], atlas->Position(i)[1]);
        }
        fileout.close();
        fileout.clear();
    }

    Population pop(atlas, 200);
    BattleRoyale pubg(rank, seltype);
    /* if(rank == 0)  */tributes.set_size(atlas->getNCities(), size);
    filename = fmt::format("{0}/ex10_2_r{2}_values_{1}_{3}.dat", paths::path_DATA.string(), argv[1], rank, seltype);
    fileout.open(filename.c_str());
    for(int i{0}; i<500; i++){
        fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", i, pop.getDistance(0), pop.getHalfBest()/* pop.getBest() */);
        r_gen++;
        if(i%Nmigr == 0 && i != 0){
            MPI_Gather(&r_gen, 1, MPI_INT, currentgens, 1, MPI_INT, 0, MPI_COMM_WORLD);
            if(rank == 0) fmt::print("Generations: {0}\r", currentgens[0]);
        }
        pubg.Reproduce(pop);
        pubg.Mutation(pop);
    }
    // send best to root process
    pop.GiveDistance();
    arma::ivec best = pop._apopulation.col(pop.getBestIndex());
    // root process receives and saves bests
    MPI_Gather(best.memptr(), best.size(), MPI_INTEGER8, tributes.memptr(), best.size(), MPI_INTEGER8, 0, MPI_COMM_WORLD);
    if(rank == 0){
        fmt::print("\n");
        tributes.brief_print("Bests");
        fmt::print("Done. Writing champions to file ");
        filename = fmt::format("{0}/ex10_2_bests_{1}_{2}.dat", paths::path_DATA, argv[1], seltype);
        fmt::print("{0}\n", filename);
        tributes.save(filename, arma::raw_ascii);
        // creates a new population with the bests just to find the best of the best
        Population bests(atlas, tributes.n_cols);
        bests._apopulation = tributes;
        bests.GiveDistance();
        fmt::print("Writing champion to file ");
        filename = fmt::format("{0}/ex10_2_champion_{1}_{2}.dat", paths::path_DATA.string(), argv[1], seltype);
        fmt::print("{0}\n", filename);
        ofstream fileout(filename.c_str());
        best = pop._apopulation.col(pop.getBestIndex());
        for(int i{0}; i<best.size(); i++){
            fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", best(i), atlas.get()->Position(best(i))[0], atlas.get()->Position(best(i))[1]);
        }
        fileout.close();
    }
    delete[] currentgens;
    MPI_Finalize();
    return  0;
}