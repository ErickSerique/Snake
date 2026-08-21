#include <stdio.h>
#include <conio.h>
// Randomizer for fruit spawn
#include <time.h>
#include <stdlib.h>

#define COLS 10
#define ROWS 10
#define PLAYER 1
#define FRUIT 2

typedef struct Snake {
    int posX;
    int posY;
    // For now (at least) the initial direction will always be UP
    int direction;
} Snake;

int board[ROWS][COLS];
Snake snake;
int score;
bool gameEnded;

void spawnFruit() {
    int x = (rand() % ROWS);
    int y = (rand() % COLS);
    board[x][y] = FRUIT;
}

void printBoard() {
    for(int r = 0; r < ROWS; r++) {
        for(int c = 0; c < COLS; c++) {
            if(board[r][c] == 0) {
                printf(".");
            }
            else if(board[r][c] == FRUIT) {
                printf("O");
            }
            else {
                printf("#");
            }
        }
        printf("\n");
    }
}

// Cleans the board and puts the snake in the middle
void startGame() {
    snake.posX = (COLS / 2) - 1;
    snake.posY = (ROWS / 2) - 1;
    snake.direction = 'w';
    gameEnded = false;
    score = 0;

    for(int r = 0; r < ROWS; r++) {
        for(int c = 0; c < COLS; c++) {
            board[r][c] = 0;
        }
    }

    board[snake.posX][snake.posY] = PLAYER;

    spawnFruit();
    printBoard();
}

void caughtFruit() {
    score++;
    spawnFruit();
}

void moveSnake(int moveX, int moveY, int newDirection) {
    snake.posX += moveX;
    snake.posY += moveY;

    if (board[snake.posX][snake.posY] == FRUIT) {
        caughtFruit();
        board[snake.posX][snake.posY] = PLAYER;
    }
    else {
        board[snake.posX][snake.posY] = PLAYER;
    }

    snake.direction = newDirection;
    board[snake.posX][snake.posY] = PLAYER;
}

void readKeyboard() {
    switch(getch()) {
        case 'w':
            moveSnake(-1, 0, 'w');
            break;
        case 'a':
            moveSnake(0, -1, 'a');
            break;
        case 's':
            moveSnake(1, 0, 's');
            break;
        case 'd':
            moveSnake(0, 1, 'd');
            break;
        case 'k':
            gameEnded = true;
            break;
        default:
            // Moves towards the direction the snake is facing
            if () {

            }
    }
}

int main(int argc, char argv[]) {
    srand(time(NULL));
    startGame();
    
    do {
        readKeyboard();
        system("cls"); // Clears terminal
        printBoard();
    } while(gameEnded = false);

    printf("GAME ENDED!!\nSCORE -> %d", score);
    return 0;
}