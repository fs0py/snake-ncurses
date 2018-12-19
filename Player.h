#include <vector>
#include <ncurses.h>
#ifndef PLAYER_H
#define PLAYER_H

class Player {
    public:
        Player(WINDOW *win, WINDOW *scwin, int y, int x, char chara, char ap);
        void mvup();
        void mvdown();
        void mvleft();
        void mvright();
        void display();
        void setApple();
        void checkApple();
        void score();
        void endGame();
        void add();
        void collision();
        void appleCollision();
        void debug();
        void refreshScreen();
        bool isEnd();
        bool hasCollided();
        int getMov();
    private:
        WINDOW *gamewin;
        WINDOW *scorewin;
        int yLoc, xLoc;
        int yMax, xMax;
        int apxLoc, apyLoc;
        int userMovement;
        int apC;
        int tailLong;
        char apple;
        char character;
        bool broken;
        bool collided;
        std::vector<int> xTail;
        std::vector<int> yTail;
};
#endif
