//*----------------------------
//* Javier Marín; 2018.        
//* This is the code for  
//* my own implementation of
//* the snake game.
//*----------------------------
#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include "Player.h"

int main() {
    // Start ncurses.
    initscr();
    noecho();
    cbreak();

    // Get screen size.
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    // Create the box in which the game will be played and create a box on it.
    WINDOW *boxwin = newwin(12, yMax=46, xMax=12, 64);
    box(boxwin, 0, 0);

    // Create the box which will be the score handler and create a box on it.
    WINDOW *scorewin = newwin(12, yMax=32, xMax=12, 32);
    box(scorewin, 0, 0);

    // Refresh and refresh game and score handler window.
    refresh();
    wrefresh(boxwin);
    wrefresh(scorewin);

    // Create a pointer to a new Player object.
    Player *x = new Player(boxwin, scorewin, 1, 1, 'x', 'o');

    // Main game loop; while user doesn't press 'X' and the game
    // hasn't ended, don't exit the game.
    do {
        if (x->isEnd() == true)
            break;
    }
    while (x->getMov() != 'x');

    // Wait user types something before exiting game.
    getch();

    // End ncurses.
    endwin();
    return 0;
}
