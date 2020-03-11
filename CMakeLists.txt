cmake_minimum_required(VERSION 3.15)
project(GNQTS_Stock)

set(CMAKE_CXX_STANDARD 14)

add_executable(GNQTS_Stock main.cpp GNQTS.cpp GNQTS.h Particle.h Model.cpp Model.h Stock.h Logger.h Result.h)
