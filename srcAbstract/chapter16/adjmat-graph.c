/*
 * File: graph.c (adjacency matrix version)
 * ----------------------------------------
 * This file implements the preliminary version of the graph.h
 * interface using an adjacency matrix.
 */

#include <stdio.h>
#include "genlib.h"
#include "graph.h"

/*
 * Constant: MaxNodes
 * ------------------
 * This constant defines the maximum number of nodes in a
 * graph and is used in this implementation to simplify the
 * presentation.  In practice, it would be better to let the
 * structure expand dynamically.
 */

#define MaxNodes 25

/*
 * Type: nodeCDT
 * -------------
 * This type defines the concrete representation of a node
 * in the adjacency matrix representation.  The connections
 * are stored as part of the graph.  The index in the node
 * structure identifies the position of this node in the
 * adjacency matrix.
 */

struct nodeCDT {
    graphADT graph;
    int index;
};

/*
 * Type: graphCDT
 * --------------
 * This definition provides the concrete type for a graphADT in
 * the adjacency matrix representation.  The graph includes a
 * fixed array of nodes and a boolean matrix representing the
 * connections.  Note that the array uses the concrete node type,
 * which means that the addresses of these elements are nodeADTs.
 */

struct graphCDT {
    struct nodeCDT nodes[MaxNodes];
    bool matrix[MaxNodes][MaxNodes];
    int nNodes;
};

/* Exported entries */

graphADT NewGraph(void)
{
    graphADT graph;

    graph = New(graphADT);
    graph->nNodes = 0;
    return (graph);
}

nodeADT NewNode(graphADT graph)
{
    nodeADT node;
    int i, index;

    if (graph->nNodes >= MaxNodes) Error("NewNode: Too many nodes");
    index = graph->nNodes++;
    node = &graph->nodes[index];
    node->graph = graph;
    node->index = index;
    for (i = 0; i < graph->nNodes; i++) {
        graph->matrix[i][index] = FALSE;
        graph->matrix[index][i] = FALSE;
    }
    return (node);
}

void Connect(nodeADT n1, nodeADT n2)
{
    if (n1->graph != n2->graph) {
        Error("Connect: Nodes are in different graphs");
    }
    n1->graph->matrix[n1->index][n2->index] = TRUE;
}

bool IsConnected(nodeADT n1, nodeADT n2)
{
    if (n1->graph != n2->graph) {
        Error("Connect: Nodes are in different graphs");
    }
    return (n1->graph->matrix[n1->index][n2->index]);
}
