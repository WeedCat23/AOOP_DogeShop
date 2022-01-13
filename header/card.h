#ifndef CARD_H
#define CARD_H

//#include <QString>
#include <string>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

using namespace std;

class Card
{
public:
    string name;
    string type;
    string url;

    void load_pic(const string &strPicUrl, QLabel &lable, int height);
    void detail();
};

class Card_in_shop: public Card
{
public:
    int num;
    int price;
    QString state;

    void set_data(Card x, int num, int price);
};
class Card_in_bag: public Card
{
public:
    int num;
    bool star;


    Card_in_bag(){}
    Card_in_bag(string SQLname,string SQLtype,string SQLurl,int amount,bool like)
    {
        name=SQLname;
        type=SQLtype;
        url=SQLurl;
        num=amount;
        star=like;
    }
    void set_data(Card_in_shop x, int num);
    void change_star();
};
#endif // CARD_H
