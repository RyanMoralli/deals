#ifndef SORCUDACTI_H
#define SORCUDACTI_H

#include "Dacti.h"

struct InvSlot{
    Dacti *dacti = nullptr;
    InvSlot *next = nullptr;
};

struct Inv{
    Dacti **dacti = nullptr;
    int size = 0;
};

class SorcuDacti
{
    public:
        SorcuDacti();
        SorcuDacti(int maxLoad);
        virtual ~SorcuDacti();
        int getLoad(){return load;}
        int getMaxLoad(){return maxLoad;}
        /*throws 1 if new size is smaller than current size*/
        void increaseMaxLoad(int maxLoad);

        /*throws 1 if item is nullptr or does not fit in inventory*/
        void add(Dacti*);
        /*throws 1 if item is not in inventory*/
        void remove(Dacti*);
        int size(){return invSize;}
        Inv getInv();
    protected:
    private:
        InvSlot *inv = nullptr;
        int invSize = 0;
        int maxLoad=0;
        int load=0;
};

#endif // SORCUDACTI_H
