//
// Created by Lab114 on 2020/3/11.
//

#ifndef PORTFOLIO_OPTIMIZATION_RESULT_H
#define PORTFOLIO_OPTIMIZATION_RESULT_H

#include "Stock.h"
#include "Logger.h"
#include <vector>

using std::vector;

class Result {
public:
    Result() = delete;

    Result(int _numOfStocks, int _numOfDays) :
            numOfStocks{_numOfStocks},
            numOfDays{_numOfDays},
            balance(_numOfStocks),
            allocatedFund(_numOfStocks),
            amount(_numOfStocks),
            solution(_numOfStocks),
            totalFS(_numOfDays),
            line(_numOfDays) {
        for (int i = 0; i < _numOfStocks; i++)
            stocks.emplace_back(_numOfDays);
    };

    void totalTestResult(string &market, string &alias, string &mode, string &sw, vector<string> &trainingPeriod,
                         vector<string> &testingPeriod) const {
        Logger logger("../log/" + market + " " + alias + "/" + mode + "/" + sw + "/" + alias + "_" + mode + "_" + sw +
                      "_total_test_result.csv");

        logger.writeComma("Testing period");
        logger.writeSpace(testingPeriod.front());
        logger.writeSpace("-");
        logger.writeLine(testingPeriod.back());

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
        logger.writeComma("Fluctuation");
        logger.writeLine(this->fluctuation);
        logger.writeComma("Emotion index");
        logger.writeLine(this->emotionIndex);
        logger.writeLine("");

        logger.writeComma("Expected return");
        logger.writeLine(this->expectedReturn);
        logger.writeComma("Risk");
        logger.writeLine(this->risk);
        logger.writeComma("Global best");
        logger.writeLine(this->gBest);
        logger.writeLine("");

        logger.writeComma("Total number of days");
        logger.writeLine(this->totalFS.size());
        for (int i = 0; i < this->numOfDays; i++) {
            logger.write("FS(");
            logger.write(i + 1);
            logger.writeComma(")");

            logger.writeComma(this->totalFS[i]);

            logger.writeSpace("Line");
            logger.writeComma(i + 1);
            logger.writeLine(this->line[i]);
        }
    }

    void generateOutput(int period, bool isTraining, string &market, string &alias, string &mode, string &sw,
                        vector<string> &trainingPeriod, vector<string> &testingPeriod) const {
        Logger logger;

        if (isTraining)
            logger.setPath("../log/" + market + " " + alias + "/" + mode + "/" + sw + "/" + alias + "_" + mode + "_" +
                           trainingPeriod[period]);
        else
            logger.setPath("../log/" + market + " " + alias + "/" + mode + "/" + sw + "/" + alias + "_" + mode + "_" +
                           testingPeriod[period]);

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
        logger.writeComma("Fluctuation");
        logger.writeLine(this->fluctuation);
        logger.writeComma("Emotion index");
        logger.writeLine(this->emotionIndex);
        logger.writeLine("");

        logger.writeComma("Expected return");
        logger.writeLine(this->expectedReturn);
        logger.writeComma("Risk");
        logger.writeLine(this->risk);
        logger.writeComma("Global best");
        logger.writeLine(this->gBest);
        if (isTraining) {
            logger.writeComma("Found at generation");
            logger.writeLine(this->atGen + 1);
            logger.writeComma("Found at round");
            logger.writeLine(this->atRound + 1);
            logger.writeComma("Found the best solution frequency");
            logger.writeLine(this->foundBestCount);
            logger.writeLine("");
        }

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
            logger.writeSpace("Line");
            logger.writeComma(i + 1);
            logger.writeLine(this->line[i]);
        }
        logger.writeLine("");
    };

    void finalOutput(int period, bool isTraining, string &market, string &alias, string &mode, string &sw,
                     vector<string> &trainingPeriod, vector<string> &testingPeriod) const {
        Logger logger;
        if (isTraining)
            logger.setPath(
                    "../log/" + market + " " + alias + "/" + mode + "/" + sw + "/" + alias + "_" + mode + "_" + sw +
                    "_final_result.csv");
        else
            logger.setPath(
                    "../log/" + market + " " + alias + "/" + mode + "/" + sw + "/" + alias + "_" + mode + "_" + sw +
                    "_final_test_result.csv");

        if (period == 0) {
            logger.writeComma(sw);
            logger.writeComma("Period");
            logger.writeComma("Number of chosen");
            logger.writeComma("Portfolio[index](amount)(fund)");
            logger.writeComma("gBest");
            logger.writeComma("Expected return");
            logger.writeComma("Risk");
            if (isTraining) {
                logger.writeComma("at round");
                logger.writeComma("at generation");
                logger.writeLine("Found best count");
            } else {
                logger.writeComma("Emotion index");
                logger.writeComma("Real return");
                logger.writeLine("Fluctuation");
            }
        }
        logger.writeComma(period + 1);
        if (isTraining)
            logger.writeComma(trainingPeriod[period]);
        else
            logger.writeComma(testingPeriod[period]);
        logger.writeComma(this->numOfChosen);
        for (int i = 0; i < this->numOfStocks; i++) {
            if (this->solution[i] == 1 && this->allocatedFund[i] > 0) {
                logger.write(this->stocks[i].code);
                logger.write("[");
                logger.write(i);
                logger.write("](");
                logger.write(this->amount[i]);
                logger.write(")(");
                logger.write(this->allocatedFund[i]);
                logger.writeSpace(")");
            }
        }
        logger.writeComma("");
        logger.writeComma(this->gBest);
        logger.writeComma(this->expectedReturn);
        logger.writeComma(this->risk);
        if (isTraining) {
            logger.writeComma(this->atRound + 1);
            logger.writeComma(atGen + 1);
            logger.writeLine(this->foundBestCount);
        } else {
            logger.writeComma(this->emotionIndex);
            logger.writeComma(this->realReturn);
            logger.writeLine(this->fluctuation);
        }
    }

    void convergence(string &market, string &alias, string &mode, string &sw, string periodName, int size,
                     double *convergence, int generation) {
        Logger logger(10);
        logger.setPath(
                "../log/" + market + " " + alias + "/" + mode + "/" + sw + "/" + alias + "_" + mode + "_convergence_" +
                periodName);
        logger.writeComma("Generation");
        logger.writeLine("Fitness");
        for (int i = 0; i < generation; i++) {
            logger.writeComma(i + 1);
            logger.writeLine(convergence[i] / (float) size);
        }
    }

    int generation{};
    int population{};
    vector<int> solution{};
    double uBound{}; // the upper bound of rotation angle
    double lBound{}; // the lower bound of rotation angle
    double theta{}; // rotation angle
    int round{}; // times of experiments

    double initFund{};
    double finalFund{};
    double realReturn{};
    double fluctuation{};
    double emotionIndex{};
    vector<double> line{};
    double expectedReturn{};
    double risk{};
    double gBest{}; // the best trend value
    double atGen{}; // found the best solution at which generation
    double atRound{}; // found the best solution at which round
    double foundBestCount{}; // how many times it found the best solution in N rounds

    double numOfChosen{}; // how many stock been chosen
    vector<Stock> stocks{}; // allocate `numOfChosen` stocks array
    vector<int> amount{}; // allocate `numOfChosen` amount array
    vector<int> allocatedFund{}; // allocated fund of each stock
    vector<double> balance{}; // balance of each stock
    vector<double> totalFS{};
    int numOfStocks{};
    int numOfDays{};
};

#endif //PORTFOLIO_OPTIMIZATION_RESULT_H
