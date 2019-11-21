//
// Created by Mark Hsu on 2019/11/6.
//

#ifndef GNQTS_STOCK_GNQTS_H
#define GNQTS_STOCK_GNQTS_H

#include "Model.h"

class GNQTS {
public:
    GNQTS();

    void run();

    void measure();

    void calcFitness(int generation);

    void update();

    void mutate();

private:
    Model model;
    Particle *particle;
    Particle bestParticle;
    Particle worstParticle;
    int bestGeneration;
    double *pMatrix;
};

#endif //GNQTS_STOCK_GNQTS_H
