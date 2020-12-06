//
// Created by Mark Hsu on 2019/11/6.
//

#ifndef GNQTS_STOCK_GNQTS_H
#define GNQTS_STOCK_GNQTS_H

#include "../Model/Model.h"
#include "Particle.h"

class GNQTS {
public:
    GNQTS() = delete;

    explicit GNQTS(Model *m);

    ~GNQTS();

    void run();

    void measure();

    void evaluate(int generation);

    void update();

    int getBestGeneration() const;

private:
    Model *model;
    vector<Particle> particle;
    Particle *bestParticle;
    Particle *worstParticle;
    int bestGeneration;
    double *pMatrix;
};

#endif //GNQTS_STOCK_GNQTS_H
