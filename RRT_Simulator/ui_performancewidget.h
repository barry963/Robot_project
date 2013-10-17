/********************************************************************************
** Form generated from reading UI file 'performancewidget.ui'
**
** Created: Sun Jul 21 09:49:37 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PERFORMANCEWIDGET_H
#define UI_PERFORMANCEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PerformanceWidget
{
public:
    QFormLayout *formLayout;
    QLabel *lb_iteration;
    QLabel *lb_iteration_val;
    QLabel *lb_nodeCount;
    QLabel *lb_pathLength;
    QLabel *lb_pathLength_val;
    QLabel *lb_nodeCount_val;

    void setupUi(QWidget *PerformanceWidget)
    {
        if (PerformanceWidget->objectName().isEmpty())
            PerformanceWidget->setObjectName(QString::fromUtf8("PerformanceWidget"));
        PerformanceWidget->resize(152, 83);
        formLayout = new QFormLayout(PerformanceWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        lb_iteration = new QLabel(PerformanceWidget);
        lb_iteration->setObjectName(QString::fromUtf8("lb_iteration"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lb_iteration);

        lb_iteration_val = new QLabel(PerformanceWidget);
        lb_iteration_val->setObjectName(QString::fromUtf8("lb_iteration_val"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lb_iteration_val);

        lb_nodeCount = new QLabel(PerformanceWidget);
        lb_nodeCount->setObjectName(QString::fromUtf8("lb_nodeCount"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lb_nodeCount);

        lb_pathLength = new QLabel(PerformanceWidget);
        lb_pathLength->setObjectName(QString::fromUtf8("lb_pathLength"));

        formLayout->setWidget(2, QFormLayout::LabelRole, lb_pathLength);

        lb_pathLength_val = new QLabel(PerformanceWidget);
        lb_pathLength_val->setObjectName(QString::fromUtf8("lb_pathLength_val"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lb_pathLength_val);

        lb_nodeCount_val = new QLabel(PerformanceWidget);
        lb_nodeCount_val->setObjectName(QString::fromUtf8("lb_nodeCount_val"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lb_nodeCount_val);


        retranslateUi(PerformanceWidget);

        QMetaObject::connectSlotsByName(PerformanceWidget);
    } // setupUi

    void retranslateUi(QWidget *PerformanceWidget)
    {
        PerformanceWidget->setWindowTitle(QApplication::translate("PerformanceWidget", "Form", 0, QApplication::UnicodeUTF8));
        lb_iteration->setText(QApplication::translate("PerformanceWidget", "Iteration", 0, QApplication::UnicodeUTF8));
        lb_iteration_val->setText(QString());
        lb_nodeCount->setText(QApplication::translate("PerformanceWidget", "NodeCount", 0, QApplication::UnicodeUTF8));
        lb_pathLength->setText(QApplication::translate("PerformanceWidget", "PathLength", 0, QApplication::UnicodeUTF8));
        lb_pathLength_val->setText(QString());
        lb_nodeCount_val->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PerformanceWidget: public Ui_PerformanceWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERFORMANCEWIDGET_H
