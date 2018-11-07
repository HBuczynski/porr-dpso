#! /usr/bin/python3

import os
import re
import sys

import matplotlib.pyplot as plt
import networkx as nx

if len(sys.argv) != 3:
    print("Please provide 2 args: utah_graph_path and nodes_cnt")
    exit(1)
path = sys.argv[1]
node_cnt = int(sys.argv[2])

print("CWD", os.getcwd())

graph = nx.Graph()

with open(path + "_nodes.txt") as f:
    for line in f:
        if line.startswith("#"):
            continue
        m = re.match('(.*) (.*) (.*)', line)
        idx = int(m.group(1))
        x = float(m.group(2))
        y = float(m.group(3))
        if idx < node_cnt:
            graph.add_node(idx, pos=(x, y))

with open(path + "_edges.txt") as f:
    for line in f:
        if line.startswith("#"):
            continue
        m = re.match('(.*) (.*) (.*) (.*)', line)
        idx = int(m.group(1))
        start = int(m.group(2))
        end = int(m.group(3))
        weight = float(m.group(3))
        if start < node_cnt and end < node_cnt:
            graph.add_edge(start, end, weight=weight)

print("drawing...")
pos = nx.get_node_attributes(graph, 'pos')
nx.draw(graph, pos, with_labels=True, node_size=80)
plt.show()
