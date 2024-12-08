#include "c4lib.h"
#include <iostream>
using namespace std;

// Prototype internal helper functions
/**
 * @brief returns the smallest y coordinate that is BLANK in 
 *        column x or -1 if no location is BLANK
 */
int findYValue(BoardValue** board, int ydim, int x);


// ------------------------------------------------------------
// Global variabes

// converts integer player value: 0 or 1 to appropriate enum
enum BoardValue playerToValue[2] = {RED, YELLOW};

// ----------------------------------------------------------------------


// Helper function 
int findYValue(BoardValue** board, int ydim, int x)
{
  for (int i = ydim - 1; i >= 0; i--){
    // loop throuogh the values, from the bottom to up 

    if (board[ydim - i - 1][x] == BLANK){
      // find earliest value that is blank and return i 
      return ydim - i - 1; 
    }

  }

  return -1; 
  // if there is no such value, return -1 
}

BoardValue** allocateBoard(int ydim, int xdim)
{
  BoardValue** board = new BoardValue*[ydim]; 
  // allocate each element in the array to a row 

  for (int i = 0; i < ydim; i++){
    // loop through all ydim values 
    board[i] = new BoardValue[xdim]; 
    // assign corresponding ydim (row) to a column (xdim value)
  }

  for (int i = 0; i < ydim; i++){
    for (int j = 0; j < xdim; j++){
      board[i][j] = BLANK; 
      // loop through all elements on the board and assign them as blank
    }
  }

  return board; 
  // return the board once it is all allocated 
}


void deallocateBoard(BoardValue** board, int ydim)
{
  for (int i = 0; i < ydim; i++){
    // loop through all the rows 
    delete [] board[i]; 
    // delete each column 
  }

  delete[] board; 
  // delete all the rows 

}

