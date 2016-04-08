/*
 * File: graph.c (adjacency list version)
 * --------------------------------------
 * This file implements the preliminary version of the graph.h
 * interface using linked lists to store the nodes and arcs.
 */

#include <stdio.h>
#include "genlib.h"
#include "list.h"
#include "graph.h"

/*
 * Type: graphCDT
 * --------------
 * This definition provides the concrete type for a graphADT,
 * which is implemented as a list of nodes.
 */

struct graphCDT {
    listADT nodes;
};

/*
 * Type: nodeCDT
 * -------------
 * This type defines the concrete structure of a node, which
 * consists of the graph pointer and a list of connected nodes.
 */

struct nodeCDT {
    graphADT graph;
    listADT arcs;
};

/* Exported entries */

graphADT NewGraph(void)
{
    graphADT graph;

    graph = New(graphADT);
    graph->nodes = NULL;
    return (graph);
}

nodeADT NewNode(graphADT graph)
{
    nodeADT node;

    node = New(nodeADT);
    node->graph = graph;
    node->arcs = NULL;
    graph->nodes = ListCons(node, graph->nodes);
    return (node);
}

void Connect(nodeADT n1, nodeADT n2)
{
    if (n1->graph != n2->graph) {
        Error("Connect: Nodes are in different graphs");
    }
    if (IsConnected(n1, n2)) return;
    n1->arcs = ListCons(n2, n1->arcs);
}

bool IsConnected(nodeADT n1, nodeADT n2)
{
    listADT list;

    if (n1->graph != n2->graph) {
        Error("Connect: Nodes are in different graphs");
    }
    list = n1->arcs;
    while (list != NULL) {
        if (n2 == ListHead(list)) return (TRUE);
        list = ListTail(list);
    }
    return (FALSE);
}
