#include "library.h"

/* int rng */
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

/* base class for blocking average */
BlockingAvg::BlockingAvg()
{
}

BlockingAvg::~BlockingAvg()
{
}

void BlockingAvg::Calculate(int n_throws, int n_blocks, Random &rnd, ofstream &file){
    //reset cumulative avg
    cum_avg = 0.;
    cum_avg2 = 0.;
    // set lenght of each block
    l_block = (double)n_throws/(double)n_blocks;
    for(int i{0}; i<n_blocks; i++){
        //reset current sum
        sum = 0;
        fmt::print(cerr, "Running block # {0}/{1}\r", i+1, n_blocks);
        Average(rnd);
        //write out to file
        fmt::print(file, "{0}\t{1}\t{2}\n", (i+1)*l_block, cum_avg/(double)(i+1), Error(cum_avg/(double)(i+1), cum_avg2/(double)(i+1), i));
    }
    std::fflush(stdout);
}

// the need of a overridable Average is needed by the buffon needle and may be needed by other strange distributions
void BlockingAvg::Average(Random &rnd){
    for (int i=0; i<l_block; i++){
        Funny(rnd);
    }
    cum_avg += sum/(double)l_block;
    cum_avg2 += pow(sum/(double)l_block, 2);
}

// Funny is interchangable, it's the characteristic of the distribution
void BlockingAvg::Funny(Random &rnd){
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
void BlockingVar::Funny(Random &rnd){
        sum += pow(rnd.Rannyu() - 0.5, 2);
}

/* child class fo buffon's needle */
BlockingBuffon::BlockingBuffon() : l{0.75}, d{1}{
}

BlockingBuffon::BlockingBuffon(double needle, double distance) : l{needle}, d{distance}{
}

void BlockingBuffon::Average(Random &rnd){
    for(int i{0}; i<l_block; i++){
        Funny(rnd);
    }
    // calculate pi = 2*l/(d*P) where P is how many needles have passed a line
    double curr_pi = ((double)l_block/sum)*(2.*(l/d));
    cum_avg += curr_pi;
    cum_avg2 += curr_pi*curr_pi;
}

// in this case funny is the needle throw
// the needle is thrown and one is added when the needle crosses a line
void BlockingBuffon::Funny(Random &rnd){
    double x{rnd.Rannyu()}, y{rnd.Rannyu()};
    // regenerate x,y until x and y are inside the unit circle
    while(x*x+y*y>1){
        x = rnd.Rannyu();
        y = rnd.Rannyu();
    }
    // check if needle passes the line
    if((rnd.Rannyu() * sqrt(x*x+y*y))/ x /* sin function! */ <=(l/d)){
        sum += 1.;
    }
    //sum += ((double)rnd.Rannyu()/(acos(1.-(double)rnd.Rannyu()))>(l/d))?0.:1.;
}


/*  */
BlockingMonte::BlockingMonte() : a{0.}, b{1.}{

}

BlockingMonte::BlockingMonte(double a, double b) : a{a}, b{b}{

}

void BlockingMonte::SetInterval(double a, double b){
    a=a;
    b=b;
}

void BlockingMonte::Funny(Random &rnd){
    sum += (b-a)*(double)M_PI_2*cos(Probability(rnd)*(double)M_PI_2);
}

double BlockingMonte::Probability(Random &rnd){
    return rnd.Rannyu(a,b);
}

/*  */
MonteLin::MonteLin(){
    SetInterval(0., 1.);
}


MonteLin::MonteLin(double b, double a){
    SetInterval(a, b);
}

void MonteLin::Funny(Random &rnd){
    double x{Probability(rnd)};
    double den{2.-2.*x};
    sum += (double)M_PI_2*cos(x*(double)M_PI_2)/den;
}

double MonteLin::Probability(Random &rnd){
    double x{rnd.Rannyu(a, b)};
    return 1.-sqrt(1.-x);
}

/* inverse distribution */
double exponential_distribution(double rand, double lambda){
    return (-1./lambda) * log(1. - rand);
}

double cauchy_distribution(double rand, double center, double gamma){
    return center + gamma * tan(M_PI * (rand - 0.5));
}

/* discrete lattice */
BlockingLattice::BlockingLattice(/* args */) : x{0.}, y{0.}, z{0.}, nstep{100}
{
    postep = vector<double>(nstep);
    posavg = vector<double>(nstep);
    posavg2 = vector<double>(nstep);
    //fill(postep.begin(), postep.end(), 0.); //probably not needed
}

BlockingLattice::~BlockingLattice()
{
}

void BlockingLattice::Calculate(int throws, int blocks, Random &rnd, ofstream &file){
    BlockingLattice::n_blocks = blocks;
    BlockingLattice::n_throws = throws;
    // set lenght of each block
    l_block = (double)n_throws/(double)n_blocks;
    for(int i{0}; i<n_blocks; i++){
       Average(rnd);
       for(int j{0}; j<nstep; j++){
           posavg[j] += postep[j]/(double)l_block;
           posavg2[j] += (postep[j]*postep[j])/(double)l_block;
       }
       fill(postep.begin(), postep.end(), 0.);
    }
    //write out to file
    for(int i{0}; i<nstep; i++){
        // calculate uncertainty: this is the error of the mean
        // should be propagated with the error of the sum
        double uncert = Error(posavg[i]/n_blocks, posavg2[i]/n_blocks, n_blocks);
        fmt::print(file, "{0}\t{1}\t{2}\n", i+1, posavg[i]/(double)n_blocks, uncert);
    }
    std::fflush(stdout);
}

void BlockingLattice::Funny(Random &rnd){
    int step{(rnd.Rannyu(0,3))};
    int which_way{rnd.Rannyu(-1.,1.)>0?1:-1};
    double cart[3] = {x, y, z};
    cart[step] += which_way;
    x = cart[0];
    y = cart[1];
    z = cart[2];
}

// generate one walk and store the sum of the square of the steps for each step
void BlockingLattice::Average(Random &rnd){
    for(int j{0}; j<l_block; j++){
        x = 0.;
        y = 0.;
        z = 0.;
        for(int i{0}; i<nstep; i++){
            Funny(rnd);
            postep[i] += (x*x+y*y+z*z);
        }
    }
}

/* contiune random walk */

ContinueLattice::ContinueLattice()
{
}

void ContinueLattice::Funny(Random &rnd){
    double theta{rnd.Rannyu(0,2.*(double)M_PI)};
    double r{rnd.Rannyu()};
    double phi{acos(1.-2.*r)};
    x += sin(phi)*cos(theta);
    y += sin(phi)*sin(theta);
    z += cos(phi);
}

