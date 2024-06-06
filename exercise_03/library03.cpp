#include <library03.h>

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

/* put option with direct price estimation */
BlockingDirectPut::BlockingDirectPut(){}
BlockingDirectPut::BlockingDirectPut(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility)
    : BlockingDirectCall(starting_price, final_time, strike_price, risk_free_interest, volatility)
{}

void BlockingDirectPut::Funny(Random &rnd){
    sum += exp(-m_r*m_T) * max(0., m_K-Price(rnd));
}

/* put option with discrete price estimation */
BlockingDiscretePut::BlockingDiscretePut(){}
BlockingDiscretePut::BlockingDiscretePut(double starting_price, double final_time, double strike_price, double risk_free_interest, double volatility)
    : BlockingDiscreteCall(starting_price, final_time, strike_price, risk_free_interest, volatility)
{}

void BlockingDiscretePut::Funny(Random &rnd){
    sum += exp(-m_r*m_T) * max(0., m_K-Price(rnd));
}

