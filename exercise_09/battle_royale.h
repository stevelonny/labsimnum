#include <iostream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <map>
#include <set>
#include <deque>
#include <random.h>
#include <armadillo>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;
using namespace arma;

#define _NCITIES 34
#define _NPOPULACE 1000
#define _SELECTION_TYPE 0
#define _SWAP_RATE 0.10
#define _PERMUTATION_RATE 0.05
#define _SHIFT_RATE 0.05
#define _INVERSION_RATE 0.10
#define _CROSSOVER_RATE 0.80

/* base class for circle distributed cities */
class Mapper {
    public:
        Mapper() : Mapper(_NCITIES) {}
        Mapper(unsigned int n_cities);
        Mapper(arma::dmat coords);
        void InitCirlce();
        void InitSquare();
        void InitCoords(arma::dmat coords);
        virtual double Distance(int first_city, int second_city);
        const int getNCities(){return _ncities;}
        virtual vector<double> Position(int city); /* we'll use this method to print out coordinates */
        const int _pbc(int index){return index<1?1:(index>_ncities?index%_ncities:index);}
    private:
        unsigned int _ncities;
        unordered_map<int, pair<double, double>> _atlas;
        Random rnd;
};

/* class for population */
class Population {
    public:
        Population(shared_ptr<Mapper>& atlas) : Population(atlas, _NPOPULACE) {};
        Population(shared_ptr<Mapper>& atlas, int n_populace);
        arma::imat _apopulation; // may consider getters and putters for safety
        vector<int> Check();
        // map to store distance score of each vector column
        void GiveDistance();
        const double getDistance(int index){return _distance[index].second;};
        const double getTotalScore(){return _total_score;};
        const double getBest();
        const int getBestIndex();
        const double getHalfBest();
        vector<pair<int, double>> _distance;
        const int pbc(int index){return 1 + (index%(_n_genes-1));};
        const int getNPopulace(){return _n_populace;}
        const int getNGenes(){return _n_genes;}

    private:
        void TotalScore();
        shared_ptr<Mapper> _atlas;
        double _total_score;
        unsigned int _n_populace;
        unsigned int _n_genes;
};


/* class to handle evolutions and generations */
class BattleRoyale{
    public:
        BattleRoyale() : BattleRoyale(0) {};
        BattleRoyale(int mrank) : BattleRoyale(mrank, _SELECTION_TYPE, _SWAP_RATE, _PERMUTATION_RATE, _SHIFT_RATE, _INVERSION_RATE, _CROSSOVER_RATE){}
        BattleRoyale(int mrank, int seltype) : BattleRoyale(mrank, seltype, _SWAP_RATE, _PERMUTATION_RATE, _SHIFT_RATE, _INVERSION_RATE, _CROSSOVER_RATE){}
        BattleRoyale(int mrank, int seltype, double swap_rate, double permutation_rate, double shift_rate, double inversion_rate, double crossover_rate);

        // Evolve the population
        void Reproduce(Population& pop);
        // Handle all mutations
        void Mutation(Population& pop);
        int Selection(Population& pop);
        const int getNGenerations(){return _n_generations;}
    private:
        Random rnd;
        int _rank;
        int _selectiontype;
        double _swap_rate;
        double _permutation_rate;
        double _shift_rate;
        double _inversion_rate;
        double _crossover_rate;
        // number of generations to go by
        int _n_generations;
        // int _n_populace;
        // int _n_genes;
        int _n_mutation;
        // Selects a chromosome by extracting from the distribution given by distance scores
        // Generate two sons crossing over
        arma::imat Crossover(Population& pop, int first, int second);
        // Swaps two genes in a chromosome
        void Swap(Population &pop, int column);
        // Permutation on same chromosome, base is just a pair
        void Permutation(Population &pop, int column);
        // Shifts genes in a chromosome
        void Shift(Population& pop, int column);
        // Inverts order of a chromosome
        void Inversion(Population& pop, int column);
        //Todo: Crossover
};

