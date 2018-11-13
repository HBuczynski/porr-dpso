cmake_minimum_required(VERSION 3.12)
project(porr_dpso)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(ENABLE_DRAWING)

add_executable(porr_dpso
        src/main.cpp
        src/graph/Edge.cpp
        src/graph/Edge.h
        src/graph/Graph.cpp
        src/graph/Graph.h
        src/graph/Node.h
        src/io/UtahGraphLoader.cpp
        src/io/UtahGraphLoader.h src/dpso/DPSO.cpp src/dpso/DPSO.h src/dpso/Particle.cpp src/dpso/Particle.h src/dpso/DPSOEdge.cpp src/dpso/DPSOEdge.h src/dpso/DPSOConfig.h)
