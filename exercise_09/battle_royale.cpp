#include "battle_royale.h"

Mapper::Mapper(unsigned int n_cities) : _ncities(n_cities), _atlas(n_cities), rnd() {
    InitAtlas();
}

void Mapper::InitAtlas()
{
    for (int i{0}; i < _ncities; i++)
    {
        // _atlas[i] = (2 * M_PI * static_cast<double>(i)) / static_cast<double>(_ncities);
        _atlas[i] = rnd.Rannyu(0, 2 * M_PI);
    }
}

double Mapper::Distance(int first_city, int second_city){
    double angle1 = _atlas[_pbc(first_city)];
    double angle2 = _atlas[_pbc(second_city)];
    return pow(cos(angle1) - cos(angle2), 2) + pow(sin(angle1) - sin(angle2), 2);
}

vector<double> Mapper::Position(int city){
    double angle = _atlas[_pbc(city)];
    double x = cos(angle);
    double y = sin(angle);
    return {x, y};
}

SquareMapper::SquareMapper(unsigned int n_cities) : Mapper(n_cities) {
    InitAtlas();
}

void SquareMapper::InitAtlas(){
    for(int i{0}; i < getNCities(); i++){
        _atlas[i] = make_pair(rnd.Rannyu(-1., 1.), rnd.Rannyu(-1., 1));
    }
}

double SquareMapper::Distance(int first_city, int second_city){
    double x1{_atlas[_pbc(first_city)].first};
    double y1{_atlas[_pbc(first_city)].second};
    double x2{_atlas[_pbc(second_city)].first};
    double y2{_atlas[_pbc(second_city)].second};
    return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}

vector<double> SquareMapper::Position(int city){
    return { _atlas[_pbc(city)].first, _atlas[_pbc(city)].second};
}


Population::Population(shared_ptr<Mapper>& atlas, int n_populace) :
    _apopulation(atlas->getNCities(), n_populace), _rnd(), _n_populace(n_populace), _n_genes(atlas->getNCities()), _atlas(atlas), _distance(n_populace), _total_score{0.}
{
    // _atlas = move(atlas);
    for (int i{0}; i < _n_populace; i++) {
        _apopulation.col(i).subvec(1, _n_genes-1) = shuffle(linspace<ivec>(2, _n_genes, _n_genes-1));
        _apopulation(0, i) = 1;
    }
    GiveDistance();
    vector<int> check{Check()};
    if(check.size() > 0){
        fmt::print(cerr, "Error: population is not well formed\n");
        for(auto i : check){
            fmt::print(cerr, "Error: chromosome {}\n", i);
        }
    }
    else{
        fmt::print("Population is well formed\n");
    }
}

void Population::GiveDistance(){
    for(int i{0}; i < _n_populace; i++){
        double distance{0};
        for(int j{0}; j<_n_genes-1; j++){
            distance += _atlas->Distance(_apopulation(j, i), _apopulation((j + 1), i));
        }
        distance += _atlas->Distance(_apopulation(_n_genes - 1, i), _apopulation(0, i));
        this->_distance[i] = {i, distance};
    }
    std::sort(_distance.begin(), _distance.end(), [](pair<int, double> a, pair<int, double> b) {return a.second < b.second;});
    TotalScore();
}

void Population::TotalScore(){
    _total_score = 0;
    std::for_each(_distance.begin(), _distance.end(), [this](pair<int, double> a) {this->_total_score += 1./a.second;});
}

vector<int> Population::Check(){
    vector<int> check;
    for(int i{0}; i < _n_populace; i++){
        if(_apopulation(0, i) != 1){
            fmt::print(cerr, "Error: first city is not 1\n");
            check.push_back(i);
        }

        std::set<int> unique_cities(_apopulation.col(i).begin(), _apopulation.col(i).end());
        if(unique_cities.size() != _n_genes){
            fmt::print(cerr, "Error: not all cities are visited in chromosome {}\n", unique_cities.size());
            check.push_back(i);
        }

    }
    return check;
}

const double Population::getBest(){
    return min_element(_distance.begin(), _distance.end(), [](pair<int, double> a, pair<int, double> b) {return a.second < b.second;})->second;
}

const int Population::getBestIndex(){
    return min_element(_distance.begin(), _distance.end(), [](pair<int, double> a, pair<int, double> b) {return a.second < b.second;})->first;
}

const double Population::getHalfBest(){
    int half_size = _n_populace / 2;
    double sum{0.};
    std::for_each(_distance.begin(), _distance.begin() + half_size, [&sum](pair<int, double> a) {sum += a.second;});
    return sum / static_cast<double>(half_size);
}

BattleRoyale::BattleRoyale(double swap_rate, double permutation_rate, double shift_rate, double inversion_rate, double crossover_rate) :
    _swap_rate(swap_rate), _permutation_rate(permutation_rate), _shift_rate(shift_rate), _inversion_rate(inversion_rate), _crossover_rate(crossover_rate), rnd()
{}

