//
// Created by Mark Hsu on 2019/11/13.
//

#include "Model.h"

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

void Model::setFund(int fund) {
    this->fund = fund;
}

double Model::getFund() {
    return this->fund;
}

void Model::setLength(int length) {
    this->length = length;
}

int Model::getLength() {
    return this->length;
}