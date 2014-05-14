#include "Danlu.h"

Danlu::Danlu(string name, char repChar, Stats stats){
    this->name = name;
    this->repChar = repChar;
    this->stats = stats;
    this->health = getMaxHealth();
    sorcu = new SorcuDacti(stats.strength*100);
}

Danlu::~Danlu()
{

}

void Danlu::levelUp(){
    stats.strength++;
    stats.stamina++;
    sorcu->increaseMaxLoad(stats.strength*100);
}

Attack Danlu::getAttack(){
    Attack atk;
    atk.damage = stats.strength;
    atk.pierce = stats.strength/10;
    if(equiped.mainHand){
        atk.damage+=equiped.mainHand->damage;
        atk.pierce+=equiped.mainHand->pierce;
    }
    return atk;
}

int Danlu::applyAttack(Attack atk){
    int damage = atk.damage;
    if(equiped.armor){
        damage -= equiped.armor->defense-atk.pierce;
    }
    if(damage < 0)//ensure attack will not heal
        damage = 0;
    health -= damage;
    return damage;
}

void Danlu::heal(int h){
    health += h;
    if(health > getMaxHealth())
        health = getMaxHealth();
}

void Danlu::equip(Dacti *dacti){
    Xarci *x = dynamic_cast<Xarci*>(dacti);
    Taxfu *t = dynamic_cast<Taxfu*>(dacti);

    if(x){//item is a weapon
        if(equiped.mainHand){
            try{
                sorcu->add(equiped.mainHand);
                equiped.mainHand = nullptr;
            }catch(int e){
                throw 3;
            }
        }
        equiped.mainHand = x;
    }else if(t){//item is armor
        if(equiped.armor){
            try{
                sorcu->add(equiped.armor);
                equiped.armor = nullptr;
            }catch(int e){
                throw 3;
            }
        }
        equiped.armor = t;
    }else{//item is not equip-able
        throw 2;
    }

    try{
        sorcu->remove(dacti);
    }catch(int e){
        if(x)
            equiped.mainHand = nullptr;
        if(t)
            equiped.armor = nullptr;
        throw 1;
    }
}

void Danlu::unequip(){
    try{
        sorcu->add(equiped.mainHand);
    }catch(int e){}
    equiped.mainHand = nullptr;
    try{
        sorcu->add(equiped.armor);
    }catch(int e){}
    equiped.armor = nullptr;
}






