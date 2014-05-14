#ifndef XARCI_H
#define XARCI_H

#include <Dacti.h>
using namespace std;


class Xarci : public Dacti
{
    public:
        Xarci(string name, int weight, int damage);
        Xarci(string name, int weight, int damage, int pierce);
        virtual ~Xarci();

        int damage=2;
        int pierce=0;
    protected:
    private:
};

#endif // XARCI_H
