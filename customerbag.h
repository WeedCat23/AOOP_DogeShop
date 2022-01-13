#ifndef CUSTOMERBAG_H
#define CUSTOMERBAG_H
#include"header/card.h"
#include<vector>
using namespace std;
enum sorttype{
  longest,most,type
};
class CustomerBag
{
    vector<Card_in_bag> deck;

public:
    CustomerBag();
    bool filter_on=false;
    void addCard(Card_in_bag*);
    int deck_size(){return deck.size();}
    vector<Card_in_bag> & getdeck(){return deck;}

};
class Card_sorter{
    sorttype s_type;
public:
    Card_sorter(sorttype chosen){
        s_type=chosen;
    }
    bool operator()(Card_in_bag a,Card_in_bag b){
        switch (s_type) {
            case longest:
                if(a.name.length()==b.name.length())
                   return a.name.compare(b.name)<0;
                return a.name.length()>b.name.length();
                break;
            case most:
                if(a.num==b.num)
                    return a.name.compare(b.name)<0;
                return a.num>b.num;
                break;
            case type:
                if(a.type==b.type)
                    return a.name.compare(b.name)<0;
                else if(a.type=="monster")
                    return true;
                else if(a.type=="magic"&&b.type!="monster")
                    return true;
                else
                    return false;
        }
        return true;
    }
};

#endif // CUSTOMERBAG_H