void printBoard(BoardValue** board, int ydim, int xdim)
{
  const char* boardToString[] = { "\U000026ab", "\U0001F534", "\U0001F7E1" }; 
                                  //  "⚫",          "🔴",         "🟡"}
  for(int y = ydim-1; y >= 0; y--){
    for(int x = 0; x < xdim; x++) {
      cout << boardToString[(int)board[y][x]] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

bool getNextHumanInput(
  BoardValue** board, 
  int ydim, int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  cout << "Enter the location: " << endl; 
  cin >> *x; 
  // get location by cin statement 
  if ((*x >= 0) && (*x <= xdim-1)){
    if (findYValue(board, ydim, *x) == -1){
      return true; 
     // call the find yvalue function here and return true if it is full
    }
    *y = findYValue(board, ydim, *x); 
    // get the y value 
    board[*y][*x] = playerToValue[currentPlayer]; 
    return false; 
    // update the value and return false 
  }
  return true; 
  // this executes if the value is out of bounds 
}


bool hasWon(
  BoardValue** board,
  int ydim, int xdim,
  int sy, int sx,
  int currentPlayer)
{
  // const int NDIRS = 4;
  // const int xDirDelta[NDIRS] = { 0, +1, -1, +1};
  // const int yDirDelta[NDIRS] = {+1,  0, +1, +1};

  // get the value of the player 
  BoardValue playerval = playerToValue[currentPlayer];

  if (sy < 0 || sy >= ydim || sx < 0 || sx >= xdim){
    return false; 
  }
  // make sure the dimensions are appropriate 

  // check the vertical lines 
  for (int i = 0; i < xdim; i ++){
    for (int j = 0; j <= ydim - 4; j++){
      if ((board[j][i] == playerval) && (board[j+1][i] == playerval) && (board[j+2][i] == playerval) && (board[j+3][i] == playerval)){
        return true; 
      }
    }
  }
  // check the horizontal lines 
  for (int j = 0; j < ydim; j++){
    for (int i = 0; i <= xdim - 4; i++){
      if ((board[j][i] == playerval) && (board[j][i+1] == playerval) && (board[j][i+2] == playerval) && (board[j][i+3] == playerval)){
        return true; 
      }
    }
  }
  // check diagonal lines in one direction
  for (int j = 0; j <= ydim - 4; j++){
    for (int i = 0; i <= xdim - 4; i++){
      if ((board[j][i] == playerval) && (board[j+1][i+1] == playerval) && (board[j+2][i+2] == playerval) && (board[j+3][i+3] == playerval)){
        return true; 
      }
    }
  }
  // check diagonal lines in the other direction
  for (int j = 3; j < ydim; j++){
    for (int i = 0; i <= xdim - 4; i++){
      if ((board[j][i] == playerval) && (board[j-1][i+1] == playerval) && (board[j-2][i+2] == playerval) && (board[j-3][i+3] == playerval)){
        return true; 
      }
    }
  }
  // if no winner was found 
  return false; 
}

bool isDraw(
  BoardValue** board,
  int ydim, int xdim)
{
  bool isfull = true; 
  // variable to track if board is full 
  for (int i = 0; i < ydim; i++){
    for (int j = 0; j < xdim; j++){
      if (board[i][j] == BLANK){
        isfull = false; 
      } 
      // loop through to see if there is any blank spaces on the board 
    }
  }
  bool redwin = false; 
  bool yelwin = false; 
  redwin = hasWon(board, ydim, xdim, 0, 0, 0);
  yelwin = hasWon(board, ydim, xdim, 0, 0, 1);
  // variables to track if any player has won 
  if ((!redwin) && (!yelwin) && (isfull)){
    return true; 
    // conditions to see if any player won or if board is full 
  }
  else{
    return false; 
  }

}



bool getUserAIInput(
  BoardValue** board, 
  int ydim, int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  // Check if the current player can win


  // get the value of the player 
  BoardValue playerval = playerToValue[currentPlayer];
  BoardValue oppval = playerToValue[(currentPlayer + 1) % 2];
  // these variables have the player value and opponenet value 

  // check the vertical lines 
  for (int i = 0; i < xdim; i ++){
    for (int j = 0; j <= ydim - 4; j++){
      if ((board[j][i] == oppval) && (board[j+1][i] == oppval) && (board[j+2][i] == oppval) && (board[j+3][i] == BLANK)){
        board[j+3][i] = playerval; 
        *y = j+3;
        *x = i; 
        return false; 
        // reassign if it can be blocked vertically and update variables 
      }
    }
  }
  // check the horizontal lines 
  for (int j = 0; j < ydim; j++){
    for (int i = 0; i < xdim; i++){
      if ((board[j][i] == oppval) && (board[j][i+1] == oppval) && (board[j][i+2] == oppval) && (board[j][i+3] == BLANK)){
        board[j][i+3] = playerval; 
        *y = j;
        *x = i + 3;
        return false; 
        // reassign if it can be blocked horizontally and update variables 
      }
    }
  }
  // check diagonal lines in one direction
  for (int j = 0; j <= ydim - 4; j++){
    for (int i = 0; i <= xdim - 4; i++){
      if ((board[j][i] == oppval) && (board[j+1][i+1] == oppval) && (board[j+2][i+2] == oppval) && (board[j+3][i+3] == BLANK)){
        board[j+3][i+3] = playerval; 
        *y = j+3;
        *x = i+3;
        return false; 
        // reassign if it can be blocked diagonally and update variables 
      }
    }
  }
  // check diagonal lines in the other direction
  for (int j = 3; j < ydim; j++){
    for (int i = 0; i <= xdim - 4; i++){
      if ((board[j][i] == oppval) && (board[j-1][i+1] == oppval) && (board[j-2][i+2] == oppval) && (board[j-3][i+3] == BLANK)){
        board[j-3][i+3] = playerval; 
        *y = j - 3; 
        *x = i+3;
        return false; 
        // reassign if it can be blocked diagonally and update variables 
      }
    }
  }



  // Check if we need to block 
  //  We can greedily play the first blocking location since
  //  if they can win in multiple ways it does not matter which
  //  we choose.


  // Add any other logic for how to choose a location to play
  // if neither case above occurs
  for (int i = 0; i < ydim; i++){
    for (int j = 0; j < xdim; j++){
      if (board[i][j] == BLANK){
        board[i][j] = playerval;
        return false;
      } 
      // loop through all elements on the board and assign value to first blank (if it has not been assigned to block a value)
    }
  }
  return true; 
  // if there is nowhere to place it 

  
}

bool getRandomAIInput(
  BoardValue** board, 
  int ydim, int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  // Allocate variable size array (we don't know xdim when we compile)
  int* possible = new int[xdim];
  int numX = 0;
  bool error = true;
  // find all the possible columns with a blank
  for(int i = 0; i < xdim; i++){
    if(board[ydim-1][i] == BLANK){
      possible[numX++] = i;
    }
  }
  // If at least 1 column has a blank
  if(numX != 0){
    // Choose one column
    *x = possible[rand()%numX];
    *y = findYValue(board, ydim, *x);
    board[*y][*x] = playerToValue[currentPlayer];
    error = false;  
  }
  delete [] possible;
  return error;
}


