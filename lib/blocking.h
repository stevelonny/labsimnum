#pragma once

#include <fstream>
#include <iostream>
#include <cmath>
#include <random.h>
#include <version_config.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <vector>
#include <algorithm>

using namespace std;

/* mother class for blocking avg */
class BlockingAvg
{
private:

protected:
    vector<double> output;
    double cum_avg, cum_avg2;
    double sum;
    int n_throws, n_blocks, l_block;
public:
    BlockingAvg();
    BlockingAvg(int n_throws, int n_blocks);
    const int getNThrows(){return n_throws;}
    const int getNBlocks(){return n_blocks;}
    const int getLBlock(){return l_block;}
    virtual ~BlockingAvg();
    void Calculate(int n_throws, int n_blocks, Random &rnd, ofstream &file);
    virtual void Funny(Random &rnd);
    virtual void Average(Random &rnd);
    double Error(double ave, double av2, int i);
    void Reset();
    void Print(std::ofstream &file, int i);
};
