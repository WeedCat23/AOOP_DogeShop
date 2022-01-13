#include"customer.h"
#include<random>
//#include<QTimer>
bool Customer::purchase(int price){
    if(money.isaffordable(price))
    {
        money.spendCash(price);
        return true;
    }
    return false;
}
void Customer::addToBag(Card_in_bag *ptr)
{
    bag.addCard(ptr);
}
void Customer::earnPoint(double a)
{
    if(a<0)
        money.addPoint(((rand()+1.)/RAND_MAX)*50);
    else
        money.addPoint(a);
}
void Customer::exchangePoint(int share)
{
    money.addPoint(share*(-20)).addCash(share);
}
void Customer::set_point(double p)
{
    money.setPoint(p);
}



