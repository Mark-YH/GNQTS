//
// Created by Mark Hsu on 2019/11/13.
//

#ifndef PORTFOLIO_OPTIMIZATION_MODEL_H
#define PORTFOLIO_OPTIMIZATION_MODEL_H

#include "Result.h"

#define US_MARKET 1
#define EXIST_BANK 0
#define ROUND 50
#define RUN_TESTING 1
#define COMPOUND_INTEREST 1
#define MODE 1

/** @param MODE
 *  0: execute stock selection program
 *  1: execute fund allocation program
 *  2: execute fund allocation with method of exhaustion program
 *  3: execute ranking program
 *  4: execute single stock program
 *  5: execute given portfolio program
 */

class Model {
public:
    inline static string slidingWindow = "M2M";
    inline static string market = "DJI30";
    /**
     * @param market
     * TW0050: Component stocks of ETF 0050 in Taiwan.
     * US_TOP20: Top 20 stocks firm by market capitalization in the U.S. stock market.
     * DJI30: Component stocks of DJI.
     * DJIA: Dow Jones industrial average.
     */
    vector<string> trainingPeriod;
    vector<string> testingPeriod;
    Result *result;

    Model() = delete;

    Model(int population, int generation, double theta, double fund, double fee, double tax);

    ~Model();

    void init();

    int getGeneration() const;

    int getPopulation() const;

    double getTheta() const;

    double getFitness(vector<int> &solution, int pIndex, const vector<double> &allocRatio, bool isTraining);

    void setResult(Result *rs);

    int getNumOfStocks() const;

    int getNumOfDays() const;

    void nextPeriod(int period, bool isTraining);

    string getStockSymbol(int i);

    void setInitialFund(double fund);

    double getTrendRatio(vector<double> &totalFS, int _numOfDays, double _initFund, int pIndex, bool isTraining) const;

    double getSharpeRatio(vector<double> &totalFS, int _numOfDays, double _initFund, int pIndex, bool isTraining) const;

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

    double calcRisk(vector<double> &totalFS, vector<double> &line, int _numOfDays) const;
};

#endif //PORTFOLIO_OPTIMIZATION_MODEL_H
