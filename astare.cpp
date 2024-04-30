#include <iostream>
#include <queue>
#include <set>
#include <cmath>
using namespace std;

const int MAX_N = 10;
const int dx[] = { 1, -1, 0, 0 };
const int dy[] = { 0, 0, 1, -1 };

struct State {
    char board[MAX_N][MAX_N];
    int cost;
    int heuristic;
    pair<int, int> emptyPosition;

    bool operator<(const State& other) const {
        return cost + heuristic > other.cost + other.heuristic;
    }
};

int calculateHeuristic(const char board[MAX_N][MAX_N], const char goal[MAX_N][MAX_N], int n) {
    int h = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (board[i][j] != goal[i][j] && board[i][j] != '-')
                h++;

    return h;
}

bool isValid(int x, int y, int n) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

void printSolution(const State& finalState, int expandedStates) {
    if (expandedStates > 0) {
        cout << "A solution:" << endl;
        State currentState = finalState;

        while (currentState.cost > 0) {
            int x = currentState.emptyPosition.first;
            int y = currentState.emptyPosition.second;

            int newX = currentState.board[x][y] - 'A';
            int newY = currentState.board[x][y] - 'A' + 1;

            cout << "move " << currentState.board[x][y] << " up" << endl;

            currentState.board[x][y] = currentState.board[newX][newY];
            currentState.board[newX][newY] = '-';
            currentState.emptyPosition = { newX, newY };
        }

        cout << "The number of states expanded is: " << expandedStates << endl;
    }
    else {
        cout << "No solution found." << endl;
    }
}

void solveAStar(const char initial[MAX_N][MAX_N], const char goal[MAX_N][MAX_N], int n) {
    State startState{ {}, 0, 0, {} };
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            startState.board[i][j] = initial[i][j];
            if (initial[i][j] == '-')
                startState.emptyPosition = { i, j };
        }

    startState.heuristic = calculateHeuristic(startState.board, goal, n);

    priority_queue<State> openList;
    set<string> closedList;

    openList.push(startState);

    int expandedStates = 0;

    while (!openList.empty()) {
        State current = openList.top();
        openList.pop();

        string currentString;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                currentString += current.board[i][j];

        bool isGoal = true;
        for (int i = 0; i < n && isGoal; i++)
            for (int j = 0; j < n && isGoal; j++)
                if (current.board[i][j] != goal[i][j])
                    isGoal = false;

        if (isGoal) {
            printSolution(current, expandedStates);
            return;
        }

        if (closedList.find(currentString) == closedList.end()) {
            closedList.insert(currentString);
            expandedStates++;

            for (int i = 0; i < 4; i++) {
                int x = current.emptyPosition.first;
                int y = current.emptyPosition.second;

                int newX = x + dx[i];
                int newY = y + dy[i];

                if (isValid(newX, newY, n)) {
                    State nextState = current;
                    swap(nextState.board[x][y], nextState.board[newX][newY]);
                    nextState.cost++;
                    nextState.heuristic = calculateHeuristic(nextState.board, goal, n);
                    nextState.emptyPosition = { newX, newY };

                    openList.push(nextState);
                }
            }
        }
    }

    printSolution({}, 0);
}

int main() {
    int n;
    cout << "Enter the board size: ";
    cin >> n;

    char initial[MAX_N][MAX_N];
    char goal[MAX_N][MAX_N];

    cout << "Enter the initial state:" << endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> initial[i][j];

    cout << "Enter the goal state:" << endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> goal[i][j];

    solveAStar(initial, goal, n);

    return 0;
}
