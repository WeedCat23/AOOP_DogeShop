#ifndef LOGINWINDOWPOPUPFORM_H
#define LOGINWINDOWPOPUPFORM_H

#include <QWidget>
#include <QSqlResult>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <qstackedwidget.h>
#include "customer.h"
#include "seller.h"
#include "manager.h"

enum state{
    login_,signup_
};
enum identity{
    cus,sel,man,test
};
namespace Ui {
class LoginWindowPopUpForm;

}
class MainWindow;
class LoginWindowPopUpForm : public QWidget{
    Q_OBJECT

public:
    explicit LoginWindowPopUpForm(QWidget *parent = nullptr);
    ~LoginWindowPopUpForm();
    void syncpage(QStackedWidget* pages){mypage=pages;}
    void syncdatabase(QSqlDatabase& db){database=db;}
    void sync_C_pointer(Customer*&cp){cp=c;}
    void sync_S_pointer(Seller*&sp){sp=s;}
    void sync_M_pointer(Manager*&mp){mp=dynamic_cast<Manager*>(s);}

    void setprePage(int n){prePage=n;}
    bool is_loggedin(){
        return login_success;
    };
    void signOrLog(state);
    void logout();
signals:
    void cusLoggedin();
    void selLoggedin();
    void manLoggedin();
    void testLoggedin();
private:
    Ui::LoginWindowPopUpForm *ui;
    QStackedWidget* mypage;
    QSqlDatabase database;
    QSqlQuery *query;
    QString name_in, password_in,sql_command,realcode;
    Customer* c;
    Seller* s;
    identity identity;
    int prePage;
    bool login_success,valid_user;


    bool validName(QString);
    bool validPass(QString);
    void reset_error_labels();
    void initialize_lineedits();
    bool c_duplicate_username(QString);
    bool s_duplicate_username(QString);
    void retrieve_customer(QString);
    void retrieve_seller(QString);


private slots:
    void on_c_signup_button_clicked();
    void on_s_signup_button_clicked();
    void on_Login_button_clicked();
    void on_lineedit_ID_returnPressed();
    void on_btn_return_clicked();
    void on_lineedit_ID_selectionChanged();
    void on_commandLinkButton_clicked();
    void on_commandLinkButton_2_clicked();
    void on_commandLinkButton_3_clicked();
    void on_to_main_btn_clicked();
    void on_Login_button_pass_clicked();
    void on_lineedit_password_selectionChanged();
    void on_toolbtn_c_revealPass_pressed();
    void on_toolbtn_c_revealPass_released();
    void on_toolbtn_c_revealPass2_pressed();
    void on_toolbtn_c_revealPass2_released();
    void on_toolbtn_s_revealPass_pressed();
    void on_toolbtn_s_revealPass_released();
    void on_toolbtn_s_revealPass2_pressed();
    void on_toolbtn_s_revealPass2_released();
    void on_tabWidget_tabBarClicked(int index);
    void on_lineedit_password_returnPressed();
    void on_Hidden_butt_clicked();
};

#endif // LOGINWINDOWPOPUPFORM_H
