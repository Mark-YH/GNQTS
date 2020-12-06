#include "EWFA/QTS.h"
#include "FundAllocation/QTS.h"
#include <chrono>
#include <cstring>
#include <tuple>
#include <sstream>

using std::tuple;
using std::get;

void ranking() {
    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.init();
    for (int period = 0; period < model.trainingPeriod.size(); period++) {
        int portfolio_a = 0;
        model.nextPeriod(period, true);
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
            double gBest = model.getFitness(particle.solution, -1, allocRatio, true);
            double risk = result.risk;
            double expectedReturn = result.expectedReturn;
            string symbol = model.getStockSymbol(count);
            rank.emplace_back(symbol, gBest, expectedReturn, risk);
            portfolio_a++;
        }
        string path = "../log/";
        path += Model::market;
        path += "/";
        path += Model::slidingWindow;
        path += "/rank_";
        path += model.trainingPeriod[period];
        Logger logger(path);
        logger.writeComma("Period");
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
            logger.writeComma(period + 1);
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
    int period = 0;
    int portfolio_a = 0;
    int portfolio_b = 1;
    int portfolio_c = 0;
    int portfolio_d = 0;
    int portfolio_e = 0;
    int precision = 100;

    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.init();
    model.nextPeriod(period, true);
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
        model.getFitness(particle.solution, -1, allocRatio, true);
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
            result.generateOutput(period, true, Model::market, Model::slidingWindow, model.trainingPeriod,
                                  model.testingPeriod);
            result.finalOutput(period, true, Model::market, Model::slidingWindow, model.trainingPeriod,
                               model.testingPeriod);
        }
    }
//        }
//    }
//    }
    for (auto &bestResult : bestResults) {
        bestResult.generateOutput(period, true, Model::market, Model::slidingWindow, model.trainingPeriod,
                                  model.testingPeriod);
        bestResult.finalOutput(period, true, Model::market, Model::slidingWindow, model.trainingPeriod,
                               model.testingPeriod);
    }
}

