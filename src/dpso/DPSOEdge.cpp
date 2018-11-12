//
// Created by mariusz on 12.11.18.
//

#include "DPSOEdge.h"

bool operator==(const DPSOEdge &lhs, const DPSOEdge &rhs) {
    return lhs.propability == rhs.propability && lhs.edge == rhs.edge;
}
