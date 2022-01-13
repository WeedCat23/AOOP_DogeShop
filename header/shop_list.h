#ifndef SHOP_LIST_H
#define SHOP_LIST_H

#include <QDialog>
#include <QSqlResult>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "addgoods_list.h"
#include "customer.h"

namespace Ui {
class Shop_list;
}

class Shop_list : public AddGoods_list
{
    Q_OBJECT

public:
    explicit Shop_list(Customer *&cp, QWidget *parent = nullptr);
    ~Shop_list();
    void card_grid_layout(QGridLayout *grid) override;

signals:
    void update_money_request();
    void update_bag_request();

private:
    Ui::Shop_list *ui;
    vector<Card_in_shop> sub_v;
    QSqlDatabase database;
    QSqlQuery *query;
    Customer *c;

    void on_add_clicked() override;
    void on_sort_box_currentTextChanged(const QString &arg1) override;
    void on_search_clicked() override;

    void reject() override;

    void sub_layout(QTableWidget *tableWidget, int i);
};

#endif // SHOP_LIST_H
