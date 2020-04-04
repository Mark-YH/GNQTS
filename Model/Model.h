//
// Created by Mark Hsu on 2019/11/13.
//

#ifndef GNQTS_STOCK_MODEL_H
#define GNQTS_STOCK_MODEL_H

#include "Result.h"

#define US_MARKET 1
#define ROUND 50
#define MODE 1

/** @param MODE
 *  0: execute stock selection program
 *  1: execute fund allocation program
 *  2: execute fund allocation with method of exhaustion program
 *  3: execute ranking program
 */

class Model {
public:
    Result *result;

    Model() = delete;

    Model(int population, int generation, double theta, double fund, double fee, double tax);

    ~Model();

    int getGeneration() const;

    int getPopulation() const;

    double getTheta() const;

    double getFitness(vector<int> &solution, int pIndex, const vector<double> &allocRatio);

    void setResult(Result *rs);

    int getNumOfStocks() const;

    int getNumOfDays() const;

    void nextSection(int section);

    string getStockSymbol(int i);

private:
    int population;
    int generation;
    vector<Stock> stocks;
    int numOfStocks;
    int numOfDays;
    double initFund;
    double theta;
    double fee;
    double tax;

    void getNumOfRowColumn(const std::string &path);

    void readData(const std::string &path);

    void calcFS(vector<int> &solution, vector<int> &allocatedFund, int pIndex);
};

#endif //GNQTS_STOCK_MODEL_H
