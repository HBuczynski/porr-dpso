//
// Created by mariusz on 24.12.18.
//

#include "DPSOStrategyMPI.h"
#include "../utility/logger.h"

enum DPSOMessage : int {
    LOCAL_BEST_LENGTH = 1
};

DPSOStrategyMPI::DPSOStrategyMPI(const Graph &graph, NodeID begin, NodeID end, DPSOConfig config)
        : DPSO(graph, begin, end, std::move(config)) {
    MPI_STATUS(MPI_Comm_size(MPI_COMM_WORLD, &comm_size));
    MPI_STATUS(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
}

void DPSOStrategyMPI::update_best_position() {
    DPSO::update_best_position();

    int best_path_owner_id = find_best_path_owner();

    int new_path_length;
    if (best_path_owner_id == rank) {
        new_path_length = static_cast<int>(best_position.size());
    }
    MPI_STATUS(MPI_Bcast(&new_path_length, 1, MPI_INT, best_path_owner_id, MPI_COMM_WORLD));
    printf("new_path_length %d\n", new_path_length);

    // TODO send buffer
}

int DPSOStrategyMPI::find_best_path_owner() {
    MPI_Status status;

    int best_path_owner_id;
    if (rank != 0) {
        // slave
        printf("Sending best_path_length %f from %d to master\n", best_path_length, rank);
        MPI_STATUS(MPI_Send(&best_path_length, 1, MPI_FLOAT, 0, DPSOMessage::LOCAL_BEST_LENGTH, MPI_COMM_WORLD));
    } else {
        // master
        std::vector<std::pair<float, int>> paths;
        paths.emplace_back(best_path_length, 0);

        float length;
        for (auto i = 1; i < comm_size; i++) {
            MPI_STATUS(MPI_Recv(&length, 1, MPI_FLOAT, MPI_ANY_SOURCE, DPSOMessage::LOCAL_BEST_LENGTH, MPI_COMM_WORLD, &status));
            paths.emplace_back(length, status.MPI_SOURCE);
            printf("Receiving best_path_length %f from %d\n", length, status.MPI_SOURCE);
        }

        std::sort(paths.begin(), paths.end());
        for (const auto &e : paths)
            printf("%d %f\n", e.second, e.first);

        best_path_owner_id = paths.begin()->second;
    }

    MPI_STATUS(MPI_Bcast(&best_path_owner_id, 1, MPI_INT, 0, MPI_COMM_WORLD));
    printf("best_path_owner_id %d\n", best_path_owner_id);

    return best_path_owner_id;
}
