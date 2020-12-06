//
// Created by Mark Hsu on 2019/11/6.
//

#ifndef PORTFOLIO_OPTIMIZATION_PARTICLE_H
#define PORTFOLIO_OPTIMIZATION_PARTICLE_H

#include <cfloat>

class Particle {
public:
    vector<int> solution{};
    double fitness{-DBL_MAX};

    explicit Particle(int size) : solution(size), fitness{-DBL_MAX} {};
};

#endif //PORTFOLIO_OPTIMIZATION_PARTICLE_H
