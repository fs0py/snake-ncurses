#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iterator>
#include "Player.h"

Player::Player(WINDOW *win, WINDOW *scwin, int y, int x, char chara, char ap) {

    // This is used for randomization
    // of the creation of apples
    // (make them appear in random places).
    srand(time(NULL));
    
    // Create a pointer to game window
    // and score window.
    gamewin = win;
    scorewin = scwin;

    // If terminal supports
    // colors, make the score window
    // appear with a magenta foreground.
    if (has_colors) {
        start_color();
        init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
        wbkgdset(scorewin, COLOR_PAIR(1));
    }
    // Enable keypad and get the max
    // y and x location of the game window.
    // (the borders).
    keypad(gamewin, true);
    getmaxyx(gamewin, yMax, xMax);
    
    // Set snake's x and y to a random
    // number between the max y and x
    // of the game window minus 1
    // (just to don't go to the edges
    // and keep the snake in the box).
    xLoc = rand() % xMax-1;
    yLoc = rand() % yMax-1;

    // Set what character will be used
    // for drawing the snake and the
    // apple; set the apple counter
    // to 0, the snake body to 1;
    // set the broken boolean to
    // false.
    character = chara;
    apple = ap;
    apC = 0;
    tailLong = 1;
    broken = false;

    // Add actual values of xLoc
    // and yLoc to xTail and yTail,
    // respectively.
    add();

    // Set apple into the game.
    setApple();

    // Teaches user how to play.
    intro();

    // Set snake into the game.
    display();

    // Refresh the screen.
    refreshScreen();
}

// All the move functions
// (mvup, mvdown, mvleft, mvright)
// work as follows:
// Call the add function for
// saving the xLoc and yLoc
// actual values and modify them.
// For example: the mvup function
// calls the add function and
// substracts 1 to yLoc.

void Player::mvup() {
    add();
    yLoc--;
}

void Player::mvdown() {
    add();
    yLoc++;
}

void Player::mvleft() {
    add();
    xLoc--;
}

void Player::mvright() {
    add();
    xLoc++;
}

// The display function work
// as follows:
// The variables yEnd and xEnd
// saves the actual end of
// yTail and xTail, respectively.
// With those, we print the snake
// and refresh the screen.
// (More details inside the function!)

void Player::display() {
    auto yEnd = std::end(yTail);
    auto xEnd = std::end(xTail);

    if (tailLong > 1) {
        // If apple's x and y location are "behind" the snake,
        // don't put a space there (or it would override the
        // apple from the game). Else, do it and show the
        // snake.
        if (apxLoc != *xEnd-tailLong-1 && apyLoc != *yEnd-tailLong-1)
            mvwaddch(gamewin, *(yEnd-tailLong-1), *(xEnd-tailLong-1), ' ');
        mvwaddch(gamewin, yLoc, xLoc, character);
    }

    else {
        // If apple's x and y location aren't "behind" the snake,
        // don't put a space there (or it would override the
        // apple from the game). Else, do it and show the
        // snake.
        if (apxLoc != *xEnd-tailLong && apyLoc != *yEnd-tailLong)
            mvwaddch(gamewin, *(yEnd-tailLong), *(xEnd-tailLong), ' ');
        mvwaddch(gamewin, yLoc, xLoc, character);
    }

    refreshScreen();
}

// The setApple function
// work as follows:
// It changes the apple
// x and y to a random value
// between 0 and the edges
// of xMax and yMax, respectively,
// and check if the apple isn't
// inside the snake.
// (More details inside!

void Player::setApple() {
    apxLoc = rand() % xMax-1;
    apyLoc = rand() % yMax-1;
    
    // This iterates all over the snake.
    // While the x and y value of the apple is on the body of the snake,
    // create a new value for it.
    // This will be iterated all over the snake.
    for (auto i = (std::end(xTail))-tailLong, j = (std::end(yTail))-tailLong; i != (std::end(xTail))-1 && j != (std::end(yTail))-1; i++, j++)
        while (apxLoc == *i && apyLoc == *j) {
            apxLoc = rand() % xMax-1;
            apyLoc = rand() % yMax-1;
            appleCollision();
        }
    appleCollision();

    mvwaddch(gamewin, apyLoc, apxLoc, apple);
}

// The checkApple function
// work as follows:
// It checks if the snake
// has reached the apple;
// if it has, call
// the score, setApple
// and refreshScreen.
// If it hasn't, don't
// do nothing.

void Player::checkApple() {
    if (yLoc == apyLoc && xLoc == apxLoc) {
        score();
        setApple();
        refreshScreen();
    }
}

