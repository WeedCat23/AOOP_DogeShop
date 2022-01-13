#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindowpopupform.h"
#include <QDialog>
#include <QMessageBox>
#include <QIntValidator>
#include <QDesktopServices>
#include "header/loading_window.h"
#include "header/addgoods_window.h"
#include "header/addgoods_list.h"
#include "header/shop_window.h"
#include "header/shop_list.h"
#include "header/managegoods_window.h"
#include "header/manage_list.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    is_test = false;

    c=NULL;
    s=NULL;
    m=NULL;
    //authorization code
    ui->lineEdit_cellphone->setInputMask("9999-999-999");

    ui->dateEdit->setMaximumDate(QDate(2004,1,1));

    //error message color
    QPalette sample_palette_error;
    QColor color;
    color.setRgba(0x50FF0000);
    sample_palette_error.setColor(QPalette::Window,color);
    sample_palette_error.setColor(QPalette::WindowText, Qt::red);



    //SQL connection
    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("140.113.89.173");
    database.setDatabaseName("doge_shop"); // schema name
    database.setUserName("testuser");
    database.setPassword("password"); // your password
    database.setPort(3306);
    bool ok = database.open();
    if(ok){
        qDebug()<<"Successful Connection.";
    }else{
        qDebug()<<database.lastError();
    }
    query=new QSqlQuery(database);
   // query->exec("INSERT INTO seller_list VALUE(1234,'manager','manager');");


    //default set to empty record
    myinfo_default();

    //login popup
    m_login_window=new LoginWindowPopUpForm(this);
    m_login_window->syncpage(ui->stackedWidget);
    m_login_window->syncdatabase(database);
    m_login_window->hide();

    //github button
    ui->github_butt->setStyleSheet("QPushButton{border-image: url(../AOOP_DogeShop/src/github.png); width: 80px; height: 80px;}"
                                   "QPushButton:hover{border-image: url(../AOOP_DogeShop/src/github2.png);}");

    //initalize display
    logout_display();
    ui->piechart->hide();
    ui->menuCustomer_Center->setUpdatesEnabled(true);
    ui->menuCustomer_Center->setFont(ui->menuBar->font());

    //popup window signals
    connect(m_login_window,SIGNAL(cusLoggedin()),this,SLOT(popup_close_cus()));
    connect(m_login_window,SIGNAL(selLoggedin()),this,SLOT(popup_close_sel()));
    connect(m_login_window,SIGNAL(manLoggedin()),this,SLOT(popup_close_man()));
    connect(m_login_window,SIGNAL(testLoggedin()),this,SLOT(popup_close_test()));

    font_record=ui->c_s_table->horizontalHeaderItem(0)->font();
    ui->c_s_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->c_s_table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->c_s_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->comboBox_filter->setCurrentIndex(0);
    ui->comboBoxsort->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_actionLogin_triggered()
{
    m_login_window->signOrLog(login_);
    m_login_window->setprePage(ui->stackedWidget->currentIndex());
    ui->stackedWidget->setCurrentIndex(emptypage);
    m_login_window->show();

}
void MainWindow::on_actionSignup_triggered()
{
    m_login_window->signOrLog(signup_);
    m_login_window->setprePage(ui->stackedWidget->currentIndex());
    ui->stackedWidget->setCurrentIndex(emptypage);
    m_login_window->show();

}
void MainWindow::on_actionHome_triggered()
{
    ui->stackedWidget->setCurrentIndex(frontpage);
}
void MainWindow::returnPage(){
    ui->stackedWidget->setCurrentIndex(prePage);
}

void MainWindow::on_lineEdit_cellphone_selectionChanged()
{
    ui->lineEdit_cellphone->setReadOnly(0);
}
void MainWindow::on_actionmyInfo_triggered()
{
    customer_info_callin();
    ui->stackedWidget->setCurrentIndex(c_info_page);
}

