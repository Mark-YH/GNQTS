#include "GNQTS.h"
#include <iostream>
#include <chrono>

int main() {
    std::remove("../log/result.csv");
    std::remove("../log/best&worst_particle.csv");
    std::remove("../log/fitness.csv");
    std::remove("../log/fund_standardization.csv");
    std::remove("../log/measure.csv");
    std::remove("../log/mutate.csv");
    std::remove("../log/random_matrix.csv");
    std::remove("../log/read_data_for_debug.csv");
    std::remove("../log/stock.csv");
    std::remove("../log/stock2.csv");
    std::remove("../log/trend_value.csv");
    std::remove("../log/update.csv");
    std::remove("../log/section_result.csv");
    auto start = std::chrono::steady_clock::now();
    srand(114);
    Model *model = new Model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    for (int j = 0; j < numOfSection; j++) { // section
        model->nextSection(j);
        Result *result = new Result();
        Result *finalResult = new Result();
        result->setStock(model->getNumOfStocks(), model->getNumOfDays());
        finalResult->setStock(model->getNumOfStocks(), model->getNumOfDays());
        model->setResult(result);
        result->foundBestCount = 1;
        double gBest = -std::numeric_limits<double>::max();
        for (int i = 0; i < ROUND; i++) { // round
            GNQTS *qts = new GNQTS(model);
            qts->run();

            if (gBest == result->gBest) {
                result->foundBestCount++;
                finalResult->foundBestCount++;
            }
            else if (gBest < result->gBest) { // found the global best which is a brand new solution
                gBest = result->gBest;
                result->foundBestCount = 1;
                result->atRound = i;
                result->atGen = qts->getBestGeneration();

                finalResult->numOfDays = result->numOfDays;
                finalResult->numOfStocks = result->numOfStocks;
                finalResult->numOfChosen = result->numOfChosen;
                finalResult->initFund = result->initFund;
                finalResult->foundBestCount = result->foundBestCount;
                finalResult->gBest = result->gBest;
                finalResult->expectedReturn = result->expectedReturn;
                finalResult->realReturn = result->realReturn;
                finalResult->finalFund = result->finalFund;
                finalResult->round = result->round;
                finalResult->theta = result->theta;
                finalResult->lBound = result->lBound;
                finalResult->uBound = result->uBound;
                finalResult->risk = result->risk;
                finalResult->atRound = result->atRound;
                finalResult->atGen = result->atGen;
                for (int j = 0; j < model->getNumOfStocks(); j++) {
                    finalResult->solution[j] = result->solution[j];
                    finalResult->allocatedFund[j] = result->allocatedFund[j];
                    finalResult->balance[j] = result->balance[j];
                    finalResult->amount[j] = result->amount[j];
                    finalResult->stocks[j].setPriceSize(model->getNumOfDays());
                    finalResult->stocks[j].code = result->stocks[j].code;
                    finalResult->stocks[j].fs = result->stocks[j].fs;
                    finalResult->stocks[j].price = result->stocks[j].price;
                }
                for (int j = 0; j < model->getNumOfDays(); j++) {
                    finalResult->totalFS[j] = result->totalFS[j];
                }
            }
            delete qts;
        }
        finalResult->generateOutput(j);
        finalResult->finalOutput(j);
        delete result;
    }
    delete model;
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
    return 0;
}