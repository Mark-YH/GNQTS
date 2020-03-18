//
// Created by Mark Hsu on 2019/11/13.
//

#ifndef GNQTS_STOCK_MODEL_H
#define GNQTS_STOCK_MODEL_H

#include "Particle.h"
#include "Result.h"
#include <string>
#include <utility>

#define ROUND 1

class Model {
public:
    Result *result;

    Model() = delete;

    Model(int population, int generation, double theta, double fund, double fee, double tax);

    ~Model();

    int getGeneration() const;

    int getPopulation() const;

    double getTheta() const;

    int getLength() const;

    double getFitness(Particle *p, int generation, int pIndex, std::pair<int, int> allocRatio);

    void setResult(Result *rs);

    int getNumOfStocks() const;

    int getNumOfDays() const;

    void nextSection(int section);

private:
    int population;
    int generation;
    Stock *stocks;
    int numOfStocks;
    int numOfDays;
    double initFund;
    double theta;
    double fee;
    double tax;

    void getNumOfRowColumn(const std::string &path);

    void readData(const std::string &path);

    void calcFS(Particle *p, int *allocatedFund, int gen, int pIndex);
};

#endif //GNQTS_STOCK_MODEL_H
