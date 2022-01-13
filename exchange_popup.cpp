#include"exchange_popup.h"
#include"ui_exchange_popup.h"

Exchange_popup::Exchange_popup(Customer* c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Exchange_popup)
{
    ui->setupUi(this);
    ratio=20;
    x=c;
    m=point_to_cash;
    ui->spinBox->setRange(0,c->get_money_point()/ratio);
    ui->spinBox->setSuffix("$");
    ui->spinBox->setSingleStep(1);
    ui->spinBox->setValue(0);
    ui->label_mode->setText("Points : Cash");
    ui->label_earned->setText("Cash earned after exchange:");
    ui->label_A->setText(QString::number(x->get_money_cash())+"$");
    ui->label_remain->setText("Points remain after exchange:");
    ui->label_B->setText(QString::number(x->get_money_point(),'f',2)+"p");
    ui->label_exchange_rate->setText(QString::number(ratio)+" : 1");
}
Exchange_popup::Exchange_popup(Customer* c,int i,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Exchange_popup)
{
    ui->setupUi(this);
    ratio=100;
    x=c;
    index=i;
    card_x=c->mybag()[i];
    m=card_to_point;
    ui->spinBox->setRange(0,card_x.num);
    ui->spinBox->setSingleStep(1);
    ui->spinBox->setValue(0);
    ui->label_mode->setText("Cards : Points");
    ui->label_earned->setText("Points earned after exchange:");
    ui->label_A->setText(QString::number(x->get_money_point(),'f',2)+"p");
    ui->label_remain->setText("Cards remain after exchange:");
    ui->label_B->setText(QString::number(card_x.num));
    ui->label_exchange_rate->setText("1 : "+QString::number(ratio));
}
Exchange_popup   ::~Exchange_popup   ()
{
    delete ui;
}

void Exchange_popup   ::on_spinBox_valueChanged(int arg1)
{
    if(m==point_to_cash)
    {
        ui->label_A->setText(QString::number(x->get_money_cash()+arg1)+"$");
        ui->label_B->setText(QString::number(x->get_money_point()-arg1*ratio,'f',2)+"p");
    }
    else if(m==card_to_point)
    {
        ui->label_A->setText(QString::number(x->get_money_point()+arg1*ratio,'f',2)+"p");
        ui->label_B->setText(QString::number(card_x.num-arg1));
    }
}
void Exchange_popup   ::on_btn_confirm_clicked()
{
    if(m==point_to_cash)
        x->exchangePoint(ui->spinBox->value());
    else if(m==card_to_point)
    {
        x->earnPoint(ui->spinBox->value()*ratio);
        x->mybag()[index].num-=ui->spinBox->value();
        if(x->mybag()[index].num==0)
            x->mybag().erase(x->mybag().begin()+index);
        card_x.num-=ui->spinBox->value();
        emit update_bag_request(card_x);
    }
    emit update_money_request();
    close();
}

