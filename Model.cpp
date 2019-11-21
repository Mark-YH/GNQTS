//
// Created by Mark Hsu on 2019/11/13.
//

#include "Model.h"
#include <fstream>
#include <sstream>
#include <cmath>

#define DEBUG 1

using std::ifstream;
using std::string;
using std::stringstream;
using std::ios;

void Model::init() {
    string path = "../data/M2M/train_2009_12(2009 Q1).csv";
    initStock(path);
    readData(path);
    for (int i = 0; i < this->numOfStocks; i++) {
        for (int j = 0; j < this->numOfDays; j++) {
            this->stocks[i].level[j] = 0;
        }
    }
}

// get numbers of stocks and days in order to allocate memory for `Stock`
void Model::initStock(const string &path) {
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

        this->stocks = new Stock[this->numOfStocks];

        for (int i = 0; i < this->numOfStocks; i++) {
            this->stocks[i].setPriceSize(this->numOfDays);
        }
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
#if DEBUG
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

double Model::getFitness(Particle p, int gen) {
    int numOfChosen = 0; // how much stock is chosen
    double totalBalance;
    double fund = this->fund;

    for (int i = 0; i < this->numOfStocks; i++) {
        if (p.solution[i] == 1) {
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
    logStock.writeComma("Generation:");
    logStock.writeLine(gen);
    logStock.writeComma("number of chosen stock:");
    logStock.writeLine(numOfChosen);
    logStock.writeComma("Chosen stocks:");
    for (int i = 0; i < this->numOfStocks; i++) {
        if (p.solution[i] == 1) {
            logStock.writeComma(this->stocks[i].code);
        }
    }
    logStock.writeLine("");

    logStock.writeComma("Allocated fund:");
    logStock.writeLine(fund);
    logStock.writeComma("Balance:");
    logStock.writeLine(totalBalance);
#endif

    // calculate water level
    for (int i = 0; i < this->numOfStocks; i++) {
        if (p.solution[i] == 1) { // the chosen stock
            double balance = 0;
            int amount = 0; // how much stock can buy

            for (int j = 0; j < this->numOfDays; j++) {
                if (j == 0) { // first day
                    double pricePerStock = this->stocks[i].price[j] * 1000.0 * (1.0 + this->fee);
                    amount = floor(fund / pricePerStock);
                    balance = fund - amount * pricePerStock;
                    this->stocks[i].level[j] =
                            fund - (amount * this->stocks[i].price[j] * 1000 * this->fee);
                } else { // the other days
                    this->stocks[i].level[j] =
                            this->stocks[i].price[j] * amount * 1000 * (1 - (this->fee + this->tax)) + balance;
                }
            }
        }
    }

    // calculate total water level
    double *totalLevel = new double[this->numOfDays];

    for (int i = 0; i < this->numOfDays; i++) {
        totalLevel[i] = 0;
    }
    for (int i = 0; i < numOfStocks; i++) {
        if (p.solution[i] == 1) {
            for (int j = 0; j < this->numOfDays; j++) {
                totalLevel[j] += this->stocks[i].level[j];
            }
        }
    }

    for (int i = 0; i < numOfDays; i++) {
        totalLevel[i] += totalBalance; // add the balance from allocation part
    }

#if DEBUG
    Logger logLevel("../log/level.csv", 15);
    logLevel.writeComma("Generation:");
    logLevel.writeLine(gen);
    logLevel.writeLine("Water level:");

    for (int i = 0; i < this->numOfStocks; i++) {
        if (p.solution[i] == 1) {
            for (int j = 0; j < this->numOfDays; j++) {
                logLevel.writeComma(this->stocks[i].level[j]);
            }
            logLevel.writeLine("");
        }
    }

    for (int i = 0; i < this->numOfDays; i++) {
        logLevel.writeComma(totalLevel[i]);
    }
    logLevel.writeLine("");
#endif

    // calculate slope
    double tmp = 0, tmp2 = 0;

    for (int i = 1; i <= this->numOfDays; i++) {
        if (p.solution[i] == 1) {
            tmp += i * totalLevel[i] - i * fund;
            tmp2 += i * i;
        }
    }
    double slope = tmp / tmp2;

    double trendLine = 0; // daily expected water level

    // calculate trend line
    for (int i = 1; i <= this->numOfDays; i++) {
        trendLine = slope * i + fund;
    }

    // calculate risk
    tmp = 0;

    for (int i = 0; i < this->numOfDays; i++) {
        tmp += (totalLevel[i] - trendLine) * (totalLevel[i] - trendLine);
    }
    double risk = sqrt(tmp / this->numOfDays);

    // calculate trend value
    double trendVal = slope / risk;

#if DEBUG
    Logger logTrend("../log/trend_value.csv");
    logTrend.writeComma("Generation:");
    logTrend.writeLine(gen);
    logTrend.writeComma("slope:");
    logTrend.writeLine(slope);
    logTrend.writeComma("trend line:");
    logTrend.writeLine(trendLine);
    logTrend.writeComma("risk:");
    logTrend.writeLine(risk);
    logTrend.writeComma("trend value:");
    logTrend.writeLine(trendVal);
#endif

    return trendVal;
}

void Model::setGeneration(int generation) {
    this->generation = generation;
}

int Model::getGeneration() {
    return this->generation;
}

void Model::setPopulation(int population) {
    this->population = population;
}

int Model::getPopulation() {
    return this->population;
}

void Model::setTheta(double theta) {
    this->theta = theta;
}

double Model::getTheta() {
    return this->theta;
}

void Model::setFund(double fund) {
    this->fund = fund;
}

void Model::setFee(double fee) {
    this->fee = fee;
}

void Model::setTax(double tax) {
    this->tax = tax;
}

int Model::getLength() {
    return this->numOfStocks;
}