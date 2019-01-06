#include <iostream>
#include <unistd.h>

using namespace std;

#define NUM_PIECES_TO_WIN 5

#define ROW_SIZE 10
#define COL_SIZE 10

#define EMPTY '.'

#define PIECE_PLAYER 'O'
#define PIECE_COMPUTER '@'

#define TURN_COMPUTER true
#define TURN_PLAYER false

#define COMPUTER_THINKING_DELAY_MIN_IN_SEC 4.0
#define COMPUTER_THINKING_DELAY_MAX_IN_SEC 8.0

void initBoard(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize);
void printBoard(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize);
bool isBoardFull(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize);

bool validate(int rowSize, int colSize, int row, int col);
// pre-condition: validate(rowSize, colSize, row, col) == true
bool isOccupied(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize, int row, int col);

bool winCheck(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize, int row, int col);

//char TEST_BOARD_01[ROW_SIZE][COL_SIZE+1] = {
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//};

char TEST_BOARD_01[ROW_SIZE][COL_SIZE+1] = {
    "..........",
    "..........",
    "..........",
    "..........",
    "..........",
    "..........",
    "..........",
    "..........",
    "..........",
    "..........",
};

//char TEST_BOARD_01[ROW_SIZE][COL_SIZE+1] = {
//    "..........",
//    "..........",
//    ".......@..",
//    "......@...",
//    "..........",
//    "....@.....",
//    "...@......",
//    "..........",
//    "..........",
//    "..........",
//};

// test case : when the board is full
//char TEST_BOARD_01[ROW_SIZE][COL_SIZE+1] = {
//    ".@@@@@@@@@",
//    "@@@@@@@@@@",
//    "@@@@@@@@@@",
//    "@@@@@@@@@@",
//    "@@@@@@@@@@",
//    "@@@@@@@@@@",
//    "@@@@@@@@@@",
//    "@@@@@@@@@@",
//    "@@@@@@@@@@",
//    "@@@@@@@@@@",
//};

//int numPiecesOnBoard = 0;

void copyTestBoard(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize, char testBoard[ROW_SIZE][COL_SIZE+1], int *numPiecesOnBoard) {
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            if (testBoard[i][j] != EMPTY) {
                (*numPiecesOnBoard)++;
            }
            board[i][j] = testBoard[i][j];
        }
    }
}

int main() {
//    srand(0); // random seed
    unsigned seed = (unsigned)time(NULL);
    srand(seed); // random seed
    cout << "seed: " << seed << endl;
    
    char board[ROW_SIZE][COL_SIZE];
    bool turn = TURN_PLAYER;
    int totalNumPieces = ROW_SIZE * COL_SIZE;
    int numPiecesOnBoard = 0;
//    numPiecesOnBoard = 0;
    
    initBoard(board, ROW_SIZE, COL_SIZE); // -> calling a function??
    
    copyTestBoard(board, ROW_SIZE, COL_SIZE, TEST_BOARD_01, &numPiecesOnBoard);
    
    while (true) { // infinite loop
        printBoard(board, ROW_SIZE, COL_SIZE);
    
        int row = -1;
        int col = -1;
        
        if (turn == TURN_PLAYER) {
            while (true) {
                cout << "Enter row(-1 to exit): ";
                cin >> row;
                if (row == -1) {
                    break;
                }
                
                cout << "Enter col(-1 to exit): ";
                cin >> col;
                if (col == -1) {
                    break;
                }
                if (validate(ROW_SIZE, COL_SIZE, row, col) &&
                    !isOccupied(board, ROW_SIZE, COL_SIZE, row, col)) {
                    break;
                }
            }
            
            if (row == -1 || col == -1) {
                cout << "End!!!!" << endl;
                break;
            }
        }
        else { // if (turn == TURN_COMPUTER) {
            // IQ 80: Random AI
            while (true) {
                row = rand() % ROW_SIZE;
                col = rand() % COL_SIZE;
                
                if (!isOccupied(board, ROW_SIZE, COL_SIZE, row, col)) {
                    break;
                }
            }
            //------------------------------------------------------------------
            // computer delay
            //------------------------------------------------------------------
//            #define COMPUTER_THINKING_DELAY_MIN_IN_SEC 4.0
//            #define COMPUTER_THINKING_DELAY_MAX_IN_SEC 8.0
            
            // uniform random distribution:
            int randDelayInMilllsecMax = (int)(COMPUTER_THINKING_DELAY_MAX_IN_SEC * 1000);
            int randDelayInMilllsecMin = (int)(COMPUTER_THINKING_DELAY_MIN_IN_SEC * 1000);
            int randDelayInMicroec = rand() % (randDelayInMilllsecMax - randDelayInMilllsecMin + 1) + randDelayInMilllsecMin;
            
            cout << "Computer is thingking...: ";
            
            int currentDelay = 1000;
            while (true) {
                usleep(currentDelay * 1000);
                cout << "=";
                
                randDelayInMicroec -= currentDelay;
                if (randDelayInMicroec <= 0) {
                    break;
                }
                else if (randDelayInMicroec >= 1000) {
                    currentDelay = 1000;
                }
                else {
                    currentDelay = randDelayInMicroec;
                }
            }
            cout << endl;
        }
        
        if (turn == TURN_COMPUTER) {
            board[row][col] = PIECE_COMPUTER;
        }
        else {
            board[row][col] = PIECE_PLAYER;
        }

        numPiecesOnBoard++;
        
        if (winCheck(board, ROW_SIZE, COL_SIZE, row, col)) {
            if (turn == TURN_COMPUTER) {
                cout << "\nBlack win!" << endl;
            }
            else {
                cout << "\nWhite win!" << endl;
            }
            break;
        }

//        if (isBoardFull(board, ROW_SIZE, COL_SIZE)) {
//            cout << "draw" << endl;
//            break;
//        }
        if (numPiecesOnBoard == totalNumPieces) {
            cout << "draw" << endl;
            break;
        }
        
        turn = !turn;
    }
    
    printBoard(board, ROW_SIZE, COL_SIZE);
    cout << "Bye!!" << endl;
    
    return 0;
}

