//
// Created by Mark Hsu on 2019/11/6.
//

#include "GNQTS.h"
#include <climits>

/*! @param DEBUG val
 * val = 0: only generate the final result.
 * val > 0: generate output including `fitness`, `best and worst particle` and above results.
 * val > 4: generate output including `measure` and above results.
 * val > 6: generate output including `update` and above results.
 * val > 10: generate output including `random matrix` and above results.
 */
#define DEBUG 0

GNQTS::GNQTS(Model *m) {
    this->model = m;

    // allocate memory
    this->pMatrix = new double[this->model->getLength()];
    this->particle = new Particle[this->model->getPopulation()];
    for (int i = 0; i < this->model->getPopulation(); i++) {
        this->particle[i].setSolutionSize(this->model->getLength());
    }
    this->bestParticle = new Particle;
    this->worstParticle = new Particle;
    this->bestParticle->setSolutionSize(this->model->getLength());
    this->worstParticle->setSolutionSize(this->model->getLength());

    // initialize probability matrix
    for (int i = 0; i < this->model->getLength(); i++) {
        this->pMatrix[i] = 0.5;
    }

    // global best
    this->bestParticle->fitness = -std::numeric_limits<double>::max();
    this->bestGeneration = 0;
}

GNQTS::~GNQTS() {
    delete[] this->particle;
    delete[] this->pMatrix;
    delete this->bestParticle;
    delete this->worstParticle;
    this->model = nullptr;
}

void GNQTS::run() {
    for (int i = 0; i < this->model->getGeneration(); i++) {
        measure(i);
        calcFitness(i);
        update(i);
    }

    this->model->getFitness(this->bestParticle, bestGeneration, -1, nullptr);
    this->model->result->generation = this->model->getGeneration();
    this->model->result->population = this->model->getPopulation();
    this->model->result->uBound = this->model->getTheta();
    this->model->result->lBound = this->model->getTheta();
    this->model->result->theta = this->model->getTheta();
    this->model->result->round = ROUND;
}

void GNQTS::measure(int gen) {
    auto *randomMatrix = new double[this->model->getLength()];

    for (int i = 0; i < this->model->getPopulation(); i++) {
        // generate a random matrix
        for (int j = 0; j < this->model->getLength(); j++) {
            randomMatrix[j] = (double) rand() / RAND_MAX;
        }
#if DEBUG > 10
        Logger logger("../log/random_matrix.csv", 5);
        if (gen == 0 && i == 0) {
            logger.writeComma("Generation");
            logger.writeComma("Individual");
            for (int j = 0; j < this->model->getLength(); j++) {
                logger.writeComma(j);
            }
            logger.writeLine("");
        }
        logger.writeComma(gen);
        logger.writeComma(i);
        for (int j = 0; j < this->model->getLength(); j++) {
            logger.writeComma(randomMatrix[j]);
        }
        logger.writeLine("");
#endif
        for (int j = 0; j < this->model->getLength(); j++) {
            if (this->pMatrix[j] > randomMatrix[j]) {
                this->particle[i].solution[j] = 1;
            } else {
                this->particle[i].solution[j] = 0;
            }
        }
    }

#if DEBUG > 4
    Logger logger("../log/measure.csv");

    for (int i = 0; i < this->model->getPopulation(); i++) {
        if (gen == 0 && i == 0) {
            logger.writeComma("Generation");
            logger.writeComma("Individual");
            for (int j = 0; j < this->model->getLength(); j++) {
                logger.writeComma(j);
            }
            logger.writeLine("");
        }
        logger.writeComma(gen);
        logger.writeComma(i);

        for (int j = 0; j < this->model->getLength(); j++) {
            logger.writeComma(this->particle[i].solution[j]);
        }
        logger.writeLine("");
    }
#endif
    delete[] randomMatrix;
}

void GNQTS::calcFitness(int gen) {
    // local worst
    this->worstParticle->fitness = INT_MAX;

    for (int i = 0; i < this->model->getPopulation(); i++) {
        this->particle[i].fitness = this->model->getFitness((this->particle + i), gen, i, nullptr);

        // Check if it needs to update best particle
        if (this->particle[i].fitness > this->bestParticle->fitness) {
            if (this->particle[i].fitness >= 0) {
                for (int j = 0; j < this->model->getLength(); j++) {
                    this->bestParticle->solution[j] = this->particle[i].solution[j];
                }
                this->bestParticle->fitness = this->particle[i].fitness;
            } else {
                for (int j = 0; j < this->model->getLength(); j++) {
                    this->bestParticle->solution[j] = 0;
                }
                this->bestParticle->fitness = 0.0;
            }
            bestGeneration = gen;
        }
        // Check if it needs to update worst particle
        if (this->worstParticle->fitness > this->particle[i].fitness) {
            for (int j = 0; j < this->model->getLength(); j++) {
                this->worstParticle->solution[j] = this->particle[i].solution[j];
            }
            this->worstParticle->fitness = this->particle[i].fitness;
        }

#if DEBUG
        Logger logger("../log/fitness.csv");
        if (gen == 0 && i == 0) {
            logger.writeComma("Generation");
            logger.writeComma("Individual");
            logger.writeComma("Fitness");
            logger.writeLine("");
        }
        logger.writeComma(gen);
        logger.writeComma(i);
        logger.writeLine(this->particle[i].fitness);
        if (i == this->model->getPopulation() - 1) {
            logger.writeComma(gen);
            logger.writeComma("Global best");
            logger.writeLine(this->bestParticle->fitness);
            logger.writeComma(gen);
            logger.writeComma("Local worst");
            logger.writeLine(this->worstParticle->fitness);
        }
#endif
    }
#if DEBUG
    Logger logger("../log/best&worst_particle.csv");
    if (gen == 0) {
        logger.writeComma("Generation");
        logger.writeComma("Type");
        logger.writeComma("Fitness:");
        for (int j = 0; j < this->model->getLength(); j++) {
            logger.writeComma(j);
        }
        logger.writeLine("");
    }
    logger.writeComma(gen);
    logger.writeComma("Best");
    logger.writeComma(this->bestParticle->fitness);

    for (int j = 0; j < this->model->getLength(); j++) {
        logger.writeComma(this->bestParticle->solution[j]);
    }
    logger.writeLine("");

    logger.writeComma(gen);
    logger.writeComma("Worst");
    logger.writeComma(this->worstParticle->fitness);

    for (int j = 0; j < this->model->getLength(); j++) {
        logger.writeComma(this->worstParticle->solution[j]);
    }
    logger.writeLine("");
#endif
}

void GNQTS::update(int gen) {
    for (int i = 0; i < this->model->getLength(); i++) {
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
#if DEBUG > 6
    Logger logger("../log/update.csv", 10);
    if (gen == 0) {
        logger.writeComma("Generation");
        for (int j = 0; j < this->model->getLength(); j++) {
            logger.writeComma(j);
        }
        logger.writeLine("");
    }
    logger.writeComma(gen);
    for (int i = 0; i < this->model->getLength(); i++) {
        logger.writeComma(pMatrix[i]);
    }
    logger.writeLine("");
#endif
}

int GNQTS::getBestGeneration() {
    return this->bestGeneration;
}