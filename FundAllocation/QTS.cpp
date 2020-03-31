//
// Created by Lab114 on 2020/3/27.
//

#include "QTS.h"

QTS::QTS(Model *m, int *selection) {
    this->model = m;
    this->stockSelection = selection;

    for (int i = 0; i < this->model->getPopulation(); i++) {
        this->particle.emplace_back(ParticleFA(24));
    }
    this->gBest = new ParticleFA(24);
    this->lWorst = new ParticleFA(24);

    this->pMatrix = new double[this->gBest->length];
    for (int i = 0; i < this->gBest->length; i++) {
        this->pMatrix[i] = 0.5;
    }
}

QTS::~QTS() {
    delete gBest;
    delete lWorst;
    this->model = nullptr;
}

void QTS::run() {
    for (int i = 0; i < this->model->getGeneration(); i++) {
        measure(i);
        evaluate(i);
        update(i);
    }
}

void QTS::measure(int generation) {
    for (int i = 0; i < this->model->getPopulation(); i++) {
        for (int j = 0; j < this->gBest->length; j++) {
            if (this->pMatrix[j] > (double) rand() / RAND_MAX) {
                this->particle[i].solution[j] = 1;
            } else {
                this->particle[i].solution[j] = 0;
            }
        }
    }
}

void QTS::evaluate(int generation) {
    this->lWorst->fitness = DBL_MAX;
    for (int i = 0; i < this->model->getPopulation(); i++) {

    }
}

void QTS::update(int generation) {
    for (int i = 0; i < this->model->getLength(); i++) {
        if (this->gBest->solution[i] == 0 && this->lWorst->solution[i] == 1) {
            if (this->pMatrix[i] > 0.5) {
                this->pMatrix[i] = 1 - this->pMatrix[i];
                this->pMatrix[i] -= this->model->getTheta();
            } else {
                this->pMatrix[i] -= this->model->getTheta();
            }
        }

        if (this->gBest->solution[i] == 1 && this->lWorst->solution[i] == 0) {
            if (this->pMatrix[i] < 0.5) {
                this->pMatrix[i] = 1 - this->pMatrix[i];
                this->pMatrix[i] += this->model->getTheta();
            } else {
                this->pMatrix[i] += this->model->getTheta();
            }
        }
    }
}
