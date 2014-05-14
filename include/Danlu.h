#ifndef DANLU_H
#define DANLU_H

#include "SorcuDacti.h"
#include "Taxfu.h"
#include "Xarci.h"
#include <string>
using namespace std;

struct Stats{
    int strength=0;
    int stamina=0;
};

struct Attack{
    int damage=0;
    int pierce=0;
};

struct Equiped{
    Xarci *mainHand = nullptr;
    Taxfu *armor = nullptr;
};

class Danlu{
    public:
        Danlu(string name, char repChar, Stats);
        virtual ~Danlu();
        string getName(){return name;}
        char getChar(){return repChar;}
        int getHealth(){return health;}
        int getMaxHealth(){return stats.stamina*10;}
        /*Increases strength and stamina by 1*/
        void levelUp();

        Attack getAttack();
        int applyAttack(Attack);
        void heal(int h);

        SorcuDacti *sorcu;
        /*Equips an item to a slot based on type.
        throws 2 if item is not equipable
        throws 1 if item can not be removed from inventory
        throws 3 if old item can not be placed in inventory*/
        void equip(Dacti*);
        /*Places all equiped items into inventory*/
        void unequip();
    protected:
    private:
        string name;
        char repChar=0;
        Stats stats;
        Equiped equiped;
        int health = 0;

};

#endif // DANLU_H
