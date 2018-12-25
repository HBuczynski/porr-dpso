//
// Created by mariusz on 24.12.18.
//

#ifndef PORR_DPSO_LOGGER_H
#define PORR_DPSO_LOGGER_H

#ifdef MODE_MPI

#include <mpi.h>

#define ONLY_MPI_MASTER(exec) \
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

#define ONLY_MPI_MASTER(exec) \
    do { \
        exec \
    } while (false)
#endif

#endif //PORR_DPSO_LOGGER_H
