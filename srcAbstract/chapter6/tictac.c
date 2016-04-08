/*
 * File: tictac.c
 * --------------
 * This program plays a game of tic-tac-toe with the user.
 * The program is designed to emphasize the separation between
 * those aspects of the code that are common to all games and
 * those that are specific to tic-tac-toe.
 */

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

/*
 * Constants: WinningPosition, NeutralPosition, LosingPosition
 * -----------------------------------------------------------
 * These constants define a rating system for game positions.
 * A rating is an integer centered at 0 as the neutral score:
 * ratings greater than 0 are good for the cuurent player,
 * ratings less than 0 are good for the opponent.  The
 * constants WinningPosition and LosingPosition are opposite
 * in value and indicate a position that is a forced win or
 * loss, respectively.  In a game in which the analysis is
 * complete, no intermediate values ever arise.  If the full
 * tree is too large to analyze, the EvaluatePosition function
 * returns integers that fall between the two extremes.
 */

#define WinningPosition  1000
#define NeutralPosition  0
#define LosingPosition   (-WinningPosition)

/*
 * Type: playerT
 * -------------
 * This type is used to distinguish the human and computer
 * players and keep track of who has the current turn.
 */

typedef enum { Human, Computer } playerT;

/*
 * Type: moveT
 * -----------
 * For any particular game, the moveT type must keep track of the
 * information necessary to describe a move.  For tic-tac-toe,
 * a moveT is simply an integer identifying the number of one of
 * the nine squares.
 */

typedef int moveT;

/*
 * Type: stateT
 * ------------
 * For any game, the stateT structure records the current state
 * of the game.  For tic-tac-toe, the main component of the
 * state record is the board, which is an array of characters
 * using 'X' for the first player, 'O' for the second, and ' '
 * for empty squares.  Although the board array is logically
 * a two-dimensional structure, it is stored as a linear array
 * so that its indices match the numbers used by the human
 * player to refer to the squares, as follows:
 *
 *        1 | 2 | 3
 *       ---+---+---
 *        4 | 5 | 6
 *       ---+---+---
 *        7 | 8 | 9
 *
 * Note that element 0 is not used, which requires allocation
 * of an extra element.
 *
 * In addition to the board array, the code stores a playerT
 * value to indicate whose turn it is.  In this example, the
 * stateT structure also contains the total number of moves
 * so that functions can check this entry without counting
 * the number of occupied squares.
 */

typedef struct {
    char board[(3 * 3) + 1];
    playerT whoseTurn;
    int turnsTaken;
} *stateT;

/*
 * Constant: MaxMoves
 * ------------------
 * This constant indicates the maximum number of legal moves
 * available on a turn and is used to allocate array space for
 * the legal move list.  This constant will change according
 * to the specifics of the game.  For tic-tac-toe, there are
 * never more than nine possible moves.
 */

#define MaxMoves 9

/*
 * Constant: MaxDepth
 * ------------------
 * This constant indicates the maximum depth to which the
 * recursive search for the best move is allowed to proceed.
 * The use of a very large number for this constant ensures
 * that the analysis is carried out to the end of the game.
 */

#define MaxDepth 10000

/*
 * Constant: FirstPlayer
 * ---------------------
 * This constant indicates whether the human or the computer
 * player goes first and should be one of the enumerated
 * constants: Human or Computer.
 */

#define FirstPlayer Computer

/*
 * Private variable: winningLines
 * ------------------------------
 * This two-dimensional array contains the index numbers of
 * the cells in each of the winning combinations.  Although
 * it is easy for the program to compute these values as it
 * runs, storing them in advance speeds up the execution.
 */

static int winningLines[][3] = {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 9 },
    { 1, 4, 7 },
    { 2, 5, 8 },
    { 3, 6, 9 },
    { 1, 5, 9 },
    { 3, 5, 7 }
};
static int nWinningLines = sizeof winningLines
                           / sizeof winningLines[0];

/* Private function prototypes */

static moveT FindBestMove(stateT state, int depth, int *pRating);
static int EvaluatePosition(stateT state, int depth);
static stateT NewGame(void);
static void DisplayGame(stateT state);
static void DisplayMove(moveT move);
static void GiveInstructions(void);
static char PlayerMark(playerT player);
static moveT GetUserMove(stateT state);
static bool MoveIsLegal(moveT move, stateT state);
static moveT ChooseComputerMove(stateT state);
static int GenerateMoveList(stateT state, moveT moveArray[]);
static void MakeMove(stateT state, moveT move);
static void RetractMove(stateT state, moveT move);
static void AnnounceResult(stateT state);
static bool GameIsOver(stateT state);
static int EvaluateStaticPosition(stateT state);
static bool CheckForWin(stateT state, playerT player);
static playerT WhoseTurn(stateT state);
static playerT Opponent(playerT player);

/*
 * Main program
 * ------------
 * The main program, along with the functions FindBestMove and
 * EvaluatePosition, are general in their design and can be
 * used with most two-player games.  The specific details of
 * tic-tac-toe do not appear in these functions and are instead
 * encapsulated in the stateT and moveT data structures and a
 * a variety of subsidiary functions.
 */

