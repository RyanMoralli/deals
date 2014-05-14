#include "Map.h"

Map::Map(){

}

Map::Map(int width, int height){
    buildSpaces(width, height);
}

void Map::buildSpaces(int width, int height){
    this->width = width;
    this->height = height;
    spaces = new Space **[width];
    for(int i=0;i<width;i++){
        spaces[i] = new Space *[height];
        for(int j=0;j<height;j++){
            Space *space = new Space;
            Cord cord;
            cord.x = i;
            cord.y = j;
            space->cord = cord;
            spaces[i][j] = space;
        }
    }
}

Map::~Map(){

}

bool Map::inBounds(int x, int y){
    return (x>=0 && y>=0 && x<width && y<height);
}

bool Map::inBounds(Cord cord){
    return inBounds(cord.x, cord.y);
}

Cord Map::shift(int x, int y, short int dir){
    if(dir%2==0){
        x += dir/2;
    }else if(dir%3==0){
        x += 1;
        y += dir/3;
    }else if(dir%7==0){
        x -= 1;
        y += dir/7;
    }else{
        y += dir;
    }
    Cord cord;
    cord.x = x;
    cord.y = y;
    return cord;
}

Cord Map::shift(Cord cord, short int dir){
    return shift(cord.x, cord.y, dir);
}

Space* Map::getSpace(int x, int y){
    if(inBounds(x, y)){
        return spaces[x][y];
    }
    return nullptr;
}

Space* Map::getSpace(Cord cord){
    return getSpace(cord.x, cord.y);
}


Cord Map::move(Cord from, Cord to){
    if(!inBounds(from))throw 3;
    if(spaces[from.x][from.y]->danlu == nullptr)throw 1;
    if(!inBounds(to))throw 4;
    Space *newSpace = spaces[to.x][to.y];
    if(newSpace->danlu != nullptr)throw 2;

    newSpace->danlu = spaces[from.x][from.y]->danlu;
    spaces[from.x][from.y]->danlu = nullptr;
    return to;
}

Cord Map::move(Cord cord, short int dir){
    return move(cord, shift(cord, dir));
}

Cord Map::find(Danlu* danlu){
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            if(spaces[i][j]->danlu == danlu){
                Cord cord;
                cord.x = i;
                cord.y = j;
                return cord;
            }
        }
    }
    throw 1;
}


SorcuDacti *Map::sorcu(Cord cord){
    if(!inBounds(cord.x, cord.y)){
        throw 1;
    }
    SorcuDacti *sorcu = spaces[cord.x][cord.y]->sorcu;
    if(sorcu == nullptr){
        sorcu = new SorcuDacti(0);
        spaces[cord.x][cord.y]->sorcu = sorcu;
    }
    return sorcu;
}

CombatResult Map::attack(int x1, int y1, int x2, int y2){
    if(!inBounds(x1, y1) || !inBounds(x2, y2)){
        throw 1;
    }
    Danlu *target = spaces[x2][y2]->danlu;
    if(target == wall){
        throw 2;
    }
    Danlu *attacker = spaces[x1][y1]->danlu;
    if(attacker && target){
        Attack atk = attacker->getAttack();
        int dmg = target->applyAttack(atk);
        CombatResult cr;
        if(target->getHealth() <=0){//if dead place items on ground
            Cord sCord;
            sCord.x = x2;
            sCord.y = y2;
            SorcuDacti *sSorcu = sorcu(sCord);
            SorcuDacti *dSorcu = spaces[x2][y2]->danlu->sorcu;
            spaces[x2][y2]->danlu->unequip();
            Inv inv = dSorcu->getInv();
            for(int i=0;i<inv.size;i++){
                sSorcu->add(inv.dacti[i]);
            }
            spaces[x2][y2]->danlu = nullptr;
            cr.killed = true;
        }

        cr.targetHealth = target->getHealth();
        cr.targetMaxHealth = target->getMaxHealth();
        cr.attack = atk;
        cr.damage = dmg;
        return cr;
    }
    throw 2;
}
CombatResult Map::attack(Cord attacker, Cord target){
    return attack(attacker.x, attacker.y, target.x, target.y);
}
CombatResult Map::attack(Cord attacker, short int dir){
    return attack(attacker, shift(attacker, dir));
}

Zone Map::getZone(int x, int y, int radius){
    int size = (radius*2+1)*(radius*2+1);
    Space **zoneS = new Space*[size];
    for(int i=0;i<size;i++){
        zoneS[i] = new Space[size];
    }
    int pos = 0;
    for(int i=y+radius;i>=y-radius;i--){
        for(int j=x-radius;j<=x+radius;j++){
            if(inBounds(j, i)){
                zoneS[pos] = spaces[j][i];
            }else{
                zoneS[pos] = nullptr;
            }
            pos++;
        }
    }
    Zone zone;
    zone.center.x = x;
    zone.center.y = y;
    zone.spaces = zoneS;
    zone.radius = radius;
    zone.size = size;
    return zone;
}
Zone Map::getZone(Cord cord, int radius){
    return getZone(cord.x, cord.y, radius);
}
Zone Map::getZone(int x, int y){
    return getZone(x, y, 1);
}
Zone Map::getZone(Cord cord){
    return getZone(cord, 1);
}

Zone Map::getZone(){
    int r=0;
    if(width > height)
        r = width;
    else
        r = height;
    return getZone(width/2, height/2, r/2);
}



