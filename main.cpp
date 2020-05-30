#include "StockSelection/GNQTS.h"
#include "FundAllocation/QTS.h"
#include <chrono>
#include <cstring>
#include <tuple>

using std::tuple;
using std::get;

void ranking() {
    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    for (int section = 0; section < numOfSection; section++) {
        int portfolio_a = 0;
        model.nextSection(section);
        Particle particle(model.getNumOfStocks());
        Result result(model.getNumOfStocks(), model.getNumOfDays());
        model.setResult(&result);

        vector<tuple<string, double, double, double>> rank;
        for (int count = 0; count < model.getNumOfStocks(); count++) {
            for (int i = 0; i < model.getNumOfStocks(); i++) {
                if (i == portfolio_a)
                    particle.solution[i] = 1;
                else
                    particle.solution[i] = 0;
            }
            vector<double> allocRatio(model.getNumOfStocks());
            allocRatio[portfolio_a] = 1;
            double gBest = model.getFitness(particle.solution, -1, allocRatio);
            double risk = result.risk;
            double expectedReturn = result.expectedReturn;
            string symbol = model.getStockSymbol(count);
            rank.emplace_back(symbol, gBest, expectedReturn, risk);
            portfolio_a++;
        }
#if WINDOW >= 13
        Logger logger("../log/US/" + tag + "/rank_" + trainingSection[section]);
#else
        Logger logger("../log/" + tag + "/rank_" + trainingSection[section]);
#endif
        logger.writeComma("Section");
        logger.writeComma("Stock(rank#)");
        logger.writeComma("Trend ratio");
        logger.writeComma("Expected return");
        logger.writeLine("Risk");
        int count = 0;
        int size = rank.size();
        while (count < size) {
            double max = -DBL_MAX;
            int iMax = -1;
            for (int i = 0; i < rank.size(); i++) {
                if (get<1>(rank[i]) > max) {
                    max = get<1>(rank[i]);
                    iMax = i;
                }
            }
            logger.writeComma(section + 1);
            logger.write(get<0>(rank[iMax]));
            logger.write("(");
            logger.write(count + 1);
            logger.writeComma(")");
            logger.writeComma(get<1>(rank[iMax]));
            logger.writeComma(get<2>(rank[iMax]));
            logger.writeLine(get<3>(rank[iMax]));
            rank.erase(rank.begin() + iMax);
            count++;
        }
    }
}

