#pragma once

#include <blocking.h>

#include <array>
#include <numeric>

using namespace std;


class UniformStep {
    public:
        UniformStep(double delta);
        virtual void Step(Random &rnd, const array<double, 3> &actual, array<double, 3> &next);
    protected:
        double delta;
};

class GaussStep : public UniformStep{
    public:
        GaussStep(double delta);
        void Step(Random &rnd, const array<double, 3> &actual, array<double, 3> &next) override;
};

class Acceptance {
    public:
        virtual double pSample(const array<double, 3> &actual, const array<double, 3> &next) = 0;
};

class GroundState : public Acceptance {
    public:
        double pSample(const array<double, 3> &actual, const array<double, 3> &next) override;
};

class FirstState : public GroundState {
    public:
        double pSample(const array<double, 3> &actual, const array<double, 3> &next) override;
};

/* mother class for blocking avg */

class Metro : public BlockingAvg
{
    public:
        Metro();
        Metro(double x, double y, double z, double delta);
        Metro(double x, double y, double z, std::shared_ptr<UniformStep> &Step);
        Metro(double x, double y, double z, double delta, std::shared_ptr<Acceptance> &Acc);
        Metro(double x, double y, double z, std::shared_ptr<Acceptance> &Acc, std::shared_ptr<UniformStep> &Step);
    protected:
        //accepted position
        array<double, 3> actual;
        //proposed position
        array<double, 3> next;
    private:
        void Funny(Random &rnd) override;
        double rDistance(const array<double, 3> &pos);
        std::shared_ptr<UniformStep> mstep;
        std::shared_ptr<Acceptance> macc;
};

