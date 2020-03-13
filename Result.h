//
// Created by Lab114 on 2020/3/11.
//

#ifndef GNQTS_STOCK_RESULT_H
#define GNQTS_STOCK_RESULT_H

#include "Stock.h"
#include "Logger.h"

class Result {
public:
    Result() = default;

    void setStock(int numOfStocks, int numOfDays) {
        this->numOfStocks = numOfStocks;
        this->numOfDays = numOfDays;
        this->balance = new double[numOfStocks];
        this->allocatedFund = new int[numOfStocks];
        this->amount = new int[numOfStocks];
        this->stocks = new Stock[numOfStocks];
        this->solution = new int[numOfStocks];
        this->totalFS = new double[numOfDays];
        for (int i = 0; i < numOfStocks; i++) {
            this->stocks[i].setPriceSize(numOfDays);
        }
    }

    ~Result() {
        delete[] this->stocks;
        delete[] this->amount;
        delete[] this->allocatedFund;
        delete[] this->balance;
        delete[] this->solution;
        delete[] this->totalFS;
    };

    void generateOutput() {
        Logger logger("../log/section_result.csv");
        logger.writeComma("Generation");
        logger.writeLine(this->generation);
        logger.writeComma("Population");
        logger.writeLine(this->population);
        logger.writeComma("Upper bound of theta");
        logger.writeLine(this->uBound);
        logger.writeComma("Lower bound of theta");
        logger.writeLine(this->lBound);
        logger.writeComma("theta");
        logger.writeLine(this->theta);
        logger.writeComma("Round");
        logger.writeLine(this->round);
        logger.writeLine("");

        logger.writeComma("Initial fund");
        logger.writeLine(this->initFund);
        logger.writeComma("Final fund");
        logger.writeLine(this->finalFund);
        logger.writeComma("Real return");
        logger.writeLine(this->realReturn);
        logger.writeLine("");

        logger.writeComma("Expected return");
        logger.writeLine(this->expectedReturn);
        logger.writeComma("Risk");
        logger.writeLine(this->risk);
        logger.writeComma("Global best");
        logger.writeLine(this->gBest);
        logger.writeComma("Found at generation");
        logger.writeLine(this->atGen + 1);
        logger.writeComma("Found at round");
        logger.writeLine(this->atRound + 1);
        logger.writeComma("Found the best solution frequency");
        logger.writeLine(this->foundBestCount);
        logger.writeLine("");

        logger.writeComma("Number of chosen");
        logger.writeLine(this->numOfChosen);
        logger.writeLine("");

        logger.writeComma("Stock#");
        for (int i = 0; i < this->numOfStocks; i++) {
            if (this->solution[i] == 1) {
                logger.write(this->stocks[i].code);
                logger.write("(");
                logger.write(i);
                logger.writeComma(")");
            }
        }
        logger.writeLine("");
        logger.writeComma("Amount");
        for (int i = 0; i < this->numOfStocks; i++) {
            if (solution[i] == 1)
                logger.writeComma(this->amount[i]);
        }
        logger.writeLine("");
        logger.writeComma("Allocated fund");
        for (int i = 0; i < this->numOfStocks; i++) {
            if (solution[i] == 1)
                logger.writeComma(this->allocatedFund[i]);
        }
        logger.writeLine("");

        logger.writeComma("Balance of each stock");
        for (int i = 0; i < this->numOfStocks; i++) {
            if (solution[i] == 1)
                logger.writeComma(this->balance[i]);
        }
        logger.writeLine("");
        for (int i = 0; i < this->numOfDays; i++) {
            logger.write("FS(");
            logger.write(i + 1);
            logger.writeComma(")");
            for (int j = 0; j < numOfStocks; j++) {
                if (this->solution[j] == 1) {
                    logger.writeComma(this->stocks[j].fs[i]);
                }
            }
            logger.writeComma(this->totalFS[i]);
            logger.writeLine("");
        }
        logger.writeLine("");
    };

    int generation;
    int population;
    int *solution;
    double uBound; // the upper bound of rotation angle
    double lBound; // the lower bound of rotation angle
    double theta; // rotation angle
    int round; // times of experiments

    double initFund;
    double finalFund;
    double realReturn;
    double expectedReturn;
    double risk;
    double gBest; // the best trend value
    double atGen; // found the best solution at which generation
    double atRound; // found the best solution at which round
    double foundBestCount; // how many times it found the best solution in N rounds

    double numOfChosen; // how many stock been chosen
    Stock *stocks; // allocate `numOfChosen` stocks array
    int *amount; // allocate `numOfChosen` amount array
    int *allocatedFund; // allocated fund of each stock
    double *balance; // balance of each stock
    double *totalFS;
    int numOfStocks;
    int numOfDays;
};

#endif //GNQTS_STOCK_RESULT_H
