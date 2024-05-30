#include <library02.h>

/* coseno funciton */
double Coseno::Eval(double x){
    return (double)M_PI_2*cos(x*(double)M_PI_2);
}


/* integrals with montecarlo mean method, with blck avg */
//base class with uniform distribution
BlockingMonte::BlockingMonte(){}

BlockingMonte::BlockingMonte(Function &fun) : a{0.}, b{1.}, fun{&fun}{}

BlockingMonte::BlockingMonte(Function &fun, double a, double b) : a{a}, b{b}, fun{&fun}{}

void BlockingMonte::SetInterval(double a, double b){
    a=a;
    b=b;
}

void BlockingMonte::Funny(Random &rnd){
    double x{Probability(rnd)};
    sum += (b-a)*(fun->Eval(x))/Denominator(x);
}

double BlockingMonte::Denominator(double x){
    return 1.;
}

double BlockingMonte::Probability(Random &rnd){
    return rnd.Rannyu(a,b);
}

/* montecarlo mean method with samples from a linear dist */
MonteLin::MonteLin(){}

//method to copy over from already constructed uniform montecarlo
MonteLin::MonteLin(BlockingMonte &monte) : BlockingMonte(monte) {}

MonteLin::MonteLin(Function &fun, double a, double b) : BlockingMonte(fun, a, b) {}

/* void MonteLin::Funny(Random &rnd){
    double x{Probability(rnd)};
    double den{2.-2.*x};
    sum += (b-a)*(double)M_PI_2*cos(x*(double)M_PI_2)/den;
} */

double MonteLin::Denominator(double x){
    return 2.-2.*x;
}

double MonteLin::Probability(Random &rnd){
    double x{rnd.Rannyu(a, b)};
    return 1.-sqrt(1.-x);
}

/* discrete random walk */
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
    //set number of blocks and throws
    BlockingLattice::n_blocks = blocks;
    BlockingLattice::n_throws = throws;
    // set lenght of each block
    l_block = (double)n_throws/(double)n_blocks;
    for(int i{0}; i<n_blocks; i++){
        // compute sum for this block
        Average(rnd);
        for(int j{0}; j<nstep; j++){
           posavg[j] += sqrt(postep[j]/(double)l_block) /* postep[j]/(double)l_block */;
           posavg2[j] += postep[j]/(double)l_block /* (postep[j]*postep[j])/(double)l_block */;
        }
        //reset cumulative square position
        fill(postep.begin(), postep.end(), 0.);
    }
    //write out to file
    fmt::print(file, "{0}\t{1}\t{2}\n", 0, 0, 0);
    for(int i{0}; i<nstep; i++){
        // calculate uncertainty: this is the error of the mean
        double uncert = Error(posavg[i]/n_blocks, posavg2[i]/n_blocks, n_blocks);
        fmt::print(file, "{0}\t{1}\t{2}\n", i+1, posavg[i]/(double)n_blocks, uncert);
    }
    std::fflush(stdout);
}

void BlockingLattice::Funny(Random &rnd){
    //decide which direction will be the step
    int which_dir{(rnd.Rannyu(0,3))};
    //decide if you want to step forward or backward
    int which_way{rnd.Rannyu(-1.,1.)<0?-1:1};
    double step[3] = {x, y, z};
    step[which_dir] += which_way;
    x = step[0];
    y = step[1];
    z = step[2];
}

// generate one walk and store addup to the current square position for each step
void BlockingLattice::Average(Random &rnd){
    for(int j{0}; j<l_block; j++){
        //reset position for each block
        x = 0.;
        y = 0.;
        z = 0.;
        //lets walk
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
    //theta is uniform
    double theta{rnd.Rannyu(0,2.*(double)M_PI)};
    //phi should be distributed as 1/2 sin(x)
    double r{rnd.Rannyu()};
    //inverse cdf of 1/2 sin(x)
    double phi{acos(1.-2.*r)};
    x += sin(phi)*cos(theta);
    y += sin(phi)*sin(theta);
    z += cos(phi);
}

