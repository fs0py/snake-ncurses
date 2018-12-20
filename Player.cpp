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


    start_color();
    keypad(gamewin, true);
    getmaxyx(gamewin, yMax, xMax);

    xLoc = rand() % xMax-1;
    yLoc = rand() % yMax-1;

    character = chara;
    apple = ap;
    apC = 0;
    tailLong = 1;
    broken = false;

    add();
    setApple();
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
        mvwaddch(gamewin, *(yEnd-tailLong), *(xEnd-tailLong), ' ');
        mvwaddch(gamewin, yLoc, xLoc, 'x');
    }

    refreshScreen();
}

void Player::setApple() {
    apxLoc = rand() % xMax-1;
    apyLoc = rand() % yMax-1;
    
    // I guess this can be improved... Let's leave this like it actually is, by now.
    // Sorry, you->name-san!!!
    for (auto i = (std::end(xTail))-tailLong-1, j = (std::end(yTail))-tailLong-1; i != (std::end(xTail))-1 && j != (std::end(yTail))-1; i++, j++)
        while (apxLoc == *i && apyLoc == *j) {
            apxLoc = rand() % xMax-1;
            apyLoc = rand() % yMax-1;
            appleCollision();
        }
    mvwaddch(gamewin, apyLoc, apxLoc, apple);
}

void Player::checkApple() {
    if (yLoc == apyLoc && xLoc == apxLoc) {
        score();
        setApple();
        refreshScreen();
    }
}

void Player::score() {
    tailLong++;
    apC++;
    init_pair(1, COLOR_WHITE, COLOR_MAGENTA);
    wbkgdset(scorewin, COLOR_PAIR(1));
    mvwprintw(scorewin, 1, 2, "Apples ate: %i", apC);
}

void Player::endGame() {
    mvwprintw(scorewin, 2, 2, "You've lost!");
    mvwprintw(scorewin, 3, 2, "Press any key to exit.");
    broken = true;
}

void Player::add() {
    xTail.push_back(xLoc);
    yTail.push_back(yLoc);
}

void Player::collision() {
    if (yLoc < 1) {
        yLoc = 1;
        endGame();
    }

    if (yLoc > yMax-1) {
        yLoc = yMax-1;
        endGame();
    }

    if (xLoc < 1) {
        xLoc = 1;
        endGame();
    }

    if (xLoc > xMax-1) {
        xLoc = xMax-1;
        endGame();
    }

    // I mean... Isn't like I did this monster code in purpose...
    // I just... B-be gentle to me, you->name-san!!!
    if (tailLong > 1)
        for (auto i = (std::end(xTail))-tailLong-1, j = (std::end(yTail))-tailLong-1; i != (std::end(xTail))-1 && j != (std::end(yTail))-1; i++, j++)
            if (xLoc == *i && yLoc == *j)
                endGame();
}

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

void Player::debug() {
    mvwprintw(scorewin, 4, 2, "apxLoc: %i, apyLoc: %i", apxLoc, apyLoc);
    mvwprintw(scorewin, 6, 2, "xLoc: %i, yLoc: %i", xLoc, yLoc);
}

void Player::refreshScreen() {
    wrefresh(scorewin);
    if (!broken)
        wrefresh(gamewin);
}

bool Player::isEnd() {
    return broken;
}

int Player::getMov() {
    userMovement = wgetch(gamewin);

    switch (userMovement) {
        case KEY_UP:
            mvup();
            collision();
            checkApple();
            display();
            break;
        case KEY_DOWN:
            mvdown();
            collision();
            checkApple();
            display();
            break;
        case KEY_RIGHT:
            mvright();
            collision();
            checkApple();
            display();
            break;
        case KEY_LEFT:
            mvleft();
            collision();
            checkApple();
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
        case 'j':
            setApple();
            checkApple();
            refreshScreen();
    }
    return userMovement;
}
