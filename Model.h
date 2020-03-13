//
// Created by Mark Hsu on 2019/11/13.
//

#ifndef GNQTS_STOCK_MODEL_H
#define GNQTS_STOCK_MODEL_H

#include "Particle.h"
#include "Result.h"
#include <string>

#define ROUND 1

class Model {
public:
    Result *result;

    Model() = delete;

    Model(int population, int generation, double theta, double fund, double fee, double tax);

    ~Model();

    int getGeneration();

    int getPopulation();

    double getTheta();

    int getLength();

    double getFund();

    double getFitness(Particle *p, int generation, int pIndex);

    void setResult(Result *rs);

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

    void getNumOfRowColumn(const std::string &path);

    void readData(const std::string &path);

    void calcFS(Particle *p, double lFund, int gen, int pIndex);
};

#endif //GNQTS_STOCK_MODEL_H