void MainWindow::popup_close_cus()
{
    if(m_login_window->is_loggedin())
    {
        m_login_window->sync_C_pointer(c);
        s=NULL,m=NULL;
        ui->label_memberinfo_id->setText(QString::number(c->getID()));
        ui->label_memberinfo_name->setText(c->getName());
        myinfo_default();
        customer_info_callin();
        customer_wallet_callin();
        customer_bag_calltobag();
        ui->menuMyInfo->menuAction()->setVisible(true);
        ui->menuGuest->menuAction()->setVisible(false);
        is_test = false;
        ui->btn_c_infoupdate->setEnabled(true);
        ui->btn_cus_change_pwd->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(frontpage);
        ui->comboBox_filter->setCurrentIndex(0);
        ui->comboBoxsort->setCurrentIndex(0);
    }
}
void MainWindow::popup_close_sel()
{
    if(m_login_window->is_loggedin()){
        m_login_window->sync_S_pointer(s);
        c=NULL,m=NULL;
        ui->menuSeller_Center->menuAction()->setVisible(true);
        ui->menuGuest->menuAction()->setVisible(false);
        is_test = false;
        ui->stackedWidget->setCurrentIndex(frontpage);
    }
}
void MainWindow::popup_close_test()
{
    if(m_login_window->is_loggedin()){
        ui->menuMyInfo->menuAction()->setVisible(true);
        ui->menuSeller_Center->menuAction()->setVisible(true);
        is_test = true;
        ui->btn_c_infoupdate->setEnabled(false);
        ui->btn_cus_change_pwd->setEnabled(false);
        ui->label_memberinfo_id->setText("0");
        ui->label_memberinfo_name->setText("test");
        ui->stackedWidget->setCurrentIndex(frontpage);
    }
}
void MainWindow::popup_close_man()
{
    if(m_login_window->is_loggedin()){
        m_login_window->sync_M_pointer(m);
        c=NULL,s=NULL;
        ui->menuBack_End_Manage->menuAction()->setVisible(true);
        ui->menuGuest->menuAction()->setVisible(false);
        is_test = false;
        ui->stackedWidget->setCurrentIndex(frontpage);
    }
}
void MainWindow::update_password(QString q)
{
    if(c)
    {
        query->exec("UPDATE customer_list SET password='"+q+"' WHERE username='"+c->getName()+"';");
        m_login_window->logout();
        logout_display();
    }
    else if(s)
    {
        query->exec("UPDATE seller_list SET password='"+q+"' WHERE username='"+s->getName()+"';");
        m_login_window->logout();
        logout_display();
    }
    else
        qDebug()<<"c or s pointer error!";
}
void MainWindow::update_money()
{
    query->exec("UPDATE customer_list SET cash="+QString::number(c->get_money_cash())+" WHERE username='"+c->getName()+"';");
    query->exec("UPDATE customer_list SET point="+QString::number(c->get_money_point())+" WHERE username='"+c->getName()+"';");
    customer_wallet_callin();
}
void MainWindow::update_bag()
{
    query->exec("TRUNCATE TABLE c_"+QString::number(c->getID())+";");
    for (auto &x :c->mybag()) {
        QString boolbit="false";
        if(x.star) boolbit="true";
        query->exec("INSERT INTO c_"+QString::number(c->getID())+" VALUES('"+QString::fromStdString(x.name)+
                                                        "','"+QString::fromStdString(x.type)+
                                                        "','"+QString::fromStdString(x.url)+
                                                        "',"+QString::number(x.num)+","+boolbit+");");
    }

}
void MainWindow::delete_card(Card_in_bag tmp)
{
    qDebug()<<QString::fromStdString(tmp.name)<<tmp.num;
    if(tmp.num==0)
        query->exec("DELETE FROM c_"+QString::number(c->getID())+" WHERE card_name = '"+QString::fromStdString(tmp.name)+"';");
    else
        query->exec("UPDATE c_"+QString::number(c->getID())+" SET count ="+QString::number(tmp.num)+" WHERE card_name = '"+QString::fromStdString(tmp.name)+"';");

}

void MainWindow::delete_cus(QString tmpname,QString tmpid)
{
    query->exec("DELETE FROM customer_list WHERE username='"+tmpname+"';");
    query->exec("DELETE FROM customer_info WHERE username='"+tmpname+"';");
    query->exec("DROP TABLE c_"+tmpid+";");
}
void MainWindow::on_actionLog_out_triggered()
{
    m_login_window->logout();
    logout_display();

    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("登出");
    load_window->set_text("SUCCESS");
    load_window->show();
}
void MainWindow::on_actionLog_out_2_triggered()
{
    on_actionLog_out_triggered();
}
void MainWindow::logout_display()
{
    ui->comboBox_filter->setCurrentIndex(0);
    ui->comboBoxsort->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(frontpage);
    ui->menuMyInfo->menuAction()->setVisible(false);
    ui->menuSeller_Center->menuAction()->setVisible(false);
    ui->menuGuest->menuAction()->setVisible(true);
    ui->menuBack_End_Manage->menuAction()->setVisible(false);
}


