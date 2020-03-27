//
// Created by Mark Hsu on 2019/11/5.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <iomanip>
#include <iostream>
#include <fstream>

using std::ofstream;
using std::ios;
using std::ios_base;
using std::string;
using std::cerr;
using std::endl;
using std::setprecision;
using std::exception;

class Logger {
private:
    ofstream fOut;
    int precision;
public:
    explicit Logger(const string &path) {
        try {
            fOut.open(path, ios::app);
            this->precision = 20;
        } catch (exception &e) {
            cerr << "Got an exception: " << e.what() << endl;
        }
    }

    Logger(const string &path, int precision) {
        try {
            fOut.open(path, ios::app);
            this->precision = precision;
        } catch (exception &e) {
            cerr << "Got an exception: " << e.what() << endl;
        }
    }

    Logger(const string &path, ios_base::openmode mode, int precision) {
        try {
            fOut.open(path, mode);
            this->precision = precision;
        } catch (exception &e) {
            cerr << "Got an exception: " << e.what() << endl;
        }
    }

    ~Logger() {
        fOut.close();
    }

    template<class T>
    void write(T num) {
        fOut << setprecision(precision) << num;
    }

    template<class T>
    void writeComma(T num) {
        fOut << setprecision(precision) << num << ',';
    }

    template<class T>
    void writeTab(T num) {
        fOut << setprecision(precision) << num << '\t';
    }

    template<class T>
    void writeSpace(T num) {
        fOut << setprecision(precision) << num << ' ';
    }

    template<class T>
    void writeLine(T num) {
        fOut << setprecision(precision) << num << endl;
    }
};

#endif //LOGGER_H
