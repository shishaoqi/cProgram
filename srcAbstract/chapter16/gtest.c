/*
 * File: gtest.c
 * -------------
 * This program tests the graph abstraction.  The user types in
 * command lines, which must to be in one of the following forms:
 *
 *   node name        Create a new node
 *   connect n1 n2    Add an arc between n1 and n2
 *   list             List the contents of the graph
 *   dfs start        Perform a depth-first search from start
 *   bfs start        Perform a breadth-first search from start
 *   source "file"    Read commands from a file
 *   help             Generate a list of commands
 *   quit             Exit from the program
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "symtab.h"
#include "iterator.h"
#include "foreach.h"
#include "scanadt.h"
#include "set.h"
#include "cmpfn.h"
#include "graph.h"
#include "cmdscan.h"

/* Private function prototypes */

static void InitCommandTable(cmdScannerADT cs);
static void NodeCmdFn(cmdScannerADT cs);
static void ConnectCmdFn(cmdScannerADT cs);
static void ListCmdFn(cmdScannerADT cs);
static void DFSCmdFn(cmdScannerADT cs);
static void BFSCmdFn(cmdScannerADT cs);
static void SourceCmdFn(cmdScannerADT cs);
static void HelpCmdFn(cmdScannerADT cs);
static void DepthFirstSearch(nodeADT start);
static void BreadthFirstSearch(nodeADT start);
static nodeADT ReadNamedNode(cmdScannerADT cs);
static void UnmarkAllNodes(graphADT graph);
static void Visit(nodeADT node);

/* Main program */

main()
{
    cmdScannerADT cs;
    scannerADT scanner;
    graphADT graph;

    printf("Graph test program\n");
    cs = NewCommandScanner();
    scanner = GetTokenScanner(cs);
    SetScannerStringOption(scanner, ScanQuotesAsStrings);
    graph = NewGraph();
    SetGraphData(graph, NewSymbolTable());
    SetCommandData(cs, graph);
    InitCommandTable(cs);
    CommandLoop(cs, "G> ");
    FreeCommandScanner(cs);
}

static void InitCommandTable(cmdScannerADT cs)
{
    DefineCommand(cs, "node",    NodeCmdFn);
    DefineCommand(cs, "connect", ConnectCmdFn);
    DefineCommand(cs, "list",    ListCmdFn);
    DefineCommand(cs, "dfs",     DFSCmdFn);
    DefineCommand(cs, "bfs",     BFSCmdFn);
    DefineCommand(cs, "source",  SourceCmdFn);
    DefineCommand(cs, "help",    HelpCmdFn);
    DefineCommand(cs, "quit",    QuitCmdFn);
}

static void NodeCmdFn(cmdScannerADT cs)
{
    scannerADT scanner;
    symtabADT nodeTable;
    graphADT graph;
    nodeADT node;
    string label;

    scanner = GetTokenScanner(cs);
    graph = GetCommandData(cs);
    nodeTable = GetGraphData(graph);
    label = ReadToken(scanner);
    if (Lookup(nodeTable, label) != UNDEFINED) {
        printf("That node already exists\n");
    } else {
        node = NewNode(graph);
        SetNodeData(node, label);
        Enter(nodeTable, label, node);
    }
}

static void ConnectCmdFn(cmdScannerADT cs)
{
    nodeADT n1, n2;

    n1 = ReadNamedNode(cs);
    n2 = ReadNamedNode(cs);
    Connect(n1, n2);
}

static void ListCmdFn(cmdScannerADT cs)
{
    scannerADT scanner;
    symtabADT nodeTable;
    graphADT graph;
    nodeADT node, target;

    graph = GetCommandData(cs);
    foreach (node in Nodes(graph)) {
        printf("%s:", (string) GetNodeData(node));
        foreach (target in ConnectedNodes(node)) {
            printf(" %s", (string) GetNodeData(target));
        }
        printf("\n");
    }
}

static void DFSCmdFn(cmdScannerADT cs)
{
    UnmarkAllNodes((graphADT) GetCommandData(cs));
    DepthFirstSearch(ReadNamedNode(cs));
    printf("\n");
}

