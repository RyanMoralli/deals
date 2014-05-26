#include <iostream>
#include <conio.h>
#include "Map.h"
#include "ParsedMap.h"
#include "Bandit.h"
using namespace std;

/*
Danlu - biologically animate, creature
Dacti - an object
SorcuDacti - a store of dacti
Taxfu - something that can be worn
Xarci - weapon
*/

void printZone(Zone &z){
    for(int i=0;i<z.size;i++){
        Space *s = z.spaces[i];
        cout << s;
        if(s){
            cout << "(" << s->cord.x << "," << s->cord.y << ")" << " ";
            Danlu *d = s->danlu;
            cout << d << " ";
            if(d){
                cout << d->getName() << " ";
            }
            SorcuDacti *sorcu = s->sorcu;
            if(sorcu){
                cout << " (inv)";
            }
            cout << endl;
        }
    }
}
void printDanlu(Zone &z){
    for(int i=0;i<z.size;i++){
        Space *s = z.spaces[i];
        //cout << s;
        if(s){
            Danlu *d = s->danlu;
            //cout << d << " ";
            if(d && d->getHealth() > 0){
                cout << "(" << s->cord.x << "," << s->cord.y << ")" << " ";
                cout << d->getName() <<"("<<d->getChar()<<")" << " ";
                cout << d->getHealth() << "/" << d->getMaxHealth() << " ";
                cout << endl;
            }
        }
        //cout << endl;
    }
}
void displayZone(Zone &z, Danlu *d, int sel){
    int pos=0;
    int max = z.radius*2+1;
    Inv inv = d->sorcu->getInv();
    for(int i=0;i<max;i++){
        cout << endl;
        for(int j=0;j<max;j++){
            Space *s = z.spaces[pos];
            if(s){
                if(s->danlu){
                    cout << s->danlu->getChar();
                }else{
                    if(s->sorcu && s->sorcu->size()>0){
                        cout << ',';
                    }else{
                        cout << '.';
                    }
                }
            }else{
                cout << 'X';
            }

            pos++;
        }
        if(i==0){
            cout << d->getHealth()<<"/"<<d->getMaxHealth();
            cout << " "<<d->sorcu->getLoad()<<"/"<<d->sorcu->getMaxLoad();
        }else if(i >= max-inv.size){
            int j = i-(max-inv.size);
            if(sel == j)
                cout << '*';
            else
                cout << ' ';
            cout << inv.dacti[j]->getName() <<"("<<inv.dacti[j]->getWeight()<<")";
        }
    }
    delete[] inv.dacti;
}

void printCombat(Danlu *atk, CombatResult cr){
    cout << endl;
    cout << atk->getHealth()<<"/"<<atk->getMaxHealth()<<endl;
    cout << cr.targetHealth<<"/"<<cr.targetMaxHealth<<" "<<cr.damage;
}

void printCombat(CombatResult cr, Danlu *atk){
    cout << endl;
    cout << cr.targetHealth<<"/"<<cr.targetMaxHealth<<" "<<cr.damage<<endl;
    cout << atk->getHealth()<<"/"<<atk->getMaxHealth()<<endl;
}

void printInv(Inv sorcu, int sel){
    for(int i=0;i<sorcu.size;i++){
        if(i==sel){
            cout << "*";
        }else{
            cout << " ";
        }
        cout << sorcu.dacti[i]->getName() <<"("<<sorcu.dacti[i]->getWeight()<<")" << endl;
    }
}

void moveItem(SorcuDacti *src, SorcuDacti *dest, Dacti *dacti){
    try{
        src->remove(dacti);
    }catch(int e){
        return;
    }
    try{
        dest->add(dacti);
    }catch(int e){
        src->add(dacti);
    }
}

short int getDir(Cord ai, Cord p){
    if(ai.y == p.y){
        if(ai.x > p.x){
            return LEFT;
        }return RIGHT;
    }
    short int sign = (ai.y>p.y) ? -1:1;
    if(ai.x==p.x){
        return 1*sign;
    }
    if(ai.x>p.x){
        return 7*sign;
    }
    return 3*sign;
}

void delZone(Zone &z){
    delete[] z.spaces;
}

Zone getZone(Zone &old, Map &m, Cord cord, int vision){
    delZone(old);
    return m.getZone(cord, vision);
}

void moveAI(Map &m, Bandit *d, Danlu *player, double turn){
    Zone z = m.getZone(m.find(d));
    for(int i=0;i<z.size;i++){
        if(z.spaces[i] && z.spaces[i]->danlu && z.spaces[i]->danlu == player){
            CombatResult cr = m.attack(m.find(d),m.find(player));
            printCombat(cr, d);
            return;
        }
    }
    if((int)turn%d->speed != 0)
        return;
    delZone(z);
    z = m.getZone(m.find(d), d->agroRange);
    for(int i=0;i<z.size;i++){
        if(z.spaces[i] && z.spaces[i]->danlu && z.spaces[i]->danlu == player){
            Cord ai = z.center;
            Cord p = z.spaces[i]->cord;
            try{
                m.move(z.center, getDir(ai, p));
            }catch(int e){}
            return;
        }
    }
    delZone(z);
}

