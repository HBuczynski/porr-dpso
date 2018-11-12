//
// Created by mariusz on 12.11.18.
//

#ifndef PORR_DPSO_DPSOEDGE_H
#define PORR_DPSO_DPSOEDGE_H


#include "../graph/Edge.h"

struct DPSOEdge {
    float propability;
    Edge edge;

    friend bool operator==(const DPSOEdge &lhs, const DPSOEdge &rhs);
};

namespace std {
    template<>
    struct hash<DPSOEdge> {
        const size_t operator()(const DPSOEdge &obj) const noexcept {
            return hash<Edge>{}(obj.edge)
                   ^ (hash<float>{}(obj.propability) << 1);
        }
    };
}


#endif //PORR_DPSO_DPSOEDGE_H
