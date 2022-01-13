#include "header/shop_list.h"
#include "ui_shop_list.h"
#include "header/loading_window.h"
#include "header/csv.h"
#include <QIntValidator>

Shop_list::Shop_list(Customer *&cp, QWidget *parent) :
    AddGoods_list(0, parent),
    ui(new Ui::Shop_list)
{
    ui->setupUi(this);
    c = cp;

    Csv *csvObj = new Csv;
    shop_v = csvObj->read_shop("../AOOP_DogeShop/src/shop.csv");
    sub_v = shop_v;
    delete csvObj;

    ui->how_many->setText("全[" + QString::number(shop_v.size()) + "]種商品");
    ui->shop_title->setText("DOGE SHOP");
    ui->money->setText( QString::number(c->get_money_cash()) + "$");
    ui->add->setStyleSheet("QPushButton{background-color:rgba(212,109,104,100%); color:white; border-radius:0px;}"
                           "QPushButton:hover{background-color:rgba(183,78,73,100%); color:white;}");

    card_grid_layout(ui->gridLayout);
}

void Shop_list::card_grid_layout(QGridLayout *grid)
{
    QTableWidget *tableWidget = new QTableWidget(shop_v.size(),7);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->horizontalHeader()->hide();
    tableWidget->verticalHeader()->hide();
    tableWidget->setColumnWidth(0,270);
    tableWidget->setColumnWidth(1,30);
    tableWidget->setColumnWidth(2,60);
    tableWidget->setColumnWidth(3,60);
    tableWidget->setColumnWidth(4,75);
    tableWidget->setColumnWidth(5,105);
    tableWidget->setColumnWidth(6,280);
    grid->addWidget(tableWidget);

    for(int i = 0; i < (int)shop_v.size(); i++)
    {
        QLabel *name = new QLabel;
        name->setText(QString::fromStdString(shop_v[i].name));
        name->setAlignment(Qt::AlignCenter);
        name->setMinimumWidth(270);
        if(shop_v[i].type == "monster")
            name->setStyleSheet("QLabel{background-color:rgb(197, 152, 75); color:white; border:2px solid; font:bold;}");
        else if(shop_v[i].type == "magic")
            name->setStyleSheet("QLabel{background-color:rgb(19, 147, 129); color:white; border:2px solid; font:bold;}");
        else if(shop_v[i].type == "trap")
            name->setStyleSheet("QLabel{background-color:rgb(171, 29, 134); color:white; border:2px solid; font:bold;}");
        tableWidget->setCellWidget(i, 0, name);

        sub_layout(tableWidget, i);

        QPushButton *button = new QPushButton(" 點此查看卡片詳細 ");
        button->setMinimumWidth(300);
        button->setAutoDefault(false);
        button->setStyleSheet("QPushButton{background-color:rgba(217,182,80,100%);\
                              color:white; border-radius:0px; font:bold;}"
                              "QPushButton:hover{background-color:rgba(255,220,110,100%); color:rgb(61,61,61);}");
        connect(button, &QPushButton::clicked, this, [=](){shop_v[i].detail();});
        tableWidget->setCellWidget(i, 6, button);
    }
}

void Shop_list::sub_layout(QTableWidget *tableWidget, int i)
{
    if(shop_v[i].state != " ")       //判斷有沒有HOT那些label
    {
        QLabel *state = new QLabel(shop_v[i].state);
        if(state->text() == "NEW") state->setStyleSheet("font:bold; border:1px solid green; color:green; font-size:8px;");
        else if(state->text() == "HOT") state->setStyleSheet("font:bold; border:1px solid red; color:red; font-size:8px;");
        else if(state->text() == "CUT") state->setStyleSheet("font:bold; border:1px solid blue; color:blue; font-size:8px;");
        state->setAlignment(Qt::AlignCenter);
        tableWidget->setCellWidget(i, 1, state);
    }

    QLabel *num = new QLabel("庫存 " + QString::number(shop_v[i].num));
    QLabel *price = new QLabel("價格 " + QString::number(shop_v[i].price));
    num->setAlignment(Qt::AlignVCenter);
    price->setAlignment(Qt::AlignVCenter);
    num->setStyleSheet("font:bold; color:red");
    price->setStyleSheet("font:bold; color:red");

    QLabel *num2 = new QLabel("輸入數量");
    num2->setStyleSheet("border:2px solid; font:bold;");
    num2->setAlignment(Qt::AlignCenter);
    QLineEdit *num_in = new QLineEdit;
    num_in->setMinimumWidth(75);
    num_in->setValidator(new QIntValidator(0, shop_v[i].num, this));
    num_in_v.push_back(num_in);

    tableWidget->setCellWidget(i, 2, num);
    tableWidget->setCellWidget(i, 3, price);
    tableWidget->setCellWidget(i, 4, num2);
    tableWidget->setCellWidget(i, 5, num_in);
}

