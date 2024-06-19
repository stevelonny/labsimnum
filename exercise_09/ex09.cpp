#include <iostream>
#include <version_config.h>
#include <fmt/ostream.h>
#include <fmt/format.h>
#include <battle_royale.h>

int main(int argc, char *argv[]){
    
    auto atlas = std::make_shared<Mapper>();
    if(argc >= 2 && strcmp(argv[1], "circle") == 0){
        atlas->InitCirlce();
    }
    else if(argc >= 2 && strcmp(argv[1], "square") == 0){
        atlas->InitSquare();
    }
    else if(argc >= 2 && strcmp(argv[1], "provita") == 0){
        arma::dmat coords(110, 2);
        ifstream filein(fmt::format("{0}/{1}/cap_prov_ita.dat", paths::path_ROOT, "/exercise_10"));
        if(filein.is_open()){   
            int n_cities{110};
            for(int i{0}; i<n_cities; i++){
                filein >> coords(i, 0) >> coords(i, 1);
            }
        }
        else{
            fmt::print("File not found\n");
            return 1;
        }
        fmt::print(cerr, "File read\n");
        atlas->InitCoords(coords);
    }
    else{
        fmt::print(cerr, "Usage: ./ex09_acc <mapper> [<seltype>]\nwhere <mapper> is either 'circle' or 'square' or 'provita'\n");
        fmt::print(cerr, "and optionally <seltype> is either 0(exp selection) 1(roulette) 2(tournament) 3(stoccazz)\n");
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
    Population pop(atlas, 200);
    string filename = fmt::format("{0}/ex09_atlas_{1}.dat", paths::path_DATA.string(), argv[1], seltype);
    ofstream fileout(filename.c_str());
    for(int i{1}; i<atlas->getNCities()+1; i++){
        fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", i, atlas->Position(i)[0], atlas->Position(i)[1]);
    }
    fileout.close();
    fileout.clear();

    BattleRoyale hungergames(0, seltype);

    filename = fmt::format("{0}/ex09_values_{1}_{2}.dat", paths::path_DATA.string(), argv[1], seltype);
    fileout.open(filename.c_str());

    pop._apopulation.brief_print("Population");

    for(int i{0}; i<500; i++){
        fmt::print("Generation: {}\r", i);
        fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f} {3:<10.5f} {4:<10.5f}\n",
                                i, pop.getDistance(0), pop.getHalfBest()/* pop.getBest() */, pop.getMean(), pop.getStdDev());
        hungergames.Reproduce(pop);
        hungergames.Mutation(pop);
    }
    pop.GiveDistance();
    fileout.close();
    fileout.clear();
    fmt::print("\n");
    fmt::print("Done. Writing to file ");
    filename = fmt::format("{0}/ex09_best_{1}_{2}.dat", paths::path_DATA.string(), argv[1], seltype);
    fmt::print("{0}\n", filename);
    fileout.open(filename.c_str());
    arma::ivec best = pop._apopulation.col(pop.getBestIndex());
    for(int i{0}; i<best.size(); i++){
        fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", best(i), atlas->Position(best(i))[0], atlas->Position(best(i))[1]);
    }

    pop._apopulation.brief_print("Population");
    // best.save(fmt::format("{0}/ex09_best_{1}.dat", paths::path_DATA.string(), argv[1]), arma::raw_ascii);
    // vector<int> check = pop.Check();

    // fmt::print("Check: {}\n", check.size());

    return 0;
}