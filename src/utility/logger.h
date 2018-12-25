//
// Created by mariusz on 24.12.18.
//

#ifndef PORR_DPSO_LOGGER_H
#define PORR_DPSO_LOGGER_H

#ifdef MODE_MPI

#include <mpi.h>

#define ONLY_MASTER(exec) \
    do { \
        int _rank; \
        MPI_Comm_rank(MPI_COMM_WORLD, &_rank); \
        if (_rank == 0) { \
            exec \
        } \
    } while (false)

#define MPI_STATUS(exec) \
        do { \
            int _status = exec; \
            if (_status != MPI_SUCCESS) { \
                printf("\n\nMPI ERROR %d\n\n", _status); \
                exit(1); \
            } \
        } while (false)

#else
#define ONLY_MASTER(exec) \
    do { \
        exec \
    } while (false)

#define MPI_STATUS(exec)
#endif // MODE_MPI


#define DEBUG_PRINTING_LEVEL_OFF    0
#define DEBUG_PRINTING_LEVEL_INFO   1
#define DEBUG_PRINTING_LEVEL_DEBUG  2

#ifndef DEBUG_PRINTING_LEVEL
#define DEBUG_PRINTING_LEVEL DEBUG_PRINTING_LEVEL_OFF
#endif

#if DEBUG_PRINTING_LEVEL >= DEBUG_PRINTING_LEVEL_INFO
#define INFO(...) printf(__VA_ARGS__)
#define DEBUG_INFO_WANTED
#else
#define INFO(...) do { } while(0)
#endif

#if DEBUG_PRINTING_LEVEL >= DEBUG_PRINTING_LEVEL_DEBUG
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...) do { } while(0)
#endif

#endif //PORR_DPSO_LOGGER_H
