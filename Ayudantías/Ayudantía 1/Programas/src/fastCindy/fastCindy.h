#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <string.h>

/** Solves the pruning decision when the Hole Position is near a border.*/
int planWithBorders(char *board, int holePosition, int boardSize, int ballsToTheLeft, int ballsToTheRight);

/** Pruning for Cindy's Puzzle.*/
int planNextMove(char *board, int holePosition, int boardSize);

/** Checks if the state of the puzzle is a solved state.*/
bool isSolved(char *board, int boardSize);

/** Solves Cindy's Puzzle. It can call itself.*/
bool solveCindy(char *board, int holePosition, int boardSize, int *numSteps);

/** Solves Cindy's Puzzle with a loop. Doesn't need a stack because the planning is optimum.*/
bool solveCindyLoop(char *board, int holePosition, int boardSize, int *numSteps);
