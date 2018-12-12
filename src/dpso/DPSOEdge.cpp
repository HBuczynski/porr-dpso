//
// Created by mariusz on 12.11.18.
//

#include "DPSOEdge.h"

bool operator==(const DPSOEdge &lhs, const DPSOEdge &rhs) {
    return lhs.propability == rhs.propability && lhs.edge == rhs.edge;
}

DPSOEdge operator*(DPSOEdge dpsoEdge, float scalar) {
    dpsoEdge.propability *= scalar;
    if (dpsoEdge.propability > 1.0f)
        dpsoEdge.propability = 1.0f;
    return dpsoEdge;
}

DPSOEdge operator*(float scalar, const DPSOEdge &dpsoEdge) {
    return dpsoEdge * scalar;
}

