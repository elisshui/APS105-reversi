#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

//constants
const char white = 'W';  //white marker
const char black = 'B';  //black marker
const char unoccupied = 'U';  //empty space marker
const char letterCoordsRow = 'a', letterCoordsCol = 'a';  //axis inital coordinates

//functions
int getBoardSize();
void initalBoardSetUp(char board[][26], int size);
void getPlayMode(int *playMode);
void getLevel(int *level);
void getCompColour(char *compColourPtr, char *turnPtr);

void changeTurn(char *turnPtr, char *opponentPtr);
void printBoard(char board[][26], int n);
bool positionInBounds(int n, int row, int col);
void updateBoard(char board[][26], int n, char turn, int  row, int col, char opponent, int *whiteCountPtr, int *blackCountPtr);
bool checkingSandwich(char board[26][26], int n, int row, int col, int deltaRow, int deltaCol, char colour);
void getResult(int *whiteCountPtr, int *blackCountPtr);

void remainingMovesResult(char board[][26], int size, char *turnPtr, char *opponentPtr, bool *gameEndPtr);
double checkRemainingMoves(char board[][26], int n, char playerInCheck, char opponent);
bool CheckPosMovesLegal(char board[][26], int n, int row, int col, char turnColour, char opponent, int deltaRow, int deltaCol, int *possibleMovesPtr);

bool getPlayerMove(char board[][26], int n, char turn, char opponent, int *row, int *col, bool *invalidTerminateGamePtr, bool *gameEndPtr); //gets player input --> sends to be checked
bool directionsPlayerMove(char board[][26], int n, char turn, char opponent, int rowInputted, int colInputted);
bool checkLegalPlayerMove(char board[][26], int n, int rowInputted, int colInputted, char turn, int deltaRow, int deltaCol, int opponent);

int makeMove(char board[][26], int n, char turn, int *row, int *col, int level);
bool possibleCompMoves(char board[][26], int n, char turn, char opponent, int *bestRow, int *bestCol, int *highestScore, int testPrevNumOppMoves);
bool checkCompMoveLegal(char board[][26], int n, int rowInputted, int colInputted, char compColor, char opponent, int deltaRow, int deltaCol, int *bestRow, int *bestCol, int *highestScore, int testPrevNumOppMoves); //checking if marker can be placed in position specified in directions()
void flipOnCopyBoard(char copyBoard[][26], int n, char compColor, char opponent, int rowInputted, int colInputted, int *bestRow, int *bestCol, int *highestScore, int testPrevNumOppMoves); //flip markers after legal move
void getScore(int rowInputted, int colInputted, int numFlipped, int cornersFlipped, int numOppMoves, int testPrevNumOppMoves, int *highestScore, int *bestRow, int *bestCol, int level);  
void bestMove(int moveScore, int rowInputted, int colInputted, int *highestScorePtr, int *bestRow, int *bestCol); //will get highest score
bool checkLegalInDirectionComp(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol, int *highestScore, int *testRow, int *testCol, int level); //checking if marker can be placed in position specified in directions()
void updateCopyBoard(char copyBoard[][26], char board[][26], int n, char colour, int  row, int col, int *highestScore, int *testRow, int *testCol, int level); //flip markers after legal move


