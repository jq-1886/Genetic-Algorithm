cmake_minimum_required(VERSION 3.16.3)
project(acse-4-gormanium-rush-acanthite)

set(CMAKE_CXX_STANDARD 14)

include_directories(includes)

add_executable(main
        includes/CCircuit.h
        includes/CUnit.h
        includes/Genetic_Algorithm.h
        src/CCircuit.cpp
        src/Genetic_Algorithm.cpp
        src/main.cpp
        )

add_executable(test1
        includes/CUnit.h
        includes/CCircuit.h
        src/CCircuit.cpp
        src/Genetic_Algorithm.cpp
        tests/test1.cpp
        )

add_executable(test2
        includes/CUnit.h
        includes/CCircuit.h
        includes/Genetic_Algorithm.h
        src/CCircuit.cpp
        src/Genetic_Algorithm.cpp
        tests/test2.cpp
        )


FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

endif()