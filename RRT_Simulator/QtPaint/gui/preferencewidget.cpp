#include "gui/preferencewidget.h"
#include "ui_preferencewidget.h"

#include <QDebug>

PreferenceWidget::PreferenceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreferenceWidget)
{
    ui->setupUi(this);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->cb_algorithm->addItem("RRT");
    ui->cb_algorithm->addItem("RRT+");
    ui->cb_algorithm->addItem("RRT*");

    ui->cb_edgeLength->addItem("4");
    ui->cb_edgeLength->addItem("5");
    ui->cb_edgeLength->addItem("8");
    ui->cb_edgeLength->addItem("10");
    ui->cb_edgeLength->addItem("15");
    ui->cb_edgeLength->addItem("20");
    ui->cb_edgeLength->addItem("25");
    ui->cb_edgeLength->addItem("99999");

    ui->cb_iteration->addItem("500");
    ui->cb_iteration->addItem("1000");
    ui->cb_iteration->addItem("1500");
    ui->cb_iteration->addItem("2000");
    ui->cb_iteration->addItem("5000");
    ui->cb_iteration->addItem("10000");
    ui->cb_iteration->addItem("100000");
    ui->cb_iteration->addItem("999999");

    ui->cb_tryGoal->addItem("Never");
    ui->cb_tryGoal->addItem("1%");
    ui->cb_tryGoal->addItem("10%");
    ui->cb_tryGoal->addItem("20%");
    ui->cb_tryGoal->addItem("25%");
    ui->cb_tryGoal->addItem("33");
    ui->cb_tryGoal->addItem("50%");
    ui->cb_tryGoal->addItem("Always");
    ui->cb_tryGoal->addItem("Adaptive");

    connect(ui->cb_algorithm,SIGNAL(currentIndexChanged(QString)),this,SIGNAL(rrtTextChangedSignal(QString)));
}


PreferenceWidget::~PreferenceWidget()
{
    delete ui;
}
