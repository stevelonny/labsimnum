#include "library.h"

Metro::Metro() : BlockingAvg(), actual{{0.,0.,1.5/2.}}, next{{0.,0.,1.5/2.}}
{
    output = {0.};
    mstep = new UniformStep(1.5/2.);
    macc = new GroundState();
}

Metro::Metro(double x, double y, double z, double delta)
    : BlockingAvg(), actual{{x,y,z}}, next{{x,y,z}}
{
    output = {0.};
    mstep = new UniformStep(delta);
    macc = new GroundState();
}

Metro::Metro(double x, double y, double z, UniformStep &Step) :
    BlockingAvg(), actual{{x,y,z}}, next{{x,y,z}}, mstep(&Step)
{
    output = {0.};
    macc = new GroundState();
}

Metro::Metro(double x, double y, double z, double delta, Acceptance &Acc) :
    BlockingAvg(), actual{{x,y,z}}, next{{x,y,z}}, macc(&Acc)
{
    output = {0.};
    mstep = new UniformStep(delta);
}

Metro::Metro(double x, double y, double z, Acceptance &Acc, UniformStep &Step) :
    BlockingAvg(), actual{{x,y,z}}, next{{x,y,z}}, macc(&Acc), mstep(&Step)
{
    output = {0.};
}


void Metro::Funny(Random &rnd){
    mstep->Step(rnd, actual, next);
    double acc{min(1., macc->pSample(actual, next))};
    double r{rnd.Rannyu()};
    if(r>acc){
        sum += rDistance(actual);
        next = actual;
    }
    else{
        sum += rDistance(next);
        actual = next;
        output[0] += 1.;
    }
}

double Metro::rDistance(const array<double, 3> &pos){
    return sqrt(pos[0]*pos[0]+pos[1]*pos[1]+pos[2]*pos[2]);
}

UniformStep::UniformStep(double delta) : delta{delta} {}

void UniformStep::Step(Random &rnd, const array<double, 3> &actual, array<double, 3> &next){
    for(int i{0}; i<3; i++){
        next[i] = actual[i] + rnd.Rannyu(-delta, delta);
    }
}

GaussStep::GaussStep(double delta) : UniformStep(delta) {}

void GaussStep::Step(Random &rnd, const array<double, 3> &actual, array<double, 3> &next){
    for(int i{0}; i<3; i++){
        next[i] = actual[i] + rnd.Gauss(0, delta);
    }
}

double GroundState::pSample(const array<double, 3> &actual, const array<double, 3> &next){
    double p_old{exp(-2*sqrt(actual[0]*actual[0]+actual[1]*actual[1]+actual[2]*actual[2]))};
    double p_prop{exp(-2*sqrt(next[0]*next[0]+next[1]*next[1]+next[2]*next[2]))};
    return fabs(p_prop/p_old);
}


double FirstState::pSample(const array<double, 3> &actual, const array<double, 3> &next){
    double old_r{sqrt(actual[0]*actual[0]+actual[1]*actual[1]+actual[2]*actual[2])};
    double prop_r{sqrt(next[0]*next[0]+next[1]*next[1]+next[2]*next[2])};
    double old_cost{(actual[2]*actual[2])/(actual[0]*actual[0]+actual[1]*actual[1]+actual[2]*actual[2])};
    double prop_cost{(next[2]*next[2])/(next[0]*next[0]+next[1]*next[1]+next[2]*next[2])};
    double p_old{old_r*old_r*exp(-old_r)*old_cost};
    double p_prop{prop_r*prop_r*exp(-prop_r)*prop_cost};
    return fabs(p_prop/p_old);
}