int main(){
  //VARIABLES
  char board[26][26];  //the board itself
  int size;  //size of board

  int playMode;
  int level; //1=easy, 2=hard
  char compColour, *compColourPtr=&compColour; //setting computer color

  char turn, *turnPtr=&turn; //stores current turn's marker color
  char opponent, *opponentPtr=&opponent;  //store the current player's opponent

  int whiteCount=2, *whiteCountPtr=&whiteCount; //number of white markers
  int blackCount=2, *blackCountPtr=&blackCount; //number of black markers

  int row, col; //row and col coords chosen by computer/player
  
  bool gameEnd=false, *gameEndPtr=&gameEnd; //if game end, change to true (ends while loop)

  bool invalidTerminateGame=false, *invalidTerminateGamePtr=&invalidTerminateGame;  //invalid move (end prematurely)

  //GET PLAYING MODE:
  getPlayMode(&playMode);
  
  if(playMode==1){ //single player (against AI)
    //if single player (get level & color)
    getLevel(&level); //get game difficulty
    getCompColour(compColourPtr, turnPtr);  //get color of marker for computer

    //INITAL GAME SET UP:
    size = getBoardSize(); //instalizing size of board
    initalBoardSetUp(board, size);  //setting up inital condition
    printBoard(board, size);  //print inital board

    //ACTUAL GAME STARTS HERE:
    while(gameEnd==false){ //while no winner found
      if(turn==compColour){ //turn = same as computer --> play computer
        makeMove(board, size, turn, &row, &col, level); //call computer function
        updateBoard(board, size, turn, row, col, opponent, whiteCountPtr, blackCountPtr);  //update board with legal move
        printBoard(board, size);  //print out updated board
      }
      else if(turn!=compColour){  //turn is player --> play human
        if(getPlayerMove(board, size, turn, opponent, &row, &col, invalidTerminateGamePtr, gameEndPtr)==true){ //call human function - GET ROW AND COL
          updateBoard(board, size, turn, row, col, opponent, whiteCountPtr, blackCountPtr);  //update board with legal move and flips
          printBoard(board, size);  //print out updated board
        }
      }
      changeTurn(turnPtr, opponentPtr);  //change turns
      remainingMovesResult(board, size, turnPtr, opponentPtr, gameEndPtr); //check if moves are avalible (change turns done in checkRemainingMoves function)   
    }  
    if(invalidTerminateGame!=true){ //invalid move wasn't entered (if inval. move entered, result = printed)
      getResult(whiteCountPtr, blackCountPtr);  //get game result
    }
    return 0; //END PROGRAM
  }
    
  else if (playMode==2){ //two-players (no AI)
    //INITAL GAME SET UP:
    printf("\nYou have selected the two-player mode!\n");
    size = getBoardSize(); //instalizing size of board
    initalBoardSetUp(board, size);  //setting up inital condition
    printBoard(board, size);  //print inital board
    printf("\nThe black counter will start first.\n\n");

    turn='B';

    //ACTUAL GAME STARTS HERE:
    while(gameEnd==false){ //while no winner found
      if(getPlayerMove(board, size, turn, opponent, &row, &col, invalidTerminateGamePtr, gameEndPtr)==true){ //call human function - GET ROW AND COL
          updateBoard(board, size, turn, row, col, opponent, whiteCountPtr, blackCountPtr);  //update board with legal move and flips
          printBoard(board, size);  //print out updated board
        }
      changeTurn(turnPtr, opponentPtr);  //change turns
      remainingMovesResult(board, size, turnPtr, opponentPtr, gameEndPtr); //check if moves are avalible (change turns done in checkRemainingMoves function)
    } 
    if(invalidTerminateGame!=true){ //invalid move wasn't entered
      getResult(whiteCountPtr, blackCountPtr);  //get game result
    }
    return 0; //END PROGRAM
  }
  
  return 0; //END PROGRAM
}

//--------Game set-up functions-------
int getBoardSize(){  //gets size of board
  int size;
  bool rightSize=false;
  
  do{
    printf("\nEnter the board's size: ");
    scanf("%d", &size);
    if(size>26 || size<=2 || size%2!=0){
      printf("Invalid size.\n");
      printf("The size must be between 4 and 26 inclusive and must be an even number.\n");
    }
    else{
      rightSize=true;
      return size;
    }
  }
  while(rightSize==false);
  return size;
}

void initalBoardSetUp(char board[][26], int size){  //inital setup (2'B', 2'W')
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
        board[i][j]='U';
    }
  }
  board[(size/2)-1][(size/2)-1]=board[size/2][size/2]='W';
  board[(size/2)-1][size/2]=board[size/2][(size/2)-1]='B';
}

void getPlayMode(int *playMode){
  bool choice=false;
  
  do{
    printf("Welcome!\n\nPlease choose your playing mode.\n");
    printf("Single Player (press 1)\n");
    printf("Two Players (press 2)\n");
    scanf("%d", playMode);
    if(*playMode!=1 && *playMode!=2){
      printf("Sorry, that was not an option.\n");
    }
    else{
      choice=true;
      return;
    }
  }
  while(choice==false);
}
  
  
void getLevel(int *level){
  bool choice=false;

  do{
    printf("\nYou have selected: single player mode.\n\n");
    printf("Which level would you like to play?\n");
    printf("Easy (press 1)\nHard (press 2)\n");
    scanf("%d", level);
    if(*level!=1 && *level!=2){
      printf("Sorry, that was not an option.\n");
    }
    else{
      choice=true;
      return;
    }
  }
  while(choice==false);
}
  
