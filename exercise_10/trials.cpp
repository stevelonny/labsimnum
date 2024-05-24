#include <iostream>
#include <version_config.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <random.h>
#include <armadillo>
#include <vector>
#include <memory>
#include <algorithm>
#include <mpi.h>

int main(int argc, char *argv[]){

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size < 2 || size > 10){
        fmt::print("This program must be run with 2 to 6 processes\n");
        MPI_Finalize();
        return 1;
    }
    MPI_Request request;
    int* currentgens = new int[size-1];
    int r_gen{0};
    int genes{5};
    MPI_Status status;
    if(rank==0){
        
        Random rnd(0);
        arma::imat continents(genes, size-1);
        arma::ivec tribute(genes);
        fmt::print("Waiting for champions from others continents...\n");
        for(int i{1}; i<size; i++){
            MPI_Recv(continents.colptr(i-1), genes, MPI_INTEGER8, i, i, MPI_COMM_WORLD, &status);
            // tribute.print("Champion from continent 1");
            // tribute += 10;
            continents.col(i-1).print(fmt::format("Champion from continent {0}", i));
            continents.col(i-1) += 10;
            MPI_Send(continents.colptr(i-1), genes, MPI_INTEGER8, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        arma::ivec tribute(genes);
        tribute.fill(rank);
        MPI_Send(tribute.memptr(), genes, MPI_INTEGER8, 0, rank, MPI_COMM_WORLD);
        MPI_Recv(tribute.memptr(), tribute.size(), MPI_INTEGER8, 0, 0, MPI_COMM_WORLD, &status);
        tribute.save(fmt::format("{1}/champion_{0}.dat", rank, paths::path_DATA), arma::raw_ascii);
    }

    MPI_Finalize();

    return 0;
}
