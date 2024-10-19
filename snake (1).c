#include <ncurses.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int dirX,dirY,initLength,snakeLength,fruitx,fruity,randomTime;

int apple(int snakeBody[][2]) { //Primary Author: Robert Fanzutti ; Main Functionality: creates a trophy at a random position on the screen that is not inside of the snake
    fruity = rand() % (COLS - 3) + 2;
    fruitx = rand() % (LINES - 3) + 2;
    int index = 0;
    while (index < snakeLength) {
      if(snakeBody[index][0] == fruity && snakeBody[index][1] == fruitx){
        fruity = rand() % (COLS - 3) + 2;
        fruitx = rand() % (LINES - 3) + 2;
        index = 0;
     }
     index++;
    }
    randomTime = (rand() % 9) + 1;
    char valueString[10];
    sprintf(valueString, "%d", randomTime);
    mvprintw(fruitx, fruity, "%s", valueString);
    return randomTime;
}

int newApple(int snakeBody[][2]) { // Primary Author: Robert Fanzutti ; Main Functionality: used when the trophy disappears or when the trophy is eaten by the snake
    mvprintw(fruitx, fruity, " ");
    return apple(snakeBody);
}

void moveSnake(int snakeBody[][2],int dirX,int dirY) { //Primary Author: Mehmet Balik ; Main Functionality: controls the movement of the snake including changing directions
    for(int i = 0; i < initLength; i++) {
        mvprintw(snakeBody[i][1], snakeBody[i][0], " ");
    }

    for(int i = initLength; i > 0; i--) {
        snakeBody[i][0] = snakeBody[i-1][0];
        snakeBody[i][1] = snakeBody[i-1][1];
    }
      snakeBody[0][0] += dirX;
      snakeBody[0][1] += dirY;

    for(int i = 0; i < initLength; i++) {
        mvprintw(snakeBody[i][1], snakeBody[i][0], "S");
    }

    if(initLength < snakeLength) {
    initLength++;
    }
}

void drawBorder() { //Primary author Richard McBride ; Main Functionality: draws the border at the start of the game
    for(int i = 0; i < COLS; i++) { //Draw top and bottom borders
        mvprintw(0, i, "#");
        mvprintw(LINES-1, i, "#");
    }

    for(int i=1;i<LINES-1;i++) { //Draw left and right borders
        mvprintw(i, 0, "#");
        mvprintw(i, COLS-1, "#");
    }
}

void handleInput(int keyPressed){ //Primary Author: Mehmet Balik ; Main Functionality: changes the direction values based on input from the user (using arrow keys)
    if(keyPressed==KEY_UP) {
        dirY = -1, dirX = 0;
    }else if(keyPressed==KEY_DOWN) {
        dirY = 1, dirX = 0;
    }else if(keyPressed==KEY_LEFT) {
        dirY = 0, dirX = -1;
    }else if(keyPressed==KEY_RIGHT) {
        dirY = 0, dirX = 1;
    }
}

bool borderDeath(int snakeBody[][2]) { //Primary Author: Richard McBride ; Main Functionality: when the snake runs into the border you lose
    if(snakeBody[0][0] >= COLS-1 || snakeBody[0][0] <= 0){
        return false;
    }    
    
    if(snakeBody[0][1] >= LINES-1 || snakeBody[0][1]<= 0){
        return false;
    }

    return true;
}

void randomStart(int randValues[]) { //Primary Author: Richard McBride ; Main Functionality: The snake starts in a random direction
    int randomX, randomY, randomDirX, randomDirY;
    int min = -1;
    int max = 1;
    srand(time(0));
    randomDirX = min + rand() % (max - min + 1);
    
    if(randomDirX != 0){
        randomDirY = 0;
    } else{
        randomDirY = min + rand() % (max - min + 1);
        while(randomDirY == 0){
            randomDirY = min + rand() % (max - min + 1);
        }
    }
    randValues[0] = randomDirX;
    randValues[1] = randomDirY;
}

bool collisionDeath(int snakeBody [][2], int snakeLength) { //Primary Author: Richard McBride ; Main Functionality: Snake will die if it collides with itself or it tries to go in the opposite direction that it is currently moving
  for(int i = 1; i < snakeLength; i++){
    if(snakeBody[i][0] == snakeBody[0][0] && snakeBody[i][1] == snakeBody[0][1]) {
        return false;
    }
  }
  return true;
}

bool victory(int snakeLength) { //Primary Author: Richard Mcbride ; Main Functionality: Game ends with a victory if the snake length becomes the size of half of the perimeter of the snake pit
 if(snakeLength >= (COLS - 1) + (LINES - 1)) {
    return true;
 } 
 return false;
}

void gameOutcome(int snakeLength, int snakeBody[][2], bool isVictory) { //Primary Author: Mehmet Balik ; Main Functionality: Determines if game was won or lost and prints the corresponding message
        char valueString [ COLS + LINES ];
        sprintf(valueString, "%d", snakeLength-5);
    
    if (isVictory) {
        clear();
        mvprintw(((LINES - 1) / 2) - 1, ((COLS-1) / 2) -3, "You won!");
    } else {
        mvprintw(snakeBody[0][1], snakeBody[0][0], "X");
        refresh();
        sleep(1);
        clear();
        mvprintw(((LINES - 1) / 2) - 1, ((COLS-1) / 2) -3, "You lost!");
    }
    mvprintw(((LINES - 1) / 2), ((COLS-1) / 2) -3, "Points:");
    mvprintw(((LINES - 1) / 2), ((COLS-1) / 2) + 4, "%s", valueString);
    refresh();
    sleep(5);
}

int main(){ //Worked on by each member
    initscr();
    curs_set(0);
    timeout(0);
    noecho();
    keypad(stdscr, TRUE);

    int sleepTime = 100000;
    int index = 0;

    int randValues[4];
    randomStart(randValues);
    int snakeBody[COLS + LINES][2];
    snakeBody[0][0] = COLS / 2;
    snakeBody[0][1] = LINES / 2;
    initLength = 1;
    snakeLength = 5;
    dirX = randValues[0];
    dirY = randValues[1];

    signal(SIGINT, SIG_IGN);

    drawBorder();
    apple(snakeBody);
    refresh();
    
    while(borderDeath(snakeBody) && collisionDeath(snakeBody,snakeLength) && !victory(snakeLength)) {
        
        int keyPressed = getch();
        if(keyPressed != ERR){
            handleInput(keyPressed);                     
        }
        moveSnake(snakeBody, dirX, dirY);

        if(snakeBody[0][1] == fruitx && snakeBody[0][0] == fruity) {
            snakeLength += randomTime;
            apple(snakeBody);
            sleepTime = sleepTime - (sleepTime / 20);
            index = 0;
        }

        if (index >= ((randomTime*1000000)+250000)) { 
            newApple(snakeBody);
            index = 0;
        }

        refresh();
        index += sleepTime;
        usleep(sleepTime);
        
    }
    
    gameOutcome(snakeLength, snakeBody, victory(snakeLength));

    endwin();
    return 0;
}