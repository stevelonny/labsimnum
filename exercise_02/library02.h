#pragma once

#include <library01.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <random.h>
#include <version_config.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace std;

/* functions classes for montecarlo simulations */
class Function
{
    public:
        virtual double Eval(double x) = 0;
};

class Coseno : public Function
{
    public:
        double Eval(double x) override;
};

/* montecarlo blocking avg with uniform distribution */
class BlockingMonte : public BlockingAvg
{
    public:
        BlockingMonte();
        BlockingMonte(Function &fun);
        BlockingMonte(Function &fun, double a, double b);
        void SetInterval(double a, double b);
    protected:
        double a, b;
        Function *fun;
    private:
        void Funny(Random &rnd) override;
        virtual double Denominator(double x);
        virtual double Probability(Random &rnd);    
};

/* montecarlo blck avg with gaussian distribution */
class MonteLin : public BlockingMonte
{
    public:
        MonteLin();
        MonteLin(BlockingMonte &monte);
        MonteLin(Function &fun, double a, double b);
    private:
        double Denominator(double x) override;
        double Probability(Random &rnd) override;
};


/* inverse distribution */
double exponential_distribution(double rand, double lambda);
double cauchy_distribution(double rand, double center, double gamma);

/* discrete random walk */
//as the class is heavily modified in respect to the inherited methods of blockingavg, rewrite should be in order
class BlockingLattice : public BlockingAvg
{
private:
    void Funny(Random &rnd);
    void Average(Random &rnd);
    vector<double> postep;
    vector<double> posavg;
    vector<double> posavg2;
    int nstep;
protected:
    double x, y, z;
public:
    void Calculate(int throws, int blocks, Random &rnd, ofstream &file) override;
    BlockingLattice();
    ~BlockingLattice();
};


/* continue random path */
class ContinueLattice : public BlockingLattice
{
    private:
        void Funny(Random &rnd) override;
    public:
        ContinueLattice();
};