void MainWindow::on_actionRelease_Card_triggered()
{
    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    AddGoods_window *add_window = new AddGoods_window(this);
    add_window->setWindowTitle("卡片上架");

    ui->stackedWidget->setCurrentIndex(frontpage);
    add_window->show();

    delete load_window;
}

void MainWindow::on_actionDOGE_SHOP_triggered()
{
    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    ui->stackedWidget->setCurrentIndex(frontpage);
    load_window->show();

    if(is_test)
    {
        QString name = "test", pass = "test";
        c = new Customer(0, name, pass, 81000, 81000);
    }
    Shop_window *shop_window = new Shop_window(c, this);
    shop_window->setWindowTitle("卡片購買");

    connect(shop_window,SIGNAL(update_money_request()),this,SLOT(update_money()));
    connect(shop_window,SIGNAL(update_bag_request()),this,SLOT(update_bag()));

    ui->stackedWidget->setCurrentIndex(frontpage);
    shop_window->show();

    delete load_window;
}

void MainWindow::on_actionShop_Manage_triggered()
{
    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    ManageGoods_window *manage_window = new ManageGoods_window(this);
    manage_window->setWindowTitle("商品管理");

    ui->stackedWidget->setCurrentIndex(frontpage);
    manage_window->show();

    delete load_window;
}

void MainWindow::myinfo_default(){
    ui->comboBox_gender->setCurrentIndex(-1);
    ui->comboBox_house->setCurrentIndex(-1);
    ui->dateEdit->setCalendarPopup(1);
    ui->lineEdit_cellphone->setPlaceholderText("0900-000-000");
}

void MainWindow::set_piechart(vector<Card_in_bag> deck,bool count_unique)
{

    if(deck.size()==0)
    {
        ui->label_piechart->hide();
        ui->piechart->hide();
        ui->groupBox_4->hide();
    }
    else
    {
        int magic=0,trap=0,monster=0;
        if(count_unique)
        {
            for(auto &x:deck)
            {
                if(x.type=="magic")
                    magic++;
                else if(x.type=="trap")
                    trap++;
                else
                    monster++;
            }
        }
        else
        {
            for(auto &x:deck)
            {
                if(x.type=="magic")
                    magic+=x.num;
                else if(x.type=="trap")
                    trap+=x.num;
                else
                    monster+=x.num;
            }
        }
        ui->piechart->setpercentage(magic,trap,monster);
        ui->label_piechart->show();
        ui->piechart->show();
        ui->groupBox_4->show();
    }
}

