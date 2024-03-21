#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <random.h>
#include <version_config.h>
#include <fmtlib.h>
#include <ostream.h>

using namespace std;

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

/*  */
class BlockingDirectCall : public BlockingAvg
{
    private:
        virtual void Funny(Random &rnd) override;
    protected:
        virtual double Price(Random &rnd);
        double m_s0;    //starting price
        double m_T;     //final time
        double m_K;     //strike price
        double m_r;     //risk-free interest rate
        double m_vol;   //volatility - sigma
    public:
        BlockingDirectCall();
        BlockingDirectCall(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility);

};

class BlockingDiscreteCall : public BlockingDirectCall
{
    protected:
        double Price(Random &rnd) override;
    public:
        BlockingDiscreteCall();
        BlockingDiscreteCall(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility);

};

class BlockingDirectPut : public BlockingDirectCall
{
    private:
        virtual void Funny(Random &rnd) override;
    public:
        BlockingDirectPut();
        BlockingDirectPut(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility);
};

class BlockingDiscretePut : public BlockingDiscreteCall
{
    private:
        virtual void Funny(Random &rnd) override;
    public:
        BlockingDiscretePut();
        BlockingDiscretePut(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility);
};

