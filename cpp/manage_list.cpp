#include "header/manage_list.h"
#include "ui_manage_list.h"
#include "header/loading_window.h"
#include "header/csv.h"
#include <QIntValidator>

Manage_list::Manage_list(QWidget *parent) :
    AddGoods_list(1, parent),
    ui(new Ui::Manage_list)
{
    ui->setupUi(this);

    Csv *csvObj = new Csv;
    shop_v = csvObj->read_shop("../AOOP_DogeShop/src/shop.csv");
    delete csvObj;

    for(int i = 0; i < (int)shop_v.size(); i++)
        if(shop_v[i].state == "HOT" || shop_v[i].state == "CUT")
            shop_v[i].state = " ";

    sub_v = shop_v;

    ui->how_many->setText("全[" + QString::number(shop_v.size()) + "]種商品");
    ui->shop_title->setText("DOGE SHOP - Manage");
    ui->add->setStyleSheet("QPushButton{background-color:rgba(212,109,104,100%); color:white; border-radius:0px;}"
                           "QPushButton:hover{background-color:rgba(183,78,73,100%); color:white;}");

    card_grid_layout(ui->gridLayout);
}

void Manage_list::card_grid_layout(QGridLayout *grid)
{
    QTableWidget *tableWidget = new QTableWidget(shop_v.size(),9);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->horizontalHeader()->hide();
    tableWidget->verticalHeader()->hide();
    tableWidget->setColumnWidth(0,270);
    tableWidget->setColumnWidth(1,30);
    tableWidget->setColumnWidth(2,60);
    tableWidget->setColumnWidth(3,60);
    tableWidget->setColumnWidth(4,40);
    tableWidget->setColumnWidth(5,50);
    tableWidget->setColumnWidth(6,40);
    tableWidget->setColumnWidth(7,50);
    tableWidget->setColumnWidth(8,280);
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
        tableWidget->setCellWidget(i, 8, button);
    }
}

void Manage_list::sub_layout(QTableWidget *tableWidget, int i)
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

    QLabel *num2 = new QLabel("補貨"), *price2 = new QLabel("改價");
    num2->setStyleSheet("border:2px solid; font:bold;");
    num2->setAlignment(Qt::AlignCenter);
    price2->setStyleSheet("border:2px solid; font:bold;");
    price2->setAlignment(Qt::AlignCenter);
    QLineEdit *num_in = new QLineEdit, *price_in = new QLineEdit;
    num_in->setMinimumWidth(50);
    price_in->setMinimumWidth(50);
    num_in->setValidator(new QIntValidator(-(shop_v[i].num), 1000, this));
    price_in->setValidator(new QIntValidator(0, 10000, this));
    num_in_v.push_back(num_in);         //把輸入數量的LineEdit加入num_in_v
    price_in_v.push_back(price_in);     //把輸入價格的LineEdit加入price_in_v

    tableWidget->setCellWidget(i, 2, num);
    tableWidget->setCellWidget(i, 3, price);
    tableWidget->setCellWidget(i, 4, num2);
    tableWidget->setCellWidget(i, 5, num_in);
    tableWidget->setCellWidget(i, 6, price2);
    tableWidget->setCellWidget(i, 7, price_in);
}

Manage_list::~Manage_list()
{
    delete ui;
}

void Manage_list::reject()
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

void Manage_list::on_add_clicked()
{
    if(ui->lineEdit->hasFocus())        //在輸入搜尋文字時enter
    {
        on_search_clicked();            //就跳過去，不是這
        return;
    }

    int count = 0;

    for(int i = 0; i < (int)num_in_v.size(); i++)
    {
        if(num_in_v[i]->text() != "")
        {
            qDebug() << "add" << QString::fromStdString(shop_v[i].name) << num_in_v[i]->text();
            shop_v[i].num += num_in_v[i]->text().toInt();
            count++;
        }
        if(price_in_v[i]->text() != "")
        {
            if(shop_v[i].price > price_in_v[i]->text().toInt())
                shop_v[i].state = "CUT";
            else
                shop_v[i].state = "HOT";
            qDebug() << "change price" << QString::fromStdString(shop_v[i].name) << price_in_v[i]->text();
            shop_v[i].price = price_in_v[i]->text().toInt();
            count++;
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

        clear_lineEdit_v();
        clear_layout(ui->gridLayout);
        card_grid_layout(ui->gridLayout);
    }
    else
    {
        Loading_window *load_window = new Loading_window(this);
        load_window->setWindowTitle("修改失敗");
        load_window->set_text("FAILED");
        load_window->show();
    }
}

void Manage_list::on_sort_box_currentTextChanged(const QString &arg1)
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

void Manage_list::on_search_clicked()
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
