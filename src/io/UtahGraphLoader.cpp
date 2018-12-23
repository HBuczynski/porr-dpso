//
// Created by mariusz on 07.11.18.
//

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include "UtahGraphLoader.h"
#include "../dpso/Particle.h"

namespace {
    std::string resources = "resources/";

    void assertOpenFile(const std::ifstream &fs) {
        if (!fs.is_open())
            throw std::domain_error("Error in opening file");
    }

    void removeHeader(std::ifstream &fs) {
        char line[255];
        fs.getline(line, 255);
    }

    std::ifstream getFileHandler(std::string path) {
        std::ifstream fs(path, std::ios::in);
        assertOpenFile(fs);
        removeHeader(fs);
        return fs;
    }
}

UtahGraphLoader::UtahGraphLoader(DPSOConfig config)
        : path(resources + config.dataset_name), graph(config.number_of_nodes),
          nodes_cnt(config.number_of_nodes) {
    std::ifstream fs = getFileHandler(path + "_nodes.txt");

    int id = -1;
    float x, y;
    while (fs) {
        fs >> id >> x >> y;
    }
    graph = Graph(nodes_cnt);
}


Graph UtahGraphLoader::load() {
    load_nodes();
    load_edges();
    return graph;
}

void UtahGraphLoader::load_nodes() {
    std::ifstream fs = getFileHandler(path + "_nodes.txt");

    NodeID id;
    float x, y;
    for (auto i = 0u; i < nodes_cnt; ++i) {
        fs >> id >> x >> y;
        if (fs)
            graph.addNode(id, x, y);
        else
            break;
    }

    fs.close();
}

void UtahGraphLoader::load_edges() {
    std::ifstream fs = getFileHandler(path + "_edges.txt");

    NodeID id;
    NodeID from, to;
    float weight;
    while (fs) {
        fs >> id >> from >> to >> weight;
        if (fs) {
            if (from < nodes_cnt && to < nodes_cnt) {
                graph.addEdge({from, to, weight});
                graph.addEdge({to, from, weight});
            }
        } else
            break;
    }

    fs.close();
}

void UtahGraphLoader::show(std::optional<EdgesSet> path_to_show) const {
#ifdef ENABLE_DRAWING
    auto pid = fork();
    if (pid > 0)
        return;
    if (pid < 0)
        throw std::runtime_error("Error in forking process");

    if(path_to_show){
        std::stringstream selected_edges;
        for (auto const &edge : path_to_show.value()) {
            selected_edges << " " << edge.edge.from << " " << edge.edge.to;
        }

        execl("tools/draw_graph_utah.py", "draw_graph_utah.py",
              path.c_str(),
              std::to_string(nodes_cnt).c_str(),
              selected_edges.str().c_str(),
              (char *) nullptr);
    } else {
        execl("tools/draw_graph_utah.py", "draw_graph_utah.py",
              path.c_str(),
              std::to_string(nodes_cnt).c_str(),
              (char *) nullptr);
    }
    std::cout << "Error in spawning tools process" << std::endl;
#else
    // suppress unused variable warning
    (void) path_to_show;
#endif
}

