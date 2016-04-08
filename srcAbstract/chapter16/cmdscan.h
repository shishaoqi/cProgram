/*
 * File: cmdscan.h
 * ---------------
 * This interface provides a simple command-scanning interface
 * that is primarily used to construct test programs.  The
 * typical pattern of use requires the following steps:
 *
 * 1. Call NewCommandScanner to create an empty command scanner.
 * 2. Call DefineCommand to associate commands with functions.
 * 3. Call CommandLoop to execute a command loop.
 */

#ifndef _cmdscan_h
#define _cmdscan_h

#include "genlib.h"
#include "scanadt.h"

/*
 * Type: cmdScannerADT
 * -------------------
 * This abstract type represents the command scanner.
 */

typedef struct cmdScannerCDT *cmdScannerADT;

/*
 * Type: cmdFnT
 * ------------
 * This type represents the space of functions that can be used
 * as commands.  Each command takes the entire command scanner
 * as an argument, which gives the callback function access to
 * the various data fields associated with the scanner.
 */

typedef void (*cmdFnT)(cmdScannerADT cs);

/*
 * Functions: NewCommandScanner, FreeCommandScanner
 * Usage: cs = NewCommandScanner();
 *        FreeCommandScanner(cs);
 * ------------------------------------------------
 * These functions allocate and free command scanners.
 */

cmdScannerADT NewCommandScanner(void);
void FreeCommandScanner(cmdScannerADT cs);

/*
 * Function: DefineCommand
 * Usage: DefineCommand(cs, cmdName, cmdFn);
 * -----------------------------------------
 * This function adds an entry to the internal command table for
 * the command scanner that associates the command name with the
 * specified function.
 */

void DefineCommand(cmdScannerADT cs, string cmdName, cmdFnT cmdFn);

/*
 * Function: CommandLoop
 * Usage: CommandLoop(cs, prompt);
 * -------------------------------
 * This function executes a simple command loop that repeatedly
 *
 * 1. Prints the specified prompt
 * 2. Reads in a line from the user
 * 3. Checks to see if the first token on the line is a command
 * 4. Executes the function associated with that command
 *
 * If a command is undefined, CommandLoop displays a message
 * to that effect and allows the user to enter a new command.
 */

void CommandLoop(cmdScannerADT cs, string prompt);

/*
 * Function: ExecuteCommand
 * Usage: ok = ExecuteCommand(cs, line);
 * -------------------------------------
 * This function executes a command line as if it were entered
 * by the user.  The function returns TRUE if the command is
 * defined, and FALSE otherwise.
 */

bool ExecuteCommand(cmdScannerADT cs, string line);

/*
 * Functions: SetCommandData, GetCommandData
 * Usage: SetCommandData(cs, data);
 *        data = GetCommandData(cs);
 * ------------------------------------------
 * These functions make it possible for the client to associate
 * a data block with the command scanner, which makes it available
 * to the callback functions used for the individual commands.
 */

void SetCommandData(cmdScannerADT cs, void *data);
void *GetCommandData(cmdScannerADT cs);

/*
 * Function: ReadCommandToken(cs);
 * Usage: token = ReadCommandToken(cs);
 * ------------------------------------
 * This function returns the next token from the command line.  If
 * no more tokens exist, ReadCommandToken returns the empty string.
 */

string ReadCommandToken(cmdScannerADT cs);

/*
 * Functions: GetCommandLine, GetCommandName, GetTokenScanner
 * Usage: line = GetCommandLine(cs);
 *        name = GetCommandName(cs);
 *        scanner = GetTokenScanner(cs);
 * ----------------------------------------------------------
 * These functions return the current command line, the current
 * command name, and the token scanner used by the command
 * processor.  Providing these values to the client makes it
 * possible to perform more sophisticated command analysis than
 * is possible with ReadCommandToken alone.
 */

string GetCommandLine(cmdScannerADT cs);
string GetCommandName(cmdScannerADT cs);
scannerADT GetTokenScanner(cmdScannerADT cs);

/*
 * Function: QuitCmdFn
 * Usage: DefineCommand(cs, "quit", QuitCmdFn);
 * --------------------------------------------
 * This callback function is exported by the command scanner
 * interface so that clients can define a command that exits
 * from the command loop.
 */

void QuitCmdFn(cmdScannerADT cs);

#endif