int main(){
    cout << "Deals v1." << endl;

    ParsedMap *mP = new ParsedMap("map.txt");
    Map m = *mP;
    Danlu *player = mP->getPlayer();
    int banditCount = mP->getBanditCount();
    Bandit **bandits = mP->getBandits();

    Cord pCord = m.find(player);

    int inp=-1;
    int vision = 10;
    Zone z = m.getZone(pCord, vision);
    bool locked = true;
    bool noTurn = false;
    bool noDisp = false;
    double turnCount=1;

    bool selInv=true;
    int sel = 0;
    int gSel = 0;
    bool isFKey = false;
    bool isArrowKey = false;
    int xp = 0;
    while(player->getHealth() > 0){
        inp=-1;
        inp = getch();
        cout << string(100, '\n');//clear the console
        if(xp >= 10){//check if level up
            xp=0;
            player->levelUp();
        }
        if(inp!=-1){
            if(inp == 27)
                break;
            if(inp == 224){//next key is arrow key 75/77 left right
                isArrowKey = true;
                noTurn = true;
            }else if(isArrowKey){
                if(inp == 72){//up arrow
                    Inv inv;
                    cout << endl;
                    sel--;
                    if(sel < 0)
                        sel = 0;
                    if(selInv){
                        inv = player->sorcu->getInv();
                        printInv(inv, sel);
                        delete[] inv.dacti;
                    }else{
                        inv = m.getSpace(pCord)->danlu->sorcu->getInv();
                        printInv(inv, sel);
                        delete[] inv.dacti;
                    }
                    noTurn = true;
                }else if(inp == 80){//down arrow
                    Inv inv;
                    cout << endl;
                    sel++;
                    if(selInv){
                        inv = player->sorcu->getInv();
                        if(sel > inv.size-1){
                            sel = inv.size-1;
                        }
                        printInv(inv, sel);
                    }else{
                        inv = m.getSpace(pCord)->danlu->sorcu->getInv();
                        if(sel > inv.size-1){
                            sel = inv.size-1;
                        }
                        printInv(inv, sel);
                        delete[] inv.dacti;
                    }
                    noTurn = true;
                }else if(inp == 77){//right arrow
                    Inv inv = m.sorcu(pCord)->getInv();
                    gSel++;
                    if(gSel >= inv.size){
                        gSel = 0;
                    }
                    printInv(inv, gSel);
                    delete[] inv.dacti;
                }
                isArrowKey = false;
                noTurn = true;
            }else if(inp == 0){//next input is function key
                isFKey = true;
                noTurn = true;
            }else if(isFKey){
                if(inp == 59){//f1
                    vision --;
                    if(vision != 1){
                        delZone(z);
                        z = getZone(z, m, z.center, vision);
                    }
                    noTurn = true;
                }else if(inp == 60){//f2
                    vision++;
                    delZone(z);
                    z = getZone(z, m, z.center, vision);
                    noTurn = true;
                }else if(inp == 61){//f3
                    locked = !locked;
                    cout << "Lock:" << locked;
                    noTurn = true;
                }else if(inp == 62){//f4
                    printDanlu(z);
                    noTurn = true;
                }else if(inp == 63){//f5
                    pCord = m.find(player);
                    noTurn = true;
                }else if(inp == 64){//f6
                    printZone(z);
                    noTurn = true;
                }else if(inp == 65){//f7
                    cout << endl;
                    Inv inv = player->sorcu->getInv();
                    printInv(inv, sel);
                    delete[] inv.dacti;
                    noTurn = true;
                }else if(inp == 66){//f8
                    cout << endl;
                    Space *s = m.getSpace(pCord);
                    if(s->sorcu){
                        Inv inv = s->sorcu->getInv();
                        printInv(inv, gSel);
                        delete[] inv.dacti;
                    }
                    noTurn = true;
                }else if(inp == 67){//f9
                    Dacti *a = new Dacti("object", 10);
                    player->sorcu->add(a);
                    cout << "add"<<a;
                    noTurn = true;
                }else if(inp == 68){//f10
                    string name;
                    cout << "Name:";
                    getline(cin, name);
                    Dacti *d = new Dacti(name, 5);
                    player->sorcu->add(d);
                    noTurn = true;
                }
                noTurn = true;
                isFKey = false;
            }else if(inp == 97){//A
                try{
                    pCord = m.move(pCord, LEFT);
                }catch(int e){}
                if(locked)z = getZone(z, m, pCord, vision);
            }else if(inp == 100){//D
                try{
                    pCord = m.move(pCord, RIGHT);
                }catch(int e){}
                if(locked)z = getZone(z, m, pCord, vision);
            }else if(inp == 119){//W
                try{
                    pCord = m.move(pCord, UP);
                }catch(int e){}
                if(locked)z = getZone(z, m, pCord, vision);
            }else if(inp == 115){//S
                try{
                    pCord = m.move(pCord, DOWN);
                }catch(int e){}
                if(locked)z = getZone(z, m, pCord, vision);
            }else if(inp == 101){//E
                try{
                    pCord = m.move(pCord, UPR);
                }catch(int e){}
                if(locked)z = getZone(z, m, pCord, vision);
            }else if(inp == 113){//Q
                try{
                    pCord = m.move(pCord, UPL);
                }catch(int e){}
                if(locked)z = getZone(z, m, pCord, vision);
            }else if(inp == 122){//Z
                try{
                    pCord = m.move(pCord, DOWNL);
                }catch(int e){}
                if(locked)z = getZone(z, m, pCord, vision);
            }else if(inp == 99){//C
                try{
                    pCord = m.move(pCord, DOWNR);
                }catch(int e){}
                if(locked)z = getZone(z, m, pCord, vision);
            }else if(inp == 120){//X
                player->heal(1);
            }else if(inp == 87){//shift W
                z=m.getZone(m.shift(z.center, UP), vision);
                noTurn = true;
            }else if(inp == 65){//shift S
                z=m.getZone(m.shift(z.center, LEFT), vision);
                noTurn = true;
            }else if(inp == 83){//shift S
                z=m.getZone(m.shift(z.center, DOWN), vision);
                noTurn = true;
            }else if(inp == 68){//shift D
                z=m.getZone(m.shift(z.center, RIGHT), vision);
                noTurn = true;
            }else if(inp == 105){//I
                try{
                    CombatResult cr = m.attack(pCord, UP);
                    printCombat(player, cr);
                    if(cr.killed == true){
                        xp++;
                    }
                }catch(int e){noTurn = true;}
            }else if(inp == 106){//J
                try{
                    CombatResult cr = m.attack(pCord, LEFT);
                    printCombat(player, cr);
                    if(cr.killed == true){
                        xp++;
                    }
                }catch(int e){noTurn = true;}
            }else if(inp == 107){//K
                try{
                    CombatResult cr = m.attack(pCord, DOWN);
                    printCombat(player, cr);
                    if(cr.killed == true){
                        xp++;
                    }
                }catch(int e){noTurn = true;}
            }else if(inp == 108){//L
                try{
                    CombatResult cr = m.attack(pCord, RIGHT);
                    printCombat(player, cr);
                    if(cr.killed == true){
                        xp++;
                    }
                }catch(int e){noTurn = true;}
            }else if(inp == 46){//. pick up item
                SorcuDacti *gSorcu = m.sorcu(pCord);
                SorcuDacti *pSorcu = player->sorcu;
                Inv inv = gSorcu->getInv();
                Dacti *dacti = inv.dacti[gSel];
                delete[] inv.dacti;
                moveItem(gSorcu, pSorcu, dacti);
                noTurn=true;
            }else if(inp == 44){//, drop item
                SorcuDacti *gSorcu = m.sorcu(pCord);
                SorcuDacti *pSorcu = player->sorcu;
                Inv inv = pSorcu->getInv();
                Dacti *dacti = inv.dacti[sel];
                delete[] inv.dacti;
                moveItem(pSorcu, gSorcu, dacti);
                noTurn=true;
            }else if(inp == 47){// / equip item
                Inv inv = player->sorcu->getInv();
                if(sel < inv.size){
                    Dacti *d = inv.dacti[sel];
                    try{
                        player->equip(d);
                    }catch(int e){
                        cout << "Equip Error:" << e << endl;
                    }
                }
                delete[] inv.dacti;
                noTurn = true;
            }else if(inp == 39){//' unequip items
                player->unequip();
                noTurn=true;
            }else if(inp == 63){//? inspect item
                Inv inv = player->sorcu->getInv();
                Dacti *dacti = inv.dacti[sel];
                delete[] inv.dacti;
                Xarci *x = dynamic_cast<Xarci*>(dacti);
                Taxfu *t = dynamic_cast<Taxfu*>(dacti);
                if(x){
                    cout << x->getName();
                    cout << "("<<x->getWeight()<<")"<<endl;
                    cout << "Damage: "<<x->damage<<endl;
                    cout << "Pierce: "<<x->pierce<<endl;
                }else if(t){
                    cout << t->getName();
                    cout << "("<<t->getWeight()<<")"<<endl;
                    cout << "Armor: "<<t->defense<<endl;
                }
                cout << endl;
                noTurn = true;
            }else{
                cout << "Key:" << inp << endl;
                noTurn = true;
            }
            //cout << "Key:" << inp << endl;
        }
        if(!noTurn){
            cout << inp << " " << player->getHealth() << "/" << player->getMaxHealth() << " Turn:" << turnCount;
            for(int i=0;i<banditCount;i++){
                if(bandits[i] != nullptr && bandits[i]->getHealth() > 0){
                    moveAI(m, bandits[i], player, turnCount);
                }
            }
            //cout << endl;
            turnCount++;
        }
        if(!noDisp){
            displayZone(z, player, sel);
        }
        noTurn = false;
        noDisp = false;
    }
    cout << endl << "You Lose." << endl;
    cin.ignore();

    //m.setSpace(3, 3, 4);
    //cout << m.getSpace(3, 3)->placeholder << endl;
    //cout << m.getSpace(9, 9)->placeholder << endl;
    //cout << LEFT;
    return 0;
}
