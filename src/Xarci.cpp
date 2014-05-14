#include "Xarci.h"

Xarci::Xarci(string name, int weight, int damage):Dacti(name, weight){
    this->damage = damage;
}

Xarci::Xarci(string name, int weight, int damage, int pierce):Dacti(name, weight){
    this->damage = damage;
    this->pierce = pierce;
}

Xarci::~Xarci()
{

}
