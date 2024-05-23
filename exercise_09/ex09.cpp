#include <iostream>
#include <version_config.h>
#include <fmt/ostream.h>
#include <fmt/format.h>
#include <battle_royale.h>

int main(int argc, char *argv[]){
    
    std::shared_ptr<Mapper> atlas;
    if(argc == 2 && strcmp(argv[1], "circle") == 0){
        atlas = std::make_shared<Mapper>(Mapper());   
    }
    else if(argc == 2 && strcmp(argv[1], "square") == 0){
        atlas = static_pointer_cast<Mapper>(std::make_shared<SquareMapper>(SquareMapper()));
    }
    else{
        fmt::print("Usage: ./ex09_acc <mapper>\nwhere <mapper> is either 'circle' or 'square'\n");
        return 1;
    }

    // auto atlas = std::shared_ptr<Mapper>(new SquareMapper());
    atlas->whoami();
    // atlasRef->whoami();
    Population pop(atlas, 1000);
    string filename = fmt::format("{0}/ex09_atlas_{1}.dat", paths::path_DATA.string(), argv[1]);
    ofstream fileout(filename.c_str());
    for(int i{0}; i<atlas->getNCities(); i++){
        fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", i, atlas->Position(i)[0], atlas->Position(i)[1]);
    }
    fileout.close();
    fileout.clear();

    BattleRoyale hungergames;

    filename = fmt::format("{0}/ex09_values_{1}.dat", paths::path_DATA.string(), argv[1]);
    fileout.open(filename.c_str());

    pop._apopulation.brief_print("Population");

    for(int i{0}; i<500; i++){
        fmt::print("Generation: {}\r", i);
        fmt::print(fileout, "{0:<5} {1:<10.5f} {2:<10.5f}\n", i, pop.getDistance(0), pop.getHalfBest()/* pop.getBest() */);
        hungergames.Reproduce(pop);
        hungergames.Mutation(pop);
    }
    pop.GiveDistance();
    fileout.close();
    fileout.clear();

    filename = fmt::format("{0}/ex09_best_{1}.dat", paths::path_DATA.string(), argv[1]);
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