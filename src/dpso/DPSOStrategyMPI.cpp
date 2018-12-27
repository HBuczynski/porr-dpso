//
// Created by mariusz on 24.12.18.
//

#ifdef MODE_MPI

#include "DPSOStrategyMPI.h"
#include "../utility/logger.h"

enum DPSOMsg : int {
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
    int new_path_edges_cnt = get_new_path_edges_cnt(best_path_owner_id);
    share_and_update_best_position(best_path_owner_id, new_path_edges_cnt);
}

int DPSOStrategyMPI::find_best_path_owner() const {
    MPI_Status status;

    const auto master_id = 0;
    const auto best_path_length_size_float = 1;
    int best_path_owner_id;
    if (rank != 0) {
        // slave
        DEBUG("[%d] Sending best_path_length = %f to master\n", rank, best_path_length);
        MPI_STATUS(MPI_Send(&best_path_length, best_path_length_size_float, MPI_FLOAT,
                            master_id, DPSOMsg::LOCAL_BEST_LENGTH, MPI_COMM_WORLD));
    } else {
        // master
        std::vector<std::pair<float, int>> paths;
        paths.emplace_back(best_path_length, master_id);

        float length;
        for (auto i = 1; i < comm_size; i++) {
            MPI_STATUS(MPI_Recv(&length, best_path_length_size_float, MPI_FLOAT,
                                MPI_ANY_SOURCE, DPSOMsg::LOCAL_BEST_LENGTH, MPI_COMM_WORLD, &status));
            paths.emplace_back(length, status.MPI_SOURCE);
            DEBUG("[0] Receiving best_path_length = %f from %d\n", length, status.MPI_SOURCE);
        }
        std::sort(paths.begin(), paths.end());
        best_path_owner_id = paths.begin()->second;

#ifdef DEBUG_INFO_WANTED
        INFO("[%d] owner_id | length\n", rank);
        for (const auto &e : paths)
            INFO("[%d] %d %f\n", rank, e.second, e.first);
#endif
    }

    auto msg_size_int = 1;
    MPI_STATUS(MPI_Bcast(&best_path_owner_id, msg_size_int, MPI_INT, master_id, MPI_COMM_WORLD));
    DEBUG("[%d] best_path_owner_id = %d\n", rank, best_path_owner_id);

    return best_path_owner_id;
}

int DPSOStrategyMPI::get_new_path_edges_cnt(int owner_id) const {
    int new_path_edges_cnt;
    if (rank == owner_id)
        new_path_edges_cnt = static_cast<int>(best_position.size());

    auto msg_size_int = 1;
    MPI_STATUS(MPI_Bcast(&new_path_edges_cnt, msg_size_int, MPI_INT, owner_id, MPI_COMM_WORLD));
    DEBUG("[%d] new_path_edges_cnt = %d\n", rank, new_path_edges_cnt);

    return new_path_edges_cnt;
}

void DPSOStrategyMPI::share_and_update_best_position(int owner_id, int edges_cnt) {
    // allocate memory, do not call default constructor for DPSOEdge
    std::vector<DPSOEdge> buffer;
    buffer.reserve(static_cast<unsigned long>(edges_cnt));

    if (rank == owner_id) {
        for (const DPSOEdge &e : best_position)
            buffer.push_back(e);
    }

    auto best_path_length_size_float = 1;
    MPI_STATUS(MPI_Bcast(&best_path_length, best_path_length_size_float, MPI_FLOAT, owner_id, MPI_COMM_WORLD));
    MPI_STATUS(MPI_Bcast(buffer.data(), edges_cnt * sizeof(DPSOEdge), MPI_BYTE, owner_id, MPI_COMM_WORLD));
    DEBUG("[%d] best_path_length = %f\n", rank, best_path_length);

    if (rank == owner_id)
        return;

    best_position.clear();
    // this is hack because buffer thinks ist size = 0
    for (int i = 0; i < edges_cnt; ++i)
        best_position.insert(buffer[i]);
}

#endif
