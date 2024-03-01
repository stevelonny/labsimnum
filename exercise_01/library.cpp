#include "library.h"


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
