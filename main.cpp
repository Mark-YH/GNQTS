#include "GNQTS.h"
#include <iostream>
#include <chrono>

void test() {
    int section = 11;
    int portfolio_a = 2;
    int portfolio_b = 46;

    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.nextSection(section);
    Particle particle;
    particle.setSolutionSize(model.getNumOfStocks());
    Result result;
    result.setStock(model.getNumOfStocks(), model.getNumOfDays());
    model.setResult(&result);

    for (int i = 0; i < model.getNumOfStocks(); i++) {
        if (i == portfolio_a || i == portfolio_b)
            particle.solution[i] = 1;
        else
            particle.solution[i] = 0;
    }

    for (int i = 0; i <= 1000; i++) {
        model.getFitness(&particle, 0, -1,
                         std::make_pair(std::make_pair(portfolio_a, i), std::make_pair(portfolio_b, 1000 - i)));
        result.generateOutput(0);
        result.finalOutput(1);
    }
}

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
#if TESTING
    test();
#else
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
            } else if (gBest < result->gBest) { // found the global best which is a brand new solution
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
                finalResult->generation = result->generation;
                finalResult->population = result->population;
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
                    finalResult->stocks[j].price = result->stocks[j].price;
                    for (int k = 0; k < model->getNumOfDays(); k++) {
                        finalResult->stocks[j].fs[k] = result->stocks[j].fs[k];
                    }
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
#endif
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
    return 0;
}