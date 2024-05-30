#include "library08.h"

Metro::Metro() : BlockingAvg(), actual{0.}, next{0.}
{
    output.resize(1, 0.);
    mstep = make_shared<UniformStep>(1.5/2.);
    macc = make_shared<DoubleGaussian>();
}

Metro::Metro(double x, double delta)
    : BlockingAvg(), actual{x}, next{x}
{
    output.resize(1, 0.);
    mstep = make_shared<UniformStep>(delta);
    macc = make_shared<DoubleGaussian>();
}


Metro::Metro(double x, std::shared_ptr<UniformStep> &Step) :
    BlockingAvg(), actual{x}, next{x}, mstep{Step}
{
    output.resize(1, 0.);
    macc = make_shared<DoubleGaussian>();
}

Metro::Metro(double x, double delta, std::shared_ptr<DoubleGaussian> &Acc) :
    BlockingAvg(), actual{x}, next{x}, macc{Acc}
{
    output.resize(1, 0.);
    mstep = make_shared<UniformStep>(delta);
}

Metro::Metro(double x, int n_throws, int n_blocks, std::shared_ptr<DoubleGaussian> &Acc, std::shared_ptr<UniformStep> &Step) :
    BlockingAvg(n_throws, n_blocks), actual{x}, next{x}, macc(Acc), mstep(Step)
{
    // int l_block = n_throws/n_blocks;
    output.resize(1, 0.);
}

void Metro::Funny(Random &rnd){
    mstep->Step(rnd, actual, next);
    double acc{min(1., macc->pSample(actual, next))};
    double r{rnd.Rannyu()};
    if(r>acc){
        sum += macc->Hamilt(actual);
        next = actual;
    }
    else{
        sum += macc->Hamilt(next);
        actual = next;
        output[0] += 1.;
    }
    // output[1] = actual;
    // output[2] = macc->Sample(actual);
}

double Metro::rDistance(const double &pos){
    return pos;
}

UniformStep::UniformStep(double delta) : delta{delta} {}

void UniformStep::Step(Random &rnd, const array<double, 3> &actual, array<double, 3> &next){
    for(int i{0}; i<3; i++){
        next[i] = actual[i] + rnd.Rannyu(-delta, delta);
    }
}

void UniformStep::Step(Random &rnd, const double &actual, double &next){
    next += rnd.Rannyu(-delta, delta);
}

DoubleGaussian::DoubleGaussian() : _mu{0.}, _sigma{1.} {}

void DoubleGaussian::SetParameters(double mu, double sigma){
    this->_mu = mu;
    this->_sigma = sigma;
}

double DoubleGaussian::Sample(double x){
    return exp(-(x-_mu)*(x-_mu)/(2*_sigma*_sigma))+exp(-(x+_mu)*(x+_mu)/(2*_sigma*_sigma));
}

double DoubleGaussian::D2Sample(double x){
    double minus{exp(-(x-_mu)*(x-_mu)/(2*_sigma*_sigma))};
    double plus{exp(-(x+_mu)*(x+_mu)/(2*_sigma*_sigma))};
    return minus*((x-_mu)*(x-_mu)/(pow(_sigma, 4))-1/(_sigma*_sigma))+plus*((x+_mu)*(x+_mu)/(pow(_sigma, 4))-1/(_sigma*_sigma));
}

double DoubleGaussian::Hamilt(double x){
    return -0.5*D2Sample(x)/Sample(x)+(pow(x, 4)-5.*x*x/2.);
}

double DoubleGaussian::pSample(const double &actual, const double &next){
    double p_old{pow(this->Sample(actual), 2)};
    double p_prop{pow(this->Sample(next), 2)};
    return fabs(p_prop/p_old);
}

BoltzDoubleGauss::BoltzDoubleGauss() : DoubleGaussian() {}

double BoltzDoubleGauss::BoltzmannWeight(double hamilt){
    return exp(-_beta*hamilt);
}

double BoltzDoubleGauss::pSample(const double &actual, const double &next){
    double p_old{this->BoltzmannWeight(this->Hamilt(actual))};
    double p_prop{this->BoltzmannWeight(this->Hamilt(next))};
    return fabs(p_old/p_prop);
}

