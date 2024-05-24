#include <mpi.h>
#include <version_config.h>
#include <iostream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <battle_royale.h>

bool isThisTheEnd(int gen[], int size){
    for(int i{0}; i<size; i++){
        if(gen[i] < 500) return false;
    }
    return true;
}

// remember that sword of arma in 64bit systems is int64_t, so MPI_INTEGER8 (otherwhise 32bit for 32bit systems)

int main(int argc, char* argv[]){

    auto atlas = make_shared<Mapper>();
    atlas->whoami();
    int Nmigr{10};
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
        if(size < 3 || size > 11){
        fmt::print("This program must be run with 3 to 11 processes\n");
        MPI_Finalize();
        return 1;
    }
    MPI_Request request;
    int* currentgens = new int[size-1];
    int r_gen{0};
    int* indexes = new int[size-1];
    if(rank == 0){
        for(int i{0}; i<size-1; i++){
            currentgens[i] = 0;
            indexes[i] = atlas->getNCities()+1;
        }

        
    }

    if(rank==0){
        int gen{0};
        string filename = fmt::format("{0}/ex10_atlas.dat", paths::path_DATA.string(), argv[1]);
        ofstream fileout(filename.c_str());
        for(int i{0}; i<atlas->getNCities(); i++){
            fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", i, atlas->Position(i)[0], atlas->Position(i)[1]);
        }
        fileout.close();
        fileout.clear();
        Random rnd(0);
        arma::imat tributes(atlas->getNCities(), size-1);
        while(gen < 490){
            // tributes.set_size(atlas->getNCities(), size-1);
            gen += 10;
            fmt::print(cerr, "Waiting for champions from others continents...\n");
            for(int i{1}; i<size; i++){
                MPI_Recv(tributes.colptr(i-1), tributes.n_rows, MPI_INTEGER8, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                fmt::print(cerr, "Received champion from continent {0}\n", i );
            }
            int dest{1};
            while(0<tributes.n_cols){
                int champion = static_cast<int>(rnd.Rannyu(0, tributes.n_cols));
                MPI_Send(tributes.colptr(champion), tributes.n_rows, MPI_INTEGER8, dest, 0, MPI_COMM_WORLD);
                // MPI_Send(homecoming.memptr(), homecoming.size(), MPI_INT, i, 0, MPI_COMM_WORLD);
                dest++;
                tributes.shed_col(champion);
            }
            // MPI_Bcast(indexes, size-1, MPI_INT, 0, MPI_COMM_WORLD);
            // MPI_Gather(&r_gen, 1, MPI_INT, currentgens, 1, MPI_INT, 0, MPI_COMM_WORLD);
            fmt::print("Generations: {0}\n", gen);
            tributes.set_size(atlas->getNCities(), size-1);
        }
        fmt::print("Done\n");
        for(int i{1}; i<size; i++){
            arma::ivec best(atlas->getNCities());
            MPI_Recv(tributes.colptr(i-1), tributes.n_rows, MPI_INTEGER8, i, 10+i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fmt::print(cerr, "Received best from continent {0}\n", i);
        }
        tributes.print("Bests");
        tributes.save(fmt::format("{0}/ex10_bests.dat", paths::path_DATA), arma::raw_ascii);
    }
    else{
        Population pop(atlas);
        BattleRoyale pubg(rank);
        for(int i{0}; i<500; i++){
            r_gen++;
            if(i%Nmigr == 0 && i != 0){
                int champion = pubg.Selection(pop);
                arma::ivec tribute = pop._apopulation.col(champion);
                arma::ivec homecoming(atlas->getNCities());
                // MPI_Isend(tribute.memptr(), tribute.size(), MPI_INT, 0, rank, MPI_COMM_WORLD, &request);
                MPI_Send(tribute.memptr(), tribute.size(), MPI_INTEGER8, 0, rank, MPI_COMM_WORLD);
                MPI_Recv(homecoming.memptr(), homecoming.size(), MPI_INTEGER8, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                fmt::print(cerr, "{1:<3}:Received champion from continent {0}\n", 0, rank);
                pop._apopulation.col(champion) = homecoming;
            }
            pubg.Reproduce(pop);
            pubg.Mutation(pop);
        }
        fmt::print(cerr, "{1:<3}:Generations: {0}\n", r_gen, rank);
        pop.GiveDistance();
        arma::ivec best = pop._apopulation.col(pop.getBestIndex());
        MPI_Send(best.memptr(), best.size(), MPI_INTEGER8, 0, 10+rank, MPI_COMM_WORLD);
    }


    MPI_Finalize();
    return  0;
}
