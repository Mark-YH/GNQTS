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
#define DEBUG 0

using std::ifstream;
using std::string;
using std::stringstream;
using std::ios;

Model::Model(int population, int generation, double theta, double fund, double fee, double tax) {
    this->population = population;
    this->generation = generation;
    this->theta = theta;
    this->initFund = fund;
    this->fee = fee;
    this->tax = tax;
    this->numOfDays = 0;
}

Model::~Model() {
    delete[] this->stocks;
    this->result = nullptr;
}

void Model::nextSection(int section) {
    string path = "../data/" + tag + "/" + trainingSection[section];
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

// get numbers of stocks and days in order to allocate memory for `Stock`
void Model::getNumOfRowColumn(const string &path) {
    ifstream fin;
    try {
        fin.open(path, ios::in);
        string line;

        getline(fin, line);

        this->numOfDays = std::count(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>(), '\n');
        this->numOfStocks = 0;
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
    double totalBalance = this->initFund;
    int *allocatedFund = new int[this->numOfStocks];

    for (int i = 0; i < this->numOfStocks; i++) {
        if (p->solution[i] == 1) {
            numOfChosen++;
        }
    }

    if (pIndex == -1) {
        this->result->numOfChosen = numOfChosen;
    }

    // allocate fund
    for (int i = 0; i < this->numOfStocks; i++) {
        if (p->solution[i] == 1) {
            allocatedFund[i] = floor(this->initFund / numOfChosen);
        } else {
            allocatedFund[i] = 0;
        }
    }

    // calculate the rest fund
    for (int i = 0; i < this->numOfStocks; i++) {
        if (p->solution[i] == 1)
            totalBalance -= allocatedFund[i];
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
    calcFS(p, allocatedFund, gen, pIndex);

    // calculate total fund standardization
    auto *totalFS = new double[this->numOfDays];

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

    if (pIndex == -1) {
        this->result->finalFund = totalFS[this->numOfDays - 1];
        this->result->realReturn = totalFS[this->numOfDays - 1] - this->initFund;
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
        tmp += (i + 1) * totalFS[i] - (i + 1) * this->initFund;
        tmp2 += (i + 1) * (i + 1);
    }
    double slope = tmp / tmp2;

    auto *trendLine = new double[this->numOfDays]; // daily expected standardization fund

    // calculate trend line
    for (int i = 0; i < this->numOfDays; i++) {
        trendLine[i] = slope * (i + 1) + this->initFund;
    }

    // calculate risk
    tmp = 0.0;

    for (int i = 0; i < this->numOfDays; i++) {
        tmp += (totalFS[i] - trendLine[i]) * (totalFS[i] - trendLine[i]);
    }

    double risk = sqrt(tmp / this->numOfDays);

    // calculate trend value
    double trendVal = 0.0;

    if (risk > 0) { // if risk == 0 then trendVal = 0
        if (slope >= 0) {
            trendVal = slope / risk;
        } else {
            trendVal = slope * risk;
        }
    }

    if (pIndex == -1) {
        this->result->initFund = this->initFund;
        this->result->expectedReturn = slope;
        this->result->risk = risk;
        this->result->gBest = trendVal;
        for (int i = 0; i < this->numOfDays; i++) {
            this->result->totalFS[i] = totalFS[i];
        }
        for (int i = 0; i < this->numOfStocks; i++) {
            this->result->solution[i] = p->solution[i];
            for (int j = 0; j < this->numOfDays; j++) {
                this->result->stocks[i].code = this->stocks[i].code;
                this->result->stocks[i].fs[j] = this->stocks[i].fs[j];
            }
        }
    }
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

    delete[] allocatedFund;
    delete[] totalFS;
    delete[] trendLine;
    return trendVal;
}

void Model::calcFS(Particle *p, int *allocatedFund, int gen, int pIndex) {
    // calculate fund standardization
    for (int i = 0; i < this->numOfStocks; i++) {
        if (p->solution[i] == 1) { // the chosen stock
            double balance = 0;
            int amount = 0; // how many stocks can buy

            for (int j = 0; j < this->numOfDays; j++) {
                if (j == 0) { // first day
                    amount = int(allocatedFund[i] /
                                 (this->stocks[i].price[j] * 1000.0 +
                                  this->stocks[i].price[j] * (1000.0 * this->fee)));
                    balance = allocatedFund[i] - amount * this->stocks[i].price[j] * 1000.0 -
                              amount * this->stocks[i].price[j] * 1000.0 * this->fee;

                    this->stocks[i].fs[j] =
                            allocatedFund[i] - this->stocks[i].price[j] * amount * 1000.0 * this->fee;
                    if (pIndex == -1) {
                        this->result->amount[i] = amount;
                        this->result->balance[i] = balance;
                        this->result->allocatedFund[i] = allocatedFund[i];
                    }
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
                            this->stocks[i].price[j] * amount * 1000.0 -
                            this->stocks[i].price[j] * amount * 1000.0 * this->fee -
                            this->stocks[i].price[j] * amount * 1000.0 * this->tax + balance;
                }
            }
        }
    }
}

int Model::getGeneration() const {
    return this->generation;
}

int Model::getPopulation() const {
    return this->population;
}

double Model::getTheta() const {
    return this->theta;
}

int Model::getLength() const {
    return this->numOfStocks;
}

void Model::setResult(Result *rs) {
    this->result = rs;
}

int Model::getNumOfStocks() const {
    return this->numOfStocks;
}

int Model::getNumOfDays() const {
    return this->numOfDays;
}