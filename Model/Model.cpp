//
// Created by Mark Hsu on 2019/11/13.
//

#include "Model.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

using std::ifstream;
using std::string;
using std::stringstream;
using std::ios;
using std::pair;

Model::Model(int population, int generation, double theta, double fund, double fee, double tax) {
    this->population = population;
    this->generation = generation;
    this->theta = theta;
    this->initFund = fund;
    this->fee = fee;
    this->tax = tax;
    this->numOfDays = 0;
    this->numOfStocks = 0;
    this->result = nullptr;
}

Model::~Model() {
    this->result = nullptr;
}

void Model::nextSection(int section) {
    this->stocks.clear();
#if US_MARKET
    string path = "../data/US/" + tag + "/" + trainingSection[section];
#else
    string path = "../data/" + tag + "/" + trainingSection[section];
#endif
    getNumOfRowColumn(path);

    for (int i = 0; i < this->numOfStocks; i++) {
        this->stocks.emplace_back(this->numOfDays);
    }

    readData(path);

    for (int i = 0; i < this->numOfStocks; i++) {
        for (int j = 0; j < this->numOfDays; j++) {
            this->stocks[i].fs.push_back(0);
        }
    }/*
    // kick the particular stock out
    auto it = this->stocks.begin();
    while (it != this->stocks.end()) {
        if (it->code == "BRK.B" ||
//            it->code == "TCEHY" ||
//            it->code == "JNJ" ||
//            it->code == "XOM" ||
//            it->code == "JPM" ||
//            it->code == "V" ||
//            it->code == "WMT" ||
//            it->code == "BAC" ||
//            it->code == "PG" ||
//            it->code == "VZ" ||
//            it->code == "MA" ||
//            it->code == "INTC" ||
//            it->code == "CSCO" ||
//            it->code == "UNH" ||
            it->code == "GOOGL") {
            it--;
            this->stocks.erase(it + 1);
            this->numOfStocks--;
            continue;
        }
        it++;
    }*/
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
                    tmp.erase(remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
                    this->stocks[stockCount].code = tmp;
                } else {
                    this->stocks[stockCount].price[lineCount] = stod(tmp);
                }
                stockCount++;
            }
            lineCount++;
        }
        fin.close();
    } catch (std::exception &e) {
        fin.close();
        Logger logger("../log/error");
        logger.writeTab("void readData():");
        logger.writeLine(e.what());
    }
}

double Model::getFitness(vector<int> &solution, int pIndex, const vector<double> &allocRatio) {
    int numOfChosen = 0; // how much stock is chosen
    double totalBalance = this->initFund;
    for (int i = 0; i < this->numOfStocks; i++) {
        if (solution[i] == 1) {
            numOfChosen++;
        }
    }

    if (pIndex == -1) {
        this->result->numOfChosen = numOfChosen;
        // fix the real number of chosen stocks
        for (int i = 0; i < this->numOfStocks; i++) {
            if (solution[i] == 1 && allocRatio[i] == 0) {
                this->result->numOfChosen--;
            }
        }
    }

    // allocate fund
    vector<int> allocatedFund(this->numOfStocks);
    for (int i = 0; i < this->numOfStocks; i++) {
        if (solution[i] == 1) {
#if MODE == 0
            allocatedFund[i] = floor(this->initFund / numOfChosen);
#else
            allocatedFund[i] = floor(this->initFund * allocRatio[i]);
#endif
        } else {
            allocatedFund[i] = 0;
        }
    }

    // calculate the rest fund
    for (int i = 0; i < this->numOfStocks; i++) {
        if (solution[i] == 1)
            totalBalance -= allocatedFund[i];
    }

    // calculate fund standardization
    calcFS(solution, allocatedFund, pIndex);

    // calculate total fund standardization
    vector<double> totalFS(this->numOfDays); // initialized with value 0

    for (int i = 0; i < numOfStocks; i++) {
        if (solution[i] == 1) {
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

    // calculate slope
    double tmp = 0, tmp2 = 0;

    for (int i = 0; i < this->numOfDays; i++) {
        tmp += (i + 1) * totalFS[i] - (i + 1) * this->initFund;
        tmp2 += (i + 1) * (i + 1);
    }
    double slope = tmp / tmp2;

    vector<double> trendLine(this->numOfDays); // daily expected standardization fund
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
            this->result->solution[i] = solution[i];
            this->result->stocks[i] = this->stocks[i];
        }
    }
    return trendVal;
}

void Model::calcFS(vector<int> &solution, vector<int> &allocatedFund, int pIndex) {
    // calculate fund standardization
    for (int i = 0; i < this->numOfStocks; i++) {
        if (solution[i] == 1) { // the chosen stock
            double balance = 0;
            int amount = 0; // how many stocks can buy

            for (int j = 0; j < this->numOfDays; j++) {
                if (j == 0) { // first day
#if US_MARKET
                    amount = int(allocatedFund[i] / this->stocks[i].price[j]);
                    balance = allocatedFund[i] - amount * this->stocks[i].price[j];

                    this->stocks[i].fs[j] = allocatedFund[i];
#else
                    amount = int(allocatedFund[i] /
                                 (this->stocks[i].price[j] * 1000.0 +
                                  this->stocks[i].price[j] * (1000.0 * this->fee)));
                    balance = allocatedFund[i] - amount * this->stocks[i].price[j] * 1000.0 -
                              amount * this->stocks[i].price[j] * 1000.0 * this->fee;

                    this->stocks[i].fs[j] =
                            allocatedFund[i] - this->stocks[i].price[j] * amount * 1000.0 * this->fee;
#endif
                    if (pIndex == -1) {
                        this->result->amount[i] = amount;
                        this->result->balance[i] = balance;
                        this->result->allocatedFund[i] = allocatedFund[i];
                    }
                } else { // the other days
#if US_MARKET
                    this->stocks[i].fs[j] =
                            this->stocks[i].price[j] * amount + balance;
#else
                    this->stocks[i].fs[j] =
                            this->stocks[i].price[j] * amount * 1000.0 -
                            this->stocks[i].price[j] * amount * 1000.0 * this->fee -
                            this->stocks[i].price[j] * amount * 1000.0 * this->tax + balance;
#endif
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

void Model::setResult(Result *rs) {
    this->result = rs;
    this->result->generation = this->getGeneration();
    this->result->population = this->getPopulation();
    this->result->uBound = this->getTheta();
    this->result->lBound = this->getTheta();
    this->result->theta = this->getTheta();
    this->result->round = ROUND;
}

int Model::getNumOfStocks() const {
    return this->numOfStocks;
}

int Model::getNumOfDays() const {
    return this->numOfDays;
}

string Model::getStockSymbol(int i) {
    return this->stocks[i].code;
}