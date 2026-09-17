// Version of the Snake code that runs in the terminal
#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
// Randomizer for fruit spawn
#include <time.h>
#include <stdlib.h>
// Sleep function for pause between movements
#include <windows.h>

// Size of the board
#define COLS 10
#define ROWS 10
// Player and fruit value for symbols on the terminal
#define HEAD 0
#define PLAYER 1
#define TAIL 2
#define FRUIT 3

// Points per fruit
const int POINT_VALUE = 1;

// Snake movement ticks
LARGE_INTEGER frequency;
LARGE_INTEGER lastMove;
LARGE_INTEGER currentTime;

typedef struct Position {
    int posX;
    int posY;
} Position;

typedef struct Snake {
    Position body[ROWS * COLS];
    int direction;
    int size;
} Snake;

enum directions {
    UP = 'w', 
    LEFT = 'a',
    DOWN = 's', 
    RIGHT = 'd'
};

int board[ROWS][COLS];
Snake snake;
int score;
bool gameEnded;
// Time to wait between snake movement
int waitingTime;

bool isSnakeAt(int x, int y) {
    for(int i = 0; i < snake.size; i++) {
        if(snake.body[i].posX == x && snake.body[i].posY == y) {
            return true;
        }
    }

    return false;
}

void spawnFruit() {
    int x, y;

    if(snake.size < (ROWS * COLS)) {
        do {
            x = (rand() % ROWS);
            y = (rand() % COLS);
        } while (isSnakeAt(x, y));

        board[x][y] = FRUIT;
    }
}

void printBoard() {
    for(int r = 0; r < ROWS; r++) {
        for(int c = 0; c < COLS; c++) {
            if(board[r][c] == FRUIT) {
                printf("O");
            }
            else if(isSnakeAt(r, c)) {
                if(snake.body[0].posX == r && snake.body[0].posY == c) {
                    printf("@");
                }
                else {
                    printf("#");
                }
            }
            else {
                printf(".");
            }
        }
        printf("\n");
    }
}

// Cleans the board and puts the snake in the middle
void startGame() {
    snake.body[0].posX = (COLS / 2) - 1;
    snake.body[0].posY = (ROWS / 2) - 1;
    // (For now, at least) The initial direction will always be UP
    snake.direction = UP;
    snake.size = 1;
    gameEnded = false;
    score = 0;
    waitingTime = 800;

    for(int r = 0; r < ROWS; r++) {
        for(int c = 0; c < COLS; c++) {
            board[r][c] = 0;
        }
    }

    spawnFruit();
    printBoard();
}

void caughtFruit() {
    score += POINT_VALUE;
    snake.size++;
    spawnFruit();
}

void kill() {
    gameEnded = true;
}

void moveSnake() {
    int moveX = 0, moveY = 0;
    Position newHead;

    switch(snake.direction) {
        case UP:
            moveX = -1;
            break;
        case LEFT:
            moveY = -1;
            break;
        case DOWN:
            moveX = 1;
            break;
        case RIGHT:
            moveY = 1;
            break;
    }

    newHead.posX = snake.body[0].posX + moveX;
    newHead.posY = snake.body[0].posY + moveY;

    // If snake hits wall kill()
    if((newHead.posX < 0 || newHead.posX >= ROWS) || (newHead.posY < 0 || newHead.posY >= COLS)) {
        kill();
    }
    // If snake hits itself(body, not tail) kill()
    else if(isSnakeAt(newHead.posX, newHead.posY) && (newHead.posX != snake.body[snake.size - 1].posX || newHead.posY != snake.body[snake.size - 1].posY)) {
        kill();
    }
    else {
        // If player gets a fruit
        if(board[newHead.posX][newHead.posY] == FRUIT) {
            // Checks if the snake is the size of the board, if so, the player wins
            if(snake.size + 1 >= (ROWS*COLS)) {
                gameEnded = true;
            }
            else {
                for(int i = (snake.size-1); i >= 0; i--) {
                    snake.body[i+1].posX = snake.body[i].posX;
                    snake.body[i+1].posY = snake.body[i].posY;
                }

                snake.body[0] = newHead;
                board[newHead.posX][newHead.posY] = 0;
                caughtFruit();
            }
        }
        else {
            for(int i = (snake.size-1); i > 0; i--) {
                snake.body[i].posX = snake.body[i-1].posX;
                snake.body[i].posY = snake.body[i-1].posY;
            }

            snake.body[0] = newHead;
        }
    }
}

void readKeyboard() {
    // If player inputs a direction, changes snake.direction
    if(kbhit()) {
        // Move towards the player input direction
        switch(getch()) {
            case UP:
                if(snake.direction != DOWN) {
                    snake.direction = UP;
                }
                break;
            case LEFT:
                if(snake.direction != RIGHT) {
                    snake.direction = LEFT;
                }
                break;
            case DOWN:
                if(snake.direction != UP) {
                    snake.direction = DOWN;
                }
                break;
            case RIGHT:
                if(snake.direction != LEFT) {
                    snake.direction = RIGHT;
                }
                break;
            case 'k':
                gameEnded = true;
                break;
        }
    }
}

int main() {
    // Starts randomizer time and tick frequencies
    srand(time(NULL));
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastMove);

    startGame();
    
    do {
        readKeyboard();

        // Gets the current tick
        QueryPerformanceCounter(&currentTime);

        // Checks if the snake can move based on the current tick and waitingTime
        if((currentTime.QuadPart - lastMove.QuadPart) * 1000 / frequency.QuadPart >= waitingTime) {
            moveSnake();
            lastMove = currentTime;
            system("cls"); // Clears terminal
            printBoard();
        }
    } while(!gameEnded);

    printf("GAME ENDED!!\nSCORE -> %d", score);
    return 0;
}