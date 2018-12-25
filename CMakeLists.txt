cmake_minimum_required(VERSION 3.12)

# PROJECT_NAME
project(porr_dpso CXX)

# if cmake do not use new default value, remove CMakeCache.txt
option(OPT_SEQN "enable sequential mode" OFF)
option(OPT_OPEN_MP "enable OpenMP mode" OFF)
option(OPT_MPI "enable MPI mode" OFF)

option(OPT_DRAWING "enable drawing" OFF)


set(CMAKE_CXX_COMPILER /usr/bin/g++-7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

if (NOT WIN32)
    string(ASCII 27 Esc)
    set(ColourReset "${Esc}[m")
    set(Green "${Esc}[32m")
endif ()

if (OPT_SEQN)
    add_compile_definitions(MODE_SEQN)
    message(STATUS "${Green}SEQUENTIAL MODE ACTIVE${ColourReset}")
endif ()
if (OPT_OPEN_MP)
    add_compile_definitions(MODE_OPEN_MP)
    find_package(OpenMP REQUIRED)
    message(STATUS "${Green}PARALEL MODE ACTIVE${ColourReset}")
endif ()
if (OPT_MPI)
    add_compile_definitions(MODE_MPI)
    find_package(MPI REQUIRED)
    message(STATUS "${Green}MPI MODE ACTIVE${ColourReset}")
endif ()
if (OPT_DRAWING)
    add_compile_definitions(ENABLE_DRAWING)
    message(STATUS "${Green}DRAWING ENABLED${ColourReset}")
endif ()

if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

file(GLOB source_list
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/dpso/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/graph/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/*.cpp")

add_executable(${PROJECT_NAME} ${source_list} ${header_list})
if (OPT_OPEN_MP)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif ()
if (OPT_MPI)
    target_link_libraries(${PROJECT_NAME} PRIVATE MPI::MPI_CXX)
endif()
