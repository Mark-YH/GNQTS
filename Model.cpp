//
// Created by Mark Hsu on 2019/11/13.
//

#include "Model.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

/*! @param DEBUG val
 * val > 0: generate output including `chosen stock`, `best individual's total fund standardization and fund standardization of each stock symbol`, `slope`, `risk`, `trend line`, `trend value` results.
 * val > 4: generate output including `each individual's total fund standardization` and above results.
 * val > 6: generate output including `each individual's fund standardization and fund standardization of each stock symbol` and above results.
 * val > 10: generate output including `read data` and above results.
 */
#define DEBUG 3

using std::ifstream;
using std::string;
using std::stringstream;
using std::ios;

Model::Model(int population, int generation, double theta, double fund, double fee, double tax) {
    this->population = population;
    this->generation = generation;
    this->theta = theta;
    this->fund = fund;
    this->fee = fee;
    this->tax = tax;
    this->numOfDays = 0;
    this->numOfStocks = 0;

    string path = "../data/M2M/train_2009_12(2009 Q1).csv";
    getNumOfRowColumn(path);
    this->stocks = new Stock[this->numOfStocks];

    for (int i = 0; i < this->numOfStocks; i++) {
        this->stocks[i].setPriceSize(this->numOfDays);
    }

    readData(path);

    for (int i = 0; i < this->numOfStocks; i++) {
        for (int j = 0; j < this->numOfDays; j++) {
            this->stocks[i].fs[j] = 0;
        }
    }
}

Model::~Model() {
    delete[] this->stocks;
}

// get numbers of stocks and days in order to allocate memory for `Stock`
void Model::getNumOfRowColumn(const string &path) {
    ifstream fin;
    try {
        fin.open(path, ios::in);
        string line;

        getline(fin, line);

        this->numOfDays = std::count(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>(), '\n');

        for (int pos = line.string::find(',', 0);
             line.string::find(',', pos) != string::npos;
             pos = line.string::find(',', pos) + 1) {
            this->numOfStocks++;
        }
        fin.close();

    } catch (std::exception &e) {
        fin.close();
        Logger logger("../log/error");
        logger.writeTab("void initStock():");
        logger.writeLine(e.what());
    }
}

void Model::readData(const string &path) {
    ifstream fin;
    try {
        fin.open(path, ios::in);

        string line;
        int lineCount = -1;
        while (getline(fin, line)) {
            string tmp;
            stringstream ss(line);

            int stockCount = 0;
            while (getline(ss, tmp, ',')) {
                if (tmp == "\r")
                    continue;
                if (lineCount == -1) {
                    this->stocks[stockCount].code = stoi(tmp);
                } else {
                    this->stocks[stockCount].price[lineCount] = stod(tmp);
                }
                stockCount++;
            }
            lineCount++;
        }
        fin.close();
#if DEBUG > 10
        Logger logger("../log/read_data_for_debug.csv", ios::out);
        for (int i = 0; i < numOfStocks; i++) {
            logger.writeComma(stocks[i].code);
            for (int j = 0; j < numOfDays; j++) {
                logger.writeComma(stocks[i].price[j]);
            }
            logger.writeLine("");
        }
#endif
    } catch (std::exception &e) {
        fin.close();
        Logger logger("../log/error");
        logger.writeTab("void readData():");
        logger.writeLine(e.what());
    }
}

