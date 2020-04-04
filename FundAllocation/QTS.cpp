//
// Created by Mark Hsu on 2020/3/27.
//

#include "QTS.h"
#include <cstring>
#include <cmath>

QTS::QTS(Model &m, vector<int> &selection) : model{&m}, stockSelection{selection} {
    this->numOfBit = 7;
    for (int i = 0; i < this->model->getNumOfStocks(); i++)
        if (this->stockSelection[i] == 1)
            this->indexOfChosen.push_back(i);
    this->bestGeneration = 0;

    for (int i = 0; i < this->model->getPopulation(); i++) {
        this->particle.emplace_back(ParticleFA(this->numOfBit, this->indexOfChosen.size()));
    }
    this->gBest = new ParticleFA(this->numOfBit, this->indexOfChosen.size());
    this->lWorst = new ParticleFA(this->numOfBit, this->indexOfChosen.size());

    this->pMatrix = new double *[this->indexOfChosen.size()];
    for (int i = 0; i < this->indexOfChosen.size(); i++) {
        this->pMatrix[i] = new double[this->numOfBit];
        for (int j = 0; j < this->numOfBit; j++)
            this->pMatrix[i][j] = 0.5;
    }
}

QTS::~QTS() {
    delete this->gBest;
    delete this->lWorst;
    for (int i = 0; i < this->indexOfChosen.size(); i++)
        delete[] this->pMatrix[i];
    delete[] this->pMatrix;
    this->model = nullptr;
}

void QTS::run() {
    for (int i = 0; i < this->model->getGeneration(); i++) {
        measure();
        evaluate(i);
        update();
    }
    vector<double> allocRatio(this->model->getNumOfStocks()); // the fund ratio of each chosen stock
    for (int i = 0; i < this->indexOfChosen.size(); i++) {
        for (int j = 0; j < this->numOfBit; j++) {
            if (this->gBest->solution[i][j] == 1) {
                allocRatio[this->indexOfChosen[i]] += powf(2.0, float(-(j + 1)));
            }
        }
    }
    normalize(allocRatio);
    this->model->getFitness(this->stockSelection, -1, allocRatio);
}

void QTS::measure() {
    for (int i = 0; i < this->model->getPopulation(); i++) {
        for (int j = 0; j < this->indexOfChosen.size(); j++) {
            for (int k = 0; k < this->numOfBit; k++) {
                if (this->pMatrix[j][k] > (double) rand() / RAND_MAX) {
                    this->particle[i].solution[j][k] = 1;
                } else {
                    this->particle[i].solution[j][k] = 0;
                }
            }
        }
    }
}

void QTS::evaluate(int generation) {
    this->lWorst->fitness = DBL_MAX;
    for (int i = 0; i < this->model->getPopulation(); i++) {
        vector<double> allocRatio(this->model->getNumOfStocks()); // the fund ratio of each chosen stock;
        for (int j = 0; j < this->indexOfChosen.size(); j++) {
            for (int k = 0; k < this->numOfBit; k++) {
                if (this->particle[i].solution[j][k] == 1) {
                    allocRatio[this->indexOfChosen[j]] += powf(2.0, float(-(k + 1)));
                }
            }
        }
        normalize(allocRatio);
        this->particle[i].fitness = this->model->getFitness(this->stockSelection, i, allocRatio);

        // update best particle
        if (this->particle[i] > *this->gBest) {
            *this->gBest = this->particle[i];
            this->bestGeneration = generation;
        }
        // update worst particle
        if (this->particle[i] < *this->lWorst)
            *this->lWorst = this->particle[i];
    }
}

void QTS::normalize(vector<double> &_allocRatio) const {
    double sum = 0.0;
    for (int i = 0; i < this->indexOfChosen.size(); i++) {
        sum += _allocRatio[indexOfChosen[i]];
    }
    if (sum == 0) {
        for (int i = 0; i < this->indexOfChosen.size(); i++)
            _allocRatio[indexOfChosen[i]] = 1.0 / this->indexOfChosen.size();
    } else if (sum != 1) {
        for (int i = 0; i < this->indexOfChosen.size(); i++)
            _allocRatio[indexOfChosen[i]] = _allocRatio[indexOfChosen[i]] / sum;
    }
}

void QTS::update() {
    for (int i = 0; i < this->indexOfChosen.size(); i++) {
        for (int j = 0; j < this->numOfBit; j++) {
            if (this->gBest->solution[i][j] == 0 && this->lWorst->solution[i][j] == 1) {
                if (this->pMatrix[i][j] > 0.5) {
                    this->pMatrix[i][j] = 1 - this->pMatrix[i][j];
                    this->pMatrix[i][j] -= this->model->getTheta();
                } else {
                    this->pMatrix[i][j] -= this->model->getTheta();
                }
            }

            if (this->gBest->solution[i][j] == 1 && this->lWorst->solution[i][j] == 0) {
                if (this->pMatrix[i][j] < 0.5) {
                    this->pMatrix[i][j] = 1 - this->pMatrix[i][j];
                    this->pMatrix[i][j] += this->model->getTheta();
                } else {
                    this->pMatrix[i][j] += this->model->getTheta();
                }
            }
        }
    }
}

int QTS::getBestGeneration() const {
    return this->bestGeneration;
}