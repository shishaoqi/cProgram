/*
 * File: graph.h
 * -------------
 * This interface allows the client to manipulate graphs, which
 * are defined in keeping with their mathematical formulation
 * as a set of nodes connected by a set of arcs.
 */

#ifndef _graph_h
#define _graph_h

#include "genlib.h"
#include "set.h"

/*
 * Types: graphADT, nodeADT, arcADT
 * --------------------------------
 * These abstract types are used to represent graphs, nodes, and
 * arcs, respectively.
 */

typedef struct graphCDT *graphADT;
typedef struct nodeCDT *nodeADT;
typedef struct arcCDT *arcADT;

/*
 * Function: NewGraph
 * Usage: graph = NewGraph();
 * --------------------------
 * This function returns a new graph with no nodes.
 */

graphADT NewGraph(void);

/*
 * Function: FreeGraph
 * Usage: FreeGraph(graph);
 * ------------------------
 * This function frees the storage for the graph, along with
 * its nodes and arcs.
 */

void FreeGraph(graphADT graph);

/*
 * Function: NewNode
 * Usage: node = NewNode(graph);
 * -----------------------------
 * This function creates a new node and adds it to the graph.
 */

nodeADT NewNode(graphADT graph);

/*
 * Function: NewArc
 * Usage: arc = NewArc(n1, n2);
 * ----------------------------
 * This function creates a connection from node n1 to n2
 * and returns a new arcADT representing that connection.
 */

arcADT NewArc(nodeADT n1, nodeADT n2);

/*
 * Function: Connect
 * Usage: Connect(n1, n2);
 * -----------------------
 * This function creates a new arc from node n1 to n2, but
 * does not actually return the arcADT.
 */

void Connect(nodeADT n1, nodeADT n2);

/*
 * Function: IsConnected
 * Usage: if (IsConnected(n1, n2)) . . .
 * -------------------------------------
 * This function returns TRUE if there is an arc from n1 to n2.
 */

bool IsConnected(nodeADT n1, nodeADT n2);

/*
 * Functions: Nodes, ConnectedNodes
 * Usage: nodeSet = Nodes(graph);
 *        nodeSet = ConnectedNodes(node);
 * --------------------------------------
 * These functions return a set consisting of all nodes in
 * the graph and a set of the nodes to which a given node
 * is connected, respectively.  These functions are typically
 * used to initialize an iterator.  For example, the following
 * idiom iterates over the nodes connected to start:
 *
 *     foreach (node in ConnectedNodes(start))
 */

setADT Nodes(graphADT graph);
setADT ConnectedNodes(nodeADT node);

/*
 * Functions: Arcs, ArcsFrom, ArcsTo
 * Usage: arcSet = Arcs(graph);
 *        arcSet = ArcsFrom(node);
 *        arcSet = ArcsTo(node);
 * ---------------------------------
 * Each of these functions returns a set of arcs.   The Arcs
 * function returns all the arcs in the graph; the ArcsFrom
 * and ArcsTo functions return the set of arcs that begin or
 * end at a given node, respectively.  These functions are
 * typically used in conjunction with an iterator, as in the
 * following foreach idiom:
 *
 *     foreach (arc in Arcs(graph)) . . .
 */

setADT Arcs(graphADT graph);
setADT ArcsFrom(nodeADT node);
setADT ArcsTo(nodeADT node);

/*
 * Functions: StartOfArc, EndOfArc
 * Usage: node = StartOfArc(arc);
 *        node = EndOfArc(arc);
 * -------------------------------
 * These functions return the nodes at the endpoints of the
 * specified arc.
 */

nodeADT StartOfArc(arcADT arc);
nodeADT EndOfArc(arcADT arc);

/*
 * Function: GetGraph
 * Usage: graph = GetGraph(node);
 * ------------------------------
 * This function returns the graph of which a node is a part.
 */

graphADT GetGraph(nodeADT node);

/*
 * Functions: SetVisitedFlag, ClearVisitedFlag, HasBeenVisited
 * Usage: SetVisitedFlag(node);
 *        ClearVisitedFlag(node);
 *        if (HasBeenVisitedFlag(node)) . . .
 * -----------------------------------------------------------
 * These functions allow clients to manipulate the state of an
 * internal flag associated with each node.  That flag is used
 * to indicate whether the node has been visited as part of a
 * traversal operation.
 */

void SetVisitedFlag(nodeADT node);
void ClearVisitedFlag(nodeADT node);
bool HasBeenVisited(nodeADT node);

/*
 * Functions: SetGraphData, SetNodeData, SetArcData
 *            GetGraphData, GetNodeData, GetArcData
 * Usage: SetGraphData(graph, data);
 *        data = GetGraphData(graph);
 * ------------------------------------------------
 * These functions make it possible for the client to associate
 * data blocks with graphs, nodes, and arcs.
 */

void SetGraphData(graphADT graph, void *data);
void *GetGraphData(graphADT graph);
void SetNodeData(nodeADT node, void *data);
void *GetNodeData(nodeADT node);
void SetArcData(arcADT arc, void *data);
void *GetArcData(arcADT arc);

#endif
