#include "library.h"

/* base class for blocking average */
BlockingAvg::BlockingAvg()
{
}

BlockingAvg::~BlockingAvg()
{
}

void BlockingAvg::Calculate(int n_throws, int n_blocks, Random &rnd, ofstream &file){
    cum_sum = 0.;
    cum_sum2 = 0.;
    l_block = (double)n_throws/(double)n_blocks;
    for(int i{0}; i<n_blocks; i++){
        sum = 0;
        for(int j{0}; j<l_block; j++){
            funny(rnd.Rannyu());
        }
        //cumulative avg?: this is the (i+1)-th point
        cum_sum += sum / (double)l_block;           
        cum_sum2 += pow(sum / (double)l_block, 2);
        //write out to file
        fmt::print(file, "{0}\t{1}\t{2}\n", (i+1)*l_block, cum_sum/(double)(i+1), Error(cum_sum/(double)(i+1), cum_sum2/(double)(i+1), i));
    }
}

void BlockingAvg::funny(double rand){
    sum += rand;
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
void BlockingVar::funny(double rand){
    sum += pow(rand - 0.5, 2);
}


void initRandom(Random &rnd, string seedfile = string(ROOT_PATH) + "/random/seed.in"){

    double p1, p2;
    int seed[4];
    ifstream Primes(string(ROOT_PATH) + "/random/Primes");
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

