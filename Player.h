#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <ncurses.h>

class Player {
    public:
        Player(WINDOW *win, WINDOW *scwin, int y, int x, char chara, char ap);
        void add();
        void appleCollision();
        void checkApple();
        void collision();
        void display();
        void debug();
        void endGame();
        void mvup();
        void mvdown();
        void mvleft();
        void mvright();
        void setApple();
        void score();
        void refreshScreen();
        bool isEnd();
        int getMov();
    private:
        WINDOW *gamewin;
        WINDOW *scorewin;
        int apxLoc, apyLoc;
        int yLoc, xLoc;
        int yMax, xMax;
        int userMovement;
        int apC;
        int tailLong;
        char apple;
        char character;
        bool broken;
        std::vector<int> xTail;
        std::vector<int> yTail;
};
#endif
