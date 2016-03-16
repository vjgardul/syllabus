#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <string.h>

#include "../common/stack.h"

/** For debugging output in console.*/
bool debug = false;

/** Checks if the state of the puzzle is a solved state.*/
bool isSolved(char *board, int boardSize);

/** Solves Cindy's Puzzle. It can call itself.*/
bool solveCindy(char *board, int holePosition, int boardSize, int *numSteps);

/** Solves Cindy's Puzzle using a loop and a stack.*/
bool solveCindyLoop(char *board, int holePosition, int boardSize, int *numSteps);

/** Tells if a candidate move is valid or not. */
bool canMove(char *board, int holePosition, int boardSize, int candidateMove);

/** Gives out possible moves (not always valid).
The ordering makes backtracking smarter.*/
int getMoveByIndex(int idx, int holePosition);
