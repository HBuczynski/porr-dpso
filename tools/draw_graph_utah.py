#! /usr/bin/python3

import os
import re
import sys

import matplotlib.pyplot as plt
import networkx as nx

if len(sys.argv) < 3:
    print("Please provide at least 2 args: utah_graph_path and nodes_cnt")
    exit(1)
path = sys.argv[1]
node_cnt = int(sys.argv[2])
selected_nodes = []
if len(sys.argv) == 4:
    selected_nodes = [int(x) for x in sys.argv[3].strip().split(" ")]
print("CWD", os.getcwd())

graph = nx.Graph()
selected_graph = nx.Graph()

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
        if idx in selected_nodes:
            selected_graph.add_node(idx, pos=(x, y))

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
selected_pos = nx.get_node_attributes(selected_graph, 'pos')
nx.draw(graph, pos, with_labels=True, node_size=80)
nx.draw_networkx_nodes(selected_graph, selected_pos, node_color='b')
plt.show()
