//
// Created by mariusz on 12.11.18.
//

#ifndef PORR_DPSO_DPSOEDGE_H
#define PORR_DPSO_DPSOEDGE_H


#include "../graph/Edge.h"

struct DPSOEdge {
    Edge edge;
    float propability;

    friend bool operator==(const DPSOEdge &lhs, const DPSOEdge &rhs);

    friend DPSOEdge operator*(DPSOEdge dpsoEdge, float scalar);

    friend DPSOEdge operator*(float scalar, const DPSOEdge &dpsoEdge);
};

namespace std {
    template<>
    struct hash<DPSOEdge> {
        size_t operator()(const DPSOEdge &obj) const noexcept {
            return hash<Edge>{}(obj.edge);
        }
    };
}


#endif //PORR_DPSO_DPSOEDGE_H
