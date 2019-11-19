//
// Created by Mark Hsu on 2019/11/6.
//

#include "GNQTS.h"
#include "Logger.h"
#include <sstream>

using std::ifstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::ios;

GNQTS::GNQTS() {
    string path = "../data/M2M/train_2009_12(2009 Q1).csv";
    this->model.setPopulation(10);
    this->model.setGeneration(100);
    this->model.setTheta(0.0004);
    this->model.setFund(10000000.0);

    initStock(path);
    readData(path);

    this->model.setLength(this->numOfStocks);

    this->particles = new Particle[this->model.getPopulation()];

    for (int i = 0; i < this->model.getPopulation(); i++) {
        this->particles[i].setSolutionSize(this->model.getLength());
    }

    for (int i = 0; i < this->model.getPopulation(); i++) {
        for (int j = 0; j < this->model.getLength(); j++) {
            this->particles[i].solution[j] = rand() % 2;
        }
    }
#if DEBUG
    Logger logger("../log/init");
    for (int i = 0; i < this->model.getPopulation(); i++) {
        for (int j = 0; j < this->model.getLength(); j++) {
            logger.write(this->particles[i].solution[j]);
        }
        logger.writeLine("");
    }
#endif
}

void GNQTS::run() {

}

void GNQTS::initStock(string path) {
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

void GNQTS::readData(string path) {
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
        Logger logger("../log/readData.csv", ios::out);
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

void GNQTS::measure() {

}

void GNQTS::calcFitness() {

}

void GNQTS::update() {

}



