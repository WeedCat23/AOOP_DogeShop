#ifndef EXCHANGE_POPUP_H
#define EXCHANGE_POPUP_H

#include <QDialog>
#include"customer.h"

namespace Ui {
class Exchange_popup;
}
enum mode{
  point_to_cash,card_to_point
};
class Exchange_popup : public QDialog
{
    Q_OBJECT

public:
    explicit Exchange_popup(Customer*,QWidget *parent = nullptr);
    explicit Exchange_popup(Customer*,int,QWidget *parent = nullptr);
    ~Exchange_popup();
signals:
    void update_money_request();
    void update_bag_request(Card_in_bag);
private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_btn_confirm_clicked();

private:
    Ui::Exchange_popup *ui;
    Customer* x;
    Card_in_bag card_x;
    int ratio;
    mode m;
    int index;
};

#endif // EXCHANGE_POPUP_H
