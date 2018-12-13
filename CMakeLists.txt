cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME porr_dpso)
project(${PROJECT_NAME})

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_compile_definitions(ENABLE_DRAWING)

file(GLOB source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/dpso/*.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/graph/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")

add_executable(${PROJECT_NAME} ${source_list} ${header_list})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
