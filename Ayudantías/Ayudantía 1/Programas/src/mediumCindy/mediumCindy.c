#include "mediumCindy.h"

bool isSolved(char *board, int boardSize)
{
  int ballsPerSide = (boardSize - 1) / 2;

  for(int i = 0; i < ballsPerSide; i++)
  {
    if(board[i] == 'R' || board[i + ballsPerSide + 1] == 'L')
      return false;
  }

  if(board[ballsPerSide] == 'R' || board[ballsPerSide] == 'L')
    return false;

  return true;
}

bool canMove(char *board, int holePosition, int boardSize, int candidateMove)
{
  if(candidateMove < 0 || candidateMove >= boardSize)
    return false;

  bool goesRight = board[candidateMove] == 'R';
  bool canReallyMove;

  if(candidateMove < holePosition)
  {
    canReallyMove = board[candidateMove + 1] == 'L' || candidateMove == holePosition - 1;

    return goesRight && canReallyMove;
  }
  else
  {
    canReallyMove = board[candidateMove - 1] == 'R' || candidateMove == holePosition + 1;

    return !goesRight && canReallyMove;
  }
}

int getMoveByIndex(int idx, int holePosition)
{
  // There are 4 different moves:

  // Move from (holePosition - 2) to holePosition
  // Move from (hP - 1) to holePosition
  // And the 2 symmetric ones for those.

  // But we want to try and jump FIRST. That's our strategy.
  // Therefore we want to put jumping over other balls
  // at the top of our order.

  // Then, we have the following table of moves from an index:

  switch (idx)
  {
    case 0:
      return holePosition - 2;
      break;
    case 1:
      return holePosition + 2;
      break;
    case 2:
      return holePosition - 1;
      break;
    case 3:
      return holePosition + 1;
      break;
    default:
      printf("%s%i\n", "The move index is wrong. Index was ", idx);
      return -1;
      break;
  }
}

int levelOfDepth = 0;

// I recommend seeing the recursive form first.
// This is exactly the same algorithm, but in a loop, using
// a stack instead of recursion, for efficiency.
bool solveCindyLoop(char *board, int holePosition, int boardSize, int *numSteps)
{
  Stack *indexStack = stack_init();
  Stack *holeStack = stack_init();

  // The current move index
  int cuMoveIndex = 0;//, cuHolePosition = holePosition;

  bool deadEnd = false;

  do {

    if(debug)
    {
      for(int i = 0; i < levelOfDepth; i++)
          printf("%s", "| ");
    }
    printf("%s\n", board);

    if(isSolved(board, boardSize))
    {
      while(!stack_is_empty(indexStack))
        stack_pop(indexStack);

      stack_destroy(indexStack);

      while(!stack_is_empty(holeStack))
          stack_pop(holeStack);

        stack_destroy(holeStack);

      return true;
    }

    int candidateMove = getMoveByIndex(cuMoveIndex, holePosition);

    while(!canMove(board, holePosition, boardSize, candidateMove))
    {
      cuMoveIndex += 1;

      if(cuMoveIndex > 3)
      {
        deadEnd = true;
        break;
      }
      candidateMove = getMoveByIndex(cuMoveIndex, holePosition);
    }

    if(!deadEnd)
    {
      board[holePosition] = board[candidateMove];
      board[candidateMove] = '.';

      stack_push(indexStack, (void*) cuMoveIndex);
      stack_push(holeStack, (void*) holePosition);

      levelOfDepth += 1;

      cuMoveIndex = 0;
      holePosition = candidateMove;

      *numSteps += 1;
    }
    else
    {
      do {
        cuMoveIndex = (int)stack_pop(indexStack);
        holePosition = (int)stack_pop(holeStack);

        levelOfDepth -= 1;

        int thatMove =
        getMoveByIndex(cuMoveIndex, holePosition);

        board[thatMove] = board[holePosition];
        board[holePosition] = '.';

      } while(cuMoveIndex == 3 && !stack_is_empty(indexStack));

      if(cuMoveIndex < 3)
      {
        deadEnd = false;
        cuMoveIndex += 1;
      }
    }

  } while(!deadEnd);

  stack_destroy(indexStack);
  stack_destroy(holeStack);

  return false;
}

bool solveCindy(char *board, int holePosition, int boardSize, int *numSteps)
{
  levelOfDepth++;

  if(debug)
  {
    for(int i = 0; i < levelOfDepth; i++)
      printf("%s", "| ");
  }

  printf("%s\n", board);

  if(isSolved(board, boardSize))
    return true;

  *numSteps += 1;

  for(int i = 0; i < 4; i++)
  {
    int candidateMove = getMoveByIndex(i, holePosition);

    if(canMove(board, holePosition, boardSize, candidateMove))
    {
      board[holePosition] = board[candidateMove];
      board[candidateMove] = '.';

      if(solveCindy(board, candidateMove, boardSize, numSteps))
        return true;

      board[candidateMove] = board[holePosition];
      board[holePosition] = '.';

      levelOfDepth--;

      if(debug)
      {
        for(int i = 0; i < levelOfDepth; i++)
          printf("%s", "| ");
      }

      printf("%s\n", board);
    }
  }

  return false;
}

/** Llámese con un número para saber la cantidad de bolas de cada lado*/
int main(int argc, char const *argv[]) {

  // Error de cantidad de parámetros
  if(argc > 3)
    return 1;

  // Por default usamos 3 bolas.
  int numBolasPorLado = 3;

  // Usamos más si existe el input.
  if(argc >= 2)
    numBolasPorLado = atoi(argv[1]);

  // Opcional: usar un loop.
  bool usarRecursion = true;

  if(argc >= 3)
  {
    usarRecursion = strcmp("rec", argv[2]) == 0;
    usarRecursion = usarRecursion || strcmp("loop", argv[2]) != 0;
  }

  int numBalls = numBolasPorLado * 2;

  char *board = malloc(sizeof(char) * (numBalls + 2));

  for(int i = 0; i < numBolasPorLado; i++)
  {
    board[i] = 'R';
    board[i + numBolasPorLado + 1] = 'L';
  }

  board[numBolasPorLado] = '.';

  // Char de término del string
  board[numBalls + 1] = (char)0;

  // Primer print, para comunicarle al watcher el tamaño del tablero
  printf("%i\n", numBalls + 1);

  // Para recordar cuántos pasos da el algoritmo hasta encontrar una solución
  int numSteps = 0;

  // Cindy se encarga de imprimir el resto de los estados.
  if(usarRecursion)
    solveCindy(board, numBolasPorLado, numBalls + 1, &numSteps);
  else
    solveCindyLoop(board, numBolasPorLado, numBalls + 1, &numSteps);


  // Le comunicamos al watcher que terminamos
  printf("%s\n", "END");

  printf("%s", "Solved using ");

  if(usarRecursion)
    printf("%s\n", "recursion");
  else
    printf("%s\n", "a loop and a stack");

  printf("Number of steps: %i\n", numSteps);

  free(board);

  return 0;
}
