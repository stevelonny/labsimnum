#pragma once

#include <fstream>
#include <iostream>
#include <cmath>
#include <random.h>
#include <version_config.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace std;

/* init random number generator */
//void initRandom(Random &rnd, string seedfile);

/* mother class for blocking avg */
class BlockingAvg
{
private:
    virtual void Funny(Random &rnd);
    virtual void Average(Random &rnd);
protected:
    double Error(double ave, double av2, int i);
    double cum_avg, cum_avg2;
    double sum;
    int n_throws, n_blocks, l_block;
public:
    BlockingAvg();
    virtual ~BlockingAvg();
    virtual void Calculate(int n_throws, int n_blocks, Random &rnd, ofstream &file);
};

/* child class for sigma^2 */
class BlockingVar : public BlockingAvg
{
private:
    void Funny(Random &rnd) override;
};

/* child class for buffon's needle */
class BlockingBuffon : public BlockingAvg
{
    public:
        BlockingBuffon();
        BlockingBuffon(double needle, double distance);
    private:
        double l, d;
        void Average(Random &rnd) override;
        void Funny(Random &rnd) override;
};

/* inverse distribution */
double exponential_distribution(double rand, double lambda);
double cauchy_distribution(double rand, double center, double gamma);
