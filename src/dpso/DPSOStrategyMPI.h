//
// Created by mariusz on 24.12.18.
//

#ifndef PORR_DPSO_DPSOSTRATEGYMPI_H
#define PORR_DPSO_DPSOSTRATEGYMPI_H


#include "DPSO.h"

class DPSOStrategyMPI : public DPSO {
public:
    DPSOStrategyMPI(const Graph &graph, NodeID begin, NodeID end, DPSOConfig config);

private:
    void update_best_position() override;

    int find_best_path_owner() const;

    int get_new_path_edges_cnt(int owner_id) const;

    void share_and_update_best_position(int owner_id, int edges_cnt);

    int comm_size;
    int rank;
};


#endif //PORR_DPSO_DPSOSTRATEGYMPI_H
