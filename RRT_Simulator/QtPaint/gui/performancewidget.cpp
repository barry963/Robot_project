#include <QDebug>

#include "gui/performancewidget.h"
#include "ui_performancewidget.h"

PerformanceWidget::PerformanceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PerformanceWidget)
{
    ui->setupUi(this);
}

PerformanceWidget::~PerformanceWidget()
{
    delete ui;
}
void PerformanceWidget::currentIterationChangedSlot(int iteration)
{
    qDebug()<< "received:"<<iteration;
    ui->lb_iteration_val->setText(QString::number(iteration));
}
void PerformanceWidget::currentNodecountChangedSlot(int nodecount)
{
    ui->lb_nodeCount_val->setText(QString::number(nodecount));
}
void PerformanceWidget::currentPathLengthChangedSlot(int pathlength)
{
    ui->lb_pathLength_val->setText(QString::number(pathlength));
}
