#ifndef BANDIT_H
#define BANDIT_H

#include <Danlu.h>


class Bandit : public Danlu
{
    public:
        //Bandit();
        Bandit(string name, char repChar, Stats stats):Danlu(name, repChar, stats){}
        Bandit(string name, char repChar, Stats stats, int agroRange):Danlu(name, repChar, stats){
            this->agroRange = agroRange;
        }
        //virtual ~Bandit();
        int speed = 1;
        int agroRange = 5;
    protected:
    private:
};

#endif // BANDIT_H