void getCompColour(char *compColourPtr, char *turnPtr){  //get computer's color
  char compColour;

  printf("\nComputer plays black or white (Enter B/W)? ");
  scanf(" %c", &compColour);   //setting computer color
  *turnPtr = 'B';    //First player = always B

  if(compColour!='B' && compColour!='W'){ //end if correct letter not assigned to computer
    return;
  }
  *compColourPtr=compColour;
  return;
}
//--------------------------------------

//--------COMMON GAME FUNCTIONS---------
void changeTurn(char *turnPtr, char *opponentPtr){  //change moveCount(player) and turn(color)
  if(*turnPtr=='B'){  //change counter
    *turnPtr='W';
    *opponentPtr='B';
  }
  else{
    *turnPtr='B';
    *opponentPtr='W';
  }
  return;
}

void printBoard(char board[][26], int n){  //print board a any given time
  //print x coords above actual playing board
  printf("  ");  //space for formatting
  for(int col=0; col<n; col++){
    printf("%c", 'a'+col); //'a'+index
  }
  printf("\n"); //finished x coords (now print rest of board)

  //print rest of board
  for(int row=0; row<n; row++){
    printf("%c ", 'a'+row);  //y coords ('a'+index)
    for(int col=0; col<n; col++){
      printf("%c", board[row][col]);
    }
    printf("\n");
  }
}   

bool positionInBounds(int n, int row, int col){  //check if move is in bounds
  return (row>=0 && row<n && col>=0 && col<n);  //within bounds
}

void updateBoard(char board[][26], int n, char turn, int  row, int col, char opponent, int *whiteCountPtr, int *blackCountPtr){ //flip markers after legal move, changes turn
  int turnCount; //holds number of markers of current player
  int opponentCount; //holds number of markers of computer

  if(turn=='B'){ //READING: setting who is currently playing for counting
    turnCount = *blackCountPtr+1;
    opponentCount = *whiteCountPtr;
    opponent='W';
  }
  else{
    turnCount = *whiteCountPtr+1;
    opponentCount = *blackCountPtr;
    opponent='B';
  }
  
  board[row][col]=turn; //place player marker down

  int rowDir[] = {1, 1, 0, -1, -1, -1, 0, 1}; //directions (rows)
  int colDir[] = {0, -1, -1, -1, 0, 1, 1, 1};  //directions (cols) 

  for(int i=0; i<8; i++){ //iterating directions
    int deltaRow=rowDir[i]; //setting "steps" to move by
    int deltaCol=colDir[i]; //setting "steps" to move by
    int count = 1;
    if(positionInBounds(n, (row+deltaRow*count), (col+deltaCol*count))==true && checkingSandwich(board, n, row, col, deltaRow, deltaCol, turn)==true){ //step is in bounds
      while(board[row+deltaRow*count][col+deltaCol*count]==opponent && positionInBounds(n, row+deltaRow*count, col+deltaCol*count)==true){ //is opponent
        board[row+deltaRow*count][col+deltaCol*count]=turn; //change color to player
        turnCount = turnCount+1; //increases number of markers of player
        opponentCount = opponentCount-1; //decreases number of markers of opponent
        count++; //increase steps (flip next marker)
      }
    }
  }
  
  if(turn=='B'){ //WRITING: update number of markers on board
    *blackCountPtr = turnCount;
    *whiteCountPtr = opponentCount;
  }
  else{
    *whiteCountPtr = turnCount;
    *blackCountPtr = opponentCount;
  }
  return;
}

bool checkingSandwich(char board[26][26], int n, int row, int col, int deltaRow, int deltaCol, char colour){  //checking if flipping is allowed
  for(int count=1; positionInBounds(n, row+deltaRow*count, col+deltaCol*count)==true; count++){
    if(board[row+deltaRow*count][col+deltaCol*count]==colour){
      return true;  //yes, colour marker found
    }
    if(board[row+deltaRow*count][col+deltaCol*count]==unoccupied){
      return false; //no colour marker found
    }
  }
  return false;
}

