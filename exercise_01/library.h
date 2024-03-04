#pragma once

#include <fstream>
#include <iostream>
#include <cmath>
#include <random.h>
#include <version_config.h>
#include <fmtlib.h>
#include <ostream.h>
#include <random.h>

using namespace std;

class BlockingAvg
{
private:
    virtual void funny(Random &rnd);
    virtual void avg(Random &rnd);
    double Error(double ave, double av2, int i);
protected:
    double cum_avg, cum_avg2;
    double sum;
    int n_throws, n_blocks, l_block;
public:
    BlockingAvg();
    virtual ~BlockingAvg();
    void Calculate(int n_throws, int n_blocks, Random &rnd, ofstream &file);
};

class BlockingVar : public BlockingAvg
{
private:
    void funny(Random &rnd) override;
};

class BlockingBuffon : public BlockingAvg
{
    public:
        BlockingBuffon();
        BlockingBuffon(double needle, double distance);
    private:
        double l, d;
        void avg(Random &rnd) override;
        void funny(Random &rnd) override;
};


void initRandom(Random &rnd, string seedfile);
double error(double ave, double av2, int i);

double expo(double rand, double lambda);
double cauchy(double rand, double center, double gamma);
