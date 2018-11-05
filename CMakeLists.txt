cmake_minimum_required(VERSION 3.12)
project(porr_dpso)

set(CMAKE_CXX_STANDARD 17)

add_executable(porr_dpso
        src/main.cpp
        src/graph/Edge.cpp
        src/graph/Edge.h
        src/graph/Graph.cpp
        src/graph/Graph.h
        src/graph/Node.h)
