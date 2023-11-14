#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 30

// creating data type for the game
typedef struct {
    int hasMine;
    int revealed;
    int marked;
} Cell;

Cell gameField[MAX_SIZE][MAX_SIZE];

void initializeGameField(int rows, int cols, int mines) {
    // Initialize all of cells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            gameField[i][j].hasMine = 0;
            gameField[i][j].revealed = 0;
            gameField[i][j].marked = 0;
        }
}
// Place mines
    srand(time(NULL));
    for (int i = 0; i < mines; ) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (!gameField[r][c].hasMine) {
            gameField[r][c].hasMine = 1;
            i++;
        }
    }
}

void printGameField(int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (gameField[i][j].revealed) {
	// Placeholder for count of next mines
                printf(" %d ", gameField[i][j].hasMine ? '*' : '.'); 
            } else {
                printf(" ? ");
            }
        }
        printf("\n");
    }
}

int checkWinCondition(int rows, int cols, int mines) {
    int revealedCells = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (gameField[i][j].revealed && !gameField[i][j].hasMine) {
                revealedCells++;
            }
        }
    }
    return (revealedCells == rows * cols - mines);
}

void revealCell(int row, int col) {
    // Simple reveal logic. can be expanded to reveal next cells if not a mine
    if (row >= 0 && row < MAX_SIZE && col >= 0 && col < MAX_SIZE) {
        gameField[row][col].revealed = 1;
    }
}

int main() {
    int rows, cols, mines;
    // User input codes:

    initializeGameField(rows, cols, mines);
    printGameField(rows, cols);

    int gameOver = 0;
    while (!gameOver) {
        int row, col;
        printf("Enter row and column to reveal (0-based): ");
        scanf("%d %d", &row, &col);

// checking for the mines in rhe selected point
        if (gameField[row][col].hasMine) {
            printf("Game Over! You hit a mine!\n");
            gameOver = 1;
        } else {
            revealCell(row, col);
            printGameField(rows, cols);
            if (checkWinCondition(rows, cols, mines)) {
                printf("Congratulations! You have won!\n");
                gameOver = 1;
            }
        }
    }

    return 0;
}


