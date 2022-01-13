#include "double_check_dialog.h"
#include "ui_double_check_dialog.h"

Double_check_dialog::Double_check_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Double_check_dialog)
{
    ui->setupUi(this);
}

Double_check_dialog::~Double_check_dialog()
{
    delete ui;
}

void Double_check_dialog::on_pushButton_clicked()
{
    emit confirm();
    close();
}
void Double_check_dialog::on_pushButton_2_clicked()
{
    close();
}

