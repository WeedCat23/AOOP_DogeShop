#include "header/card.h"
#include "header/carddetail_window.h"
#include <QEventLoop>

void Card::load_pic(const string &strPicUrl, QLabel &lable, int height)
{
    QUrl url(QString().fromStdString(strPicUrl));
    QNetworkAccessManager manager;
    QEventLoop loop;

    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, &QNetworkReply::finished, &loop, [&reply, &lable, &loop, height]()
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray jpegData = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(jpegData);
            if (!pixmap.isNull())
            {
                 lable.clear();
                 lable.setPixmap(pixmap.scaledToHeight(height));
            }
            else
                lable.setText("No Image");
        }
        else
            qDebug() << "Network error!";
        loop.quit();
    });

   loop.exec();
}

void Card::detail()
{
    CardDetail_window *detail_window = new CardDetail_window;
    detail_window->setWindowTitle("卡片詳細");
    detail_window->set_layout(this->name, this->type, this->url);
    detail_window->show();
}

void Card_in_shop::set_data(Card x, int num, int price)
{
    this->name = x.name;
    this->type = x.type;
    this->url = x.url;
    this->num = num;
    this->price = price;
    this->state = "NEW";
}

void Card_in_bag::set_data(Card_in_shop x, int num)
{
    this->name = x.name;
    this->type = x.type;
    this->url = x.url;
    this->num = num;
    this->star = false;
}

void Card_in_bag::change_star()
{
    star = !star;
}
