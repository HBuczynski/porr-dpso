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
    int find_best_path_owner();

    int comm_size;
    int rank;
};


#endif //PORR_DPSO_DPSOSTRATEGYMPI_H