void BattleRoyale::Reproduce(Population &pop){
    // sort the population by score
    pop.GiveDistance();
    // vector<pair<int, double>> sorted_pop(pop._distance.begin(), pop._distance.end());
    // sort(sorted_pop.begin(), sorted_pop.end(), [](pair<int, double> a, pair<int, double> b) {return a.second < b.second;});
    // create a new population
    arma::imat new_pop(pop._apopulation.n_rows, pop._apopulation.n_cols);
    for(int i{0}; i < pop.getNPopulace(); i++){
        int first{Selection(pop)};
        int second{Selection(pop)};
        if(rnd.Rannyu() < _crossover_rate){
            arma::imat sons = Crossover(pop, first, second);
            new_pop.col(i) = sons.col(0);
            new_pop.col(i + 1) = sons.col(1);
        }
        else{
            new_pop.col(i) = pop._apopulation.col(first);
            new_pop.col(i + 1) = pop._apopulation.col(second);
        }
        i++;
    }
    vector<int> check{pop.Check()};
    // if(check.size() > 0){
    //     fmt::print(cerr, "Error: population is not well formed\n");
    //     for(auto i : check){
    //         fmt::print(cerr, "Error: chromosome {}\n", i);
    //     }
    // }
    pop._apopulation = new_pop;
}

int BattleRoyale::Selection(Population &pop){
    double total_score{pop.getTotalScore()};
    double r{rnd.Rannyu(0, total_score)};
    double chosen_score{0};
    int the_chosen{0};
    while(chosen_score < r){
        chosen_score += 1./pop._distance[the_chosen].second;
        the_chosen++;
    }
    return pop._distance[the_chosen-1].first;
}

arma::imat BattleRoyale::Crossover(Population& pop, int first, int second){
    // find the cut position
    int start{static_cast<int>(rnd.Rannyu(1, pop.getNGenes()-1))};
    // prepare the sons
    arma::imat sons(pop.getNGenes(), 2);
    sons.col(0) = pop._apopulation.col(first);
    sons.col(1) = pop._apopulation.col(second);

    std::vector<int> orderFirst, orderSecond;
    for (int i = start; i < pop.getNGenes(); i++) {
        orderFirst.push_back(pop._apopulation(i, first));
        orderSecond.push_back(pop._apopulation(i, second));
    }

    for (int i = 0; i < orderFirst.size(); i++) {
        sons(start + i, 0) = *std::find(sons.col(1).begin(), sons.col(1).end(), orderFirst[i]);
        sons(start + i, 1) = *std::find(sons.col(0).begin(), sons.col(0).end(), orderSecond[i]);
    }

    return sons;
}


void BattleRoyale::Mutation(Population &pop){
    for(int i{0}; i < pop.getNPopulace(); i++){
        if(rnd.Rannyu() < _swap_rate)
            Swap(pop, i);
        if(rnd.Rannyu() < _permutation_rate)
            Permutation(pop, i);
        if(rnd.Rannyu() < _shift_rate)
            Shift(pop, i);
        if(rnd.Rannyu() < _inversion_rate)
            Inversion(pop, i);
    }
}
                    
void BattleRoyale::Swap(Population &pop, int column){
    int first_gene{static_cast<int>(rnd.Rannyu(1, pop.getNGenes()-1))};
    int second_gene;
    if(first_gene == 1){
        second_gene = first_gene + 1;
    }
    else if(first_gene == pop.getNGenes()-1){
        second_gene = first_gene - 1;
    }
    else{
        second_gene = static_cast<int>(rnd.Rannyu(0, 2)) == 0 ? (first_gene - 1) : (first_gene + 1);
    }
    int temp_gene{pop._apopulation(first_gene, column)};
    pop._apopulation(first_gene, column) = pop._apopulation(second_gene, column);
    pop._apopulation(second_gene, column) = temp_gene;
}

void BattleRoyale::Permutation(Population &pop, int column){
    int m{static_cast<int>(rnd.Rannyu(1, pop.getNGenes()/2))};
    int center{static_cast<int>(rnd.Rannyu(1+m, pop.getNGenes()-m))};
    for(int i{0}; i < m; i++){
        int temp{pop._apopulation(pop.pbc(center + i), column)};
        pop._apopulation(pop.pbc(center + i), column) = pop._apopulation(pop.pbc(center - i), column);
        pop._apopulation(pop.pbc(center - i), column) = temp;
    }
}
    

void BattleRoyale::Shift(Population &pop, int column){
    // extract how many genes to shift
    int m{static_cast<int>(rnd.Rannyu(1, pop.getNGenes()-1))};
    // extract the starting point and the step
    int start{static_cast<int>(rnd.Rannyu(1, pop.getNGenes()))};
    int step{static_cast<int>(rnd.Rannyu(1, pop.getNGenes()))};
    
    int new_start{pop.pbc(start + step)};
    for(int i{0}; i < m; i++){
        int temp{pop._apopulation(pop.pbc(new_start + i), column)};
        pop._apopulation(pop.pbc(new_start + i), column) = pop._apopulation(pop.pbc(start + i), column);
        pop._apopulation(pop.pbc(start + i), column) = temp;
    }
}

void BattleRoyale::Inversion(Population& pop, int column){
    int start{static_cast<int>(rnd.Rannyu(1, pop.getNGenes()-1))};
    int end{static_cast<int>(rnd.Rannyu(start, pop.getNGenes()-1))};
    while(start < end){
        int temp{pop._apopulation(start, column)};
        pop._apopulation(start, column) = pop._apopulation(end, column);
        pop._apopulation(end, column) = temp;
        start++;
        end--;
    }
}

