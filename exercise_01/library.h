#pragma once

#include <fstream>
#include <iostream>
#include <cmath>
#include <random.h>
#include <version_config.h>

using namespace std;

void initRandom(Random &rnd, string seedfile);
double error(double ave, double av2, int i);
