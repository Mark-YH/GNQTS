//
// Created by Mark Hsu on 2020/3/27.
//

#ifndef GNQTS_STOCK_PARTICLE_FA_H
#define GNQTS_STOCK_PARTICLE_FA_H

class ParticleFA {
public:
    int **solution; // ratio
    double fitness;
    int numOfBit;
    int numOfChosen;

    /** @param _numOfBit: the number of bits
     *  @param _numOfChosen: the number of stocks
     */
    explicit ParticleFA(int _numOfBit, int _numOfChosen) {
        this->fitness = -DBL_MAX;
        this->numOfBit = _numOfBit;
        this->numOfChosen = _numOfChosen;
        this->solution = new int *[_numOfChosen];
        for (int i = 0; i < _numOfChosen; i++) {
            this->solution[i] = new int[_numOfBit];
            for (int j = 0; j < _numOfBit; j++)
                this->solution[i][j] = 0;
        }
    }

    ParticleFA(const ParticleFA &copy) {
        if (this != &copy) {
            this->fitness = copy.fitness;
            this->numOfBit = copy.numOfBit;
            this->numOfChosen = copy.numOfChosen;
            this->solution = new int *[copy.numOfChosen];
            for (int i = 0; i < copy.numOfChosen; i++) {
                this->solution[i] = new int[copy.numOfBit];
                for (int j = 0; j < copy.numOfBit; j++)
                    this->solution[i][j] = copy.solution[i][j];
            }
        }
    }

    ParticleFA &operator=(const ParticleFA &copy) {
        if (this != &copy) {
            this->fitness = copy.fitness;
            this->numOfBit = copy.numOfBit;
            this->numOfChosen = copy.numOfChosen;
            this->solution = new int *[copy.numOfChosen];
            for (int i = 0; i < copy.numOfChosen; i++) {
                this->solution[i] = new int[copy.numOfBit];
                for (int j = 0; j < copy.numOfBit; j++)
                    this->solution[i][j] = copy.solution[i][j];
            }
        }
        return *this;
    }

    bool operator>(const ParticleFA &instance) const {
        return this->fitness > instance.fitness;
    }

    bool operator>=(const ParticleFA &instance) const {
        return this->fitness >= instance.fitness;
    }

    bool operator<(const ParticleFA &instance) const {
        return this->fitness < instance.fitness;
    }

    bool operator<=(const ParticleFA &instance) const {
        return this->fitness <= instance.fitness;
    }

    bool operator==(const ParticleFA &instance) const {
        return this->fitness == instance.fitness;
    }

    ~ParticleFA() {
        for (int i = 0; i < this->numOfChosen; i++)
            delete[] solution[i];
        delete[] solution;
    }
};

#endif //GNQTS_STOCK_PARTICLE_FA_H
