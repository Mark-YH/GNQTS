//
// Created by Mark Hsu on 2020/3/27.
//

#ifndef GNQTS_STOCK_QTS_H
#define GNQTS_STOCK_QTS_H

#include "../Model/Model.h"
#include "Particle_FA.h"

class QTS {
public:
    QTS() = delete;

    explicit QTS(Model &m, vector<int> &selection);

    ~QTS();

    void run();

    int getBestGeneration() const;

private:
    void measure();

    void evaluate(int generation);

    void update();

    void normalize(double *_allocRatio) const;

    int numOfBit;
    Model *model;
    vector<int> stockSelection;
    vector<int> indexOfChosen;
    vector<ParticleFA> particle;
    ParticleFA *gBest;
    ParticleFA *lWorst;
    double **pMatrix;
    int bestGeneration;
};

#endif //GNQTS_STOCK_QTS_H
