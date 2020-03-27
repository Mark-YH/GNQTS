#include "GNQTS.h"
#include <iostream>
#include <chrono>
#include <vector>

using std::vector;

void test() {
    int section = 1;
    int portfolio_a = 4;
    int portfolio_b = 20;
    int portfolio_c = 44;
    int portfolio_d = 20;
    int portfolio_e = 20;
    int precision = 1000;

    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.nextSection(section);
    Particle particle;
    particle.setSolutionSize(model.getNumOfStocks());
    Result result;
    result.setStock(model.getNumOfStocks(), model.getNumOfDays());
    model.setResult(&result);

    for (int i = 0; i < model.getNumOfStocks(); i++) {
        if (i == portfolio_a || i == portfolio_b || i == portfolio_c || i == portfolio_d || i == portfolio_e)
            particle.solution[i] = 1;
        else
            particle.solution[i] = 0;
    }

    int *allocRatio = new int[model.getNumOfStocks()];
//    Result bestResult;
//    bestResult.setStock(model.getNumOfStocks(), model.getNumOfDays());
//    bestResult.gBest = -DBL_MAX;
    vector<Result> bestResults;
    bestResults.emplace_back();
    bestResults[0].gBest = -DBL_MAX;
    bestResults[0].setStock(model.getNumOfStocks(), model.getNumOfDays());

    for (int i = 0; i <= precision; i++) {
        for (int j = 0; j <= precision - i; j++) {
//            for (int k = 0; k <= precision - i - j; k++) {
//                for (int l = 0; l <= precision - i - j - k; l++) {
            allocRatio[portfolio_a] = i;
            allocRatio[portfolio_b] = j;
            allocRatio[portfolio_c] = precision - i - j;
//                    allocRatio[portfolio_c] = k;
//                    allocRatio[portfolio_d] = l;
//                    allocRatio[portfolio_e] = precision - i - j - k - l;
            model.getFitness(&particle, 0, -1, allocRatio);
            if (bestResults[0].gBest < result.gBest) { // found a better solution
                bestResults[0].copyResult(result);
                while (bestResults.size() > 1) {
                    bestResults.erase(bestResults.begin() + 1);
                }
            } else if (bestResults[0].gBest == result.gBest) { // found a solution with the same fitness
                int size = bestResults.size();
                Result *rs = new Result;
                bestResults.push_back(*rs);
                bestResults[size].setStock(model.getNumOfStocks(), model.getNumOfDays());
                bestResults[size].copyResult(result);
                rs = nullptr;
            }
            if ((allocRatio[portfolio_a] == precision) ||
                (allocRatio[portfolio_b] == precision) ||
                (allocRatio[portfolio_c] == precision) ||
                (allocRatio[portfolio_d] == precision) ||
                (allocRatio[portfolio_e] == precision)) {
                result.generateOutput(0);
                result.finalOutput(1);
            }
        }
//            }
//        }
    }
    for (int i = 0; i < bestResults.size(); i++) {
        bestResults[i].generateOutput(0);
        bestResults[i].finalOutput(1);
    }
    delete[] allocRatio;
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
        Result result;
        Result finalResult;
        result.setStock(model->getNumOfStocks(), model->getNumOfDays());
        finalResult.setStock(model->getNumOfStocks(), model->getNumOfDays());
        model->setResult(&result);
        result.foundBestCount = 1;
        double gBest = -std::numeric_limits<double>::max();
        for (int i = 0; i < ROUND; i++) { // round
            GNQTS *qts = new GNQTS(model);
            qts->run();

            if (gBest == result.gBest) {
                result.foundBestCount++;
                finalResult.foundBestCount++;
            } else if (gBest < result.gBest) { // found the global best which is a brand new solution
                gBest = result.gBest;
                result.foundBestCount = 1;
                result.atRound = i;
                result.atGen = qts->getBestGeneration();

                finalResult.copyResult(result);
                for (int j = 0; j < model->getNumOfStocks(); j++) {
                    finalResult.solution[j] = result.solution[j];
                    finalResult.allocatedFund[j] = result.allocatedFund[j];
                    finalResult.balance[j] = result.balance[j];
                    finalResult.amount[j] = result.amount[j];
                    finalResult.stocks[j].setPriceSize(model->getNumOfDays());
                    finalResult.stocks[j].code = result.stocks[j].code;
                    for (int k = 0; k < model->getNumOfDays(); k++) {
                        finalResult.stocks[j].fs[k] = result.stocks[j].fs[k];
                        finalResult.stocks[j].price[k] = result.stocks[j].price[k];
                    }
                }
                for (int j = 0; j < model->getNumOfDays(); j++) {
                    finalResult.totalFS[j] = result.totalFS[j];
                }
            }
            delete qts;
        }
        finalResult.generateOutput(j);
        finalResult.finalOutput(j);
    }
    delete model;
#endif
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
    return 0;
}