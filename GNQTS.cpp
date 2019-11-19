//
// Created by Mark Hsu on 2019/11/6.
//

#include "GNQTS.h"
#include "Logger.h"
#include <sstream>
#include <climits>

using std::ifstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::ios;

GNQTS::GNQTS() {
    string path = "../data/M2M/train_2009_12(2009 Q1).csv";
    this->model.setPopulation(10);
    this->model.setGeneration(1);
    this->model.setTheta(0.0004);
    this->model.setFund(10000000.0);

    initStock(path);
    readData(path);

    this->model.setLength(this->numOfStocks);

    // allocate memory
    this->pMatrix = new double[this->model.getLength()];
    this->particle = new Particle[this->model.getPopulation()];
    for (int i = 0; i < this->model.getPopulation(); i++) {
        this->particle[i].setSolutionSize(this->model.getLength());
    }

    // initialize solution
    for (int i = 0; i < this->model.getPopulation(); i++) {
        for (int j = 0; j < this->model.getLength(); j++) {
            this->particle[i].solution[j] = rand() % 2;
        }
    }

    // initialize probability matrix
    for (int i = 0; i < this->model.getLength(); i++) {
        this->pMatrix[i] = 0.5;
    }
    // TODO: initialize best and worst particle

    this->bestParticle.fitness = 0;
    this->worstParticle.fitness = INT_MAX;

#if DEBUG
    Logger logger("../log/init");
    for (int i = 0; i < this->model.getPopulation(); i++) {
        for (int j = 0; j < this->model.getLength(); j++) {
            logger.write(this->particle[i].solution[j]);
        }
        logger.writeLine("");
    }
#endif
}

void GNQTS::run() {
    for (int i = 0; i < this->model.getGeneration(); i++) {
        measure();
        calcFitness();
        update();
    }
}

// get numbers of stocks and days in order to allocate memory for `Stock`
void GNQTS::initStock(const string &path) {
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

void GNQTS::readData(const string &path) {
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

void GNQTS::measure() {
    auto *randomMatrix = new double[this->model.getLength()];

    for (int i = 0; i < this->model.getGeneration(); i++) {
        // generate a random matrix
        for (int j = 0; j < this->model.getLength(); j++) {
            randomMatrix[j] = rand() / RAND_MAX;
        }

        // measure the value of each item (taken or not)
        for (int j = 0; j < this->model.getLength(); j++) {
            if (this->pMatrix[j] > randomMatrix[j]) {
                this->particle[i].solution[j] = 1;
            } else {
                this->particle[i].solution[j] = 0;
            }
        }
    }
}

void GNQTS::calcFitness() {
    for (int i = 0; i < this->model.getGeneration(); i++) {
        for (int j = 0; j < this->model.getLength(); j++) {
            this->particle[i].fitness = this->model.getFitness(this->particle[i]);
        }
    }
}

void GNQTS::update() {
    for (int i = 0; i < this->model.getLength(); i++) {
        if (this->bestParticle.solution[i] == 1 && worstParticle.solution[i] == 0) {
            if (this->pMatrix[i] < 0.9)
                this->pMatrix[i] += 0.1;
        } else if (bestParticle.solution[i] == 0 && worstParticle.solution[i] == 1) {
            if (this->pMatrix[i] > 0.1)
                this->pMatrix[i] -= 0.1;
        }
    }
}


