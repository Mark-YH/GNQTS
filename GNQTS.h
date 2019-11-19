//
// Created by Mark Hsu on 2019/11/6.
//

#ifndef GNQTS_STOCK_GNQTS_H
#define GNQTS_STOCK_GNQTS_H

#include "Model.h"
#include <string>

#define DEBUG 1

class GNQTS {
public:
    GNQTS();

    void initStock(const std::string &path);

    void readData(const std::string &path);

    void run();

    void measure();

    void calcFitness();

    void update();

private:
    Model model;
    Particle *particle;
    Particle bestParticle;
    Particle worstParticle;
    double *pMatrix;
    Stock *stocks;
    int numOfStocks;
    int numOfDays;
};


#endif //GNQTS_STOCK_GNQTS_H
