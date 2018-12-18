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
    getmaxyx(gamewin, yMax, xMax);
    xLoc = rand() % xMax;
    yLoc = rand() % yMax;
    character = chara;
    apple = ap;
    broken = false;
    keypad(gamewin, true);
    add();
    setApple();
}

void Player::mvup() {
    add();
    yLoc--;
    collision();
    mvwaddch(gamewin, yLoc, xLoc, 'x');
}

void Player::mvdown() {
    add();
    yLoc++;
    collision();
    mvwaddch(gamewin, yLoc, xLoc, 'x');
}

void Player::mvleft() {
    add();
    xLoc--;
    collision();
    mvwaddch(gamewin, yLoc, xLoc, 'x');
}

void Player::mvright() {
    add();
    xLoc++;
    collision();
    mvwaddch(gamewin, yLoc, xLoc, 'x');
}

void Player::display() {
    debug();
    mvwaddch(gamewin, yLoc, xLoc, character);
}

void Player::setApple() {
    apxLoc = rand() % xMax;
    apyLoc = rand() % yMax;
    appleCollision();

    for (auto i = std::begin(tail); i != std::end(tail); i++)
        if (apyLoc == *i && apxLoc == *(i + 1)) {
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
    }
}

void Player::score() {
    apC++;
    mvwprintw(scorewin, 1, 2, "Apples ate: %i", apC);
}

void Player::endGame() {
    mvwprintw(scorewin, 3, 2, "You've lost!");
    mvwprintw(scorewin, 4, 2, "Press any key to exit.");
    broken = true;
}

void Player::add() {
    tail.push_back(xLoc);
    tail.push_back(yLoc);
}

void Player::collision() {
    // Collision system will not be enabled until it gets a
    // precise detection algorithm. WIP! Sorry!

    /*if (yLoc < 1)
      endGame();

      if (yLoc > yMax-2)
      endGame();

      if (xLoc < 1)
      endGame();

      if (xLoc > xMax-2)
      endGame();

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

    auto tailP = std::end(tail);
    if (*tailP-2 && *tailP-1)
        mvwprintw(scorewin, 5, 2, "xT: %i, yT: %i", *(tailP-2), *(tailP-1));

}

bool Player::isEnd() {
    return broken;
}

int Player::getMov() {
    userMovement = wgetch(gamewin);

    switch (userMovement) {
        case KEY_UP:
            mvup();
            checkApple();
            break;
        case KEY_DOWN:
            mvdown();
            checkApple();
            break;
        case KEY_RIGHT:
            mvright();
            checkApple();
            break;
        case KEY_LEFT:
            mvleft();
            checkApple();
            break;
        case 'X':
        case 'x':
            mvwprintw(scorewin, 6, 2, "Closing the game...");
            mvwprintw(scorewin, 7, 2, "Press any key to exit");
            wrefresh(scorewin);
            endGame();
            break;
    }

    return userMovement;
}
