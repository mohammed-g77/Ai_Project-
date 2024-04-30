#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
using namespace std;
const int BOARD_SIZE = 3;

enum CellState { EMPTY, X, O };

struct Cell {
    int x, y;
    int minimaxValue;
};

class TicTacToe {
private:
    vector<Cell> emptyCells;
    vector<vector<CellState>> board;
    vector<Cell> rootValues;

public:
    TicTacToe() {
        initializeBoard();
    }

    void initializeBoard() {
        rootValues.clear();
        board.assign(BOARD_SIZE, vector<CellState>(BOARD_SIZE, EMPTY));
    }

    bool isRunning() {
        if (isWinning(X)) return false;
        if (isWinning(O)) return false;
        if (getEmptyCells().empty()) return false;
        return true;
    }

    void makeUserMove() {
        int x, y;
        cout << "Enter your move (row ,column): ";
        cin >> x >> y;
        Cell cell = { x - 1, y - 1 };
        if (isValidMove(cell)) {
            move(cell, O);
        }
        else {
            cout << "Invalid move. Try again." << endl;
            makeUserMove();
        }
    }

    bool isValidMove(const Cell& cell) {
        return (cell.x >= 0 && cell.x < BOARD_SIZE && cell.y >= 0 && cell.y < BOARD_SIZE &&
            board[cell.x][cell.y] == EMPTY);
    }

    bool isWinning(CellState player) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            // Check rows and columns
            if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
                (board[0][i] == player && board[1][i] == player && board[2][i] == player))
                return true;
        }

        // Check diagonals
        if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
            (board[0][2] == player && board[1][1] == player && board[2][0] == player))
            return true;

        return false;
    }

     vector<Cell> getEmptyCells() {
        emptyCells.clear();

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (this->board[i][j] == CellState::EMPTY) {
                    emptyCells.push_back({ i, j });
                }
            }
        }
        return emptyCells;
    }

    void move(Cell cell, CellState player) {
        this->board[cell.x][cell.y] = player;
    }

    void displayBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                 cout << (char)(this->board[i][j] == X ? 'X' : (this->board[i][j] == O ? 'O' : '-')) << " ";
            }
             cout << endl;
        }
    }

    Cell getBestMove() {
        int max = INT_MIN;
        int bestMove = -1;

        for (int i = 0; i < rootValues.size(); i++) {
            if (max < rootValues[i].minimaxValue) {
                max = rootValues[i].minimaxValue;
                bestMove = i;
            }
        }

        return rootValues[bestMove];
    }

    void callMinimax(int depth, CellState player) {
        rootValues.clear();
        minimax(depth, player);
    }

    int minimax(int depth, CellState player) {
        if (isWinning(X)) return +1;
        if (isWinning(O)) return -1;

        vector<Cell> availableCells = getEmptyCells();

        if (availableCells.empty()) return 0;

         vector<int> scores;

        for (int i = 0; i < availableCells.size(); i++) {
            Cell point = availableCells[i];

            if (player == X) {
                move(point, X);
                int currentScore = minimax(depth + 1, O);
                scores.push_back(currentScore);

                if (depth == 0) {
                    point.minimaxValue = currentScore;
                    rootValues.push_back(point);
                }
            }
            else if (player == O) {
                move(point, O);
                int currentScore = minimax(depth + 1, X);
                scores.push_back(currentScore);
            }

            board[point.x][point.y] = EMPTY;
        }

        if (player == X) {
            return returnMax(scores);
        }

        return returnMin(scores);
    }

    int returnMin(const  vector<int>& list) {
        int min = INT_MAX;

        for (int i = 0; i < list.size(); ++i) {
            if (list[i] < min) {
                min = list[i];
            }
        }
        return min;
    }

    int returnMax(const  vector<int>& list) {
        int max = INT_MIN;

        for (int i = 0; i < list.size(); ++i) {
            if (list[i] > max) {
                max = list[i];
            }
        }
        return max;
    }

    void play() {
        while (isRunning()) {
            displayBoard();
            makeUserMove();

            if (!isRunning()) break;

            callMinimax(0, X);
            Cell bestMove = getBestMove();
            move(bestMove, X);

            if (!isRunning()) break;
        }

        displayBoard();

        if (isWinning(X)) {
             cout << "You lose! Better luck next time." << endl;
        }
        else if (isWinning(O)) {
           cout << "Congratulations! You win." <<  endl;
        }
        else {
              cout << "It's a draw!" <<  endl;
        }
    }
};

int main() {
    TicTacToe game;

    string fname;
     cout << "Enter the name of the file with the current state: ";
   cin >> fname;

     ifstream file(fname);
    if (!file.is_open()) {
       cerr << "Cannot open file" <<  endl;
        return 1;
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            char c;
            file >> c;
            if (c == 'X') {
                game.move({ i, j }, X);
            }
            else if (c == 'O') {
                game.move({ i, j }, O);
            }
        }
    }

    file.close();

    if (!game.isRunning()) {
        cerr << "The game has already ended." << endl;
        return 1;
    }

    game.play();

    return 0;
}
