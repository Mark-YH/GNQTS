//
// Created by Mark Hsu on 2019/11/13.
//

#ifndef GNQTS_STOCK_MODEL_H
#define GNQTS_STOCK_MODEL_H

#include "Logger.h"
#include "Particle.h"
#include "Stock.h"
#include <string>

class Model {
public:
    Model() = default;

    ~Model() = default;

    void init();

    void setGeneration(int generation);

    int getGeneration();

    void setPopulation(int population);

    int getPopulation();

    void setTheta(double theta);

    double getTheta();

    void setFund(double fund);

    void setFee(double fee);

    void setTax(double tax);

    int getLength();

    double getFitness(Particle p, int generation);

private:
    int population;
    int generation;
    Stock *stocks;
    int numOfStocks;
    int numOfDays;
    double fund;
    double theta;
    double fee;
    double tax;

    void initStock(const std::string &path);

    void readData(const std::string &path);
};

#endif //GNQTS_STOCK_MODEL_H
