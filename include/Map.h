#ifndef MAP_H
#define MAP_H

#include "Danlu.h"
#include "Dacti.h"

const short int UP = 1;
const short int DOWN = -1;
const short int LEFT = -2;
const short int RIGHT = 2;
const short int UPR = 3;
const short int UPL = 7;
const short int DOWNR = -3;
const short int DOWNL = -7;

struct Cord{
    int x=0;
    int y=0;
};

struct Space{
    Cord cord;
    Danlu *danlu = nullptr;
    SorcuDacti *sorcu = nullptr;
};

struct CombatResult{
    int targetHealth=0;
    int targetMaxHealth=0;
    Attack attack;
    int damage=0;
    bool killed = false;
};

struct Zone{
    Cord center;
    Space **spaces = nullptr;
    int radius = 0;
    int size = 0;
};

class Map{
    public:
        Map(int, int);
        virtual ~Map();

        /*Gives a new position that is in direction from position given*/
        Cord shift(int x, int y, short int dir);
        Cord shift(Cord, short int dir);
        Space* getSpace(int, int);
        Space* getSpace(Cord);
        /*throws 1 if no object in space
        throws 2 if target space occupied
        throws 3 if space does not exist
        throws 4 if target space does not exist*/
        Cord move(Cord, Cord);
        Cord move(int, int, short int dir);
        Cord move(Cord, short int dir);

        /*throws 1 if danlu is not on map*/
        Cord find(Danlu*);

        /*throws 1 if out of bounds*/
        SorcuDacti *sorcu(Cord cord);

        /*throws 1 if out of bounds cord
        throws 2 if no danlu in spaces*/
        CombatResult attack(int x1, int y1, int x2, int y2);
        CombatResult attack(Cord, Cord);
        CombatResult attack(Cord, short int dir);

        Zone getZone(int x, int y, int r);
        Zone getZone(Cord, int radius);
        Zone getZone(int x, int y);
        Zone getZone(Cord);
        Zone getZone();//untested
    protected:
        Map();
        int width;
        int height;
        Space ***spaces;
        void buildSpaces(int width, int height);
        bool inBounds(int x, int y);
        bool inBounds(Cord);
        Danlu *wall = nullptr;
    private:

};

#endif // MAP_H
