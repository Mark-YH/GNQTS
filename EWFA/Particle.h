//
// Created by Mark Hsu on 2019/11/6.
//

#ifndef GNQTS_STOCK_PARTICLE_H
#define GNQTS_STOCK_PARTICLE_H

#include <cfloat>

class Particle {
public:
    vector<int> solution{};
    double fitness{-DBL_MAX};

    explicit Particle(int size) : solution(size), fitness{-DBL_MAX} {};
};

#endif //GNQTS_STOCK_PARTICLE_H
