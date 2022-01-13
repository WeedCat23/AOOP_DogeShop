#ifndef ADDGOODS_LIST_H
#define ADDGOODS_LIST_H

#include <QDialog>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include "card.h"

namespace Ui {
class AddGoods_list;
}

class AddGoods_list : public QDialog
{
    Q_OBJECT

public:
    vector<Card_in_shop> shop_v;
    vector<QLineEdit *> num_in_v;
    vector<QLineEdit *> price_in_v;

    explicit AddGoods_list(QWidget *parent = nullptr);
    AddGoods_list(int flag = 0, QWidget *parent = nullptr);
    ~AddGoods_list();
    virtual void card_grid_layout(QGridLayout *grid);
    void clear_layout(QLayout* layout);
    void clear_lineEdit_v();

private slots:
    virtual void on_add_clicked();

    virtual void on_sort_box_currentTextChanged(const QString &arg1);

    virtual void on_search_clicked();

private:
    Ui::AddGoods_list *ui;
    vector<Card> all_card;
    vector<Card> sub_v;

    void reject() override;
};

#endif // ADDGOODS_LIST_H