// The score function
// work as follows:
// Add 1 to tailLong,
// apC and print the
// how many apples
// have been eat
// until now.

void Player::score() {
    tailLong++;
    apC++;
    mvwprintw(scorewin, 1, 2, "Apples ate: %i", apC);
}

// The endGame function
// work as follows:
// Tells the user
// he has lost and
// tell him he needs
// to press any key
// to exit the game
// and set the broken
// boolean to true.

void Player::endGame() {
    mvwprintw(scorewin, 2, 2, "You've lost!");
    mvwprintw(scorewin, 3, 2, "Press any key to exit.");
    broken = true;
}

// The intro function
// work as follows:
// Tells the user
// how to play and
// tells him he hasn't
// ate any apple -logically-.

void Player::intro() {
    mvwprintw(scorewin, 1, 2, "Apples ate: %i", apC);
    mvwprintw(scorewin, 4, 2, "WASD - MOVE");
    mvwprintw(scorewin, 5, 2, "X - CLOSE");
    mvwprintw(scorewin, 6, 2, "THAT'S ALL :)");
}

// The add function
// work as follows:
// Add the xLoc and yLoc
// integeres into the
// xTail and yTail vectors,
// respectively.

void Player::add() {
    xTail.push_back(xLoc);
    yTail.push_back(yLoc);
}

// The collision function
// work as follows:
// Check if snake is in the
// game box, if it isn't
// (or is trying to cross the
// border), don't make him pass.
// It also checks if the snake
// has touched himself (no pun
// intended), if it has, end
// the game.
//
void Player::collision() {

    // It's a feature in this snake
    // game that you can't lose when you
    // haven't eat any apple.
    if (yLoc < 1) {
        yLoc = 1;
    }

    if (yLoc > yMax-2) {
        yLoc = yMax-2;
    }

    if (xLoc < 1) {
        xLoc = 1;
    }

    if (xLoc > xMax-2) {
        xLoc = xMax-2;
    }

    // If snake is bigger than
    // default's one, (...)
    if (tailLong > 1)
        // Iterate over the snake;
        // if it has touched himself,
        // end the game.
        for (auto i = (std::end(xTail))-tailLong-1, j = (std::end(yTail))-tailLong-1; i != (std::end(xTail))-1 && j != (std::end(yTail))-1; i++, j++)
            if (xLoc == *i && yLoc == *j)
                endGame();
}

// The appleCollision function
// work as follows:
// Checks if apple x and y location
// are outside the game box; if it's
// outside the box, change their
// x or y values.

void Player::appleCollision() {
    if (apyLoc < 1)
        apyLoc = 1;

    if (apyLoc > yMax-1)
        apyLoc = yMax-1;

    if (apxLoc < 1)
        apxLoc = 1;

    if (apxLoc > xMax-1)
        apxLoc = xMax-1;
}

// The refreshScreen function
// work as follows:
// Refresh the score window
// and the -if player hasn't
// loss the game- game
// window.

void Player::refreshScreen() {
    wrefresh(scorewin);
    if (!broken)
        wrefresh(gamewin);
}

// The isEnd function
// work as follows:
// Returns broken's value.

bool Player::isEnd() {
    return broken;
}

// The altEnd function
// work as follows:
// Thanks the user
// for playing the game,
// notify him to press
// any key to exit
// and tell him a good
// thing, set the
// broken boolean to false
// and call the refreshScreen
// function.

void Player::altEnd() {
    mvwprintw(scorewin, 4, 2, "Thanks for playing, human!!!");
    mvwprintw(scorewin, 5, 2, "Press any key to exit");
    mvwprintw(scorewin, 6, 2, "You're such a good boy!");
    broken = false;
    refreshScreen();
}

// The getMov function work
// as follows:
// Requests user input from the player;
// If user input's the normal movement
// keys (arrow keys or WASD),
// call the respective move function,
// the collision function, the
// checkApple function and
// the display function.
// If user input's the key 'X',
// call the altEnd function.
// Returns userMovement's value
// (user input).

int Player::getMov() {
    userMovement = wgetch(gamewin);

    switch (userMovement) {
        case KEY_UP:
        case 'w':
        case 'W':
            mvup();
            collision();
            checkApple();
            display();
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            mvdown();
            collision();
            checkApple();
            display();
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            mvright();
            collision();
            checkApple();
            display();
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
            mvleft();
            collision();
            checkApple();
            display();
            break;
        case 'X':
        case 'x':
            altEnd();
            break;
    }
    return userMovement;
}

// 404 lines!
