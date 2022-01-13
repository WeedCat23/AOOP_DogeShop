#include "header/managegoods_window.h"
#include "ui_managegoods_window.h"
#include "header/loading_window.h"
#include "header/csv.h"
#include <QIntValidator>

ManageGoods_window::ManageGoods_window(QWidget *parent) :
    AddGoods_window(1, parent),
    ui(new Ui::ManageGoods_window)
{
    ui->setupUi(this);

    page = 0;
    row_cards = 8;
    Csv *csvObj = new Csv;
    shop_v = csvObj->read_shop("../AOOP_DogeShop/src/shop.csv"); 
    delete csvObj;

    for(int i = 0; i < (int)shop_v.size(); i++)
        if(shop_v[i].state == "HOT" || shop_v[i].state == "CUT")
            shop_v[i].state = " ";

    sub_v = shop_v;

    ui->how_many->setText("第[" + QString::number(page + 1) +
                          "]頁，全[" + QString::number(shop_v.size()) + "]種商品");
    ui->shop_title->setText("DOGE SHOP - Manage");
    ui->add->setStyleSheet("QPushButton{background-color:rgba(212,109,104,100%); color:white; border-radius:0px;}"
                           "QPushButton:hover{background-color:rgba(183,78,73,100%); color:white;}");
    ui->next_page->setStyleSheet("QPushButton{background-color:rgba(61,61,61,100%); color:white; border-radius:0px;}"
                                 "QPushButton:hover{background-color:rgba(80,80,80,100%); color:white;}");
    ui->previous_page->setStyleSheet("QPushButton{background-color:rgba(61,61,61,100%); color:white; border-radius:0px;}"
                                     "QPushButton:hover{background-color:rgba(80,80,80,100%); color:white;}");

    card_grid_layout(row_cards, ui->up_gridLayout, 0);
    card_grid_layout(row_cards, ui->down_gridLayout, 1);
}

void ManageGoods_window::card_grid_layout(int q, QGridLayout *grid, int idx)
{
    grid->setRowMinimumHeight(0, 180);

    Card_in_shop *cardObj = new Card_in_shop;
    for(int i = 0; i < q && (2*q*page + i + row_cards*idx) < (int)shop_v.size(); i++)
    {
        QLabel *pic = new QLabel;
        cardObj->load_pic(shop_v[2*q*page + i + row_cards*idx].url, *pic, 140);
        grid->addWidget(pic, 0, i, Qt::AlignCenter);

        QLabel *name = new QLabel;
        name->setText(QString::fromStdString(shop_v[2*q*page + i + row_cards*idx].name));
        name->setStyleSheet("border:2px solid; font:bold;");
        grid->addWidget(name, 1, i, Qt::AlignCenter);

        sub_layout(grid, idx, i);

        QPushButton *button = new QPushButton(" 點此查看卡片詳細 ");
        button->setAutoDefault(false);
        button->setStyleSheet("QPushButton{background-color:rgba(217,182,80,100%);\
                              color:white; border-radius:0px; font:bold;}"
                              "QPushButton:hover{background-color:rgba(255,220,110,100%); color:rgb(61,61,61);}");
        connect(button, &QPushButton::clicked, this, [=](){shop_v[2*q*page + i + row_cards*idx].detail();});
        grid->addWidget(button, 4, i, Qt::AlignCenter);
    }
    delete cardObj;
}

