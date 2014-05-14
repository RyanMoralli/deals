#ifndef TAXFU_H
#define TAXFU_H

#include <Dacti.h>


class Taxfu : public Dacti
{
    public:
        Taxfu(string name, int weight, int defense);
        virtual ~Taxfu();
        int defense;
    protected:
    private:
};

#endif // TAXFU_H
