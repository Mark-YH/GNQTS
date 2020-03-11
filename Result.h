//
// Created by Lab114 on 2020/3/11.
//

#ifndef GNQTS_STOCK_RESULT_H
#define GNQTS_STOCK_RESULT_H

#include "Stock.h"

class Result {
public:
    Result() {

    };

    ~Result() {
        delete[] stocks;
        delete[] amount;
    };

    void generateOutput() {

    };
    int generation;
    int population;
    double uBound; // the upper bound of rotation angle
    double lBound; // the lower bound of rotation angle
    double theta; // rotation angle
    int round; // times of experiments

    double initFund;
    double finalFund;
    double realReturn;
    double expectedReturn;
    double risk;
    double gBest;
    double atGen; // found the best solution at which generation
    double atRound; // found the best solution at which round
    double foundBestCount; // how many times it found the best solution in N rounds

    double numOfChosen; // how many stock been chosen
    Stock *stocks; // allocate `numOfChosen` stocks array
    int *amount; // allocate `numOfChosen` amount array
    int *allocatedFund; // allocated fund of each stock
    double *balance; // balance of each stock
};

#endif //GNQTS_STOCK_RESULT_H
