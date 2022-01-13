#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "person.h"
#include "customerbag.h"
#include "header/card.h"

class Customer: public Person
{
private:

    CustomerBag bag;
    //Cart cart;

public:
    Customer(int _id,QString&name,QString&pass,int c=0,double p=0):Person(_id,name,pass)
    {
        money.addCash(c).addPoint(p);
    };
    ~Customer(){};
    void earnPoint(double a=-1);
    void exchangePoint(int);
    bool purchase(int);
    void addToBag(Card_in_bag*);
    int get_money_cash(){return money.getCash();}
    double get_money_point(){return money.getPoint();}
    void set_point(double);
    int get_deck_in_bag_size(){return bag.deck_size();}
    vector<Card_in_bag> & mybag(){return bag.getdeck();}
    bool is_filter_on(){return bag.filter_on;}
    void set_filter(bool a){bag.filter_on=a;}
};



#endif // CUSTOMER_H