main()
{
    stateT state;
    moveT move;

    GiveInstructions();
    state = NewGame();
    while (!GameIsOver(state)) {
        DisplayGame(state);
        switch (WhoseTurn(state)) {
          case Human:
            move = GetUserMove(state);
            break;
          case Computer:
            move = ChooseComputerMove(state);
            DisplayMove(move);
            break;
        }
        MakeMove(state, move);
    }
    AnnounceResult(state);
}

/*
 * Function: FindBestMove
 * Usage: move = FindBestMove(state, depth, pRating);
 * --------------------------------------------------
 * This function finds the best move for the current player, given
 * the specified state of the game.  The depth parameter and the
 * constant MaxDepth are used to limit the depth of the search
 * for games that are too difficult to analyze in full detail.
 * The function returns the best move and stores its rating in
 * the integer variable to which pRating points.
 */

static moveT FindBestMove(stateT state, int depth, int *pRating)
{
    moveT moveArray[MaxMoves], move, bestMove;
    int i, nMoves, rating, minRating;

    nMoves = GenerateMoveList(state, moveArray);
    if (nMoves == 0) Error("No moves available");
    minRating = WinningPosition + 1;
    for (i = 0; i < nMoves && minRating != LosingPosition; i++) {
        move = moveArray[i];
        MakeMove(state, move);
        rating = EvaluatePosition(state, depth + 1);
        if (rating < minRating) {
            bestMove = move;
            minRating = rating;
        }
        RetractMove(state, move);
    }
    *pRating = -minRating;
    return (bestMove);
}

/*
 * Function: EvaluatePosition
 * Usage: rating = EvaluatePosition(state, depth);
 * -----------------------------------------------
 * This function evaluates a position by finding the rating of
 * the best move in that position.  The depth parameter and the
 * constant MaxDepth are used to limit the depth of the search.
 */

static int EvaluatePosition(stateT state, int depth)
{
    int rating;

    if (GameIsOver(state) || depth >= MaxDepth) {
        return (EvaluateStaticPosition(state));
    }
    (void) FindBestMove(state, depth, &rating);
    return (rating);
}

/*
 * Function: NewGame
 * Usage: state = NewGame();
 * -------------------------
 * This function starts a new game and returns a stateT that
 * has been initialized to the defined starting configuration.
 */

static stateT NewGame(void)
{
    stateT state;
    int i;

    state = New(stateT);
    for (i = 1; i <= 9; i++) {
        state->board[i] = ' ';
    }
    state->whoseTurn = FirstPlayer;
    state->turnsTaken = 0;
    return (state);
}

/*
 * Function: DisplayGame
 * Usage: DisplayGame(state);
 * --------------------------
 * This function displays the current state of the game.
 */

