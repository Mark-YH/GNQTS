//
// Created by Mark Hsu on 2020/3/27.
//

#include "QTS.h"
#include <cmath>
#include <bitset>

using std::bitset;

QTS::QTS(Model *m, int *selection) {
    this->numOfBit = 7;
    this->model = m;
    this->stockSelection = selection;
    for (int i = 0; i < this->model->getNumOfStocks(); i++)
        if (this->stockSelection[i] == 1)
            this->indexOfChosen.push_back(i);
    this->bestGeneration = 0;

    for (int i = 0; i < this->model->getPopulation(); i++) {
        this->particle.emplace_back(ParticleFA(this->numOfBit, this->indexOfChosen.size()));
    }
    this->gBest = new ParticleFA(this->numOfBit, this->indexOfChosen.size());

    this->model->result->round = ROUND;
    this->model->result->generation = this->model->getGeneration();
    this->model->result->population = this->model->getPopulation();
    this->model->result->uBound = this->model->getTheta();
    this->model->result->lBound = this->model->getTheta();
    this->model->result->theta = this->model->getTheta();
}

QTS::~QTS() {
    delete this->gBest;
    this->stockSelection = nullptr;
    this->model = nullptr;
}

void QTS::run() {
    for (int i = 0; i < this->model->getGeneration(); i++) {
        measure(i);
        evaluate(i);
    }
    auto *allocRatio = new double[this->model->getNumOfStocks()]; // the fund ratio of each chosen stock
    std::memset(allocRatio, 0, sizeof(double) * this->model->getNumOfStocks());
    for (int i = 0; i < this->indexOfChosen.size(); i++) {
        for (int j = 0; j < this->numOfBit; j++) {
            if (this->gBest->solution[i][j] == 1) {
                allocRatio[indexOfChosen[i]] += powf(2.0, float(-(j + 1)));
            }
        }
    }
    normalize(allocRatio);
    for (int i = 0; i < this->indexOfChosen.size(); i++)
        std::cout << allocRatio[indexOfChosen[i]] * 100.0 << '\t';
    std::cout << endl << this->model->getFitness(this->stockSelection, this->bestGeneration, -1, allocRatio) << endl;
    delete[] allocRatio;
}

void QTS::measure(int generation) {
    string binary = bitset<21>(generation).to_string();
    for (int j = 0; j < this->indexOfChosen.size(); j++) {
        if (j == 0) { // stock #1
            for (int k = 0; k < this->numOfBit; k++) {
                if (binary[k] == '1')
                    this->particle[0].solution[j][k - this->numOfBit * j] = 1;
                else
                    this->particle[0].solution[j][k - this->numOfBit * j] = 0;
            }
        } else if (j == 1) { // stock #2
            for (int k = this->numOfBit; k < this->numOfBit + this->numOfBit; k++) { // bit = 7~13
                if (binary[k] == '1')
                    this->particle[0].solution[j][k - this->numOfBit * j] = 1;
                else
                    this->particle[0].solution[j][k - this->numOfBit * j] = 0;
            }
        } else if (j == 2) { // stock #3
            for (int k = this->numOfBit * 2; k < this->numOfBit * 3; k++) { // bit = 14~20
                if (binary[k] == '1')
                    this->particle[0].solution[j][k - this->numOfBit * j] = 1;
                else
                    this->particle[0].solution[j][k - this->numOfBit * j] = 0;
            }
        }
    }
}

void QTS::evaluate(int generation) {
    auto *allocRatio = new double[this->model->getNumOfStocks()]; // the fund ratio of each chosen stock;
    std::memset(allocRatio, 0, sizeof(double) * this->model->getNumOfStocks());
    for (int j = 0; j < this->indexOfChosen.size(); j++) {
        for (int k = 0; k < this->numOfBit; k++) {
            if (this->particle[0].solution[j][k] == 1) {
                allocRatio[this->indexOfChosen[j]] += powf(2.0, float(-(k + 1)));
            }
        }
    }
    normalize(allocRatio);
    this->particle[0].fitness = this->model->getFitness(this->stockSelection, generation, 0, allocRatio);

    // update best particle
    if (this->particle[0] > *this->gBest) {
        *this->gBest = this->particle[0];
        this->bestGeneration = generation;
    }
    delete[] allocRatio;
}

void QTS::normalize(double *_allocRatio) const {
    double sum = 0.0;
    for (int i = 0; i < this->indexOfChosen.size(); i++) {
        sum += _allocRatio[indexOfChosen[i]];
    }
    if (sum == 0) {
        for (int i = 0; i < this->indexOfChosen.size(); i++)
            _allocRatio[this->indexOfChosen[i]] = 1.0 / indexOfChosen.size();
    } else if (sum != 1) {
        for (int i = 0; i < this->indexOfChosen.size(); i++)
            _allocRatio[indexOfChosen[i]] = _allocRatio[indexOfChosen[i]] / sum;
    }
}

int QTS::getBestGeneration() {
    return this->bestGeneration;
}