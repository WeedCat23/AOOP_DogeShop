#ifndef DOUBLE_CHECK_DIALOG_H
#define DOUBLE_CHECK_DIALOG_H

#include <QDialog>

namespace Ui {
class Double_check_dialog;
}

class Double_check_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Double_check_dialog(QWidget *parent = nullptr);
    ~Double_check_dialog();
signals:
    void confirm();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Double_check_dialog *ui;
};

#endif // DOUBLE_CHECK_DIALOG_H