void ManageGoods_window::sub_layout(QGridLayout *grid, int idx, int i)
{
    int q = row_cards;

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    QLabel *type = new QLabel;
    type->setText(QString::fromStdString(shop_v[2*q*page + i + q*idx].type));       //印卡片種類
    type->setStyleSheet("font:bold;");
    type->setAlignment(Qt::AlignCenter);
    hBoxLayout->addWidget(type, 4, Qt::AlignCenter);        //加到hlayout, stretch = 4

    if(shop_v[2*q*page + i + q*idx].state != " ")       //判斷有沒有HOT那些label
    {
        QLabel *state = new QLabel(shop_v[2*q*page + i + q*idx].state);
        if(state->text() == "NEW") state->setStyleSheet("font:bold; border:1px solid green; color:green; font-size:8px;");
        else if(state->text() == "HOT") state->setStyleSheet("font:bold; border:1px solid red; color:red; font-size:8px;");
        else if(state->text() == "CUT") state->setStyleSheet("font:bold; border:1px solid blue; color:blue; font-size:8px;");
        state->setAlignment(Qt::AlignCenter);
        hBoxLayout->addWidget(state, 1, Qt::AlignCenter);   //有就加到hlayout, stretch = 1
    }
    grid->addLayout(hBoxLayout, 2, i, Qt::AlignCenter);     //把hlayout加入grid的第i行第2列(卡名下面)


    QLabel *num = new QLabel("庫存" + QString::number(shop_v[2*q*page + i + q*idx].num));
    QLabel *price = new QLabel("價格" + QString::number(shop_v[2*q*page + i + q*idx].price));
    num->setAlignment(Qt::AlignCenter);
    price->setAlignment(Qt::AlignCenter);
    num->setStyleSheet("font:bold; color:red");
    price->setStyleSheet("font:bold; color:red");

    QLabel *num2 = new QLabel("補貨"), *price2 = new QLabel("改價");
    num2->setAlignment(Qt::AlignRight);
    price2->setAlignment(Qt::AlignRight);
    QLineEdit *num_in = new QLineEdit, *price_in = new QLineEdit;
    num_in->setValidator(new QIntValidator(-(shop_v[2*q*page + i + q*idx].num), 1000, this));
    price_in->setValidator(new QIntValidator(0, 10000, this));
    num_in_v.push_back(num_in);         //把輸入數量的LineEdit加入num_in_v
    price_in_v.push_back(price_in);     //把輸入價格的LineEdit加入price_in_v

    QGridLayout *subLayout = new QGridLayout;
    subLayout->addWidget(num, 0, 0, 1, 2, Qt::AlignCenter);
    subLayout->addWidget(price, 0, 2, 1, 2, Qt::AlignCenter);
    subLayout->addWidget(num2, 1, 0, Qt::AlignLeft);
    subLayout->addWidget(num_in, 1, 1, Qt::AlignLeft);
    subLayout->addWidget(price2, 1, 2, Qt::AlignLeft);
    subLayout->addWidget(price_in, 1, 3, Qt::AlignLeft);
    subLayout->setSpacing(2);
    grid->addLayout(subLayout, 3, i, Qt::AlignCenter);      //把sublayout加入grid的第i行第3列
}

ManageGoods_window::~ManageGoods_window()
{
    delete ui;
}

void ManageGoods_window::reject()
{
    remove("../AOOP_DogeShop/src/shop.csv");

    for(int i = 0; i < (int)sub_v.size(); i++)
        if(sub_v[i].num == 0)
        {
            sub_v.erase(sub_v.begin() + i);
            i--;
        }

    Csv *csvObj = new Csv;
    csvObj->save_shop_csv(sub_v, "../AOOP_DogeShop/src/shop.csv");
    delete csvObj;

    QDialog::reject();
}

void ManageGoods_window::on_next_page_clicked()
{
    page++;
    if(page > (int)(shop_v.size()-0.5) / (2*row_cards))
        page = 0;
    ui->how_many->setText("第[" + QString::number(page + 1) +
                          "]頁，全[" + QString::number(shop_v.size()) + "]種商品");

    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    clear_lineEdit_v();
    clear_layout(ui->up_gridLayout);
    clear_layout(ui->down_gridLayout);
    card_grid_layout(row_cards, ui->up_gridLayout, 0);
    card_grid_layout(row_cards, ui->down_gridLayout, 1);

    delete load_window;
}

void ManageGoods_window::on_previous_page_clicked()
{
    page--;
    if(page < 0)
        page = (int)(shop_v.size()-0.5) / (2*row_cards);
    ui->how_many->setText("第[" + QString::number(page + 1) +
                          "]頁，全[" + QString::number(shop_v.size()) + "]種商品");

    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    clear_lineEdit_v();
    clear_layout(ui->up_gridLayout);
    clear_layout(ui->down_gridLayout);
    card_grid_layout(row_cards, ui->up_gridLayout, 0);
    card_grid_layout(row_cards, ui->down_gridLayout, 1);

    delete load_window;
}

