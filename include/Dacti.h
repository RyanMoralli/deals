#ifndef DACTI_H
#define DACTI_H

#include <string>
using namespace std;

class Dacti
{
    public:
        Dacti(std::string name, int weight);
        virtual ~Dacti();

        string getName(){return name;}
        int getWeight(){return weight;}
    protected:
    private:
        string name;
        int weight;
};

#endif // DACTI_H