static void BFSCmdFn(cmdScannerADT cs)
{
    UnmarkAllNodes((graphADT) GetCommandData(cs));
    BreadthFirstSearch(ReadNamedNode(cs));
    printf("\n");
}

static void SourceCmdFn(cmdScannerADT cs)
{
    scannerADT scanner;
    string token, filename, line;
    int len;
    FILE *infile;

    scanner = GetTokenScanner(cs);
    token = ReadToken(scanner);
    len = StringLength(token);
    if (token[0] != '"' || token[len - 1] != '"') {
        Error("Source: filename is not enclosed in quotes");
    }
    filename = SubString(token, 1, len - 2);
    infile = fopen(filename, "r");
    if (infile == NULL) Error("Source: cannot open file");
    while ((line = ReadLine(infile)) != NULL) {
        printf("-> %s\n", line);
        if (!ExecuteCommand(cs, line)) {
            Error("Source: Illegal command (%s)", line);
        }
    }
    fclose(infile);
}

static void HelpCmdFn(cmdScannerADT cs)
{
    printf("Legal commands:\n");
    printf("  node name        Create a new node\n");
    printf("  connect n1 n2    Add an arc between n1 and n2\n");
    printf("  list             List the contents of the graph\n");
    printf("  dfs start        Perform a depth-first search\n");
    printf("  bfs start        Perform a breadth-first search\n");
    printf("  source \"file\"    Read commands from a file\n");
    printf("  help             Generate a list of commands\n");
    printf("  quit             Exit from the program\n");
}

/*
 * Function: DepthFirstSearch
 * Usage: DepthFirstSearch(start);
 * -------------------------------
 * This function visits each node in the graph containing start,
 * using a depth-first traversal beginning at the start node.
 */

static void DepthFirstSearch(nodeADT start)
{
    nodeADT node;

    if (HasBeenVisited(start)) return;
    Visit(start);
    foreach (node in ConnectedNodes(start)) {
        DepthFirstSearch(node);
    }
}

/*
 * Function: BreadthFirstSearch
 * Usage: BreadthFirstSearch(start);
 * -------------------------------
 * This function visits each node in the graph containing start,
 * using a breadth-first traversal beginning at the start node.
 */

static void BreadthFirstSearch(nodeADT start)
{
    setADT frontier, oldFrontier;
    nodeADT node, target;

    frontier = NewPtrSet(PtrCmpFn);
    AddPtrElement(frontier, start);
    while (NElements(frontier) > 0) {
        oldFrontier = frontier;
        frontier = NewPtrSet(PtrCmpFn);
        foreach (node in oldFrontier) {
            if (!HasBeenVisited(node)) {
                Visit(node);
                foreach (target in ConnectedNodes(node)) {
                    AddPtrElement(frontier, target);
                }
            }
        }
    }
}

/* Utility functions */

/*
 * Function: ReadNamedNode
 * Usage: node = ReadNamedNode(cs);
 * --------------------------------
 * This function reads the next token from the command scanner
 * and interprets it as the name of a node.
 */

static nodeADT ReadNamedNode(cmdScannerADT cs)
{
    scannerADT scanner;
    symtabADT nodeTable;
    graphADT graph;
    nodeADT node;
    string label;

    scanner = GetTokenScanner(cs);
    label = ReadToken(scanner);
    if (StringLength(label) == 0) {
        Error("%s: Node name is missing", GetCommandName(cs));
    }
    graph = GetCommandData(cs);
    nodeTable = GetGraphData(graph);
    node = Lookup(nodeTable, label);
    if (node == NULL) {
        Error("%s: No node named %s", GetCommandName(cs), label);
    }
    return (node);
}

static void UnmarkAllNodes(graphADT graph)
{
    nodeADT node;

    foreach (node in Nodes(graph)) {
        ClearVisitedFlag(node);
    }
}

static void Visit(nodeADT node)
{
    printf(" %s", (string) GetNodeData(node));
    SetVisitedFlag(node);
}
