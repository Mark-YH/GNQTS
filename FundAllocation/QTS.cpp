//
// Created by Lab114 on 2020/3/27.
//

#include "QTS.h"

QTS::QTS(Model *m, int *selection) {
    this->model = m;
    this->stockSelection = selection;
    this->numOfChosen = 0;
    for (int i = 0; i < this->model->getNumOfStocks(); i++)
        if (this->stockSelection[i] == 1)
            this->numOfChosen++;

    this->bestGeneration = 0;
    for (int i = 0; i < model->getPopulation(); i++) {
        this->particle.emplace_back(ParticleFA(this->model->getNumOfStocks()));
    }
    this->lBest = new ParticleFA(this->model->getNumOfStocks());
    this->lWorst = new ParticleFA(this->model->getNumOfStocks());
    this->gBest = new ParticleFA(this->model->getNumOfStocks());
    for (int i = 0; i < this->model->getNumOfStocks(); i++) {
        if (this->stockSelection[i] == 1)
            this->gBest->solution[i] = 0.5;
        else
            this->gBest->solution[i] = 0;
    }
    this->gBest->fitness = -DBL_MAX;
}

QTS::~QTS() {
    delete gBest;
    delete lBest;
    delete lWorst;
    this->stockSelection = nullptr;
    this->model = nullptr;
}

void QTS::run() {
    for (int i = 0; i < this->model->getGeneration(); i++) {
        generate();
        evaluate(i);
        update(i);
    }
    for (int j = 0; j < this->model->getNumOfStocks(); j++)
        if (this->stockSelection[j] == 1)
            std::cout << this->gBest->solution[j] * 100.0 << ',';
    this->gBest->fitness = this->model->getFitness(this->stockSelection, bestGeneration, -1,
                                                   this->gBest->solution);
    std::cout << endl << setprecision(20) << this->gBest->fitness << endl;
}

void QTS::generate() {
    for (int i = 0; i < this->model->getPopulation(); i++) {
        double sum = 0.0;
        for (int j = 0; j < this->model->getNumOfStocks(); j++) {
            if (this->stockSelection[j] == 1) {
                this->particle[i].solution[j] = (double) rand() / RAND_MAX;
                sum += this->particle[i].solution[j];
            } else {
                this->particle[i].solution[j] = 0;
            }
        }
        // normalize the probabilities
        if (sum != 1) {
            for (int j = 0; j < this->model->getNumOfStocks(); j++) {
                if (this->stockSelection[j] == 1)
                    this->particle[i].solution[j] = this->particle[i].solution[j] / sum;
            }
        }
    }
}

void QTS::evaluate(int generation) {
    this->lWorst->fitness = DBL_MAX;
    for (int i = 0; i < this->model->getPopulation(); i++) {
        this->particle[i].fitness = this->model->getFitness(this->stockSelection, generation, i,
                                                            this->particle[i].solution);
        this->gBest->fitness = this->model->getFitness(this->stockSelection, generation, i, this->gBest->solution);
        // check if it needs to update the local best solution
        if (this->lBest->fitness < this->particle[i].fitness) {
            this->lBest->operator=(this->particle[i]);
        }
        // check if it needs to update the local worst solution
        if (this->lWorst->fitness > this->particle[i].fitness)
            this->lWorst->operator=(this->particle[i]);
    }
}

void QTS::update(int generation) {
    double sum = 0.0;
    ParticleFA tmpBest(this->model->getNumOfStocks());
    tmpBest.operator=(*this->gBest);
    for (int i = 0; i < this->model->getNumOfStocks(); i++) {
        if (this->stockSelection[i] == 1) {
            if (lBest->solution[i] > lWorst->solution[i])
                tmpBest.solution[i] += this->model->getTheta();
            if (lBest->solution[i] < lWorst->solution[i])
                tmpBest.solution[i] -= this->model->getTheta();
            sum += tmpBest.solution[i];
        }
    }
    // normalize the probabilities
    if (sum != 1) {
        for (int i = 0; i < this->model->getNumOfStocks(); i++) {
            if (this->stockSelection[i] == 1)
                tmpBest.solution[i] = tmpBest.solution[i] / sum;
        }
    }
    tmpBest.fitness = this->model->getFitness(this->stockSelection, generation, -2, tmpBest.solution);
    if (tmpBest.fitness >= this->gBest->fitness)
        this->gBest->operator=(tmpBest);
}

int QTS::getBestGeneration() {
    return this->bestGeneration;
}
