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
    int n_throws, n_blocks, l_block;
    double cum_sum, cum_sum2;
    virtual void funny(double rand);
    double Error(double ave, double av2, int i);
protected:
    double sum;
public:
    BlockingAvg();
    virtual ~BlockingAvg();
    void Calculate(int n_throws, int n_blocks, Random &rnd, ofstream &file);
};

class BlockingVar : public BlockingAvg
{
private:
    void funny(double rand) override;
};


void initRandom(Random &rnd, string seedfile);
double error(double ave, double av2, int i);

double expo(double rand, double lambda);
double cauchy(double rand, double center, double gamma);
