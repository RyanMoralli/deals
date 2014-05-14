#ifndef PARSEDMAP_H
#define PARSEDMAP_H

#include <string.h>
#include <Map.h>
#include "Bandit.h"


class ParsedMap : public Map
{
    public:
        /*Parses map at location file.
        Map file is assumed to be properly formated.
        Does not catch any thrown exceptions.*/
        ParsedMap(string file);
        virtual ~ParsedMap();

        Danlu* getPlayer(){return player;}
        int getBanditCount(){return dSize;}
        Bandit** getBandits(){return danlu;}
    protected:
    private:
        Danlu *player;
        Bandit **danlu;
        int dSize = 0;
};

#endif // PARSEDMAP_H