double Model::getFitness(Particle *p, int gen, int pIndex) {
    int numOfChosen = 0; // how much stock is chosen
    double totalBalance;
    double fund = this->fund;

    for (int i = 0; i < this->numOfStocks; i++) {
        if (p->solution[i] == 1) {
            numOfChosen++;
        }
    }

    totalBalance = fund;

    // allocate fund
    fund = int(fund) / numOfChosen;

    // calculate the rest fund
    for (int i = 0; i < numOfChosen; i++) {
        totalBalance -= fund;
    }

#if DEBUG
    Logger logStock("../log/stock.csv");
    if (gen == 0 && pIndex == 0) {
        logStock.writeComma("Generation");
        logStock.writeComma("Individual");
        logStock.writeComma("number of chosen stock");
        logStock.writeComma("Allocated fund");
        logStock.writeComma("Total balance");
        logStock.writeComma("Chosen stocks");
        for (int i = 0; i < this->numOfStocks; i++) {
            logStock.writeComma(i);
        }
        logStock.writeLine("");
    }
    logStock.writeComma(gen);
    if (pIndex == -1) {
        logStock.writeComma("Best");
    } else {
        logStock.writeComma(pIndex);
    }
    logStock.writeComma(numOfChosen);
    logStock.writeComma(fund);
    logStock.writeComma(totalBalance);
    logStock.writeComma("");
    for (int i = 0; i < this->numOfStocks; i++) {
        if (p->solution[i] == 1) {
            logStock.writeComma(this->stocks[i].code);
        }
    }
    logStock.writeLine("");
#endif

    // calculate fund standardization
    calcFS(p, fund, gen, pIndex);

    // calculate total fund standardization
    double *totalFS = new double[this->numOfDays];

    for (int i = 0; i < this->numOfDays; i++) {
        totalFS[i] = 0;
    }
    for (int i = 0; i < numOfStocks; i++) {
        if (p->solution[i] == 1) {
            for (int j = 0; j < this->numOfDays; j++) {
                totalFS[j] += this->stocks[i].fs[j];
            }
        }
    }

    for (int i = 0; i < this->numOfDays; i++) {
        totalFS[i] += totalBalance; // add the balance from allocation part
    }

#if DEBUG
    Logger logFS("../log/fund_standardization.csv", 20);
    if (gen == 0 && pIndex == 0) {
        logFS.writeComma("Generation");
        logFS.writeComma("Individual");
        for (int i = 0; i < this->numOfDays; i++) {
            logFS.writeComma(i);
        }
        logFS.writeLine("");
    }

    logFS.writeComma(gen);
    if (pIndex == -1) {
        logFS.writeComma("Best");
    } else {
        logFS.writeComma(pIndex);
    }
#if DEBUG > 4
#if DEBUG < 6
    if (pIndex == -1) {
#endif
    for (int i = 0; i < this->numOfStocks; i++) {
        if (p->solution[i] == 1) {
            logFS.writeComma(gen);
            logFS.writeComma(pIndex);
            logFS.writeComma(this->stocks[i].code);
            for (int j = 0; j < this->numOfDays; j++) {
                logFS.writeComma(this->stocks[i].fs[j]);
            }
            logFS.writeLine("");
        }
    }
#if DEBUG < 6
    }
#endif
#endif
    for (int i = 0; i < this->numOfDays; i++) {
        logFS.writeComma(gen);
        logFS.writeComma(pIndex);
        logFS.writeComma("");
        logFS.writeComma(totalFS[i]);
    }
    logFS.writeLine("");
#endif

    // calculate slope
    double tmp = 0, tmp2 = 0;

    for (int i = 0; i < this->numOfDays; i++) {
        tmp += (i + 1) * totalFS[i] - (i + 1) * this->fund;
        tmp2 += (i + 1) * (i + 1);
    }
    double slope = tmp / tmp2;
    double *trendLine = new double[this->numOfDays]; // daily expected standardization fund

    // calculate trend line
    for (int i = 0; i < this->numOfDays; i++) {
        trendLine[i] = slope * (i + 1) + this->fund;
    }

    // calculate risk
    tmp = 0;

    for (int i = 0; i < this->numOfDays; i++) {
        tmp += (totalFS[i] - trendLine[i]) * (totalFS[i] - trendLine[i]);
    }
    double risk = sqrt(tmp / this->numOfDays);

    // calculate trend value
    double trendVal = slope / risk;

#if DEBUG
    Logger logTrend("../log/trend_value.csv");
    if (gen == 0 && pIndex == 0) {
        logTrend.writeComma("Generation");
        logTrend.writeComma("Individual");
        logTrend.writeComma("Slope");
        logTrend.writeComma("Risk");
        logTrend.writeComma("Trend value");
        logTrend.writeComma("Trend line");
        for (int i = 0; i < this->numOfDays; i++) {
            logTrend.writeComma(i);
        }
        logTrend.writeLine("");
    }
    logTrend.writeComma(gen);
    if (pIndex == -1) {
        logTrend.writeComma("Best");
    } else {
        logTrend.writeComma(pIndex);
    }
    logTrend.writeComma(slope);
    logTrend.writeComma(risk);
    logTrend.writeComma(trendVal);
    logTrend.writeComma("");
    for (int i = 0; i < this->numOfDays; i++) {
        logTrend.writeComma(trendLine[i]);
    }
    logTrend.writeLine("");
#endif

    delete[] totalFS;
    delete[] trendLine;
    return trendVal;
}

void Model::calcFS(Particle *p, double lFund, int gen, int pIndex) {
    // calculate fund standardization
    for (int i = 0; i < this->numOfStocks; i++) {
        if (p->solution[i] == 1) { // the chosen stock
            double balance = 0;
            int amount = 0; // how much stock can buy

            for (int j = 0; j < this->numOfDays; j++) {
                if (j == 0) { // first day
                    double pricePerStock = this->stocks[i].price[j] * 1000.0 * (1.0 + this->fee);
                    amount = floor(lFund / pricePerStock);
                    balance = lFund - amount * pricePerStock;
                    this->stocks[i].fs[j] =
                            lFund - (amount * this->stocks[i].price[j] * 1000 * this->fee);
#if DEBUG
                    Logger logStock2("../log/stock2.csv");
                    if (gen == 0 && pIndex == 0 && i == 0) {
                        logStock2.writeComma("Generation");
                        logStock2.writeComma("Individual");
                        logStock2.writeComma("Stock symbol");
                        logStock2.writeComma("Allocated fund");
                        logStock2.writeComma("Balance of particular stock");
                        logStock2.writeComma("Amount");
                        logStock2.writeComma("Price per stock");
                        logStock2.writeComma("Total fee");
                        logStock2.writeLine("");
                    }
                    logStock2.writeComma(gen);
                    if (pIndex == -1) {
                        logStock2.writeComma("Best");
                    } else {
                        logStock2.writeComma(pIndex);
                    }
                    logStock2.writeComma(this->stocks[i].code);
                    logStock2.writeComma(lFund);
                    logStock2.writeComma(balance);
                    logStock2.writeComma(amount);
                    logStock2.writeComma(pricePerStock);
                    logStock2.writeComma(amount * this->stocks[i].price[j] * 1000 * this->fee);
                    logStock2.writeLine("");
#endif
                } else { // the other days
                    this->stocks[i].fs[j] =
                            this->stocks[i].price[j] * amount * 1000 * (1 - (this->fee + this->tax)) + balance;
                }
            }
        }
    }
}

int Model::getGeneration() {
    return this->generation;
}

int Model::getPopulation() {
    return this->population;
}

double Model::getTheta() {
    return this->theta;
}

int Model::getLength() {
    return this->numOfStocks;
}