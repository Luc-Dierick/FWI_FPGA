cmake_minimum_required(VERSION 3.17)
project(FWI)

set(CMAKE_CXX_STANDARD 20)


set(PROJECT_HEADERS
        updateDirection.h
        )
set(PROJECT_SOURCES
        updateDirection.cpp
        )

add_executable(FWI ${PROJECT_SOURCES} ${PROJECT_HEADERS})