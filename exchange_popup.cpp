#include"exchange_popup.h"
#include"ui_exchange_popup.h"

Exchange_popup::Exchange_popup(Customer* c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Exchange_popup)
{
    ui->setupUi(this);
    ratio=20;
    x=c;
    ui->spinBox->setRange(0,c->get_money_point()/ratio);
    ui->spinBox->setSuffix("$");
    ui->spinBox->setSingleStep(1);
    ui->spinBox->setValue(0);
    ui->label_cash->setText(QString::number(x->get_money_cash())+"$");
    ui->label_point->setText(QString::number(x->get_money_point(),'f',2)+"p");
    ui->label_exchange_rate->setText(QString::number(ratio)+" : 1");
}
Exchange_popup   ::~Exchange_popup   ()
{
    delete ui;
}

void Exchange_popup   ::on_spinBox_valueChanged(int arg1)
{
    ui->label_cash->setText(QString::number(x->get_money_cash()+arg1)+"$");
    ui->label_point->setText(QString::number(x->get_money_point()-arg1*ratio)+"p");
}
void Exchange_popup   ::on_btn_confirm_clicked()
{
    x->exchangePoint(ui->spinBox->value());
    emit update_request();
    close();
}