void MainWindow::delete_card_in_bag(int i)
{
    Exchange_popup* pop=new Exchange_popup(c,i);
    pop->setWindowTitle("Cards recycle");
    connect(pop,SIGNAL(update_bag_request(Card_in_bag)),this,SLOT(delete_card(Card_in_bag)));
    connect(pop,SIGNAL(update_money_request()),this,SLOT(update_money()));
    pop->exec();
}
void MainWindow::customer_info_callin()
{
    sql_command="SELECT * FROM customer_info WHERE username = '"+c->getName()+"';";
    query->exec(sql_command);
    if(query->next())
    {
        if(!query->value("gender").isNull())
            ui->comboBox_gender->setCurrentIndex(query->value("gender").toInt());
        if(!query->value("birthday").isNull())
            ui->dateEdit->setDate(query->value("birthday").toDate());
        if(!query->value("cellphone").isNull())
            ui->lineEdit_cellphone->setText(query->value("cellphone").toString());
         if(!query->value("house").isNull())
            ui->comboBox_house->setCurrentIndex(query->value("house").toInt());
    }

}
void MainWindow::customer_wallet_callin()
{
    sql_command="SELECT * FROM customer_list WHERE username = '"+c->getName()+"';";
    query->exec(sql_command);
    if(query->next())
    {
        //ui->label_cash->setAlignment()
        ui->label_cash->setText(QString::number(query->value("cash").toInt())+"$");
        //ui->label_points->setAlignment()
        ui->label_points->setText(QString::number(query->value("point").toDouble(),'f',2)+"p");
    }
}
void MainWindow::customer_bag_calltobag()
{
    sql_command="SELECT * FROM c_"+QString::number(c->getID())+";";
    query->exec(sql_command);
    while(query->next())
    {
        Card_in_bag* cib=new Card_in_bag(query->value(0).toString().toStdString(),
                                         query->value(1).toString().toStdString(),
                                         query->value(2).toString().toStdString(),
                                         query->value(3).toInt(),query->value(4).toBool());
        c->addToBag(cib);
    }

}
void MainWindow::on_btn_cus_change_pwd_clicked()
{
    ChangePwd_Dialog* dialog=new ChangePwd_Dialog;
    dialog->setWindowTitle("Password change requested");
    dialog->setUser(c);
    connect(dialog,SIGNAL(update_request(QString)),this,SLOT(update_password(QString)));
    dialog->exec();
}
void MainWindow::on_actionManage_Password_triggered()
{
    ChangePwd_Dialog* dialog=new ChangePwd_Dialog;
    dialog->setWindowTitle("Password change requested");
    dialog->setUser(s);
    connect(dialog,SIGNAL(update_request(QString)),this,SLOT(update_password(QString)));
    dialog->exec();
}
void MainWindow::on_actionMyWallet_triggered()
{
    if(is_test)
    {
        QString name = "test", pass = "test";
        c = new Customer(0, name, pass, 81000, 81000);
    }
    else
    {
        update_money();
        customer_wallet_callin();
    }
    ui->stackedWidget->setCurrentIndex(c_wallet_page);
}