void ManageGoods_window::on_add_clicked()
{
    if(ui->lineEdit->hasFocus())        //在輸入搜尋文字時enter
    {
        on_search_clicked();            //就跳過去，不是這
        return;
    }

    int count = 0;

    for(int i = 0; i < (int)num_in_v.size(); i++)
    {
        int idx = 2*row_cards*page + i;
        bool flag = 0;

        if(num_in_v[i]->text() != "")
        {
            qDebug() << "add" << QString::fromStdString(shop_v[idx].name) << num_in_v[i]->text();
            shop_v[idx].num += num_in_v[i]->text().toInt();         
            count++;

            flag = 1;
        }
        if(price_in_v[i]->text() != "")
        {
            if(shop_v[idx].price > price_in_v[i]->text().toInt())
                shop_v[idx].state = "CUT";
            else
                shop_v[idx].state = "HOT";
            qDebug() << "change price" << QString::fromStdString(shop_v[idx].name) << price_in_v[i]->text();
            shop_v[idx].price = price_in_v[i]->text().toInt();
            count++;

            flag = 1;
        }

        if(flag)    //有變動數量或價格
        {
            num_in_v[i]->clear();       //清出輸入的東西
            price_in_v[i]->clear();

            QLayoutItem *item;
            if(i < row_cards)       //改動上面的grid
            {
                //保留被改動位置以外的line edits
                vector<QLineEdit *> nums_before(num_in_v.begin(), num_in_v.begin() + i), nums_after(num_in_v.begin() + i + 1, num_in_v.end());
                vector<QLineEdit *> prices_before(price_in_v.begin(), price_in_v.begin() + i), prices_after(price_in_v.begin() + i + 1, price_in_v.end());
                clear_lineEdit_v();

                item = ui->up_gridLayout->itemAtPosition(2, i);     //刪除第i行第二列的hlayout
                clear_layout(item->layout());
                delete item->layout();
                item = ui->up_gridLayout->itemAtPosition(3, i);     //刪除第i行第三列的subgrid
                clear_layout(item->layout());
                delete item->layout();
                sub_layout(ui->up_gridLayout, 0, i);        //重新布置剛剛被刪掉的東西

                //把line edits組合回去
                num_in_v.insert(num_in_v.begin(), nums_before.begin(), nums_before.end());
                num_in_v.insert(num_in_v.end(), nums_after.begin(), nums_after.end());
                price_in_v.insert(price_in_v.begin(), prices_before.begin(), prices_before.end());
                price_in_v.insert(price_in_v.end(), prices_after.begin(), prices_after.end());
            }
            else        //改動下面的grid
            {
                //保留被改動位置以外的line edits
                vector<QLineEdit *> nums_before(num_in_v.begin(), num_in_v.begin() + i), nums_after(num_in_v.begin() + i + 1, num_in_v.end());
                vector<QLineEdit *> prices_before(price_in_v.begin(), price_in_v.begin() + i), prices_after(price_in_v.begin() + i + 1, price_in_v.end());
                clear_lineEdit_v();

                item = ui->down_gridLayout->itemAtPosition(2, i - row_cards);       //刪除第i - row_cards行第二列的hlayout
                clear_layout(item->layout());
                delete item->layout();
                item = ui->down_gridLayout->itemAtPosition(3, i - row_cards);       //刪除第i - row_cards行第三列的subgrid
                clear_layout(item->layout());
                delete item->layout();
                sub_layout(ui->down_gridLayout, 1, i - row_cards);      //重新布置剛剛被刪掉的東西

                //把line edits組合回去
                num_in_v.insert(num_in_v.begin(), nums_before.begin(), nums_before.end());
                num_in_v.insert(num_in_v.end(), nums_after.begin(), nums_after.end());
                price_in_v.insert(price_in_v.begin(), prices_before.begin(), prices_before.end());
                price_in_v.insert(price_in_v.end(), prices_after.begin(), prices_after.end());
            }
        }
    }

    if(count)
    {
        //更新到本體
        int pos = 0;
        for(int i = 0; i < (int)shop_v.size(); i++)
        {
            for(int j = pos; j < (int)sub_v.size(); j++)
            {
                if(shop_v[i].name == sub_v[j].name)
                {
                    pos = j + 1;
                    sub_v[j].num = shop_v[i].num;
                    sub_v[j].price = shop_v[i].price;
                    sub_v[j].state = shop_v[i].state;
                    break;
                }
            }
        }

        Loading_window *load_window = new Loading_window(this);
        load_window->setWindowTitle("修改成功");
        load_window->set_text("SUCCESS");
        load_window->show();
    }
    else
    {
        Loading_window *load_window = new Loading_window(this);
        load_window->setWindowTitle("修改失敗");
        load_window->set_text("FAILED");
        load_window->show();
    }
}

