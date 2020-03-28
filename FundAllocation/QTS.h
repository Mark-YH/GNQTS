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
    void generate();

    void evaluate(int generation);

    void update(int generation);

    Model *model;
    vector<int> indexOfChosen;
    int bestGeneration;
    int *stockSelection;
    vector<ParticleFA> particle;
    ParticleFA *gBest;
    ParticleFA *lBest;
    ParticleFA *lWorst;
};

#endif //GNQTS_STOCK_QTS_H
