#include "author_code_dialog.h"
#include "ui_author_code_dialog.h"

Author_code_dialog::Author_code_dialog(Manager* m,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Author_code_dialog)
{
    ui->setupUi(this);
    tmp=m;
    qDebug()<<"ok here";
    ui->lineedit_newcode->setInputMask("9999");
    //ui->lineedit_newcode->setMaxLength(4);
    ui->lineedit_newcode->setFocus();
    update_lcd_display();
}
Author_code_dialog::~Author_code_dialog()
{
    delete ui;
}
void Author_code_dialog::update_lcd_display()
{
    ui->lcdNumber->display(tmp->getdigit(0));
    ui->lcdNumber_2->display(tmp->getdigit(1));
    ui->lcdNumber_3->display(tmp->getdigit(2));
    ui->lcdNumber_4->display(tmp->getdigit(3));
}
void Author_code_dialog::on_lineedit_newcode_returnPressed()
{
    tmp->edit_code(ui->lineedit_newcode->text());
    update_lcd_display();
    emit update_request();
}

