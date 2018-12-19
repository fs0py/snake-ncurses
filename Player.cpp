#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iterator>
#include "Player.h"

Player::Player(WINDOW *win, WINDOW *scwin, int y, int x, char chara, char ap) {
    srand(time(NULL));

    gamewin = win;
    scorewin = scwin;
    keypad(gamewin, true);

    getmaxyx(gamewin, yMax, xMax);
    xLoc = rand() % xMax;
    yLoc = rand() % yMax;

    character = chara;
    apple = ap;
    apC = 0;
    tailLong = 1;
    broken = false;
    collided = true;

    setApple();
    add();
    display();
    refreshScreen();
}

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

void Player::display() {

    auto yEnd = std::end(yTail);
    auto xEnd = std::end(xTail);
    if (tailLong > 1) {
        mvwaddch(gamewin, *(yEnd-tailLong-1), *(xEnd-tailLong-1), ' ');
        mvwaddch(gamewin, yLoc, xLoc, 'x');
    }
    else {
        mvwaddch(gamewin, *(yEnd-1), *(xEnd-1), ' ');
        mvwaddch(gamewin, yLoc, xLoc, 'x');
    }
    refreshScreen();
}

void Player::setApple() {
    apxLoc = rand() % xMax;
    apyLoc = rand() % yMax;
    
    // I guess this can be improved... Let's leave this like it actually is, by now.
    for (auto i = std::begin(xTail), j = std::begin(yTail); i != std::end(xTail) && j != std::end(yTail); i++, j++)
        if (apxLoc == *i && apyLoc == *j) {
            apxLoc = rand() % xMax;
            apyLoc = rand() % yMax;
            appleCollision();
        }

    mvwaddch(gamewin, apyLoc, apxLoc, apple);
}

void Player::checkApple() {
    if (yLoc == apyLoc && xLoc == apxLoc) {
        setApple();
        score();
        refreshScreen();
    }
}

void Player::score() {
    tailLong++;
    apC++;
    wattron(scorewin, A_REVERSE);
    mvwprintw(scorewin, 1, 2, "Apples ate: %i", apC);
    wattroff(scorewin, A_REVERSE);
}

void Player::endGame() {
    mvwprintw(scorewin, 4, 2, "You've lost!");
    mvwprintw(scorewin, 5, 2, "Press any key to exit.");
    broken = true;
    collided = true;
}

void Player::add() {
    xTail.push_back(xLoc);
    yTail.push_back(yLoc);
}

void Player::collision() {
    // Collision system isn't 100% precise so we'll remove a few features (bugs) on it.
    // Just... Don't fucking trust this.

    if (yLoc < 1)
        endGame();

    if (yLoc > yMax-2)
        endGame();

    if (xLoc < 1)
        endGame();

    if (xLoc > xMax-2)
        endGame();

    // 100% features!
    // Don't even uncomment this, there's no tail vector.
    /*
    for (auto i = std::begin(tail); i != std::end(tail); i++)
        if (yLoc == *(i-1) && xLoc == *(i-2))
            endGame();
    */
}

void Player::appleCollision() {
    if (apyLoc < 1)
        apyLoc = 1;

    if (apyLoc > yMax-2)
        apyLoc = yMax-2;

    if (apxLoc < 1)
        apxLoc = 1;

    if (apxLoc > xMax-2)
        apxLoc = xMax-2;
}

void Player::debug() {
    mvwprintw(scorewin, 4, 2, "apxLoc: %i, apyLoc: %i", apxLoc, apyLoc);
    mvwprintw(scorewin, 6, 2, "xLoc: %i, yLoc: %i", xLoc, yLoc);
}

void Player::refreshScreen() {
    wrefresh(scorewin);
    if (!collided)
        wrefresh(gamewin);
}

bool Player::isEnd() {
    return broken;
}

bool Player::hasCollided() {
    return collided;
}

int Player::getMov() {
    userMovement = wgetch(gamewin);

    switch (userMovement) {
        case KEY_UP:
            mvup();
            checkApple();
            collision();
            display();
            break;
        case KEY_DOWN:
            mvdown();
            checkApple();
            collision();
            display();
            break;
        case KEY_RIGHT:
            mvright();
            checkApple();
            collision();
            display();
            break;
        case KEY_LEFT:
            mvleft();
            checkApple();
            collision();
            display();
            break;
        case 'X':
        case 'x':
            mvwprintw(scorewin, 4, 2, "Thanks for playing, human!!!");
            mvwprintw(scorewin, 5, 2, "Press any key to exit");
            mvwprintw(scorewin, 6, 2, "You're such a good boy!");
            broken = false;
            refreshScreen();
            break;
    }

    return userMovement;
}
