#ifndef SHOP_WINDOW_H
#define SHOP_WINDOW_H

#include <QDialog>
#include <QSqlResult>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "addgoods_window.h"
#include "customer.h"

namespace Ui {
class Shop_window;
}

class Shop_window : public AddGoods_window
{
    Q_OBJECT

public:
    explicit Shop_window(Customer *&cp, QWidget *parent = nullptr);
    ~Shop_window();
    void card_grid_layout(int q, QGridLayout *grid, int row_idx) override;

signals:
    void update_money_request();
    void update_bag_request();

private:
    Ui::Shop_window *ui;
    vector<Card_in_shop> sub_v;
    QSqlDatabase database;
    QSqlQuery *query;
    Customer *c;

    void on_next_page_clicked() override;
    void on_previous_page_clicked() override;
    void on_add_clicked() override;
    void on_sort_box_currentTextChanged(const QString &arg1) override;
    void on_rownum_box_currentTextChanged(const QString &arg1) override;
    void on_search_clicked() override;

    void reject() override;

    void sub_layout(QGridLayout *grid, int idx, int i);
};

#endif // SHOP_WINDOW_H
