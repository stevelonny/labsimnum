#include "library.h"

/* base class for blocking average */
BlockingAvg::BlockingAvg()
{
}

BlockingAvg::~BlockingAvg()
{
}

void BlockingAvg::Calculate(int n_throws, int n_blocks, Random &rnd, ofstream &file){
    cum_avg = 0.;
    cum_avg2 = 0.;
    l_block = (double)n_throws/(double)n_blocks;
    for(int i{0}; i<n_blocks; i++){
        sum = 0;
        fmt::print(cerr, "Running block # {0}/{1}\r", i+1, n_blocks);
        avg(rnd);
        //write out to file
        fmt::print(file, "{0}\t{1}\t{2}\n", (i+1)*l_block, cum_avg/(double)(i+1), Error(cum_avg/(double)(i+1), cum_avg2/(double)(i+1), i));
    }
    std::fflush(stdout);
}

void BlockingAvg::avg(Random &rnd){
    for (size_t i=0; i<l_block; i++){
        funny(rnd);
    }
    cum_avg += sum/(double)l_block;
    cum_avg2 += pow(sum/(double)l_block, 2);
}

void BlockingAvg::funny(Random &rnd){
    sum += rnd.Rannyu();
}

double BlockingAvg::Error(double ave, double av2, int i){
    if(i == 0){
        return 0.;
    }
    else{
        return sqrt(fabs(av2 - ave*ave)/((double)i));
    }
}

/* child class for variance */
void BlockingVar::funny(Random &rnd){
        sum += pow(rnd.Rannyu() - 0.5, 2);
}

/* child class fo buffon's needle */

BlockingBuffon::BlockingBuffon() : l{0.75}, d{1}{
}

BlockingBuffon::BlockingBuffon(double needle, double distance) : l{needle}, d{distance}{
}

void BlockingBuffon::avg(Random &rnd){
    for(size_t i{0}; i<l_block; i++){
        funny(rnd);
    }
    double curr_pi = ((double)l_block/sum)*(2.*(l/d));
    cum_avg += curr_pi;
    cum_avg2 += curr_pi*curr_pi;
}

void BlockingBuffon::funny(Random &rnd){
    double x{rnd.Rannyu()}, y{rnd.Rannyu()};
    //double recupera{-1.};
    while(x*x+y*y>1){
        //recupera = x; /* lets reuse at least one */
        x = rnd.Rannyu();
        y = rnd.Rannyu();
    }
    //recupera = (recupera>=0?recupera:rnd.Rannyu());
    if((rnd.Rannyu() * sqrt(x*x+y*y))/ x <=(l/d)){
        sum += 1.;
    }
    //sum += ((double)rnd.Rannyu()/(acos(1.-(double)rnd.Rannyu()))>(l/d))?0.:1.;
}

void initRandom(Random &rnd, string seedfile = paths::path_SEED){

    double p1, p2;
    int seed[4];
    ifstream Primes(paths::path_PRIMES);
    if(Primes.is_open()){
        Primes >> p1 >> p2;
        Primes.close();
        ifstream input(seedfile);
        string property;
        if (input.is_open()){
            while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
        }
        else{
            cerr << "Error: Unable to open seed file " << seedfile << endl;
        }
    }
    else{
        cerr << "Error: Unable to open Primes" << endl;
    }
}

double error(double ave, double av2, int i){
    if(i == 0){
        return 0.;
    }
    else{
        return sqrt(fabs(av2 - ave*ave)/((double)i));
    }
}

double expo(double rand, double lambda){
    return (-1./lambda) * log(1. - rand);
}
double cauchy(double rand, double center, double gamma){
    return center + gamma * tan(M_PI * (rand - 0.5));
}

