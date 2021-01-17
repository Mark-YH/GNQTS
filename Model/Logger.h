//
// Created by Mark Hsu on 2019/11/5.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include <filesystem>

using std::ofstream;
using std::ios;
using std::ios_base;
using std::string;
using std::cerr;
using std::endl;
using std::setprecision;
using std::exception;
namespace fs = std::filesystem;

class Logger {
private:
    ofstream fOut;
    int precision;
public:
    Logger() {
        this->precision = 20;
    }

    Logger(int precision) {
        this->precision = precision;
    }

    explicit Logger(const fs::path &path) {
        try {
            if (!fs::is_directory(path.parent_path()))
                fs::create_directories(path.parent_path());
            fOut.open(path, ios::app);
            this->precision = 20;
        } catch (exception &e) {
            cerr << "Got an exception: " << e.what() << endl;
        }
    }

    Logger(const fs::path &path, int precision) {
        try {
            fOut.open(path, ios::app);
            this->precision = precision;
        } catch (exception &e) {
            cerr << "Got an exception: " << e.what() << endl;
        }
    }

    Logger(const fs::path &path, ios_base::openmode mode, int precision) {
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

    void setPath(const fs::path &path) {
        try {
            if (!fs::is_directory(path.parent_path()))
                fs::create_directories(path.parent_path());
            fOut.open(path, ios::app);
        } catch (exception &e) {
            cerr << "Got an exception: " << e.what() << endl;
        }
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
