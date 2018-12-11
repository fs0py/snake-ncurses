#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iterator>
#include "Player.h"

Player::Player(WINDOW *win, WINDOW *scwin, int y, int x, char chara, char ap) {
    gamewin = win;
    scorewin = scwin;
    xLoc = x;
    yLoc = y;
    getmaxyx(gamewin, yMax, xMax);
    character = chara;
    apple = ap;
    broken = false;
    keypad(gamewin, true);
    srand(time(NULL));
    setapple();
}

void Player::mvup() {
    snakeLong.push_back(yLoc);
    snakeLong.push_back(xLoc);
    mvwaddch(gamewin, yLoc, xLoc, 'x');
    yLoc--;
    if (yLoc < 1)
        yLoc = 1;
    for (auto i = std::begin(snakeLong); i != std::end(snakeLong); i++)
        if (yLoc == *i && xLoc == *(i + 1))
            endgame();
}

void Player::mvdown() {
    snakeLong.push_back(yLoc);
    snakeLong.push_back(xLoc);
    mvwaddch(gamewin, yLoc, xLoc, 'x');
    yLoc++;
    if (yLoc > yMax-2)
        yLoc = yMax-2;
    for (auto i = std::begin(snakeLong); i != std::end(snakeLong); i++)
        if (yLoc == *i && xLoc == *(i + 1))
            endgame();
}

void Player::mvleft() {
    snakeLong.push_back(yLoc);
    snakeLong.push_back(xLoc);
    mvwaddch(gamewin, yLoc, xLoc, 'x');
    xLoc--;
    if (xLoc < 1)
        xLoc = 1;
    for (auto i = std::begin(snakeLong); i != std::end(snakeLong); i++)
        if (yLoc == *i && xLoc == *(i + 1))
            endgame();
}

void Player::mvright() {
    snakeLong.push_back(yLoc);
    snakeLong.push_back(xLoc);
    mvwaddch(gamewin, yLoc, xLoc, 'x');
    xLoc++;
    if (xLoc > xMax-2)
        xLoc = xMax-2;
    for (auto i = std::begin(snakeLong); i != std::end(snakeLong); i++)
        if (yLoc == *i && xLoc == *(i + 1))
            endgame();
}

int Player::getMov() {
    userMovement = wgetch(gamewin);
    switch (userMovement) {
        case KEY_UP:
            mvup();
            checkapple();
            break;
        case KEY_DOWN:
            mvdown();
            checkapple();
            break;
        case KEY_RIGHT:
            mvright();
            checkapple();
            break;
        case KEY_LEFT:
            mvleft();
            checkapple();
            break;
    }
    return userMovement;
}

void Player::display() {
    mvwaddch(gamewin, yLoc, xLoc, character);
}

void Player::setapple() {
    apxLoc = rand() % xMax;
    apyLoc = rand() % yMax;
    for (auto i = std::begin(snakeLong); i != std::end(snakeLong); i++)
        if (apyLoc == *i && apxLoc == *(i + 1)) {
            apxLoc = rand() % xMax;
            apyLoc = rand() % yMax;
            if (apyLoc < 1)
                apyLoc = 1;
            if (apyLoc > yMax-2)
                apyLoc = yMax-2;
            if (xLoc < 1)
                xLoc = 1;
            if (apxLoc > xMax-2)
                apxLoc = xMax-2;
        }
    mvwprintw(scorewin, 4, 2, "x %d, y %d", apxLoc, apyLoc);
    mvwaddch(gamewin, apyLoc, apxLoc, apple);
}

void Player::checkapple() {
    if (yLoc == apyLoc && xLoc == apxLoc) {
        setapple();
        score();
    }
}

void Player::score() {
    apC++;
    mvwprintw(scorewin, 1, 1, "Apples ate: %d", apC);
    mvwprintw(scorewin, 2, 2, "x %d, y %d", apxLoc, apyLoc);
}

void Player::endgame() {
    broken = true;
}

bool Player::is_end() {
    return broken;
}
