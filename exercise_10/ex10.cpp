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

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    /* std::shared_ptr<Mapper> atlas;
    // double* coords;
    if(argc == 2 && strcmp(argv[1], "circle") == 0){
        // atlas.reset(new Mapper()); 
        atlas = std::make_shared<Mapper>();
    }
    else if(argc == 2 && strcmp(argv[1], "square") == 0){
        // atlas = static_pointer_cast<Mapper>(std::make_shared<SquareMapper>(SquareMapper()));
        // atlas = dynamic_pointer_cast<Mapper>(std::make_shared<SquareMapper>());
    }
    */
    /* else if(argc == 2 && strcmp(argv[1], "provita")){
        if(rank == 0){
            ifstream filein(fmt::format("{0}/ex10_provita.dat", paths::path_ROOT, "/exercise_10"));
            if(filein.is_open()){
                int n_cities{110};
                coords = new double[n_cities];
                for(int i{0}; i<n_cities; i++){
                    filein >> coords[i];
                }
            }
            else{
                fmt::print("File not found\n");
                MPI_Finalize();
                return 1;
            }
        }
        MPI_Bcast(coords, 110, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        atlas = static_pointer_cast<Mapper>(std::make_shared<SquareMapper>(SquareMapper(coords, 110)));
    } */
    /*   
    else{
        if(rank == 0) fmt::print(cerr, "Usage: {0} <mapper>\nwhere <mapper> is either 'circle' or 'square'\n", argv[0]);
        MPI_Finalize();
        return 1;
    } */
    if(size < 3 || size > 11){
        fmt::print("This program must be run with 3 to 11 processes\n");
        MPI_Finalize();
        return 1;
    }
    int* currentgens = new int[size-1];
    int r_gen{0};
    if(rank == 0){
        for(int i{0}; i<size-1; i++){
            currentgens[i] = 0;
            // indexes[i] = atlas->getNCities()+1;
        }        
    }
    int Nmigr{10};
    // std::shared_ptr<Mapper> atlas = static_pointer_cast<Mapper>(std::make_shared<SquareMapper>(SquareMapper()));
    auto atlas = make_shared<Mapper>();
    atlas->whoami();
    arma::imat tributes;
    if(rank == 0){
        // int gen{0};
        string filename = fmt::format("{0}/ex10_atlas_{1}.dat", paths::path_DATA.string(), "circle");
        ofstream fileout(filename.c_str());
        for(int i{0}; i<atlas->getNCities(); i++){
            fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", i, atlas->Position(i)[0], atlas->Position(i)[1]);
        }
        fileout.close();
        fileout.clear();
    }

/*
    if(rank==0){
        int gen{0};
        string filename = fmt::format("{0}/ex10_atlas.dat", paths::path_DATA.string(), "circle");
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
*/

    // initilize rnd with different seed for each rank
    // Random rnd(rank);
    Population pop(atlas);
    fmt::print(cerr, "{0:<3}: atlas ptr counter {1}\n", rank, atlas.use_count());
    BattleRoyale pubg(rank);
    /* if(rank == 0)  */tributes.set_size(atlas->getNCities(), size);
    for(int i{0}; i<500; i++){
        r_gen++;
        if(i%Nmigr == 0 && i != 0){
            int champion = pubg.Selection(pop);
            arma::ivec tribute = pop._apopulation.col(champion);
            //arma::ivec homecoming(atlas->getNCities());
            MPI_Gather(tribute.memptr(), tribute.size(), MPI_INTEGER8, tributes.memptr(), tribute.size(), MPI_INTEGER8, 0, MPI_COMM_WORLD);
            if(rank == 0) tributes = shuffle(tributes, 1);
            MPI_Bcast(tributes.memptr(), tributes.size(), MPI_INTEGER8, 0, MPI_COMM_WORLD);
            /*
            if(rank == 0){
                tributes.col(0) = tribute;
                for(int i{1}; i<size; i++){
                    MPI_Recv(tributes.colptr(i), tributes.n_rows, MPI_INTEGER8, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    fmt::print("Received champion from continent {0}\n", i);
                }
                int champ_0 = static_cast<int>(rnd.Rannyu(0, tributes.n_cols));
                pop._apopulation.col(champion) = tributes.col(champ_0);
                tributes.shed_col(champ_0);
                int dest{1};
                while(0<tributes.n_cols){
                    int chosen = static_cast<int>(rnd.Rannyu(0, tributes.n_cols));
                    MPI_Send(tributes.colptr(chosen), tributes.n_rows, MPI_INTEGER8, dest, 0, MPI_COMM_WORLD);
                    dest++;
                    tributes.shed_col(chosen);
                }
                tributes.set_size(atlas->getNCities(), size);
            }
            */
            /*
            else {
                MPI_Send(tribute.memptr(), tribute.size(), MPI_INTEGER8, 0, rank, MPI_COMM_WORLD);
                MPI_Recv(homecoming.memptr(), homecoming.size(), MPI_INTEGER8, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                fmt::print(cerr, "{1:<3}:Received champion from continent {0}\n", 0, rank);
                pop._apopulation.col(champion) = homecoming;
            }
            */
            pop._apopulation.col(champion) = tributes.col(rank);
            MPI_Gather(&r_gen, 1, MPI_INT, currentgens, 1, MPI_INT, 0, MPI_COMM_WORLD);
            if(rank == 0) fmt::print("Generations: {0}\n", currentgens[0]);
        }
        pubg.Reproduce(pop);
        pubg.Mutation(pop);
    }
    // fmt::print(cerr, "{1:<3}:Generations: {0}\n", r_gen, rank);
    // send best to root process
    pop.GiveDistance();
    arma::ivec best = pop._apopulation.col(pop.getBestIndex());
    // if(rank != 0) MPI_Send(best.memptr(), best.size(), MPI_INTEGER8, 0, 10+rank, MPI_COMM_WORLD);
    // root process receives and saves bests
    MPI_Gather(best.memptr(), best.size(), MPI_INTEGER8, tributes.memptr(), best.size(), MPI_INTEGER8, 0, MPI_COMM_WORLD);
    if(rank == 0){
        // tributes.col(0) = best;
        /*
        for(int i{1}; i<size; i++){
            MPI_Recv(tributes.colptr(i), tributes.n_rows, MPI_INTEGER8, i, 10+i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fmt::print("Received best from continent {0}\n", i);
        }
        */
        tributes.print("Bests");
        tributes.save(fmt::format("{0}/ex10_bests_{1}.dat", paths::path_DATA, "circle"), arma::raw_ascii);
        Population bests(atlas, tributes.n_cols);
        bests._apopulation = tributes;
        bests.GiveDistance();
        string filename = fmt::format("{0}/ex10_champion_{1}.dat", paths::path_DATA.string(), "circle");
        ofstream fileout(filename.c_str());
        best = pop._apopulation.col(pop.getBestIndex());
        for(int i{0}; i<best.size(); i++){
            fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", best(i), atlas.get()->Position(best(i))[0], atlas.get()->Position(best(i))[1]);
        }
        fileout.close();
        fmt::print(cerr, "{0:<3}: atlas ptr counter {1}\n", rank, atlas.use_count());
    }
    // fmt::print(cerr, "{0:<3}: atlas ptr counter {1}\n", rank, atlas.use_count());
    // atlas.unique();
    delete[] currentgens;
    MPI_Finalize();
    return  0;
}
