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
        void setapple();
        void checkapple();
        void score();
        void endgame();
        bool is_end();
        int getMov();
    private:
        WINDOW *gamewin;
        WINDOW *scorewin;
        int yLoc, xLoc;
        int yMax, xMax;
        int apxLoc, apyLoc;
        char character;
        int userMovement;
        char apple;
        int apC;
        bool broken;
        std::vector<int> snakeLong;
};
#endif
