#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 30

typedef struct {
    int hasMine;
    int revealed;
    int marked;
    int adjacentMines;
} Cell;

void initializeGameField(Cell ***gameField, int rows, int cols, int mines);
void freeGameField(Cell ***gameField, int rows);
void printGameField(Cell **gameField, int rows, int cols);
int checkWinCondition(Cell **gameField, int rows, int cols, int mines);
void revealCell(Cell **gameField, int row, int col, int rows, int cols);
void markCell(Cell **gameField, int row, int col, int rows, int cols);
void calculateAdjacentMines(Cell **gameField, int rows, int cols);
void autoRevealAdjacentCells(Cell **gameField, int row, int col, int rows, int cols);

// Helper functions
int isValidCell(int row, int col, int rows, int cols) {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void calculateAdjacentMines(Cell **gameField, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!gameField[i][j].hasMine) {
                int count = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int ni = i + di, nj = j + dj;
                        if (isValidCell(ni, nj, rows, cols) && gameField[ni][nj].hasMine)
                            count++;
                    }
                }
                gameField[i][j].adjacentMines = count;
            }
        }
    }
}

void initializeGameField(Cell ***gameField, int rows, int cols, int mines) {
    // Dynamically allocate memory for the game field
    *gameField = (Cell **)malloc(rows * sizeof(Cell *));
    for (int i = 0; i < rows; i++) {
        (*gameField)[i] = (Cell *)malloc(cols * sizeof(Cell));
    }

    // Initialize all cells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            (*gameField)[i][j].hasMine = 0;
            (*gameField)[i][j].revealed = 0;
            (*gameField)[i][j].marked = 0;
            (*gameField)[i][j].adjacentMines = 0;
        }
    }

    // Place mines
    srand(time(NULL));
    for (int i = 0; i < mines;) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (!(*gameField)[r][c].hasMine) {
            (*gameField)[r][c].hasMine = 1;
            i++;
        }
    }

    calculateAdjacentMines(*gameField, rows, cols);
}

void freeGameField(Cell ***gameField, int rows) {
    for (int i = 0; i < rows; i++) {
        free((*gameField)[i]);
    }
    free(*gameField);
}

void printGameField(Cell **gameField, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (gameField[i][j].marked) {
                printf(" F ");
            }
            else if (gameField[i][j].revealed) {
                if (gameField[i][j].hasMine) {
                    printf(" * ");
                }
                else {
                    printf(" %d ", gameField[i][j].adjacentMines);
                }
            }
            else {
                printf(" ? ");
            }
        }
        printf("\n");
    }
}

int checkWinCondition(Cell **gameField, int rows, int cols, int mines) {
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

void revealCell(Cell **gameField, int row, int col, int rows, int cols) {
    if (isValidCell(row, col, rows, cols) && !gameField[row][col].revealed && !gameField[row][col].marked) {
        gameField[row][col].revealed = 1;
        if (gameField[row][col].adjacentMines == 0 && !gameField[row][col].hasMine) {
            autoRevealAdjacentCells(gameField, row, col, rows, cols);
        }
    }
}

void autoRevealAdjacentCells(Cell **gameField, int row, int col, int rows, int cols) {
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            int ni = row + di, nj = col + dj;
            if (isValidCell(ni, nj, rows, cols) && !gameField[ni][nj].revealed && !gameField[ni][nj].marked) {
                revealCell(gameField, ni, nj, rows, cols);
            }
        }
    }
}

void markCell(Cell **gameField, int row, int col, int rows, int cols) {
    if (isValidCell(row, col, rows, cols) && !gameField[row][col].revealed) {
        gameField[row][col].marked = !gameField[row][col].marked;
    }
}

// Save the game state to a file
void saveGame(Cell **gameField, int rows, int cols, int mines, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file) {
        fwrite(&rows, sizeof(int), 1, file);
        fwrite(&cols, sizeof(int), 1, file);
        fwrite(&mines, sizeof(int), 1, file);
        for (int i = 0; i < rows; i++) {
            fwrite(gameField[i], sizeof(Cell), cols, file);
        }
        fclose(file);
        printf("Game saved successfully.\n");
    } else {
        printf("Failed to save game.\n");
    }
}

// Load the game state from a file
int loadGame(Cell ***gameField, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        int rows, cols, mines;
        fread(&rows, sizeof(int), 1, file);
        fread(&cols, sizeof(int), 1, file);
        fread(&mines, sizeof(int), 1, file);

        // Allocate memory for game field
        *gameField = (Cell **)malloc(rows * sizeof(Cell *));
        for (int i = 0; i < rows; i++) {
            (*gameField)[i] = (Cell *)malloc(cols * sizeof(Cell));
        }

        for (int i = 0; i < rows; i++) {
            fread((*gameField)[i], sizeof(Cell), cols, file);
        }

        fclose(file);
        return 1;
    } else {
        printf("No saved game found.\n");
        return 0;
    }
}

int main() {
    int rows, cols, mines;
    Cell **gameField = NULL;

    // Load saved game (optional)
    printf("Do you want to load a saved game? (y/n): ");
    char loadChoice;
    scanf(" %c", &loadChoice);
    if (loadChoice == 'y') {
        if (!loadGame(&gameField, "saved_game.dat")) {
            printf("Starting a new game...\n");
        }
    } else {
        printf("Enter number of rows (5-30): ");
        scanf("%d", &rows);
        printf("Enter number of columns (5-30): ");
        scanf("%d", &cols);
        printf("Enter number of mines: ");
        scanf("%d", &mines);

        if (rows < 5 || rows > 30 || cols < 5 || cols > 30 || mines >= rows * cols) {
            printf("Invalid game configuration.\n");
            return 1;
        }

        initializeGameField(&gameField, rows, cols, mines);
    }

    printGameField(gameField, rows, cols);

    int gameOver = 0;
    while (!gameOver) {
        int row, col;
        char action;
        printf("Enter action (r for reveal, m for mark) and row, column (0-based): ");
        scanf(" %c %d %d", &action, &row, &col);

        switch (action) {
        case 'r':
            if (gameField[row][col].marked) {
                printf("Cell is marked. Cannot reveal.\n");
            } else if (gameField[row][col].hasMine) {
                printf("Game Over! You hit a mine!\n");
                gameOver = 1;
            } else {
                revealCell(gameField, row, col, rows, cols);
                printGameField(gameField, rows, cols);
                if (checkWinCondition(gameField, rows, cols, mines)) {
                    printf("Congratulations! You have won!\n");
                    gameOver = 1;
                }
            }
            break;
        case 'm':
            markCell(gameField, row, col, rows, cols);
            printGameField(gameField, rows, cols);
            break;
        default:
            printf("Invalid action.\n");
        }

        // Ask user if they want to save the game
        char saveChoice;
        printf("Do you want to save the game? (y/n): ");
        scanf(" %c", &saveChoice);
        if (saveChoice == 'y') {
            saveGame(gameField, rows, cols, mines, "saved_game.dat");
        }
    }

    freeGameField(&gameField, rows);
    return 0;
}
