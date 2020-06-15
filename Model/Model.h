//
// Created by Mark Hsu on 2019/11/13.
//

#ifndef GNQTS_STOCK_MODEL_H
#define GNQTS_STOCK_MODEL_H

#include "Result.h"

#define US_MARKET 1
#define EXIST_BANK 0
#define ROUND 50
#define RUN_TESTING 1
#define MODE 1

/** @param MODE
 *  0: execute stock selection program
 *  1: execute fund allocation program
 *  2: execute fund allocation with method of exhaustion program
 *  3: execute ranking program
 *  4: execute single stock program
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
    vector<string> trainingSection;
    vector<string> testingSection;
    Result *result;

    Model() = delete;

    Model(int population, int generation, double theta, double fund, double fee, double tax);

    ~Model();

    void init();

    int getGeneration() const;

    int getPopulation() const;

    double getTheta() const;

    double getFitness(vector<int> &solution, int pIndex, const vector<double> &allocRatio);

    void setResult(Result *rs);

    int getNumOfStocks() const;

    int getNumOfDays() const;

    void nextSection(int section, bool isTraining);

    string getStockSymbol(int i);

    void setInitialFund(double fund);

    double calcTrendRatio(vector<double> &totalFS, int _numOfDays, double _initFund, int pIndex) const;

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
};

#endif //GNQTS_STOCK_MODEL_H
