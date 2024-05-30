#pragma once

#include <blocking.h>

#include <array>
#include <numeric>

using namespace std;


class UniformStep {
    public:
        UniformStep(double delta);
        void setDelta(double delta){this->delta = delta;}
        virtual void Step(Random &rnd, const array<double, 3> &actual, array<double, 3> &next);
        virtual void Step(Random &rnd, const double &actual, double &next);
    protected:
        double delta;
};


class DoubleGaussian{
    public:
        DoubleGaussian();
        void SetParameters(double mu, double sigma);
        const double getMu() {return _mu;}
        const double getSigma() {return _sigma;}
        double Sample(double x);
        double D2Sample(double x);
        double Hamilt(double x);
        virtual double pSample(const double &actual, const double &next);
    private:
        double _mu, _sigma;
};

class BoltzDoubleGauss : public DoubleGaussian{
    public:
        BoltzDoubleGauss();
        void setBeta(double beta){_beta = beta;}
        const double getBeta(){return _beta;}
        double BoltzmannWeight(double hamilt);
        double pSample(const double &actual, const double &next) override;
    private:
        double _beta;
};

/* mother class for blocking avg */

class Metro : public BlockingAvg
{
    public:
        Metro();
        Metro(double x, double delta);
        Metro(double x, std::shared_ptr<UniformStep> &Step);
        Metro(double x, double delta, std::shared_ptr<DoubleGaussian> &Acc);
        Metro(double x, int n_throws, int n_blocks, std::shared_ptr<DoubleGaussian> &Acc, std::shared_ptr<UniformStep> &Step);
        const double getActual(){return actual;}
        const double getAvg(){return cum_avg/static_cast<double>(n_blocks);}
        const double getAvg2(){return cum_avg2/static_cast<double>(n_blocks);}
        const double getAccepted(){return output[0];}
        void Funny(Random &rnd) override;
        double rDistance(const double &pos);
    protected:
        //accepted position
        double actual;
        //proposed position
        double next;
    private:
        std::shared_ptr<UniformStep> mstep;
        std::shared_ptr<DoubleGaussian> macc;
};

