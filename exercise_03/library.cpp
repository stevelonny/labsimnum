#include "library.h"

/* base class for blocking average */
BlockingAvg::BlockingAvg()
{
}

BlockingAvg::~BlockingAvg()
{
}

void BlockingAvg::Calculate(int n_throws, int n_blocks, Random &rnd, ofstream &file){
    //reset cumulative avg
    cum_avg = 0.;
    cum_avg2 = 0.;
    // set lenght of each block
    l_block = (double)n_throws/(double)n_blocks;
    for(int i{0}; i<n_blocks; i++){
        //reset current sum
        sum = 0;
        fmt::print(cerr, "Running block # {0}/{1}\r", i+1, n_blocks);
        Average(rnd);
        //write out to file
        fmt::print(file, "{0}\t{1}\t{2}\n", (i+1), cum_avg/(double)(i+1), Error(cum_avg/(double)(i+1), cum_avg2/(double)(i+1), i));
    }
    std::fflush(stdout);
}

// the need of a overridable Average is needed by the buffon needle and may be needed by other strange distributions
void BlockingAvg::Average(Random &rnd){
    for (int i=0; i<l_block; i++){
        Funny(rnd);
    }
    cum_avg += sum/(double)l_block;
    cum_avg2 += pow(sum/(double)l_block, 2);
}

// Funny is interchangable, it's the characteristic of the distribution
void BlockingAvg::Funny(Random &rnd){
    sum += rnd.Rannyu();
}

double BlockingAvg::Error(double ave, double av2, int i){
    if(i == 0){
        return 0.;
    }
    else{
        return sqrt(fabs(av2 - ave*ave)/((double)i));
    }
}

/* call option with direct price estimation */
BlockingDirectCall::BlockingDirectCall()
    : m_s0{100.}, m_T{1.}, m_K{100.}, m_r{0.1}, m_vol{0.25} {}

BlockingDirectCall::BlockingDirectCall(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility)
    : m_s0{starting_price}, m_T{final_time}, m_K{strike_price}, m_r{risk_free_interest}, m_vol{volatility} {}

void BlockingDirectCall::Funny(Random &rnd){
    sum += exp(-m_r*m_T) * max(0., Price(rnd)-m_K);
}

double BlockingDirectCall::Price(Random &rnd){
    double w{rnd.Gauss(0., m_T)};
    return m_s0*exp((m_r - 0.5*m_vol*m_vol)*m_T + m_vol*w);
}

/* call option with discret price estimation */
BlockingDiscreteCall::BlockingDiscreteCall(){}

BlockingDiscreteCall::BlockingDiscreteCall(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility)
    : BlockingDirectCall(starting_price, final_time, strike_price, risk_free_interest, volatility)
{}

double BlockingDiscreteCall::Price(Random &rnd){
    double price{m_s0};
    double h{m_T/(double)100};
    for(int i{0}; i<100; i++){
        double w{rnd.Gauss(0., 1.)};
        price = price * exp((m_r - 0.5*m_vol*m_vol)*h + m_vol*w*sqrt(h));
    }
    return price;
}

/*  */
BlockingDirectPut::BlockingDirectPut(){}
BlockingDirectPut::BlockingDirectPut(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility)
    : BlockingDirectCall(starting_price, final_time, strike_price, risk_free_interest, volatility)
{}

void BlockingDirectPut::Funny(Random &rnd){
    sum += exp(-m_r*m_T) * max(0., m_K-Price(rnd));
}

/*  */
BlockingDiscretePut::BlockingDiscretePut(){}
BlockingDiscretePut::BlockingDiscretePut(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility)
    : BlockingDiscreteCall(starting_price, final_time, strike_price, risk_free_interest, volatility)
{}

void BlockingDiscretePut::Funny(Random &rnd){
    sum += exp(-m_r*m_T) * max(0., m_K-Price(rnd));
}

