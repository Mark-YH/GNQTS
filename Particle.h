//
// Created by Mark Hsu on 2019/11/6.
//

#ifndef GNQTS_STOCK_PARTICLE_H
#define GNQTS_STOCK_PARTICLE_H

class Particle {
public:
    int *solution;
    double fitness;

    void setSolutionSize(int size) {
        this->solution = new int[size];
    }
};

#endif //GNQTS_STOCK_PARTICLE_H