static void DisplayGame(stateT state)
{
    int row, col;

    if (GameIsOver(state)) {
        printf("\nThe final position looks like this:\n\n");
    } else {
        printf("\nThe game now looks like this:\n\n");
    }
    for (row = 0; row < 3; row++) {
        if (row != 0) printf("---+---+---\n");
        for (col = 0; col < 3; col++) {
            if (col != 0) printf("|");
            printf(" %c ", state->board[row * 3 + col + 1]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
 * Function: DisplayMove
 * Usage: DisplayMove(move);
 * -------------------------
 * This function displays the computer's move.
 */

static void DisplayMove(moveT move)
{
    printf("I'll move to square %d.\n", move);
}

/*
 * Function: GiveInstructions
 * Usage: GiveInstructions();
 * --------------------------
 * This function gives the player instructions about how to
 * play the game.
 */

static void GiveInstructions(void)
{
    printf("Welcome to tic-tac-toe.  The object of the game\n");
    printf("is to line up three symbols in a row, vertically,\n");
    printf("horizontally, or diagonally.  You'll be %c and\n",
           PlayerMark(Human));
    printf("I'll be %c.\n", PlayerMark(Computer));
}

/*
 * Function: PlayerMark
 * Usage: mark = PlayerMark(player);
 * ---------------------------------
 * This function returns the mark used on the board to indicate
 * the specified player.  By convention, the first player is
 * always X, so the mark used for each player depends on who
 * goes first.
 */

static char PlayerMark(playerT player)
{
    if (player == FirstPlayer) {
        return ('X');
    } else {
        return ('O');
    }
}

/*
 * Function: GetUserMove
 * Usage: move = GetUserMove(state);
 * ---------------------------------
 * This function allows the user to enter a move and returns the
 * number of the chosen square.  If the user specifies an illegal
 * move, this function gives the user the opportunity to enter
 * a legal one.
 */

static moveT GetUserMove(stateT state)
{
    moveT move;

    printf("Your move.\n");
    while (TRUE) {
        printf("What square? ");
        move = GetInteger();
        if (MoveIsLegal(move, state)) break;
        printf("That move is illegal.  Try again.\n");
    }
    return (move);
}

/*
 * Function: MoveIsLegal
 * Usage: if (MoveIsLegal(move, state)) . . .
 * ------------------------------------------
 * This function returns TRUE if the specified move is legal
 * in the current state.
 */

static bool MoveIsLegal(moveT move, stateT state)
{
    return (move >= 1 && move <= 9 && state->board[move] == ' ');
}

/*
 * Function: ChooseComputerMove
 * Usage: move = ChooseComputerMove(state);
 * ----------------------------------------
 * This function chooses the computer's move and is primarily
 * a wrapper for FindBestMove.  This function also makes it
 * possible to display any game-specific messages that need
 * to appear at the beginning of the computer's turn.  The
 * rating value returned by FindBestMove is simply discarded.
 */

static moveT ChooseComputerMove(stateT state)
{
    int rating;

    printf("My move.\n");
    return (FindBestMove(state, 0, &rating));
}

/*
 * Function: GenerateMoveList
 * Usage: n = GenerateMoveList(state, moveArray);
 * ----------------------------------------------
 * This function generates a list of the legal moves available in
 * the specified state.  The list of moves is returned in the
 * array moveArray, which must be allocated by the client.  The
 * function returns the number of legal moves.
 */

static int GenerateMoveList(stateT state, moveT moveArray[])
{
    int i, nMoves;

    nMoves = 0;
    for (i = 1; i <= 9; i++) {
        if (state->board[i] == ' ') {
            moveArray[nMoves++] = (moveT) i;
        }
    }
    return (nMoves);
}

/*
 * Function: MakeMove
 * Usage: MakeMove(state, move);
 * -----------------------------
 * This function changes the state of the game by making the
 * indicated move.
 */

static void MakeMove(stateT state, moveT move)
{
    state->board[move] = PlayerMark(state->whoseTurn);
    state->whoseTurn = Opponent(state->whoseTurn);
    state->turnsTaken++;
}

/*
 * Function: RetractMove
 * Usage: RetractMove(state, move);
 * --------------------------------
 * This function changes the state of the game by "unmaking" the
 * indicated move.
 */

static void RetractMove(stateT state, moveT move)
{
    state->board[move] = ' ';
    state->whoseTurn = Opponent(state->whoseTurn);
    state->turnsTaken--;
}

/*
 * Function: AnnounceResult
 * Usage: AnnounceResult(state);
 * -----------------------------
 * This function announces the result of the game.
 */

static void AnnounceResult(stateT state)
{
    DisplayGame(state);
    if (CheckForWin(state, Human)) {
        printf("You win\n");
    } else if (CheckForWin(state, Computer)) {
        printf("I win\n");
    } else {
        printf("Cat's game\n");
    }
}

/*
 * Function: GameIsOver
 * Usage: if (GameIsOver(state)) . . .
 * -----------------------------------
 * This function returns TRUE if the game is complete.
 */

static bool GameIsOver(stateT state)
{
    return (state->turnsTaken == 9
            || CheckForWin(state, state->whoseTurn)
            || CheckForWin(state, Opponent(state->whoseTurn)));
}

/*
 * Function: EvaluateStaticPosition
 * Usage: rating = EvaluateStaticPosition(state);
 * ----------------------------------------------
 * This function gives the rating of a position without looking
 * ahead any further in the game tree.  Although this function
 * duplicates much of the computation of GameIsOver and therefore
 * introduces some runtime inefficiency, it makes the algorithm
 * somewhat easier to follow.
 */

static int EvaluateStaticPosition(stateT state)
{
    if (CheckForWin(state, state->whoseTurn)) {
        return (WinningPosition);
    }
    if (CheckForWin(state, Opponent(state->whoseTurn))) {
        return (LosingPosition);
    }
    return (NeutralPosition);
}

/*
 * Function: CheckForWin
 * Usage: if (CheckForWin(state, player)) . . .
 * --------------------------------------------
 * This function returns TRUE if the specified player has won
 * the game.  The check on turnsTaken increases efficiency,
 * because neither player can win the game until the fifth move.
 */

static bool CheckForWin(stateT state, playerT player)
{
    int i;
    char mark;

    if (state->turnsTaken < 5) return (FALSE);
    mark = PlayerMark(player);
    for (i = 0; i < nWinningLines; i++) {
        if (mark == state->board[winningLines[i][0]]
               && mark == state->board[winningLines[i][1]]
               && mark == state->board[winningLines[i][2]]) {
            return (TRUE);
        }
    }
    return (FALSE);
}

/*
 * Function: WhoseTurn
 * Usage: player = WhoseTurn(state);
 * ---------------------------------
 * This function returns whose turn it is, given the current
 * state of the game.
 */

static playerT WhoseTurn(stateT state)
{
    return (state->whoseTurn);
}

/*
 * Function: Opponent
 * Usage: opp = Opponent(player);
 * ------------------------------
 * This function returns the playerT value corresponding to the
 * opponent of the specified player.
 */

static playerT Opponent(playerT player)
{
    switch (player) {
      case Human:    return (Computer);
      case Computer: return (Human);
    }
}
