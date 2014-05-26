#include "SorcuDacti.h"

SorcuDacti::SorcuDacti():SorcuDacti(0){

}

SorcuDacti::SorcuDacti(int maxLoad)
{
    this->maxLoad = maxLoad;
}

SorcuDacti::~SorcuDacti(){

}

void SorcuDacti::increaseMaxLoad(int maxLoad){
    if(maxLoad >= this->maxLoad){
        this->maxLoad = maxLoad;
    }else{
        throw 1;
    }
}

void SorcuDacti::add(Dacti *dacti){
    if(dacti == nullptr || (load+dacti->getWeight() > maxLoad && maxLoad != 0)){
        throw 1;
    }

    InvSlot *newSlot = new InvSlot;
    newSlot->dacti=dacti;
    if(!inv){
        inv = newSlot;
    }else{
        InvSlot *slot = inv;
        for(int i=0;i<invSize-1;i++){
            slot = slot->next;
        }
        slot->next=newSlot;
    }

    load += dacti->getWeight();
    invSize++;
}

void SorcuDacti::remove(Dacti *dacti){
    if(invSize==0 || dacti==nullptr){
        throw 1;
    }
    if(inv->dacti == dacti){
        InvSlot *next = inv->next;
        delete inv;
        inv = next;
        invSize--;
        load-=dacti->getWeight();
        return;
    }
    InvSlot *slot = inv;
    for(int i=0;i<invSize-1;i++){
        if(slot->next->dacti == dacti){
            InvSlot *next = inv->next->next;
            delete slot->next;
            slot->next = next;
            invSize--;
            load-=dacti->getWeight();
            return;
        }
        slot = slot->next;
    }
    throw 1;
}

Inv SorcuDacti::getInv(){
    Inv inv;
    inv.size = invSize;
    inv.dacti = new Dacti*[invSize];
    InvSlot *slot = this->inv;
    for(int i=0;i<invSize;i++){
        inv.dacti[i] = slot->dacti;
        slot = slot->next;
    }
    return inv;
}
