#include "changepwd_dialog.h"
#include "ui_changepwd_dialog.h"
#include <QMessageBox>

ChangePwd_Dialog::ChangePwd_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePwd_Dialog)
{
    ui->setupUi(this);
    QPalette sample_palette_error;
    QColor color;
    color.setRgba(0x50FF0000);
    sample_palette_error.setColor(QPalette::Window,color);
    sample_palette_error.setColor(QPalette::WindowText, Qt::red);
    ui->label->setPalette(sample_palette_error);
    ui->label->setAutoFillBackground(true);
    ui->label->hide();
    ui->lineEdit_newPwd1->setEchoMode(QLineEdit::Password);
    ui->lineEdit_newPwd2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_passConfirm->setEchoMode(QLineEdit::Password);
    ui->lineEdit_passConfirm->setFocus();
    ui->lineEdit_passConfirm->setTabOrder(ui->lineEdit_passConfirm,ui->lineEdit_newPwd1);
    ui->lineEdit_passConfirm->setTabOrder(ui->lineEdit_newPwd1,ui->lineEdit_newPwd2);
    ui->lineEdit_passConfirm->setTabOrder(ui->lineEdit_newPwd2,ui->lineEdit_passConfirm);
}
ChangePwd_Dialog::~ChangePwd_Dialog()
{
    delete ui;
}
void ChangePwd_Dialog::on_btn_update_clicked()
{
    ui->label->hide();
    if(!x->login(ui->lineEdit_passConfirm->text()))
    {
        ui->label->setText("Old password incorrect.");
        ui->label->show();
        ui->lineEdit_passConfirm->clear();
        return;
    }
    if(!QString::compare(ui->lineEdit_newPwd1->text(),ui->lineEdit_passConfirm->text()))
    {
        ui->label->setText("New password shouldn't be identical to old one.");
        ui->label->show();
        ui->lineEdit_newPwd1->clear();
        ui->lineEdit_newPwd2->clear();
        return;
    }
    if(validPass(ui->lineEdit_newPwd1->text())){
        if(QString::compare(ui->lineEdit_newPwd1->text(),ui->lineEdit_newPwd2->text()))
        {
            ui->label->setText("New passwords don't match.");
            ui->label->show();
            ui->lineEdit_newPwd2->clear();
        }
        else if(x->edit_password(ui->lineEdit_passConfirm->text(),ui->lineEdit_newPwd1->text()))
        {
            QMessageBox::information(this,"Update Complete Message","New password has been updated.\nPlease relogin after this message.");
            emit update_request(ui->lineEdit_newPwd1->text());
            close();
        }
    }
    else
    {
        ui->label->setText("Invalid new Password.");
        ui->label->show();
        ui->lineEdit_newPwd1->clear();
        ui->lineEdit_newPwd2->clear();
    }
}
bool ChangePwd_Dialog::validPass(QString pass){
    bool contain_english=false,contain_num=false;
    if(pass.length()<6||pass.length()>12)
        return false;
    for(auto& c:pass)   {
        if(c>='0'&&c<='9'){contain_num=true;}
        else if(c>='a'&&c<='z'){contain_english=true;}
        else if(c>='A'&&c<='Z'){contain_english=true;}
        else
            return false;
    }
    return contain_english&&contain_num;
}
void ChangePwd_Dialog::on_toolbtn_revealPass1_pressed()
{
    ui->lineEdit_newPwd1->setEchoMode(QLineEdit::Normal);
}
void ChangePwd_Dialog::on_toolbtn_revealPass1_released()
{
    ui->lineEdit_newPwd1->setEchoMode(QLineEdit::Password);
}
void ChangePwd_Dialog::on_toolbtn_revealPass2_pressed()
{
    ui->lineEdit_newPwd2->setEchoMode(QLineEdit::Normal);
}
void ChangePwd_Dialog::on_toolbtn_revealPass2_released()
{
    ui->lineEdit_newPwd2->setEchoMode(QLineEdit::Password);
}