void MainWindow::clear_layout(QLayout* layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0)))
    {
        if(QWidget* widget = item->widget())
            delete widget;
        if(QLayout* childLayout = item->layout())
            clear_layout(childLayout);
    }
}
/*
void MainWindow::on_actionMyBag_triggered()
{
    clear_layout(ui->bag_gridLayout);
    set_piechart(c->mybag());
    for(int i = 0; i < c->get_deck_in_bag_size(); i++)
    {
        //c->mybag()[i];
        QLabel *name = new QLabel;
        name->setText(QString::fromStdString(c->mybag()[i].name));
        name->setAlignment(Qt::AlignCenter);
        name->setMinimumWidth(234);
        if(c->mybag()[i].type == "monster")
            name->setStyleSheet("QLabel{background-color:rgb(197, 152, 75); color:white; border:2px solid; font:bold;}");
        else if(c->mybag()[i].type == "magic")
            name->setStyleSheet("QLabel{background-color:rgb(19, 147, 129); color:white; border:2px solid; font:bold;}");
        else if(c->mybag()[i].type == "trap")
            name->setStyleSheet("QLabel{background-color:rgb(171, 29, 134); color:white; border:2px solid; font:bold;}");
        ui->bag_gridLayout->addWidget(name, i, 0, 1, 4, Qt::AlignCenter);

        QLabel *num = new QLabel;
        num->setText(QString::number(c->mybag()[i].num));
        num->setAlignment(Qt::AlignCenter);
        num->setMinimumWidth(117);
        num->setStyleSheet("border:2px solid; font:bold;");
        ui->bag_gridLayout->addWidget(num, i, 4, 1, 2, Qt::AlignCenter);

        QPushButton *button = new QPushButton(" 點此查看卡片詳細 ");
        button->setAutoDefault(false);
        button->setStyleSheet("QPushButton{background-color:rgba(217,182,80,100%);\
                              color:white; border-radius:0px; font:bold;}"
                              "QPushButton:hover{background-color:rgba(255,220,110,100%); color:rgb(61,61,61);}");
        connect(button, &QPushButton::clicked, this, [=](){c->mybag()[i].detail();});
        ui->bag_gridLayout->addWidget(button, i, 6, 1, 4, Qt::AlignCenter);
    }
    ui->bag_gridLayout->setSpacing(0);

    ui->stackedWidget->setCurrentIndex(c_bag_page);
}
*/
void MainWindow::on_actionMyBag_triggered()
{

    bool filter=false;
    c->set_filter(false);
    if(ui->comboBox_filter->currentIndex()!=0)
        filter=true,c->set_filter(true);

    if(is_test)
    {
        QString name = "test", pass = "test";
        c = new Customer(0, name, pass, 81000, 81000);
    }


    clear_layout(ui->bag_gridLayout);

    if(c->mybag().empty()||filter)ui->btn_delete_allcard->hide();
    else ui->btn_delete_allcard->show();


    QTableWidget *tableWidget = new QTableWidget(c->mybag().size(),5);
    //tableWidget->resize(600, 450);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->horizontalHeader()->hide();
    tableWidget->verticalHeader()->hide();
    tableWidget->setColumnWidth(0,240);
    tableWidget->setColumnWidth(1,120);
    tableWidget->setColumnWidth(2,120);
    tableWidget->setColumnWidth(3,60);
    tableWidget->setColumnWidth(4,59);
    ui->bag_gridLayout->addWidget(tableWidget);


    int j=0;
    for(int i = 0; i < (int)c->mybag().size(); i++,j++)
    {
        if(filter){
            if(ui->comboBox_filter->currentText()=="star"&&(c->mybag()[i].star)){
                //ok
            }
            else if(ui->comboBox_filter->currentText()=="no star"&&!c->mybag()[i].star){
                //ok
            }
            else if(c->mybag()[i].type!=ui->comboBox_filter->currentText().toStdString()){
                j--;
                continue;
            }
        }
        QLabel *name = new QLabel;
        name->setText(QString::fromStdString(c->mybag()[i].name));
        name->setAlignment(Qt::AlignCenter);
        name->setMinimumWidth(234);
        if(c->mybag()[i].type == "monster")
            name->setStyleSheet("QLabel{background-color:rgb(197, 152, 75); color:white; border:2px solid; font:bold;}");
        else if(c->mybag()[i].type == "magic")
            name->setStyleSheet("QLabel{background-color:rgb(19, 147, 129); color:white; border:2px solid; font:bold;}");
        else if(c->mybag()[i].type == "trap")
            name->setStyleSheet("QLabel{background-color:rgb(171, 29, 134); color:white; border:2px solid; font:bold;}");
        tableWidget->setCellWidget(j, 0, name);//let i j 不同步

        QLabel *num = new QLabel;
        num->setText(QString::number(c->mybag()[i].num));
        num->setAlignment(Qt::AlignCenter);
        num->setMinimumWidth(117);
        num->setStyleSheet("border:2px solid; font:bold;");
        tableWidget->setCellWidget(j, 1, num);

        QPushButton *button = new QPushButton("點此查看卡片詳細");
        button->setAutoDefault(false);
        button->setStyleSheet("QPushButton{background-color:rgba(217,182,80,100%);\
                              color:white; border-radius:2px; font:bold;}"
                              "QPushButton:hover{background-color:rgba(255,220,110,100%); color:rgb(61,61,61);}");
        connect(button, &QPushButton::clicked, this, [=](){c->mybag()[i].detail();});
        tableWidget->setCellWidget(j, 2, button);

        button = new QPushButton((c->mybag()[i].star) ? "✦" : "✧");
        button->setStyleSheet("QPushButton{background-color:rgba(217,182,80,100%);\
                              color:white; border-radius:2px; font:bold; font-size:25px;}"
                              "QPushButton:hover{background-color:rgba(255,220,110,100%); color:rgb(61,61,61);}");
        connect(button, &QPushButton::clicked, this, [this, i](){c->mybag()[i].change_star();
                 query->exec("UPDATE c_"+QString::number(c->getID())+" SET star= NOT star WHERE card_name='"+QString::fromStdString(c->mybag()[i].name)+"';");
                 on_actionMyBag_triggered();});
        tableWidget->setCellWidget(j, 3, button);

        button = new QPushButton("分解");
        button->setStyleSheet("QPushButton{background-color:rgba(212,109,104,100%);\
                              color:white; border-radius:2px; font:bold;}"
                              "QPushButton:hover{background-color:rgba(183,78,73,100%); color:white;}");
        connect(button, &QPushButton::clicked, this, [this, i](){qDebug( )<< "分解" << QString::fromStdString(c->mybag()[i].name);
            if(!c->mybag()[i].star)delete_card_in_bag(i);else QMessageBox::information(this,"Deletion Error","You cannot delete a card with star.");
                on_actionMyBag_triggered();});
        tableWidget->setCellWidget(j, 4, button);
    }
    tableWidget->setRowCount(j);//truncate unnecessary rows

    set_piechart(c->mybag(),ui->radioButton->isChecked());//piechart;

    ui->bag_gridLayout->setSpacing(0);
    ui->stackedWidget->setCurrentIndex(c_bag_page);
}

