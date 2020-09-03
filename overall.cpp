#include "poker.h"

void next(int &i){
    if(i == 1 || i == 0)
        i++;
    else
        i = 0;
}

bool Poker::isLeftOne(int who){
    return (no == 0 && who == 1) || (no == 1 && who == 0) || (no == 2 && who == 0);
}

Card convertToCard(int number){
    Card tmp;
    tmp.type = (number-1) / 13;
    tmp.number = number - tmp.type * 13;
//qDebug() << number << "->" << tmp.type << " " << tmp.number;
    return tmp;
}

bool Comp(const Card& a,const Card& b){
    if(a.type == 4 && b.type == 4)
        return a.number > b.number ? 1 : 0;
    else if(a.type == 4 || b.type == 4){
        return a.type == 4 ? 1 : 0;
    }else{
        return a.number > b.number ? 1 : 0;
    }
}
