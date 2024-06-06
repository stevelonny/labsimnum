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

/* base class for direct call */
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

/* child class for discrete call */
class BlockingDiscreteCall : public BlockingDirectCall
{
    protected:
        double Price(Random &rnd) override;
    public:
        BlockingDiscreteCall();
        BlockingDiscreteCall(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility);

};

/* child class for direct put */
class BlockingDirectPut : public BlockingDirectCall
{
    private:
        virtual void Funny(Random &rnd) override;
    public:
        BlockingDirectPut();
        BlockingDirectPut(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility);
};

/* child class for discrete put */
class BlockingDiscretePut : public BlockingDiscreteCall
{
    private:
        virtual void Funny(Random &rnd) override;
    public:
        BlockingDiscretePut();
        BlockingDiscretePut(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility);
};

