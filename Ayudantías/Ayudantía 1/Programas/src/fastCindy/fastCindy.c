#include "fastCindy.h"

int planWithBorders(char *board, int holePosition, int boardSize, int ballsToTheLeft, int ballsToTheRight)
{
  if(ballsToTheRight == 0)
  {
    if(board[holePosition - 2] == 'R' && board[holePosition - 1] == 'L')
    {
      return holePosition - 2;
    }

    if(board[holePosition - 1] == 'R')
    {
      return holePosition - 1;
    }
  }

  if(ballsToTheLeft == 0)
  {
    if(board[holePosition + 2] == 'L' && board[holePosition + 1] == 'R')
    {
      return holePosition + 2;
    }

    if(board[holePosition + 1] == 'L')
    {
      return holePosition + 1;
    }
  }

  if(ballsToTheRight == 1)
  {
    if(board[holePosition - 2] == 'R' && board[holePosition - 1] == 'L')
    {
      return holePosition - 2;
    }

    if(board[holePosition + 1] == 'L')
    {
      return holePosition + 1;
    }

    if(board[holePosition - 1] == 'R')
    {
      return holePosition - 1;
    }
  }

  if(ballsToTheLeft == 1)
  {
    if(board[holePosition + 2] == 'L' && board[holePosition + 1] == 'R')
    {
      return holePosition + 2;
    }

    if(board[holePosition - 1] == 'R')
    {
      return holePosition - 1;
    }

    if(board[holePosition + 1] == 'L')
    {
      return holePosition + 1;
    }
  }

  // Si todas las predicciones fallan...
  return holePosition;

}

int planNextMove(char *board, int holePosition, int boardSize)
{
  int ballsToTheLeft = holePosition;
  int ballsToTheRight = boardSize - 1 - holePosition;

  // Border (literal border) Cases
  if(ballsToTheLeft < 2 || ballsToTheRight < 2)
  {
    //printf("%s\n", ">> Planning with borders");
    return planWithBorders(board, holePosition, boardSize, ballsToTheLeft, ballsToTheRight);
  }

  // Rule of Jump

  // Wins the side that can perform a double jump.
  // If there are two candidates, the LEFT colored ball wins.

  if(board[holePosition + 1] == 'R' && board[holePosition + 2] == 'L')
    return holePosition + 2;
  if(board[holePosition - 1] == 'L' && board[holePosition - 2] == 'R')
    return holePosition - 2;

  // Unique movement or backtrack

  // If there is only one movement available, that movement is
  // chosen. If there is none, then the HOLEPOSITION is returned
  // to signal that we must BACKTRACK.

  bool hasSmallL = board[holePosition + 1] == 'L';
  bool hasSmallR = board[holePosition - 1] == 'R';

  if(hasSmallL && !hasSmallR)
  {
    return holePosition + 1;
  }
  if (hasSmallR && !hasSmallL)
  {
    return holePosition - 1;
  }
  if(!hasSmallL && !hasSmallR)
  {
    return holePosition;
  }

  // Rule of Tiebreak

  // Wins the single step ball with the longest chain
  // of its color behind it, EXCEPT if it's cut by
  // a wall, in which case the first chain to be cut wins.
  // If every condition is a tie, then RIGHT colored ball wins.

  for(int i = 2;;i++)
  {
    if(holePosition - i < 0)
      return holePosition - 1;

    if(holePosition + i == boardSize)
      return holePosition + 1;

    bool thereIsR = board[holePosition - i] == 'R';
    bool thereIsL = board[holePosition + i] == 'L';

    if(thereIsR && !thereIsL)
      return holePosition - 1;
    if(thereIsL && !thereIsR)
      return holePosition + 1;
    if(!thereIsR && !thereIsL)
      return holePosition - 1;
  }
}

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

/** Lo podemos hacer correr como loop porque nunca hará backtrack */
bool solveCindyLoop(char *board, int holePosition, int boardSize, int *numSteps)
{
  while(true)
  {
    printf("%s\n", board);

    if(isSolved(board, boardSize))
      return true;

    *numSteps += 1;

    int nextMove = planNextMove(board, holePosition, boardSize);

    if(nextMove == holePosition)
      return false;

    char auxP = board[holePosition];
    char auxN = board[nextMove];

    board[holePosition] = auxN;
    board[nextMove] = auxP;

    holePosition = nextMove;
  }
}

bool solveCindy(char *board, int holePosition, int boardSize, int *numSteps)
{
  printf("%s\n", board);

  if(isSolved(board, boardSize))
    return true;

  *numSteps += 1;

  int nextMove = planNextMove(board, holePosition, boardSize);

  if(nextMove == holePosition)
    return false;

  int auxP = board[holePosition];
  int auxN = board[nextMove];

  board[holePosition] = auxN;
  board[nextMove] = auxP;

  // Sabemos que nunca haremos backtrack, por lo que no es necesario
  // revertir los pasos realizados.
  if(!solveCindy(board, nextMove, boardSize, numSteps))
  {
    return false;
  }
  else
  {
    return true;
  }

}

/** Llámese con un número para saber la cantidad de bolas de cada lado*/
int main(int argc, char const *argv[]) {

  // Error de cantidad de parámetros
  if(argc > 2)
    return 1;

  // Por default usamos 3 bolas.
  int numBolasPorLado = 3;

  // Usamos más si existe el input.
  if(argc >= 2)
    numBolasPorLado = atoi(argv[1]);

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
  solveCindyLoop(board, numBolasPorLado, numBalls + 1, &numSteps);
  //solveCindy(board, numBolasPorLado, numBalls + 1, &numSteps);

  // Le comunicamos al watcher que terminamos
  printf("%s\n", "END");

  printf("Number of steps: %i\n", numSteps);

  free(board);

  return 0;
}
