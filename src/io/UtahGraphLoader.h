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


    Graph load();

    void show(std::optional<EdgesSet> path_to_show=std::nullopt) const;

    static UtahGraphLoader &get_instance(){
        static UtahGraphLoader instance = UtahGraphLoader(DPSOConfig());
        return instance;
    }

private:
    explicit UtahGraphLoader(DPSOConfig config);

    const std::string path;
    Graph graph;
    unsigned int nodes_cnt;

    void load_nodes();

    void load_edges();

};


#endif //PORR_DPSO_UTAHGRAPHLOADER_H