void getResult(int *whiteCountPtr, int *blackCountPtr){ //get and print result
  if(*whiteCountPtr>*blackCountPtr){ //white wins
    printf("W player wins.");
    return;
  }
  else if(*blackCountPtr>*whiteCountPtr){ //black wins
    printf("B player wins.");
    return;
  }
  else{ //draw
    printf("Draw!");
    return;
  }
  return;
}
//---------------------------------------

//--------CHECK FOR REMAINING AVALIBLE MOVES--------
void remainingMovesResult(char board[][26], int size, char *turnPtr, char *opponentPtr, bool *gameEndPtr){
  char currentMove = *turnPtr; //current move = the "next" move from the move that has just been played
  char nextMove = *opponentPtr;

  if(checkRemainingMoves(board, size, currentMove, nextMove)==0){   //current player has no more moves
    if(checkRemainingMoves(board, size, nextMove, currentMove)==0){   //check next player
      *gameEndPtr = true; //no more moves left - print result
    }
    else{
      printf("%c player has no valid move.\n", currentMove);
      changeTurn(turnPtr, opponentPtr); //change player
    }
  }
  return;
}

double checkRemainingMoves(char board[][26], int n, char playerInCheck, char opponent){
  int possibleMoves=0.0; //num of possible moves
  int *possibleMovesPtr = &possibleMoves;
  
  int deltaRow[] = {1, 1, 0, -1, -1, -1, 0, 1}; //directions (rows)
  int deltaCol[] = {0, -1, -1, -1, 0, 1, 1, 1};  //directions (cols)
  for(int r=0; r<n; r++){  
    for(int c=0; c<n; c++){  
      if(board[r][c]=='U'){  //unoccupied found
        for(int dirInd=0; dirInd<8; dirInd++){ //traversing parallel array (directions)
          if(positionInBounds(n, (r+deltaRow[dirInd]), (c+deltaCol[dirInd]))==true){ //if next step in bounds --> pass next step coords for checking
            if(CheckPosMovesLegal(board, n, (r+deltaRow[dirInd]), (c+deltaCol[dirInd]), playerInCheck, opponent, deltaRow[dirInd], deltaCol[dirInd], possibleMovesPtr)==true){
              dirInd=7; //stops double checking
            }
          } 
        }
      }
    }
  }
  return possibleMoves;
}

bool CheckPosMovesLegal(char board[][26], int n, int row, int col, char turnColour, char opponent, int deltaRow, int deltaCol, int *possibleMovesPtr){ //checking if marker can be placed in position specified in directions()
  int cnt=0; //number of iterations (b/c while loop used)
  while((positionInBounds(n, row+(cnt*deltaRow), col+(cnt*deltaCol))==true) && (board[row+(cnt*deltaRow)][col+(cnt*deltaCol)]==opponent)){  //while in bounds, is opponent
    cnt++;
      if((positionInBounds(n, row+(cnt*deltaRow), col+(cnt*deltaCol))==true) && checkingSandwich(board, n, row, col, deltaRow, deltaCol, turnColour)==true){ //reaches color
        *possibleMovesPtr = *possibleMovesPtr+1.0; //increase possible move count
        return true; //position is legal
      }
    }
  return false;
}
//---------------------------------

//------GETTING PLAYER MOVES-------
bool getPlayerMove(char board[][26], int n, char turn, char opponent, int *row, int *col, bool *invalidTerminateGamePtr, bool *gameEndPtr){ //gets player input --> sends to be checked
  char rowInputted, colInputted; //coordinates to be entered
  if(turn=='B'){
    opponent='W';
  }
  else{
    opponent='B';
  }

  printf("Enter move for %c counter (RowCol): ", turn);
  scanf(" %c%c", &rowInputted,&colInputted);

  rowInputted = rowInputted-97; //converting letter to index
  colInputted = colInputted-97; //converting letter to index
  if(directionsPlayerMove(board, n, turn, opponent, rowInputted, colInputted)==false){ //if valid, update board will do printing
    printf("Invalid move.\n");
    printf("%c player wins.\n", opponent);
    *invalidTerminateGamePtr = true; //invalid move was made
    *gameEndPtr = true; //end game
    return false; //got move and was invalid, end function
  }

  *row = rowInputted; //send back to main for update and print
  *col = colInputted; //send back to main for update and print
  return true;  //got move as was valid, end function --> move to updateBoard and printBoard
}

