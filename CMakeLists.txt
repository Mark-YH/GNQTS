cmake_minimum_required(VERSION 3.15)
project(PortfolioOptimization)

set(CMAKE_CXX_STANDARD 17)

add_executable(PortfolioOptimization main.cpp EWFA/QTS.cpp EWFA/QTS.h EWFA/Particle.h Model/Model.cpp Model/Model.h Model/Stock.h Model/Logger.h Model/Result.h FundAllocation/QTS.cpp FundAllocation/QTS.h FundAllocation/ParticleFA.h)