void MainWindow::on_btn_cus_change_pwd_2_clicked()
{
    if(c->get_money_point()<1000)
        c->earnPoint();
    else
    {
        QMessageBox::information(this,"Penalty Time","You Already Have 1000p Greedy Human\nNow You only get 87.87p :p");
        c->set_point(87.87);
    }
    update_money();
}
void MainWindow::on_btn_cus_change_pwd_3_clicked()
{
    Exchange_popup* dialog=new Exchange_popup(c);
    dialog->setWindowTitle("Points Exchanger");
    connect(dialog,SIGNAL(update_money_request()),this,SLOT(update_money()));
    dialog->exec();
}
void MainWindow::on_btn_c_infoupdate_clicked()
{
    query->exec("UPDATE customer_info SET gender="+QString::number(ui->comboBox_gender->currentIndex())+" WHERE username = '"+c->getName()+"';");
        query->exec("UPDATE customer_info SET birthday="+ui->dateEdit->date().toString("yyyyMMdd")+" WHERE username = '"+c->getName()+"';");
    if(ui->lineEdit_cellphone->text().length()<12)
        QMessageBox::information(this,"cellphone error","Please make sure you fill in 10 digits."),ui->lineEdit_cellphone->clear();
    else
        query->exec("UPDATE customer_info SET cellphone='"+ui->lineEdit_cellphone->text()+"' WHERE username = '"+c->getName()+"';");
    query->exec("UPDATE customer_info SET house="+QString::number(ui->comboBox_house->currentIndex())+" WHERE username = '"+c->getName()+"';");

}
void MainWindow::on_actionAuthorization_Code_triggered()
{
    prePage=ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(emptypage);
    Author_code_dialog* dialog=new Author_code_dialog(m);
    dialog->setWindowTitle("Authorization Code");

    connect(dialog,&Author_code_dialog::update_request,this,[this](){query->exec("UPDATE seller_list SET id="+m->getcode()+" WHERE username='"+m->getName()+"';");});
    dialog->exec();
    ui->stackedWidget->setCurrentIndex(prePage);
}
void MainWindow::on_actionLog_Out_triggered()
{
    m_login_window->logout();
    logout_display();
}
void MainWindow::on_actionCustomer_List_triggered()
{
    in_cus_list=true;
    ui->label_list_name->setText("Customer List:");
    for(int i = 0; i < ui->c_s_table->rowCount(); i++)  //刪除按鈕
        delete  ui->c_s_table->takeItem(i, 4);
    ui->c_s_table->setRowCount(0);
    ui->c_s_table->setColumnCount(5);
    ui->c_s_table->setSortingEnabled(false);
    QStringList title;
    title << "ID" << "Username" << "Password" << "Cellphone" << "-";
    ui->c_s_table->setHorizontalHeaderLabels(title);
    ui->c_s_table->setHorizontalHeaderLabels(title);
    ui->c_s_table->horizontalHeaderItem(0)->setFont(font_record);
    ui->c_s_table->horizontalHeaderItem(1)->setFont(font_record);
    ui->c_s_table->horizontalHeaderItem(2)->setFont(font_record);
    ui->c_s_table->horizontalHeaderItem(3)->setFont(font_record);
    QSqlQuery *query_for_info=new QSqlQuery(database);
    query->exec("SELECT * FROM customer_list;");

    ui->c_s_table->setColumnWidth(0,140);
    ui->c_s_table->setColumnWidth(1,140);
    ui->c_s_table->setColumnWidth(2,140);
    ui->c_s_table->setColumnWidth(3,140);
    ui->c_s_table->setColumnWidth(4,59);

    while(query->next())
    {

        QString id_from_sql=query->value(0).toString();
        QString name_from_sql=query->value(1).toString();
        QString pwd_from_sql=query->value(2).toString();
        //qDebug()<<"1 sql ok";
        query_for_info->exec("SELECT cellphone FROM customer_info WHERE username='"+name_from_sql+"';");
        query_for_info->next();
        QString phone_from_sql;
        if(query_for_info->value(0).isNull())
            phone_from_sql="---";
        else
            phone_from_sql=query_for_info->value(0).toString();
        ui->c_s_table->insertRow(ui->c_s_table->rowCount());
        ui->c_s_table->setItem(ui->c_s_table->rowCount()-1,col_id,new QTableWidgetItem(id_from_sql));
        ui->c_s_table->setItem(ui->c_s_table->rowCount()-1,col_name,new QTableWidgetItem(name_from_sql));
        ui->c_s_table->setItem(ui->c_s_table->rowCount()-1,col_pass,new QTableWidgetItem(pwd_from_sql));
        ui->c_s_table->setItem(ui->c_s_table->rowCount()-1,col_phone,new QTableWidgetItem(phone_from_sql));

        QPushButton *button = new QPushButton("刪除");
        button->setStyleSheet("QPushButton{background-color:rgba(212,109,104,100%);\
                              color:white; border-radius:2px; font:bold;}"
                              "QPushButton:hover{background-color:rgba(183,78,73,100%); color:white;}");

        connect(button, &QPushButton::clicked, this, [this,name_from_sql,id_from_sql](){qDebug( )<< "刪除";
            delete_cus(name_from_sql,id_from_sql);QMessageBox::information(this,"Deletion Done","You have deleted customer:"+name_from_sql+"from list."); on_actionCustomer_List_triggered();});

        ui->c_s_table->setCellWidget(ui->c_s_table->rowCount()-1, 4, button);
    }
    if(ui->c_s_table->rowCount()==0)ui->btn_delete_all->setDisabled(true);
    else ui->btn_delete_all->setDisabled(false);
    ui->stackedWidget->setCurrentIndex(m_account_manage_page);
    ui->c_s_table->setSortingEnabled(true);
    delete query_for_info;
}