void exhaustion() {
    int section = 0;
    int portfolio_a = 0;
    int portfolio_b = 1;
    int portfolio_c = 0;
    int portfolio_d = 0;
    int portfolio_e = 0;
    int precision = 100;

    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.nextSection(section);
    Particle particle(model.getNumOfStocks());
    Result result(model.getNumOfStocks(), model.getNumOfDays());
    model.setResult(&result);

    for (int i = 0; i < model.getNumOfStocks(); i++) {
        if (i == portfolio_a || i == portfolio_b || i == portfolio_c || i == portfolio_d || i == portfolio_e)
            particle.solution[i] = 1;
        else
            particle.solution[i] = 0;
    }

    vector<double> allocRatio(model.getNumOfStocks());
    vector<Result> bestResults;
    bestResults.emplace_back(model.getNumOfStocks(), model.getNumOfDays());
    bestResults[0].gBest = -DBL_MAX;

    for (int i = 0; i <= precision; i++) {
//        for (int j = 0; j <= precision - i; j++) {
//            for (int k = 0; k <= precision - i - j; k++) {
//                for (int l = 0; l <= precision - i - j - k; l++) {
        allocRatio[portfolio_a] = i / (double) precision;
        allocRatio[portfolio_b] = (precision - i) / (double) precision;
//        allocRatio[portfolio_c] = (k) / (double) precision;
//        allocRatio[portfolio_d] = (precision - i - j - k) / (double) precision;
//            allocRatio[portfolio_e] = (precision - i - j - k - l) / (double) precision;
        model.getFitness(particle.solution, -1, allocRatio);
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
    for (auto &bestResult : bestResults) {
        bestResult.generateOutput(section);
        bestResult.finalOutput(section);
    }
}

void fundAllocation() {
    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    const string portfolio_bank = "BANK";
    const string portfolio_1 = "MSFT";
    const string portfolio_2 = "AAPL";
    const string portfolio_3 = "AMZN";
    const string portfolio_4 = "GOOG";
    const string portfolio_5 = "BRK.A";
    const string portfolio_6 = "FB";
    const string portfolio_7 = "BABA";
    const string portfolio_8 = "TCEHY";
    const string portfolio_9 = "JNJ";
    const string portfolio_10 = "XOM";
    const string portfolio_11 = "JPM";
    const string portfolio_12 = "V";
    const string portfolio_13 = "WMT";
    const string portfolio_14 = "BAC";
    const string portfolio_15 = "PG";
    const string portfolio_16 = "VZ";
    const string portfolio_17 = "MA";
    const string portfolio_18 = "INTC";
    const string portfolio_19 = "CSCO";
    const string portfolio_20 = "UNH";

    for (int section = 0; section < numOfSection; section++) {
        model.nextSection(section);
        Result result(model.getNumOfStocks(), model.getNumOfDays());
        Result finalResult(model.getNumOfStocks(), model.getNumOfDays());
        model.setResult(&result);
        result.foundBestCount = 1;
        double gBest = -DBL_MAX;
        vector<int> stockSelection(model.getNumOfStocks());

        for (int i = 0; i < model.getNumOfStocks(); i++) {
            if (model.getStockSymbol(i) == portfolio_bank ||
                model.getStockSymbol(i) == portfolio_1 ||
                model.getStockSymbol(i) == portfolio_2 ||
                model.getStockSymbol(i) == portfolio_3 ||
                model.getStockSymbol(i) == portfolio_4 ||
                model.getStockSymbol(i) == portfolio_5 ||
                model.getStockSymbol(i) == portfolio_6 ||
                model.getStockSymbol(i) == portfolio_7 ||
                model.getStockSymbol(i) == portfolio_8 ||
                model.getStockSymbol(i) == portfolio_9 ||
                model.getStockSymbol(i) == portfolio_10 ||
                model.getStockSymbol(i) == portfolio_11 ||
                model.getStockSymbol(i) == portfolio_12 ||
                model.getStockSymbol(i) == portfolio_13 ||
                model.getStockSymbol(i) == portfolio_14 ||
                model.getStockSymbol(i) == portfolio_15 ||
                model.getStockSymbol(i) == portfolio_16 ||
                model.getStockSymbol(i) == portfolio_17 ||
                model.getStockSymbol(i) == portfolio_18 ||
                model.getStockSymbol(i) == portfolio_19 ||
                model.getStockSymbol(i) == portfolio_20) {
                stockSelection[i] = 1;
            } else
                stockSelection[i] = 0;
        }

        for (int i = 0; i < ROUND; i++) { // round
            QTS qts(model, stockSelection);
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
    }
}

void stockSelection() {
    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    for (int j = 0; j < numOfSection; j++) { // section
        model.nextSection(j);
        Result result(model.getNumOfStocks(), model.getNumOfDays());
        Result finalResult(model.getNumOfStocks(), model.getNumOfDays());
        model.setResult(&result);
        result.foundBestCount = 1;
        double gBest = -DBL_MAX;
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
    exhaustion();
#elif MODE == 3
    ranking();
#endif
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
#if WINDOW >= 13
    Logger logger("../log/US/" + tag + "/" + tag + "_final_result.csv");
#else
    Logger logger("../log/" + tag + "_final_result.csv");
#endif
    logger.writeComma("\nExecution time (s)");
    logger.writeLine(std::chrono::duration<double>(end - start).count());
    system("pause");
    return 0;
}