bool winCheck(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize, int row, int col) {
    int count;
    
    int curRow;
    int curCol;

    //--------------------------------------------------------------------------
    // horizontal check
    //--------------------------------------------------------------------------
    count = 1;
    
    // horz: go to left
    curRow = row;
    curCol = col-1;
    while (curCol >= 0 && board[curRow][curCol] == board[row][col]) {
        count++;
        curCol--;
    }
    // horz: go to right
    curRow = row;
    curCol = col+1;
    while (curCol < colSize && board[curRow][curCol] == board[row][col]) {
        count++;
        curCol++;
    }
    
    if (count == NUM_PIECES_TO_WIN) {
        return true;
    }

    //--------------------------------------------------------------------------
    // vertical check
    //--------------------------------------------------------------------------
    count = 1;
    
    // vert: go to down
    curRow = row-1;
    curCol = col;
    while (curRow >= 0 && board[curRow][curCol] == board[row][col]) {
        count++;
        curRow--;
    }
    // vert: go to up
    curRow = row+1;
    curCol = col;
    while (curRow < colSize && board[curRow][curCol] == board[row][col]) {
        count++;
        curRow++;
    }
    
    if (count == NUM_PIECES_TO_WIN) {
        return true;
    }
    
    //--------------------------------------------------------------------------
    // diag: top-left to bottom-right
    //--------------------------------------------------------------------------
    count = 1;
    
    // go up
    curRow = row-1;
    curCol = col-1;
    while (curRow >= 0 && curCol >= 0 && board[curRow][curCol] == board[row][col]) {
        count++;
        curRow--;
        curCol--;
    }
    // go down
    curRow = row+1;
    curCol = col+1;
    while (curRow < rowSize && curCol < colSize && board[curRow][curCol] == board[row][col]) {
        count++;
        curRow++;
        curCol++;
    }
    
    if (count == NUM_PIECES_TO_WIN) {
        return true;
    }

    //--------------------------------------------------------------------------
    // diag: top-right to bottom-left
    //--------------------------------------------------------------------------
    count = 1;
    
    // go up
    curRow = row-1;
    curCol = col+1;
    while (curRow >= 0 && curCol >= 0 && board[curRow][curCol] == board[row][col]) {
        count++;
        curRow--;
        curCol++;
    }
    // go down
    curRow = row+1;
    curCol = col-1;
    while (curRow < rowSize && curCol < colSize && board[curRow][curCol] == board[row][col]) {
        count++;
        curRow++;
        curCol--;
    }
    
    if (count == NUM_PIECES_TO_WIN) {
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------------

bool validate(int rowSize, int colSize, int row, int col) {
//    return !(row < 0 || row >= rowSize || col < 0 || col >= colSize);
    return row >= 0 && row < rowSize && col >= 0 && col < colSize; // demorgan의 법칙
}

// pre-condition: validate(rowSize, colSize, row, col) == true
bool isOccupied(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize, int row, int col) {
    //    return !(row < 0 || row >= rowSize || col < 0 || col >= colSize);
    return board[row][col] != EMPTY; // demorgan의 법칙
}

bool isBoardFull(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize) {
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            if (board[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

//setting up the board
void initBoard(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize) {
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            board[i][j] = EMPTY;
        }
    }
}

void printBoard(char board[ROW_SIZE][COL_SIZE], int rowSize, int colSize) {
//    cout << "  ";
//    for (int j = 0; j < colSize; j++) {
//        cout << "~";
//    }
//    cout << endl;

    cout << "  ";
    for (int j = 0; j < colSize; j++) {
        cout << " " << j;
    }
    cout << endl;
    
    cout << " +";
    for (int j = 0; j < colSize; j++) {
        cout << "--";
    }
    cout << "-+" << endl;
    
    cout << " |";
    for (int j = 0; j < colSize; j++) {
        cout << " |";
    }
    cout << " |" << endl;
    
    for (int i = 0; i < rowSize; i++) {
        cout << i << "|";
        for (int j = 0; j < colSize; j++) {
            if (board[i][j] == EMPTY) {
                cout << "-+";
            }
            else {
                cout << "-" << board[i][j];
            }
        }
        cout << "-|" << i << endl;
        
        cout << " |";
        for (int j = 0; j < colSize; j++) {
            cout << " |";
        }
        cout << " |" << endl;
    }
    
    cout << " +";
    for (int j = 0; j < colSize; j++) {
        cout << "--";
    }
    cout << "-+" << endl;
    
    cout << "  ";
    for (int j = 0; j < colSize; j++) {
        cout << " " << j;
    }
    cout << endl;
}