void MainWindow::on_actionStaff_List_triggered()
{
    in_cus_list=false;

    ui->label_list_name->setText("Staff List: ");
    for(int i = 0; i < ui->c_s_table->rowCount(); i++)  //刪除按鈕
        delete  ui->c_s_table->takeItem(i, 3);
    ui->c_s_table->setRowCount(0);
    ui->c_s_table->setColumnCount(4);
    ui->c_s_table->setSortingEnabled(false);
    QStringList title;
    title << "ID" << "Username" << "Password" << "-";
    ui->c_s_table->setHorizontalHeaderLabels(title);
    ui->c_s_table->horizontalHeaderItem(0)->setFont(font_record);
    ui->c_s_table->horizontalHeaderItem(1)->setFont(font_record);

    ui->c_s_table->setColumnWidth(0,186);
    ui->c_s_table->setColumnWidth(1,187);
    ui->c_s_table->setColumnWidth(2,187);
    ui->c_s_table->setColumnWidth(3,59);

    query->exec("SELECT * FROM seller_list;");
    while(query->next())
    {
        QString id_from_sql=query->value(0).toString();
        QString name_from_sql=query->value(1).toString();
        if(name_from_sql=="manager")
            continue;
        QString pwd_from_sql=query->value(2).toString();
        ui->c_s_table->insertRow(ui->c_s_table->rowCount());
        ui->c_s_table->setItem(ui->c_s_table->rowCount()-1,col_id,new QTableWidgetItem(id_from_sql));
        ui->c_s_table->setItem(ui->c_s_table->rowCount()-1,col_name,new QTableWidgetItem(name_from_sql));
        ui->c_s_table->setItem(ui->c_s_table->rowCount()-1,col_pass,new QTableWidgetItem(pwd_from_sql));

        QPushButton *button = new QPushButton("刪除");
        button->setStyleSheet("QPushButton{background-color:rgba(212,109,104,100%);\
                              color:white; border-radius:2px; font:bold;}"
                              "QPushButton:hover{background-color:rgba(183,78,73,100%); color:white;}");

        connect(button, &QPushButton::clicked, this, [this,name_from_sql](){qDebug( )<< "刪除";
            query->exec("DELETE FROM seller_list WHERE username='"+name_from_sql+"';");
            QMessageBox::information(this,"Deletion Done","You have deleted seller:"+name_from_sql+" from list."); on_actionStaff_List_triggered();});

        ui->c_s_table->setCellWidget(ui->c_s_table->rowCount()-1, 3, button);
    }
    if(ui->c_s_table->rowCount()==0)ui->btn_delete_all->setDisabled(true);
    else ui->btn_delete_all->setDisabled(false);
    ui->stackedWidget->setCurrentIndex(m_account_manage_page);

    ui->c_s_table->setSortingEnabled(true);

}
void MainWindow::on_actionFast_Release_triggered()
{
    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    AddGoods_list *add_window = new AddGoods_list(this);
    add_window->setWindowTitle("卡片上架");

    ui->stackedWidget->setCurrentIndex(frontpage);
    add_window->show();

    delete load_window;
}

