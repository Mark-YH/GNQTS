cmake_minimum_required(VERSION 3.15)
project(GNQTS_Stock)

set(CMAKE_CXX_STANDARD 14)

add_executable(GNQTS_Stock main.cpp StockSelection/GNQTS.cpp StockSelection/GNQTS.h StockSelection/Particle.h Model/Model.cpp Model/Model.h Model/Stock.h Model/Logger.h Model/Result.h Model/SlidingWindow.h FundAllocation/QTS.cpp FundAllocation/QTS.h FundAllocation/Particle_FA.h)
