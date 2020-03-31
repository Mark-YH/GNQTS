//
// Created by Lab114 on 2020/3/27.
//

#ifndef GNQTS_STOCK_QTS_H
#define GNQTS_STOCK_QTS_H

#include "../Model/Model.h"
#include "Particle_FA.h"
#include <vector>

using std::vector;

class QTS {
public:
    QTS() = delete;

    explicit QTS(Model *m, int *selection);

    ~QTS();

    void run();

private:
    void measure(int generation);

    void evaluate(int generation);

    void update(int generation);

    Model *model;
    int *stockSelection;
    vector<ParticleFA> particle;
    ParticleFA *gBest;
    ParticleFA *lWorst;
    double *pMatrix;
};

#endif //GNQTS_STOCK_QTS_H
