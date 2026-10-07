#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define N 3
#define CUTOFF -1
#define FAILURE -2
#define SUCCESS 1
#define MAX_DEPTH 100

int GoalState[N][N] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

// Movement vectors: UP, DOWN, LEFT, RIGHT
int rowMove[] = {-1, 1, 0, 0};
int colMove[] = {0, 0, -1, 1};

void printBoard(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool areStatesEqual(int state1[N][N], int state2[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (state1[i][j] != state2[i][j]) return false;
        }
    }
    return true;
}

void findBlankTile(int state[N][N], int *blankRow, int *blankCol) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (state[i][j] == 0) {
                *blankRow = i;
                *blankCol = j;
                return;
            }
        }
    }
}

bool applyMove(int state[N][N], int moveIndex, int newState[N][N]) {
    int blankRow, blankCol;
    findBlankTile(state, &blankRow, &blankCol);

    int newRow = blankRow + rowMove[moveIndex];
    int newCol = blankCol + colMove[moveIndex];

    if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N) {
        memcpy(newState, state, sizeof(int) * N * N);
        newState[blankRow][blankCol] = newState[newRow][newCol];
        newState[newRow][newCol] = 0;
        return true;
    }
    return false;
}

// Check if state exists anywhere in current call path (prevents cycles)
bool isVisitedInPath(int state[N][N], int path[MAX_DEPTH][N][N], int pathLength) {
    for (int i = 0; i < pathLength; i++) {
        if (areStatesEqual(state, path[i])) {
            return true;
        }
    }
    return false;
}

// DLS with full path cycle detection
int DLS(int State[N][N], int Goal[N][N], int Limit, int path[MAX_DEPTH][N][N], int pathLength) {
    if (areStatesEqual(State, Goal)) {
        printf("Step %d:\n", pathLength);
        printBoard(State);
        return SUCCESS;
    }

    if (Limit == 0) {
        return CUTOFF;
    }

    // Add current state to path history
    memcpy(path[pathLength], State, sizeof(int) * N * N);
    pathLength++;

    bool cutoff_occurred = false;

    for (int i = 0; i < 4; i++) {
        int NewState[N][N];

        if (applyMove(State, i, NewState)) {
            // Skip if state is already in current path (prevents loops)
            if (isVisitedInPath(NewState, path, pathLength)) {
                continue;
            }

            int result = DLS(NewState, Goal, Limit - 1, path, pathLength);

            if (result == CUTOFF) {
                cutoff_occurred = true;
            } else if (result != FAILURE) {
                printf("Step %d:\n", pathLength - 1);
                printBoard(State);
                return result;
            }
        }
    }

    if (cutoff_occurred) {
        return CUTOFF;
    } else {
        return FAILURE;
    }
}

void IDS(int InitialState[N][N], int GoalState[N][N]) {
    int depth = 0;
    int path[MAX_DEPTH][N][N];

    printf("Starting Iterative Deepening Search...\n\n");

    while (1) {
        printf("--- Searching at Depth Limit: %d ---\n", depth);
        
        int result = DLS(InitialState, GoalState, depth, path, 0);

        if (result != CUTOFF && result != FAILURE) {
            printf("Solution Found at Depth %d!\n", depth);
            return;
        }

        depth++;
    }
}

int main() {
    int InitialState[N][N] = {
        {1, 2, 3},
        {0, 4, 6},
        {7, 5, 8}
    };

    IDS(InitialState, GoalState);
    return 0;
}