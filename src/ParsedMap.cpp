#include "ParsedMap.h"
#include <fstream>
#include <cstdlib>

#include <iostream>
using namespace std;

/*Splits a string at delim.*/
string* getArgs(string &line, short int count, char delim=' '){
    int pos = 2;
    short int i = 0;
    string *res = new string[count];
    while(i < count){
        int rPos = line.find_first_of(delim, pos);
        res[i] = line.substr(pos, rPos-pos);
        pos = rPos+1;
        i++;
    }
    return res;
}

ParsedMap::ParsedMap(string file):Map::Map(){
    ifstream input(file);
    int y = 0;
    Stats wstats;
    wstats.stamina = 0;
    wall = new Danlu("Wall", 'X', wstats);
    Dacti **dacti;
    for(string line;getline(input, line);){
        if(line.size() != 0 && line.at(0) != '#'){
            //cout << line << endl;
            char op = line.at(0);
            if(op == 'n'){
                int xDim;
                int yDim;
                xDim = atoi(line.substr(2, 4).c_str());
                yDim = atoi(line.substr(7, 4).c_str());
                buildSpaces(xDim, yDim);
                y = yDim-1;
            }else if(op == 's'){
                string* args = getArgs(line, 2);
                if(args[0] == "maxid"){
                    dSize = atoi(args[1].c_str());
                    danlu = new Bandit*[dSize];
                    for(int i=0;i<dSize;i++){
                        danlu[i] = nullptr;
                    }
                }else if(args[0] == "maxitem"){
                    int dactiCount = atoi(args[1].c_str());
                    dacti = new Dacti*[dactiCount];
                }else{
                    cout << "Invalid set" << args[0] << "." << endl;
                }
            }else if(op == 'p'){
                string *args;
                if(line.at(1)=='`'){
                    args = getArgs(line, 4, '`');
                }else{
                    args = getArgs(line, 4);
                }
                Stats stats;
                stats.strength = atoi(args[2].c_str());
                stats.stamina = atoi(args[3].c_str());
                player = new Danlu(args[1], args[0].at(0), stats);
            }else if(op == 'b'){
                string *args;
                if(line.at(1)=='`'){
                    args = getArgs(line, 6, '`');
                }else{
                    args = getArgs(line, 6);
                }
                Stats stats;
                stats.strength = atoi(args[3].c_str());
                stats.stamina = atoi(args[4].c_str());
                danlu[atoi(args[0].c_str())] = new Bandit(args[2], args[1].at(0), stats, atoi(args[5].c_str()));
            }else if(op == 'i'){
                string *args;
                if(line.at(1)=='`'){
                    args = getArgs(line, 6, '`');
                }else{
                    args = getArgs(line, 6);
                }
                Dacti *d;
                int weight = atoi(args[3].c_str());
                if(args[5].at(0) == '-'){
                    if(args[4].at(0) == '-'){
                        d = new Dacti(args[2], weight);
                    }else{
                        int defense = atoi(args[4].c_str());
                        d = new Taxfu(args[2], weight, defense);
                    }
                }else{
                    int damage = atoi(args[4].c_str());
                    int pierce = atoi(args[5].c_str());
                    d = new Xarci(args[2], weight, damage, pierce);
                }
                char action = args[0].at(0);
                int id = atoi(args[1].c_str());
                if(action == 'e'){
                    if(id == -1){
                        player->sorcu->add(d);
                        player->equip(d);
                    }else{
                        danlu[id]->sorcu->add(d);
                        danlu[id]->equip(d);
                    }
                }else if(action == 'i'){
                    if(id == -1)
                        player->sorcu->add(d);
                    else
                        danlu[id]->sorcu->add(d);
                }else if(action == 'g'){
                    dacti[id] = d;
                }else{
                    cout << "Invalid action for i " << args[0] << endl;
                }
            }else if(op == ':'){
                string *args = getArgs(line, width);
                for(int x=0;x<width;x++){
                    if(args[x].at(0) == '.'){
                        //
                    }else if(args[x].at(0) == 'X'){
                        spaces[x][y]->danlu = wall;
                    }else if(args[x].at(0) == 'i'){
                        Cord c;c.x=x;c.y=y;
                        int id = atoi(args[x].substr(1).c_str());
                        sorcu(c)->add(dacti[id]);
                    }else if(args[x].at(0) == 'p'){
                        spaces[x][y]->danlu = player;
                    }else if(args[x].at(0) == 'b'){
                        int id = atoi(args[x].substr(1).c_str());
                        spaces[x][y]->danlu = danlu[id];
                    }
                }
                y--;
            }
        }
    }
}

ParsedMap::~ParsedMap(){

}
