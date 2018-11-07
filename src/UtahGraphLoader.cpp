//
// Created by mariusz on 07.11.18.
//

#include <unistd.h>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <zconf.h>
#include "UtahGraphLoader.h"

namespace {
    void assertOpenFile(const std::ifstream &fs) {
        if (!fs.is_open())
            throw std::domain_error("Error in opening file");
    }

    void removeHeader(std::ifstream &fs) {
        char line[255];
        fs.getline(line, 255);
    }
}

UtahGraphLoader::UtahGraphLoader(std::string name, int nodes)
        : path("resources/" + name), graph(nodes), nodes_cnt(nodes) {}

Graph UtahGraphLoader::load() {
    load_nodes();
    load_edges();
    return graph;
}

void UtahGraphLoader::load_nodes() {
    std::ifstream fs(path + "_nodes.txt", std::ios::in);
    assertOpenFile(fs);
    removeHeader(fs);

    int id;
    float x, y;
    for (auto i = 0; i < nodes_cnt; ++i) {
        fs >> id >> x >> y;
        if (fs)
            graph.addNode(id, x, y);
        else
            break;
    }

    fs.close();
}


void UtahGraphLoader::load_edges() {
    std::ifstream fs(path + "_edges.txt", std::ios::in);
    assertOpenFile(fs);
    removeHeader(fs);

    int id;
    int from, to;
    float weight;
    while (fs) {
        fs >> id >> from >> to >> weight;
        if (fs) {
            if (from < nodes_cnt && to < nodes_cnt) {
                graph.addEdge(from, {weight, to});
                graph.addEdge(to, {weight, from});
            }
        } else
            break;
    }

    fs.close();
}

void UtahGraphLoader::show() const {
#ifdef ENABLE_DRAWING
    auto pid = fork();
    if (pid > 0)
        return;
    if (pid < 0)
        throw std::runtime_error("Error in forking process");

    execl("tools/draw_graph.py", "draw_graph.py",
          path.c_str(),
          std::to_string(nodes_cnt).c_str(),
          (char *) nullptr);
    std::cout << "Error in spawning tools process" << std::endl;
#endif
}
