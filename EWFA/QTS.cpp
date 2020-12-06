//
// Created by Mark Hsu on 2019/11/6.
//

#include "QTS.h"

namespace EWFA {
    QTS::QTS(Model *m) {
        this->model = m;

        // allocate memory
        this->pMatrix = new double[this->model->getNumOfStocks()];
        for (int i = 0; i < this->model->getPopulation(); i++) {
            this->particle.emplace_back(this->model->getNumOfStocks());
        }
        this->bestParticle = new Particle{this->model->getNumOfStocks()};
        this->worstParticle = new Particle{this->model->getNumOfStocks()};

        // initialize probability matrix
        for (int i = 0; i < this->model->getNumOfStocks(); i++) {
            this->pMatrix[i] = 0.5;
        }

        this->bestGeneration = 0;
    }

    QTS::~QTS() {
        delete[] this->pMatrix;
        delete this->bestParticle;
        delete this->worstParticle;
        this->model = nullptr;
    }

    void QTS::run() {
        for (int i = 0; i < this->model->getGeneration(); i++) {
            measure();
            evaluate(i);
            update();
        }
        this->model->getFitness(this->bestParticle->solution, -1, vector<double>(), true);
    }

    void QTS::measure() {
        for (int i = 0; i < this->model->getPopulation(); i++) {
            for (int j = 0; j < this->model->getNumOfStocks(); j++) {
                if (this->pMatrix[j] > (double) rand() / RAND_MAX) {
                    this->particle[i].solution[j] = 1;
                } else {
                    this->particle[i].solution[j] = 0;
                }
            }
        }
    }

    void QTS::evaluate(int gen) {
        // local worst
        this->worstParticle->fitness = DBL_MAX;

        for (int i = 0; i < this->model->getPopulation(); i++) {
            this->particle[i].fitness = this->model->getFitness(this->particle[i].solution, i, vector<double>(), true);

            // Check if it needs to update best particle
            if (this->particle[i].fitness > this->bestParticle->fitness) {
                if (this->particle[i].fitness >= 0) {
                    *this->bestParticle = this->particle[i];
                    for (int j = 0; j < this->model->getNumOfStocks(); j++) {
                        this->bestParticle->solution[j] = this->particle[i].solution[j];
                    }
                    this->bestParticle->fitness = this->particle[i].fitness;
                } else {
                    for (int j = 0; j < this->model->getNumOfStocks(); j++) {
                        this->bestParticle->solution[j] = 0;
                    }
                    this->bestParticle->fitness = 0.0;
                }
                bestGeneration = gen;
            }
            // Check if it needs to update worst particle
            if (this->worstParticle->fitness > this->particle[i].fitness) {
                *this->worstParticle = this->particle[i];
            }
        }
    }

    void QTS::update() {
        for (int i = 0; i < this->model->getNumOfStocks(); i++) {
            if (this->bestParticle->solution[i] == 0 && this->worstParticle->solution[i] == 1) {
                if (this->pMatrix[i] > 0.5) {
                    this->pMatrix[i] = 1 - this->pMatrix[i];
                    this->pMatrix[i] -= this->model->getTheta();
                } else {
                    this->pMatrix[i] -= this->model->getTheta();
                }
            }

            if (this->bestParticle->solution[i] == 1 && this->worstParticle->solution[i] == 0) {
                if (this->pMatrix[i] < 0.5) {
                    this->pMatrix[i] = 1 - this->pMatrix[i];
                    this->pMatrix[i] += this->model->getTheta();
                } else {
                    this->pMatrix[i] += this->model->getTheta();
                }
            }
        }
    }

    int QTS::getBestGeneration() const {
        return this->bestGeneration;
    }
}