bool directionsPlayerMove(char board[][26], int n, char turn, char opponent, int rowInputted, int colInputted){ //checking legality of move
  int deltaRow[] = {1, 1, 0, -1, -1, -1, 0, 1}; //directions (rows)
  int deltaCol[] = {0, -1, -1, -1, 0, 1, 1, 1};  //directions (cols)

  if(board[rowInputted][colInputted]=='U'){  //location entered is unoccupied
    for(int dirInd=0; dirInd<8; dirInd++){ //traversing parallel array (directions)
      if(positionInBounds(n, (rowInputted+deltaRow[dirInd]), (colInputted+deltaCol[dirInd]))==true){ //if next step in bounds --> pass next step coords for checking
        if(checkLegalPlayerMove(board, n, (rowInputted+deltaRow[dirInd]), (colInputted+deltaCol[dirInd]), turn, deltaRow[dirInd], deltaCol[dirInd], opponent)==true){
          return true; //move was legal so return true --> send move to main for update and print
        }
      } 
    }
  }
  return false; //move was illegal so false
}

bool checkLegalPlayerMove(char board[][26], int n, int rowInputted, int colInputted, char turn, int deltaRow, int deltaCol, int opponent){ //step 2 of checking move legality
  int cnt=0; //number of iterations (b/c while loop used)
  while((positionInBounds(n, rowInputted+(cnt*deltaRow), colInputted+(cnt*deltaCol))==true) && (board[rowInputted+(cnt*deltaRow)][colInputted+(cnt*deltaCol)]==opponent)){  //in bounds && opponent
    cnt++; //search next position
    if((positionInBounds(n, rowInputted+(cnt*deltaRow), colInputted+(cnt*deltaCol))==true) && (checkingSandwich(board, n, rowInputted, colInputted, deltaRow, deltaCol, turn)==true)){ //reaches player's color
      return true; //legal so return true.
    }
  } 
  return false; //not legal.
}
//---------------------------------------------------

//-----------GETTING COMPUTER MOVE--------------
int makeMove(char board[][26], int n, char turn, int *row, int *col, int level){ //gets direction to search in
  int highestScore=0;
  int testRow, testCol;
  
  int deltaRow[] = {1, 1, 0, -1, -1, -1, 0, 1}; //directions (rows)
  int deltaCol[] = {0, -1, -1, -1, 0, 1, 1, 1};  //directions (cols)
  for(int r=0; r<n; r++){  
    for(int c=0; c<n; c++){  
      if(board[r][c]==unoccupied){  //unoccupied found
        for(int dirInd=0; dirInd<8; dirInd++){ //traversing parallel array (directions)
          if(positionInBounds(n, (r+deltaRow[dirInd]), (c+deltaCol[dirInd]))==true){ //if next step in bounds --> pass next step coords for checking
            if(checkLegalInDirectionComp(board, n, (r+deltaRow[dirInd]), (c+deltaCol[dirInd]), turn, deltaRow[dirInd], deltaCol[dirInd], &highestScore, &testRow, &testCol, level)==true){
              dirInd=7; //stops double checking
            }
          } 
        }
      }
    }
  }

  *row=testRow;
  *col=testCol;
  printf("Computer plays: %c%c \n", *row+97, *col+97);
  
  return 0;
}

