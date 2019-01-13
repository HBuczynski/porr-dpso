//
// Created by mariusz on 07.11.18.
//

#ifndef PORR_DPSO_UTAHGRAPHLOADER_H
#define PORR_DPSO_UTAHGRAPHLOADER_H


#include <string>
#include "../graph/Graph.h"
#include "../dpso/Particle.h"

class UtahGraphLoader {
public:
    UtahGraphLoader(const UtahGraphLoader &) = delete;

    UtahGraphLoader(UtahGraphLoader &&) = delete;

    UtahGraphLoader operator=(const UtahGraphLoader &) = delete;

    UtahGraphLoader operator=(UtahGraphLoader &&) = delete;

    Graph load();

    void show(std::optional<EdgesSet> path_to_show = std::nullopt) const;

    static UtahGraphLoader &getInstance() {
        static UtahGraphLoader instance = UtahGraphLoader(DPSOConfig());
        return instance;
    }

private:
    explicit UtahGraphLoader(DPSOConfig config);

    const std::string path;
    unsigned int nodes_cnt;

    void load_nodes(Graph &graph);

    void load_edges(Graph &graph);
};


#endif //PORR_DPSO_UTAHGRAPHLOADER_H