Shop_list::~Shop_list()
{
    delete ui;
}

void Shop_list::reject()
{
    remove("../AOOP_DogeShop/src/shop.csv");

    Csv *csvObj = new Csv;
    csvObj->save_shop_csv(sub_v, "../AOOP_DogeShop/src/shop.csv");
    delete csvObj;

    emit update_money_request();
    emit update_bag_request();
    QDialog::reject();
}

void Shop_list::on_add_clicked()
{
    if(ui->lineEdit->hasFocus())        //在輸入搜尋文字時enter
    {
        on_search_clicked();            //就跳過去，不是這
        return;
    }

    int count = 0;

    for(int i = 0; i < (int)num_in_v.size(); i++)
    {
        if(num_in_v[i]->text() != "" && num_in_v[i]->text().toInt() <= shop_v[i].num)
        {
            if(c->purchase(shop_v[i].price * num_in_v[i]->text().toInt()))
            {
                qDebug() << "buy" << QString::fromStdString(shop_v[i].name) << num_in_v[i]->text();
                shop_v[i].num -= num_in_v[i]->text().toInt();
                count++;

                qDebug() << "now i have:" << c->get_money_cash();
                ui->money->setText(QString::number(c->get_money_cash()) + "$");

                Card_in_bag *card = new Card_in_bag;
                card->set_data(shop_v[i], num_in_v[i]->text().toInt());
                c->addToBag(card);
                qDebug() << c->get_deck_in_bag_size();
            }
            else
                qDebug() << "buy" << QString::fromStdString(shop_v[i].name) << "failed";
        }

        num_in_v[i]->clear();       //清出輸入的東西
    }

    if(count)
    {
        //更新到本體
        int pos = 0;
        for(int i = 0; i < (int)shop_v.size(); i++)
        {
            for(int j = pos; j < (int)sub_v.size(); j++)
            {
                if(shop_v[i].name == sub_v[j].name && shop_v[i].price == sub_v[j].price)
                {
                    pos = j + 1;
                    sub_v[j].num = shop_v[i].num;
                    break;
                }
            }
        }

        Loading_window *load_window = new Loading_window(this);
        load_window->setWindowTitle("購買成功");
        load_window->set_text("SUCCESS");
        load_window->show();

        clear_lineEdit_v();
        clear_layout(ui->gridLayout);
        card_grid_layout(ui->gridLayout);
    }
    else
    {
        Loading_window *load_window = new Loading_window(this);
        load_window->setWindowTitle("購買失敗");
        load_window->set_text("FAILED");
        load_window->show();
    }
}

void Shop_list::on_sort_box_currentTextChanged(const QString &arg1)
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

    ui->how_many->setText("全[" + QString::number(shop_v.size()) + "]種商品");

    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    clear_lineEdit_v();
    clear_layout(ui->gridLayout);
    card_grid_layout(ui->gridLayout);

    delete load_window;
}

void Shop_list::on_search_clicked()
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

    string search_show = "搜尋:";
    for(int i = 0; i < (int)words.size(); i++)
        search_show += " 「" + words[i] + "」";
    ui->how_many->setText(QString::fromStdString(search_show) + ", 全[" + QString::number(shop_v.size()) + "]種商品");

    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    clear_lineEdit_v();
    clear_layout(ui->gridLayout);
    card_grid_layout(ui->gridLayout);

    delete load_window;
}
