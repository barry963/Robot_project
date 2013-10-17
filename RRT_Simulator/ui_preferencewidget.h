/********************************************************************************
** Form generated from reading UI file 'preferencewidget.ui'
**
** Created: Mon Jul 22 06:53:49 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCEWIDGET_H
#define UI_PREFERENCEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PreferenceWidget
{
public:
    QFormLayout *formLayout_2;
    QFormLayout *formLayout;
    QLabel *lb_algorithm;
    QLabel *lb_edgeLength;
    QComboBox *cb_edgeLength;
    QLabel *lb_iteration;
    QComboBox *cb_iteration;
    QLabel *lb_tryGoal;
    QComboBox *cb_tryGoal;
    QComboBox *cb_algorithm;

    void setupUi(QWidget *PreferenceWidget)
    {
        if (PreferenceWidget->objectName().isEmpty())
            PreferenceWidget->setObjectName(QString::fromUtf8("PreferenceWidget"));
        PreferenceWidget->resize(281, 133);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PreferenceWidget->sizePolicy().hasHeightForWidth());
        PreferenceWidget->setSizePolicy(sizePolicy);
        formLayout_2 = new QFormLayout(PreferenceWidget);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        lb_algorithm = new QLabel(PreferenceWidget);
        lb_algorithm->setObjectName(QString::fromUtf8("lb_algorithm"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lb_algorithm);

        lb_edgeLength = new QLabel(PreferenceWidget);
        lb_edgeLength->setObjectName(QString::fromUtf8("lb_edgeLength"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lb_edgeLength);

        cb_edgeLength = new QComboBox(PreferenceWidget);
        cb_edgeLength->setObjectName(QString::fromUtf8("cb_edgeLength"));

        formLayout->setWidget(1, QFormLayout::FieldRole, cb_edgeLength);

        lb_iteration = new QLabel(PreferenceWidget);
        lb_iteration->setObjectName(QString::fromUtf8("lb_iteration"));

        formLayout->setWidget(2, QFormLayout::LabelRole, lb_iteration);

        cb_iteration = new QComboBox(PreferenceWidget);
        cb_iteration->setObjectName(QString::fromUtf8("cb_iteration"));

        formLayout->setWidget(2, QFormLayout::FieldRole, cb_iteration);

        lb_tryGoal = new QLabel(PreferenceWidget);
        lb_tryGoal->setObjectName(QString::fromUtf8("lb_tryGoal"));

        formLayout->setWidget(3, QFormLayout::LabelRole, lb_tryGoal);

        cb_tryGoal = new QComboBox(PreferenceWidget);
        cb_tryGoal->setObjectName(QString::fromUtf8("cb_tryGoal"));

        formLayout->setWidget(3, QFormLayout::FieldRole, cb_tryGoal);

        cb_algorithm = new QComboBox(PreferenceWidget);
        cb_algorithm->setObjectName(QString::fromUtf8("cb_algorithm"));

        formLayout->setWidget(0, QFormLayout::FieldRole, cb_algorithm);


        formLayout_2->setLayout(0, QFormLayout::LabelRole, formLayout);


        retranslateUi(PreferenceWidget);

        QMetaObject::connectSlotsByName(PreferenceWidget);
    } // setupUi

    void retranslateUi(QWidget *PreferenceWidget)
    {
        PreferenceWidget->setWindowTitle(QApplication::translate("PreferenceWidget", "Form", 0, QApplication::UnicodeUTF8));
        lb_algorithm->setText(QApplication::translate("PreferenceWidget", "Algorithm", 0, QApplication::UnicodeUTF8));
        lb_edgeLength->setText(QApplication::translate("PreferenceWidget", "EdgeLength", 0, QApplication::UnicodeUTF8));
        lb_iteration->setText(QApplication::translate("PreferenceWidget", "Iteration", 0, QApplication::UnicodeUTF8));
        lb_tryGoal->setText(QApplication::translate("PreferenceWidget", "TryGoal", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PreferenceWidget: public Ui_PreferenceWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCEWIDGET_H
