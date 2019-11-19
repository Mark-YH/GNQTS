//
// Created by Mark Hsu on 2019/11/6.
//

#ifndef GNQTS_STOCK_GNQTS_H
#define GNQTS_STOCK_GNQTS_H

#include "Particle.h"
#include "Model.h"
#include "Stock.h"
#include <string>

#define DEBUG 1

class GNQTS {
public:
    GNQTS();

    void initStock(std::string path);

    void readData(std::string path);

    void run();

    void measure();

    void calcFitness();

    void update();

private:
    Model model;
    Particle *particles;
    Particle bestParticle;
    Particle worstParticle;
    double *pMatrix;
    Stock *stocks;
    int numOfStocks = 0;
    int numOfDays = 0;
};


#endif //GNQTS_STOCK_GNQTS_H