void ManageGoods_window::on_sort_box_currentTextChanged(const QString &arg1)
{
    while(shop_v.size())
        shop_v.pop_back();

    if(arg1 == "monster")
    {
        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].type == "monster")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "magic")
    {
        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].type == "magic")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "trap")
    {
        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].type == "trap")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "new")
    {

        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].state == "NEW")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "hot")
    {

        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].state == "HOT")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "cut")
    {

        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].state == "CUT")
                shop_v.push_back(sub_v[i]);
    }
    else
        shop_v = sub_v;

    page = 0;
    ui->how_many->setText("第[" + QString::number(page + 1) +
                          "]頁，全[" + QString::number(shop_v.size()) + "]種商品");

    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    clear_lineEdit_v();
    clear_layout(ui->up_gridLayout);
    clear_layout(ui->down_gridLayout);
    card_grid_layout(row_cards, ui->up_gridLayout, 0);
    card_grid_layout(row_cards, ui->down_gridLayout, 1);

    delete load_window;
}

void ManageGoods_window::on_rownum_box_currentTextChanged(const QString &arg1)
{
    if(arg1 == "16/page")
        row_cards = 8;
    else if(arg1 == "12/page")
        row_cards = 6;
    else if(arg1 == "8/page")
        row_cards = 4;

    page = 0;
    ui->how_many->setText("第[" + QString::number(page + 1) +
                          "]頁，全[" + QString::number(shop_v.size()) + "]種商品");

    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    clear_lineEdit_v();
    clear_layout(ui->up_gridLayout);
    clear_layout(ui->down_gridLayout);
    card_grid_layout(row_cards, ui->up_gridLayout, 0);
    card_grid_layout(row_cards, ui->down_gridLayout, 1);

    delete load_window;
}

void ManageGoods_window::on_search_clicked()
{
    if(ui->lineEdit->text() == "")      //不輸入就滾
        return;

    while(shop_v.size())
        shop_v.pop_back();
    QString arg1 = ui->sort_box->currentText();     //按sort box重整card vector
    if(arg1 == "monster")
    {
        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].type == "monster")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "magic")
    {
        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].type == "magic")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "trap")
    {
        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].type == "trap")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "new")
    {

        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].state == "NEW")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "hot")
    {

        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].state == "HOT")
                shop_v.push_back(sub_v[i]);
    }
    else if(arg1 == "cut")
    {

        for(int i = 0; i < (int)sub_v.size(); i++)
            if(sub_v[i].state == "CUT")
                shop_v.push_back(sub_v[i]);
    }
    else
        shop_v = sub_v;

    string input = ui->lineEdit->text().toStdString();
    ui->lineEdit->clear();
    vector<string> words;

    size_t pos = 0;
    while ((pos = input.find(" ")) != string::npos)     //空格分割
    {
        if(input.substr(0, pos) != "")
            words.push_back(input.substr(0, pos));
        input.erase(0, pos + 1);
    }
    if(input != "")
        words.push_back(input);     //把最後剩下的字串加入

    for (string &str : words)
        qDebug() << QString::fromStdString(str);

    int max_found = 0;      //一張卡被找到最多的指定字串量
    vector<int> count(shop_v.size(),0);       //每張卡被找到多少指定字串
    for(int i = 0; i < (int)words.size(); i++)  //對每個指定字串搜尋所有卡片
        for(int j = 0; j < (int)shop_v.size(); j++)
            if(shop_v[j].name.find(words[i]) != string::npos)
            {
                count[j]++;
                if(count[j] > max_found)
                    max_found = count[j];
            }

    vector<Card_in_shop> temp;      //儲存包含指定字串卡片的vector
    for(int i = max_found; i > 0; i--)      //被找到最多字串的優先加入
        for(int j = 0; j < (int)shop_v.size(); j++)
            if(count[j] == i)
                temp.push_back(shop_v[j]);

    while(shop_v.size())
        shop_v.pop_back();
    shop_v = temp;

    page = 0;
    string search_show = "搜尋:";
    for(int i = 0; i < (int)words.size(); i++)
        search_show += " 「" + words[i] + "」";
    ui->how_many->setText(QString::fromStdString(search_show) + ", 第[" + QString::number(page + 1) +
                          "]頁，全[" + QString::number(shop_v.size()) + "]種商品");

    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    clear_lineEdit_v();
    clear_layout(ui->up_gridLayout);
    clear_layout(ui->down_gridLayout);
    card_grid_layout(row_cards, ui->up_gridLayout, 0);
    card_grid_layout(row_cards, ui->down_gridLayout, 1);

    delete load_window;
}

