#ifndef PIECHARTWIDGET_H
#define PIECHARTWIDGET_H

#include <QWidget>

class PieChartWidget : public QWidget
{
    Q_OBJECT
    int magic_percentage;
    int trap_percentage;
    int monster_percentage;
public:
    explicit PieChartWidget(QWidget *parent = nullptr);
    void setpercentage(double ,double,double);
protected:
    void paintEvent(QPaintEvent*);
signals:

};

#endif // PIECHARTWIDGET_H
