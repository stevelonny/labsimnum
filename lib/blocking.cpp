#include "blocking.h"

/* base class for blocking average */
BlockingAvg::BlockingAvg() : output{{}}
{
}

BlockingAvg::BlockingAvg(int n_throws, int n_blocks) : n_throws{n_throws}, n_blocks{n_blocks}, output{{}}
{
    l_block = n_throws/n_blocks;
}

BlockingAvg::~BlockingAvg()
{
}

void BlockingAvg::Calculate(int n_throws, int n_blocks, Random &rnd, ofstream &file){
    Reset();
    l_block = (double)n_throws/(double)n_blocks;
    for(int i{0}; i<n_blocks; i++){
        //reset current sum
        if(!output.empty()){
            fill(output.begin(), output.end(), 0.);
        }
        sum = 0.;
        for (size_t i=0; i<l_block; i++){
            Funny(rnd);
        }
        fmt::print(cerr, "Running block # {0}/{1}\r", i+1, n_blocks);
        Average(rnd);
        //write out to file
        Print(file, i);
    }
    std::fflush(stdout);
}

void BlockingAvg::Reset()
{
    // reset cumulative avg
    cum_avg = 0.;
    cum_avg2 = 0.;
    sum = 0.;
    if(!output.empty()){
        fill(output.begin(), output.end(), 0.);
    }
}

void BlockingAvg::Print(std::ofstream &file, int i)
{
    fmt::print(file, "{0}\t{1}\t{2}\t{3}",
               (i + 1), cum_avg / (double)(i + 1), Error(cum_avg / (double)(i + 1), cum_avg2 / (double)(i + 1), i), sum / (double)l_block);
    for (int j{0}; j < output.size(); j++)
    {
        output[j] /= (double)l_block;
        fmt::print(file, "\t{0}", output[j]);
    }
    fmt::print(file, "\n");
}

// the need of a overridable Average is needed by the buffon needle and may be needed by other strange distributions
void BlockingAvg::Average(Random &rnd){
    cum_avg += sum/(double)l_block;
    cum_avg2 += pow(sum/(double)l_block, 2);
    sum = 0.;
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

