// Version of the Snake code that runs in the terminal
#include <stdio.h>
#include <conio.h>
// Randomizer for fruit spawn
#include <time.h>
#include <stdlib.h>

// Size of the board
#define COLS 10
#define ROWS 10
// Player and fruit value for symbols on the terminal
#define PLAYER 1
#define FRUIT 2
// Points per fruit (increase for a faster snake growth)
#define POINT_VALUE 5

typedef struct Snake {
    int posX;
    int posY;
    int direction;
    int snakeSize;
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

void spawnFruit() {
    int x, y;

    do {
        x = (rand() % ROWS);
        y = (rand() % COLS);
    } while (board[x][y] == PLAYER);

    board[x][y] = FRUIT;
}

void growSnake() {
    // If the snake is the size of the board, the player wins
    if(snake.snakeSize >= ROWS * COLS) {
        gameEnded = true;
    }
    else {
        //Grow snake with current point value multiplier
        snake.snakeSize += POINT_VALUE;
    }
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
    // (For now, at least) The initial direction will always be UP
    snake.direction = UP;
    snake.snakeSize = 1;
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
    score += POINT_VALUE;
    growSnake();
    spawnFruit();
}

void kill() {
    gameEnded = true;
}

void moveSnake(int moveX, int moveY, int newDirection) {
    snake.posX += moveX;
    snake.posY += moveY;

    //if snake hits wall kill()
    if((snake.posX < 0 || snake.posX >= ROWS) || (snake.posY < 0 || snake.posY >= COLS)) {
        kill();
    }
    //if snake hits itself kill()
    else if(board[snake.posX][snake.posY] == PLAYER) {
        kill();
    }
    else {
        if(board[snake.posX][snake.posY] == FRUIT) {
            caughtFruit();
            board[snake.posX][snake.posY] = PLAYER;
        }
        else {
            board[snake.posX][snake.posY] = PLAYER;
        }

        snake.direction = newDirection;
        board[snake.posX][snake.posY] = PLAYER;

        // Check snake size and remove last player point
    }
}

void readKeyboard() {
    // if snake is facing a direction, block the opposite direction so the player doesn't kill himself

    switch(getch()) {
        case UP:
            moveSnake(-1, 0, UP);
            break;
        case LEFT:
            moveSnake(0, -1, LEFT);
            break;
        case DOWN:
            moveSnake(1, 0, DOWN);
            break;
        case RIGHT:
            moveSnake(0, 1, RIGHT);
            break;
        case 'k':
            gameEnded = true;
            break;
        default:
            // Moves towards the direction the snake is facing
            /*switch(snake.direction) {
                case UP:
                    moveSnake(-1, 0, UP);
                    break; 
                case LEFT:
                    moveSnake(0, -1, LEFT);
                    break;
                case DOWN:
                    moveSnake(0, -1, DOWN);
                    break;
                case RIGHT:
                    moveSnake(0, -1, RIGHT);
                    break;
            }*/
            break;
    }
}

int main(int argc, char argv[]) {
    srand(time(NULL));
    startGame();
    
    do {
        readKeyboard();
        system("cls"); // Clears terminal
        printBoard();
    } while(!gameEnded);

    printf("GAME ENDED!!\nSCORE -> %d", score);
    return 0;
}