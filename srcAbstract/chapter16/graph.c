/*
 * File: graph.c
 * -------------
 * This file implements the graph.h interface using sets to
 * represent the nodes and arcs.
 */

#include <stdio.h>
#include "genlib.h"
#include "set.h"
#include "graph.h"
#include "foreach.h"

/*
 * Type: graphCDT
 * --------------
 * This definition provides the concrete type for a graphADT,
 * which is implemented as a set of nodes and a set of arcs.
 */

struct graphCDT {
    setADT nodes, arcs;
    void *data;
};

/*
 * Type: nodeCDT
 * -------------
 * This type defines the concrete structure of a node, which
 * consists of the graph pointer, a flag indicating whether
 * the node has been visited, and a client data field.
 */

struct nodeCDT {
    graphADT graph;
    bool visited;
    void *data;
};

/*
 * Type: arcCDT
 * ------------
 * The concrete type for an arc consists of its endpoints, plus
 * a data field for the client.
 */

struct arcCDT {
    nodeADT from, to;
    void *data;
};

/* Exported entries */

graphADT NewGraph(void)
{
    graphADT graph;

    graph = New(graphADT);
    graph->nodes = NewPtrSet(PtrCmpFn);
    graph->arcs = NewPtrSet(PtrCmpFn);
    graph->data = NULL;
    return (graph);
}

void FreeGraph(graphADT graph)
{
    nodeADT node;
    arcADT arc;

    foreach (node in graph->nodes) {
        FreeBlock(node);
    }
    foreach (arc in graph->arcs) {
        FreeBlock(arc);
    }
    FreeSet(graph->nodes);
    FreeSet(graph->arcs);
    FreeBlock(graph);
}

nodeADT NewNode(graphADT graph)
{
    nodeADT node;

    node = New(nodeADT);
    node->graph = graph;
    node->visited = FALSE;
    node->data = NULL;
    AddPtrElement(graph->nodes, node);
    return (node);
}

arcADT NewArc(nodeADT n1, nodeADT n2)
{
    arcADT arc;

    if (n1->graph != n2->graph) {
        Error("Connect: Nodes are in different graphs");
    }
    arc = New(arcADT);
    arc->from = n1;
    arc->to = n2;
    arc->data = NULL;
    AddPtrElement(n1->graph->arcs, arc);
    return (arc);
}

void Connect(nodeADT n1, nodeADT n2)
{
    (void) NewArc(n1, n2);
}

bool IsConnected(nodeADT n1, nodeADT n2)
{
    arcADT arc;

    if (n1->graph != n2->graph) {
        Error("IsConnected: Nodes are in different graphs");
    }
    foreach (arc in n1->graph->arcs) {
        if (arc->from == n1 && arc->to == n2) return (TRUE);
    }
    return (FALSE);
}

setADT Nodes(graphADT graph)
{
    return (graph->nodes);
}

setADT ConnectedNodes(nodeADT node)
{
    setADT result;
    arcADT arc;

    result = NewPtrSet(PtrCmpFn);
    foreach (arc in node->graph->arcs) {
        if (arc->from == node) AddPtrElement(result, arc->to);
    }
    return (result);
}

setADT Arcs(graphADT graph)
{
    return (graph->arcs);
}

setADT ArcsFrom(nodeADT node)
{
    setADT result;
    arcADT arc;

    result = NewPtrSet(PtrCmpFn);
    foreach (arc in node->graph->arcs) {
        if (arc->from == node) AddPtrElement(result, arc);
    }
    return (result);
}

setADT ArcsTo(nodeADT node)
{
    setADT result;
    arcADT arc;

    result = NewPtrSet(PtrCmpFn);
    foreach (arc in node->graph->arcs) {
        if (arc->to == node) AddPtrElement(result, arc);
    }
    return (result);
}

nodeADT StartOfArc(arcADT arc)
{
    return (arc->from);
}

nodeADT EndOfArc(arcADT arc)
{
    return (arc->to);
}

graphADT GetGraph(nodeADT node)
{
    return (node->graph);
}

void SetVisitedFlag(nodeADT node)
{
    node->visited = TRUE;
}

void ClearVisitedFlag(nodeADT node)
{
    node->visited = FALSE;
}

bool HasBeenVisited(nodeADT node)
{
    return (node->visited);
}

void SetGraphData(graphADT graph, void *data)
{
    graph->data = data;
}

void *GetGraphData(graphADT graph)
{
    return (graph->data);
}

void SetNodeData(nodeADT node, void *data)
{
    node->data = data;
}

void *GetNodeData(nodeADT node)
{
    return (node->data);
}

void SetArcData(arcADT arc, void *data)
{
    arc->data = data;
}

void *GetArcData(arcADT arc)
{
    return (arc->data);
}
