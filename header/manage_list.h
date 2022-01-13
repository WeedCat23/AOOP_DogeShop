#ifndef MANAGE_LIST_H
#define MANAGE_LIST_H

#include <QDialog>
#include "addgoods_list.h"

namespace Ui {
class Manage_list;
}

class Manage_list : public AddGoods_list
{
    Q_OBJECT

public:
    explicit Manage_list(QWidget *parent = nullptr);
    ~Manage_list();
    void card_grid_layout(QGridLayout *grid) override;

private:
    Ui::Manage_list *ui;
    vector<Card_in_shop> sub_v;

    void on_add_clicked() override;
    void on_sort_box_currentTextChanged(const QString &arg1) override;
    void on_search_clicked() override;

    void reject() override;

    void sub_layout(QTableWidget *tableWidget, int i);
};

#endif // MANAGE_LIST_H