bool checkLegalInDirectionComp(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol, int *highestScore, int *testRow, int *testCol, int level){ //checking if marker can be placed in position specified in directions()
  int cnt=0; //number of iterations (b/c while loop used)
    while((positionInBounds(n, row+(cnt*deltaRow), col+(cnt*deltaCol))==true) && (board[row+(cnt*deltaRow)][col+(cnt*deltaCol)]!=colour) && (board[row+(cnt*deltaRow)][col+(cnt*deltaCol)]!=unoccupied)){  //while in bounds, is opponent
      cnt++;
      if((positionInBounds(n, row+(cnt*deltaRow), col+(cnt*deltaCol))==true) && board[row+(cnt*deltaRow)][col+(cnt*deltaCol)]==colour){ //reaches color
        char copyBoard[26][26]; //creating copy of board
        for(int dr=0; dr<n; dr++){ //creating copy of board again
          for(int dc=0; dc<n; dc++){
            copyBoard[dr][dc]=board[dr][dc];
          }
        }
        updateCopyBoard(copyBoard, board, n, colour, row-deltaRow, col-deltaCol, highestScore, testRow, testCol, level);
        return true; //position is legal
      }
  }
  return false;
}

void updateCopyBoard(char copyBoard[][26], char board[][26], int n, char colour, int  row, int col, int *highestScore, int *testRow, int *testCol, int level){ //flip markers after legal move
  int numFlipped=0, cornersFlipped=0;
  int testPrevNumOppMoves=0, numOppMoves=0;
  
  //setting opponent
  char opponent; 
  if(colour==white){
    opponent = black;
  }
  else{
    opponent = white;
  }

  testPrevNumOppMoves = checkRemainingMoves(board, n, opponent, colour);
  
  copyBoard[row][col]=colour; //place player marker down

  int rowDir[] = {1, 1, 0, -1, -1, -1, 0, 1}; //directions (rows)
  int colDir[] = {0, -1, -1, -1, 0, 1, 1, 1};  //directions (cols) 

  for(int i=0; i<8; i++){ //iterating directions
    int deltaRow=rowDir[i]; //setting "steps" to move by
    int deltaCol=colDir[i]; //setting "steps" to move by

    int count = 1;
    if(positionInBounds(n, (row+deltaRow*count), (col+deltaCol*count))==true && checkingSandwich(board, n, row, col, deltaRow, deltaCol, colour)==true){ //step is in bounds
      while(board[row+deltaRow*count][col+deltaCol*count]==opponent && positionInBounds(n, row+deltaRow*count, col+deltaCol*count)==true){ //is opponent
          copyBoard[row+deltaRow*count][col+deltaCol*count]=colour; //change color to player
          
          //----------------getting score--------------
          if(((row+deltaRow*count)==0 && (col+deltaCol*count)==0)||(((row+deltaRow*count)==0 && (col+deltaCol*count)==(n-1)))||((row+deltaRow*count)==(n-1) && (col+deltaCol*count)==0)||((row+deltaRow*count)==(n-1) && (col+deltaCol*count)==(n-1))){  //corners
            cornersFlipped=cornersFlipped+1; //update number of corners flipped
          }
  
          numFlipped=numFlipped+1; //update count number flipped  
          count++;
          //------------------------------------------
        
      }
    }   
  }
  numOppMoves = checkRemainingMoves(copyBoard, n, opponent, colour);
  
  getScore(row, col, numFlipped, cornersFlipped, numOppMoves, testPrevNumOppMoves, highestScore, testRow, testCol, level);
  numFlipped=0;
  cornersFlipped=0;
  return;
}

void getScore(int rowInputted, int colInputted, int numFlipped, int cornersFlipped, int numOppMoves, int testPrevNumOppMoves, int *highestScore, int *testRow, int *testCol, int level){  
  int score;
  int cornWeight;
  int oppWeight;
  
  if(level==1){
    cornWeight=0;
    oppWeight=0;
  }
  else{
    cornWeight=15;
    oppWeight=2;
  }
  
  score = 6*numFlipped+cornWeight*cornersFlipped+oppWeight*(testPrevNumOppMoves-numOppMoves);
  bestMove(score, rowInputted, colInputted, highestScore, testRow, testCol);

  return;
}

void bestMove(int moveScore, int rowInputted, int colInputted, int *highestScorePtr, int *testRow, int *testCol){ //will get highest score
  if(*highestScorePtr==moveScore || *highestScorePtr>moveScore){   //if same, keep orginal highest score
    return;
  }
  else if(*highestScorePtr<moveScore){ //if current bigger than stored, change
    *highestScorePtr=moveScore; //update current highest score
    *testRow=rowInputted;  //updat current best row
    *testCol=colInputted;  //update current best col
  }
  return;
}
