#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlResult>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "changepwd_dialog.h"
#include "exchange_popup.h"
#include "author_code_dialog.h"
#include "double_check_dialog.h"

class LoginWindowPopUpForm;
class Customer;
class Seller;
class Manager;

enum stackpage{
    emptypage,frontpage,c_info_page,c_wallet_page,
               c_bag_page,m_account_manage_page};
enum column_num{
    col_id,col_name,col_pass,col_phone
};
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    friend void login(bool){

    };

    void returnPage();

private slots:
    void popup_close_cus();
    void popup_close_sel();
    void popup_close_man();
    void popup_close_test();
    void update_password(QString);
    void update_money();
    void update_bag();
    void delete_card(Card_in_bag);
    void delete_cus(QString,QString);
    void on_actionLogin_triggered();
    void on_actionSignup_triggered();
    void on_actionHome_triggered();
    void on_actionmyInfo_triggered();
    void on_actionLog_out_triggered();
    void on_actionLog_out_2_triggered();  
    void on_actionRelease_Card_triggered();
    void on_lineEdit_cellphone_selectionChanged();
    void on_actionDOGE_SHOP_triggered();
    void on_btn_cus_change_pwd_clicked();
    void on_actionMyWallet_triggered();
    void on_actionShop_Manage_triggered();
    void on_actionMyBag_triggered();
    void on_btn_cus_change_pwd_2_clicked();
    void on_btn_cus_change_pwd_3_clicked();
    void on_actionManage_Password_triggered();
    void on_btn_c_infoupdate_clicked();
    void on_actionAuthorization_Code_triggered();

    void on_actionLog_Out_triggered();

    void on_actionCustomer_List_triggered();

    void on_actionStaff_List_triggered();

    void on_actionFast_Release_triggered();
    void on_radioButton_toggled(bool checked);
    void on_btn_delete_all_clicked();
    void on_btn_delete_allcard_clicked();
    void on_actionFAST_SHOP_triggered();


    void on_pushButton_clicked();

    void on_actionFast_Manage_triggered();

    void on_github_butt_clicked();


private:
    bool is_test,in_cus_list;
    int prePage;
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QSqlQuery *query;
    QString sql_command;
    QFont font_record;
    Seller* s;
    Customer* c;
    Manager* m;
    LoginWindowPopUpForm* m_login_window;
    bool edit_not_finish;
    void logout_display();
    void build_cus_list();
    void build_sel_list();
    void myinfo_default();
    void set_piechart(vector<Card_in_bag>,bool);
    void delete_card_in_bag(int);
    void customer_info_callin();
    void customer_wallet_callin();
    void customer_bag_calltobag();
    void clear_layout(QLayout* layout);

};

#endif // MAINWINDOW_H
