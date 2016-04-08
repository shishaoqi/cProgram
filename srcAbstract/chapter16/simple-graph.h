/*
 * File: graph.h (preliminary)
 * ---------------------------
 * This file is a simplified version of an interface for graphs.
 */

#ifndef _graph_h
#define _graph_h

/*
 * Types: graphADT, nodeADT
 * ------------------------
 * These abstract types are used to represent graphs and nodes,
 * respectively.
 */

typedef struct graphCDT *graphADT;
typedef struct nodeCDT *nodeADT;

/*
 * Function: NewGraph
 * Usage: graph = NewGraph();
 * --------------------------
 * This function returns a new graph with no nodes.
 */

graphADT NewGraph(void);

/*
 * Function: NewNode
 * Usage: node = NewNode(graph);
 * -----------------------------
 * This function creates a new node and adds it to the graph.
 */

nodeADT NewNode(graphADT graph);

/*
 * Function: Connect
 * Usage: Connect(n1, n2);
 * -----------------------
 * This function creates an arc from node n1 to n2.
 */

void Connect(nodeADT n1, nodeADT n2);

/*
 * Function: IsConnected
 * Usage: if (IsConnected(n1, n2)) . . .
 * -------------------------------------
 * This function returns TRUE if there is an arc from n1 to n2.
 */

bool IsConnected(nodeADT n1, nodeADT n2);

#endif
