cmake_minimum_required(VERSION 3.17)
project(FWI)

set(CMAKE_CXX_STANDARD 20)


set(PROJECT_HEADERS
        dotProduct.h
        )
set(PROJECT_SOURCES
        main.cpp
        dotProduct.cpp
        )

add_executable(FWI ${PROJECT_SOURCES} ${PROJECT_HEADERS})