void fundAllocation() {
    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.init();
#if RUN_TESTING
    Model testingModel(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    testingModel.trainingPeriod = model.trainingPeriod;
    testingModel.testingPeriod = model.testingPeriod;
#endif

    double totalReturn = 0.0;
    vector<double> finalFS;
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

    for (int period = 0; period < model.trainingPeriod.size(); period++) {
        std::cout << "Period: " << std::setw(2) << period + 1 << " / " << model.trainingPeriod.size() << endl;
        model.nextPeriod(period, true);
#if RUN_TESTING
        testingModel.nextPeriod(period, false);
        Result testingResult(testingModel.getNumOfStocks(), testingModel.getNumOfDays());
        testingModel.setResult(&testingResult);
#endif
        Result result(model.getNumOfStocks(), model.getNumOfDays());
        Result finalResult(model.getNumOfStocks(), model.getNumOfDays());
        model.setResult(&result);
        result.foundBestCount = 1;
        double gBest = -DBL_MAX;
        vector<int> stockSelection(model.getNumOfStocks());

        for (int i = 0; i < model.getNumOfStocks(); i++) {
            if (1 || model.getStockSymbol(i) == portfolio_bank ||
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

        vector<double> allocRatio, tmp;
        for (int i = 0; i < ROUND; i++) { // round
            std::cout << "Round: " << std::setw(2) << i + 1 << " / " << ROUND << '\r';
            FA::QTS qts(model, stockSelection);
            tmp = qts.run();
            if (gBest == result.gBest) {
                result.foundBestCount++;
                finalResult.foundBestCount++;
            } else if (gBest < result.gBest) { // found the global best which is a brand new solution
                gBest = result.gBest;
                result.foundBestCount = 1;
                result.atRound = i;
                result.atGen = qts.getBestGeneration();
                finalResult = result;
                allocRatio = tmp;
            }
        }
#if RUN_TESTING
        if (finalResult.gBest > 0) {
            testingModel.getFitness(finalResult.solution, -1, allocRatio, false);
#if COMPOUND_INTEREST
            testingModel.setInitialFund(testingResult.finalFund);
#endif
            totalReturn += testingResult.finalFund - testingResult.initFund;
            for (auto it: testingModel.result->totalFS) {
                finalFS.push_back(it);
            }
        } else {
            double tmp;
            if (finalFS.empty())
                tmp = result.initFund;
            else
                tmp = finalFS.back();
            for (int j = 0; j < testingModel.getNumOfDays(); j++) {
                finalFS.push_back(tmp);
            }
        }
        testingResult.generateOutput(period, false, Model::market, Model::slidingWindow,
                                     model.trainingPeriod, model.testingPeriod);
        testingResult.finalOutput(period, false, Model::market, Model::slidingWindow,
                                  model.trainingPeriod, model.testingPeriod);
#endif
        finalResult.generateOutput(period, true, Model::market, Model::slidingWindow,
                                   model.trainingPeriod, model.testingPeriod);
        finalResult.finalOutput(period, true, Model::market, Model::slidingWindow,
                                model.trainingPeriod, model.testingPeriod);
    }
#if RUN_TESTING
    Result rs(model.getNumOfStocks(), finalFS.size());
    testingModel.setResult(&rs);
    rs.totalFS = finalFS;
    testingModel.getTrendRatio(finalFS, finalFS.size(), model.result->initFund, -1, false);
    rs.finalFund = finalFS.back();
    rs.realReturn = totalReturn;
    rs.totalTestResult(Model::market, Model::slidingWindow, model.trainingPeriod, model.testingPeriod);
#endif
}

void stockSelection() {
    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.init();
#if RUN_TESTING
    Model testingModel(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    testingModel.trainingPeriod = model.trainingPeriod;
    testingModel.testingPeriod = model.testingPeriod;
#endif

    double totalReturn = 0.0;
    vector<double> finalFS;
    for (int i = 0; i < model.trainingPeriod.size(); i++) {
        std::cout << "Period: " << std::setw(2) << i + 1 << " / " << model.trainingPeriod.size() << endl;
        model.nextPeriod(i, true);
#if RUN_TESTING
        testingModel.nextPeriod(i, false);
        Result testingResult(testingModel.getNumOfStocks(), testingModel.getNumOfDays());
        testingModel.setResult(&testingResult);
#endif
        Result result(model.getNumOfStocks(), model.getNumOfDays());
        Result finalResult(model.getNumOfStocks(), model.getNumOfDays());
        model.setResult(&result);
        result.foundBestCount = 1;
        double gBest = -DBL_MAX;
        for (int j = 0; j < ROUND; j++) { // round
            std::cout << "Round: " << std::setw(2) << j + 1 << " / " << ROUND << '\r';
            EWFA::QTS qts(&model);
            qts.run();

            if (gBest == result.gBest) {
                result.foundBestCount++;
                finalResult.foundBestCount++;
            } else if (gBest < result.gBest) { // found the global best which is a brand new solution
                gBest = result.gBest;
                result.foundBestCount = 1;
                result.atRound = j;
                result.atGen = qts.getBestGeneration();

                finalResult = result;
            }
        }
#if RUN_TESTING
        if (finalResult.gBest > 0) {
            testingModel.getFitness(finalResult.solution, -1, vector<double>(), false);
#if COMPOUND_INTEREST
            testingModel.setInitialFund(testingResult.finalFund);
#endif
            totalReturn += testingResult.finalFund - testingResult.initFund;
            for (auto it: testingModel.result->totalFS) {
                finalFS.push_back(it);
            }
        } else {
            double tmp;
            if (finalFS.empty())
                tmp = result.initFund;
            else
                tmp = finalFS.back();
            for (int j = 0; j < testingModel.getNumOfDays(); j++) {
                finalFS.push_back(tmp);
            }
        }
        testingResult.generateOutput(i, false, Model::market, Model::slidingWindow,
                                     model.trainingPeriod, model.testingPeriod);
        testingResult.finalOutput(i, false, Model::market, Model::slidingWindow,
                                  model.trainingPeriod, model.testingPeriod);
#endif
        finalResult.generateOutput(i, true, Model::market, Model::slidingWindow,
                                   model.trainingPeriod, model.testingPeriod);
        finalResult.finalOutput(i, true, Model::market, Model::slidingWindow,
                                model.trainingPeriod, model.testingPeriod);
    }
#if RUN_TESTING
    Result rs(model.getNumOfStocks(), finalFS.size());
    testingModel.setResult(&rs);
    rs.totalFS = finalFS;
    testingModel.getTrendRatio(finalFS, finalFS.size(), model.result->initFund, -1, false);
    rs.finalFund = finalFS.back();
    rs.realReturn = totalReturn;
    rs.totalTestResult(Model::market, Model::slidingWindow, model.trainingPeriod, model.testingPeriod);
#endif
}

void singleStock() {
    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.init();
#if RUN_TESTING
    Model testingModel(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    testingModel.trainingPeriod = model.trainingPeriod;
    testingModel.testingPeriod = model.testingPeriod;
#endif

    double totalReturn = 0.0;
    vector<double> finalFS;
    for (int i = 0; i < model.trainingPeriod.size(); i++) {
        model.nextPeriod(i, true);
        Result result(model.getNumOfStocks(), model.getNumOfDays());
        model.setResult(&result);
#if RUN_TESTING
        testingModel.nextPeriod(i, false);
        Result testingResult(testingModel.getNumOfStocks(), testingModel.getNumOfDays());
        testingModel.setResult(&testingResult);
#endif

        for (int j = 0; j < model.getNumOfStocks(); j++) {
            vector<int> solution(model.getNumOfStocks(), 0);
            vector<double> allocRatio(model.getNumOfStocks(), 0);
            solution[j] = 1;
            allocRatio[j] = 1;
            model.getFitness(solution, -1, allocRatio, true);
            result.generateOutput(i, true, Model::market, Model::slidingWindow,
                                  model.trainingPeriod, model.testingPeriod);
            result.finalOutput(i, true, Model::market, Model::slidingWindow,
                               model.trainingPeriod, model.testingPeriod);
#if RUN_TESTING
            testingModel.getFitness(solution, -1, allocRatio, false);
#if COMPOUND_INTEREST
            testingModel.setInitialFund(testingResult.finalFund);
#endif
            totalReturn += testingResult.finalFund - testingResult.initFund;
            for (auto it: testingModel.result->totalFS) {
                finalFS.push_back(it);
            }

            testingResult.generateOutput(i, false, Model::market, Model::slidingWindow,
                                         model.trainingPeriod, model.testingPeriod);
            testingResult.finalOutput(i, false, Model::market, Model::slidingWindow,
                                      model.trainingPeriod, model.testingPeriod);
#endif
        }
    }
#if RUN_TESTING
    Result rs(model.getNumOfStocks(), finalFS.size());
    testingModel.setResult(&rs);
    rs.totalFS = finalFS;
    testingModel.getTrendRatio(finalFS, finalFS.size(), model.result->initFund, -1, false);
    rs.finalFund = finalFS.back();
    rs.realReturn = totalReturn;
    rs.totalTestResult(Model::market, Model::slidingWindow, model.trainingPeriod, model.testingPeriod);
#endif
}

void givenPortfolio() {
    Model model(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    model.init();
#if RUN_TESTING
    Model testingModel(10, 10000, 0.0004, 10000000.0, 0.001425, 0.003);
    testingModel.trainingPeriod = model.trainingPeriod;
    testingModel.testingPeriod = model.testingPeriod;
#endif

    double totalReturn = 0.0;
    vector<double> finalFS;
    for (int i = 0; i < model.trainingPeriod.size(); i++) {
        model.nextPeriod(i, true);
        Result result(model.getNumOfStocks(), model.getNumOfDays());
        model.setResult(&result);
#if RUN_TESTING
        testingModel.nextPeriod(i, false);
        Result testingResult(testingModel.getNumOfStocks(), testingModel.getNumOfDays());
        testingModel.setResult(&testingResult);
#endif

        vector<int> solution(model.getNumOfStocks(), 0);
        vector<double> allocRatio(model.getNumOfStocks(), 0);
        std::ifstream fin;
        fin.open("../py/py_output/EWFA/" + Model::slidingWindow + "/" + std::to_string(i + 1) + ".csv");
        for (int j = 0; j < model.getNumOfStocks(); j++) {
            string line;
            while (std::getline(fin, line)) {
                std::stringstream ss(line);
                int a;
                long double b;
                while (ss >> a >> b) {
                    solution[a] = 1;
                    allocRatio[a] = b / 10000000.0;
                }
            }
        }
        fin.close();
        model.getFitness(solution, -1, allocRatio, true);
        result.generateOutput(i, true, Model::market, Model::slidingWindow,
                              model.trainingPeriod, model.testingPeriod);
        result.finalOutput(i, true, Model::market, Model::slidingWindow,
                           model.trainingPeriod, model.testingPeriod);
#if RUN_TESTING
        testingModel.getFitness(solution, -1, allocRatio, false);
#if COMPOUND_INTEREST
        testingModel.setInitialFund(testingResult.finalFund);
#endif
        totalReturn += testingResult.finalFund - testingResult.initFund;
        for (auto it: testingModel.result->totalFS) {
            finalFS.push_back(it);
        }

        testingResult.generateOutput(i, false, Model::market, Model::slidingWindow,
                                     model.trainingPeriod, model.testingPeriod);
        testingResult.finalOutput(i, false, Model::market, Model::slidingWindow,
                                  model.trainingPeriod, model.testingPeriod);
#endif
    }
#if RUN_TESTING
    Result rs(model.getNumOfStocks(), finalFS.size());
    testingModel.setResult(&rs);
    rs.totalFS = finalFS;
    testingModel.getTrendRatio(finalFS, finalFS.size(), model.result->initFund, -1, false);
    rs.finalFund = finalFS.back();
    rs.realReturn = totalReturn;
    rs.totalTestResult(Model::market, Model::slidingWindow, model.trainingPeriod, model.testingPeriod);
#endif
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
#elif MODE == 4
    singleStock();
#elif MODE == 5
    givenPortfolio();
#endif
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;

    Logger logger(
            "../log/" + Model::market + "/" + Model::slidingWindow + "/" + Model::slidingWindow + "_final_result.csv");
    logger.writeComma("\nExecution time (s)");
    logger.writeLine(std::chrono::duration<double>(end - start).count());
    return 0;
}
