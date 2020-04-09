#include "StockSelection/GNQTS.h"
#include "FundAllocation/QTS.h"
#include <iostream>
#include <chrono>
#include <vector>

using std::vector;

void ranking() {
    int section = 0;
    int portfolio_a = 0;

    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.nextSection(section);
    Particle particle;
    particle.setSolutionSize(model.getNumOfStocks());
    Result result;
    result.setStock(model.getNumOfStocks(), model.getNumOfDays());
    model.setResult(&result);

    for (int count = 0; count < model.getNumOfStocks(); count++) {
        for (int i = 0; i < model.getNumOfStocks(); i++) {
            if (i == portfolio_a)
                particle.solution[i] = 1;
            else
                particle.solution[i] = 0;
        }
        auto *allocRatio = new double[model.getNumOfStocks()];
        std::memset(allocRatio, 0, sizeof(double) * model.getNumOfStocks());
        allocRatio[portfolio_a] = 1;
        model.getFitness(particle.solution, 0, -1, allocRatio);
        result.finalOutput(section);
        delete[] allocRatio;
        portfolio_a++;
    }
}

void test() {
    int section = 0;
    int portfolio_a = 0;
    int portfolio_b = 1;
    int portfolio_c = 2;
    int portfolio_d = 3;
    int portfolio_e = 4;
    int precision = 100;

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

    auto *allocRatio = new double[model.getNumOfStocks()];
    vector<Result> bestResults;
    bestResults.emplace_back();
    bestResults[0].gBest = -DBL_MAX;
    bestResults[0].setStock(model.getNumOfStocks(), model.getNumOfDays());

    for (int i = 0; i <= precision; i++) {
//        for (int j = 0; j <= precision - i; j++) {
//            for (int k = 0; k <= precision - i - j; k++) {
//                for (int l = 0; l <= precision - i - j - k; l++) {
        allocRatio[portfolio_a] = i / (double) precision;
        allocRatio[portfolio_b] = (precision - i) / (double) precision;
//        allocRatio[portfolio_c] = (k) / (double) precision;
//        allocRatio[portfolio_d] = (precision - i - j - k) / (double) precision;
//            allocRatio[portfolio_e] = (precision - i - j - k - l) / (double) precision;
        model.getFitness(particle.solution, 0, -1, allocRatio);
        if (bestResults[0].gBest < result.gBest) { // found a better solution
            bestResults.clear();
            bestResults.push_back(result);
        } else if (bestResults[0].gBest == result.gBest) { // found a solution with the same fitness
            bestResults.push_back(result);
        }
        if ((allocRatio[portfolio_a] == 1) ||
            (allocRatio[portfolio_b] == 1) ||
            (allocRatio[portfolio_c] == 1) ||
            (allocRatio[portfolio_d] == 1) ||
            (allocRatio[portfolio_e] == 1)) {
            result.generateOutput(section);
            result.finalOutput(section);
        }
    }
//        }
//    }
//    }
    for (int i = 0; i < bestResults.size(); i++) {
        bestResults[i].generateOutput(section);
        bestResults[i].finalOutput(section);
    }
    delete[] allocRatio;
}

void fundAllocation() {
    int section = 39;
    int portfolio_a = 0;
    int portfolio_b = 2;
    int portfolio_c = 6;
    int portfolio_d = 20;
    int portfolio_e = 0;
    int portfolio_f = 0;
    Model model(1, 268435456, 0.0004, 10000000.0, 0.001425, 0.003);
    model.nextSection(section);
    Result result;
    result.setStock(model.getNumOfStocks(), model.getNumOfDays());
    result.foundBestCount = 1;
    model.setResult(&result);
    Result finalResult;
    finalResult.setStock(model.getNumOfStocks(), model.getNumOfDays());
    int *stockSelection = new int[model.getNumOfStocks()];
    for (int i = 0; i < model.getNumOfStocks(); i++) {
        if (i == portfolio_a || i == portfolio_b || i == portfolio_c || i == portfolio_d || i == portfolio_e ||
            i == portfolio_f)
            stockSelection[i] = 1;
        else
            stockSelection[i] = 0;
    }

    double gBest = -std::numeric_limits<double>::max();
    for (int i = 0; i < ROUND; i++) { // round
        QTS qts(&model, stockSelection);
        qts.run();
        if (gBest == result.gBest) {
            result.foundBestCount++;
            finalResult.foundBestCount++;
        } else if (gBest < result.gBest) { // found the global best which is a brand new solution
            gBest = result.gBest;
            result.foundBestCount = 1;
            result.atRound = i;
            result.atGen = qts.getBestGeneration();
            finalResult = result;
        }
    }
    finalResult.generateOutput(section);
    finalResult.finalOutput(section);
    delete[] stockSelection;
}

void stockSelection() {
    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    for (int j = 0; j < numOfSection; j++) { // section
        model.nextSection(j);
        Result result;
        Result finalResult;
        result.setStock(model.getNumOfStocks(), model.getNumOfDays());
        finalResult.setStock(model.getNumOfStocks(), model.getNumOfDays());
        model.setResult(&result);
        result.foundBestCount = 1;
        double gBest = -std::numeric_limits<double>::max();
        for (int i = 0; i < ROUND; i++) { // round
            GNQTS qts(&model);
            qts.run();

            if (gBest == result.gBest) {
                result.foundBestCount++;
                finalResult.foundBestCount++;
            } else if (gBest < result.gBest) { // found the global best which is a brand new solution
                gBest = result.gBest;
                result.foundBestCount = 1;
                result.atRound = i;
                result.atGen = qts.getBestGeneration();

                finalResult = result;
            }
        }
        finalResult.generateOutput(j);
        finalResult.finalOutput(j);
    }
}

int main() {
    auto start = std::chrono::steady_clock::now();
    srand(114);
#if MODE == 0
    stockSelection();
#elif MODE == 1
    fundAllocation();
#elif MODE == 2
    test();
#elif MODE == 3
    ranking();
#endif
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
    return 0;
}