void MainWindow::on_actionFAST_SHOP_triggered()
{
    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    if(is_test)
    {
        QString name = "test", pass = "test";
        c = new Customer(0, name, pass, 81000, 81000);
    }
    Shop_list *shop_window = new Shop_list(c, this);
    shop_window->setWindowTitle("卡片購買");

    connect(shop_window,SIGNAL(update_money_request()),this,SLOT(update_money()));
    connect(shop_window,SIGNAL(update_bag_request()),this,SLOT(update_bag()));

    ui->stackedWidget->setCurrentIndex(frontpage);
    shop_window->show();

    delete load_window;
}

void MainWindow::on_actionFast_Manage_triggered()
{
    Loading_window *load_window = new Loading_window(this);
    load_window->setWindowTitle("Loading...");
    load_window->show();

    Manage_list *manage_list = new Manage_list(this);
    manage_list->setWindowTitle("商品管理");

    ui->stackedWidget->setCurrentIndex(frontpage);
    manage_list->show();

    delete load_window;
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    set_piechart(c->mybag(),checked);
    ui->piechart->update();
    //qDebug()<<"pie chart unique:"<<checked;
}

void MainWindow::on_btn_delete_all_clicked()
{
   bool yes=false;
   Double_check_dialog* d=new Double_check_dialog;
   d->setWindowTitle("Delete All member record");
   connect(d,&Double_check_dialog::confirm,this,[&yes](){yes=true;});
   d->exec();
   if(yes)
   {
       if(in_cus_list)
       {
           vector<QString> names;
           query->exec("SELECT id FROM customer_list;");
           while(query->next())
               names.push_back(query->value(0).toString());
           for(auto &x:names)
               query->exec("DROP c_"+x+";");
           query->exec("TRUNCATE customer_list;");
           query->exec("TRUNCATE customer_info;");
           QMessageBox::information(this,"Deletion Done","You have deleted all the data.");
           on_actionCustomer_List_triggered();
       }
       else
       {
           query->exec("TRUNCATE seller_list;");
           QMessageBox::information(this,"Deletion Done","You have deleted all the data.");
           on_actionStaff_List_triggered();
       }
   }

}
void MainWindow::on_btn_delete_allcard_clicked()
{
    bool yes=false;
    Double_check_dialog* d=new Double_check_dialog;
    d->setWindowTitle("Delete All cards in bag");
    connect(d,&Double_check_dialog::confirm,this,[&yes](){yes=true;});
    d->exec();
    if(yes)
    {
        c->mybag().clear();
        query->exec("SELECT SUM(count) FROM c_"+QString::number(c->getID())+" WHERE star=false;");
        query->next();
        int totall_delete=query->value(0).toInt();
        c->earnPoint(totall_delete*100);
        query->exec("DELETE FROM c_"+QString::number(c->getID())+" WHERE star=false;");
        QMessageBox::information(this,"Deletion Done","You have deleted all the cards except cards with star.\nAnd Earned "+QString::number(totall_delete*100)+"p in return.");
    }
    customer_bag_calltobag();
    on_actionMyBag_triggered();
}

void MainWindow::on_pushButton_clicked()
{
    switch(ui->comboBoxsort->currentIndex())
    {
    case 2 ... 3:
        sort(c->mybag().begin(),c->mybag().end(),Card_sorter(longest));
        break;
    case 4 ... 5:
        sort(c->mybag().begin(),c->mybag().end(),Card_sorter(most));
        break;
    case 6 :
        sort(c->mybag().begin(),c->mybag().end(),Card_sorter(type));
        break;
    default:
        c->mybag().clear();
        customer_bag_calltobag();
    }
    if(ui->comboBoxsort->currentIndex()%2==1)
        reverse(c->mybag().begin(),c->mybag().end());
    on_actionMyBag_triggered();
}

void MainWindow::on_github_butt_clicked()
{
    QString link = "https://github.com/freefrit/AOOP_DogeShop";
    QDesktopServices::openUrl(QUrl(link));
}

