/********************************************************************************
** Form generated from reading UI file 'strategy.ui'
**
** Created: Tue Feb 11 22:06:58 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STRATEGY_H
#define UI_STRATEGY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "user_interface/field_related/field_view.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *open;
    QAction *save;
    QAction *save_as;
    QAction *exit;
    QAction *preferences;
    QWidget *strategy_central_widget;
    QMenuBar *menu_bar;
    QMenu *file;
    QMenu *menu_Edit;
    QStatusBar *status_bar;
    QDockWidget *control_dockwidget;
    QWidget *control_widget;
    QTabWidget *control_panel;
    QWidget *general_tab;
    QWidget *horizontalLayoutWidget;
    QGridLayout *general_layout;
    QGroupBox *yrobot_group_box;
    QWidget *verticalLayoutWidget_8;
    QVBoxLayout *yrobot_layout;
    QCheckBox *yrobot1_button;
    QCheckBox *yrobot2_button;
    QCheckBox *yrobot3_button;
    QCheckBox *yrobot4_button;
    QCheckBox *yrobot5_button;
    QGroupBox *brobot_group_box;
    QWidget *verticalLayoutWidget_7;
    QVBoxLayout *brobot_layout;
    QCheckBox *brobot1_button;
    QCheckBox *brobot2_button;
    QCheckBox *brobot3_button;
    QCheckBox *brobot4_button;
    QCheckBox *brobot5_button;
    QGroupBox *side_selection_box;
    QWidget *verticalLayoutWidget_10;
    QVBoxLayout *side_selection_layout;
    QRadioButton *select_up_side;
    QRadioButton *select_down_side;
    QGroupBox *color_group_box;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *color_layout;
    QRadioButton *blue_color_button;
    QRadioButton *yellow_color_button;
    QWidget *human_control_tab;
    QWidget *horizontalLayoutWidget_3;
    QVBoxLayout *human_control_layout;
    QGroupBox *robot_selection_group_box;
    QWidget *verticalLayoutWidget_4;
    QGridLayout *robot_selection_layout;
    QRadioButton *human_robot0_button;
    QRadioButton *human_robot1_button;
    QRadioButton *human_robot2_button;
    QRadioButton *human_robot3_button;
    QRadioButton *human_robot4_button;
    QRadioButton *human_robot5_button;
    QSpacerItem *human_control_spacer;
    QWidget *strategy_tab;
    QWidget *verticalLayoutWidget_11;
    QVBoxLayout *strategy_layout;
    QGroupBox *mode_box;
    QWidget *gridLayoutWidget;
    QGridLayout *mode_layout;
    QRadioButton *run_mode;
    QRadioButton *test_mode;
    QTabWidget *strategy_type_tab;
    QWidget *strategy_position_tab;
    QWidget *verticalLayoutWidget_13;
    QVBoxLayout *strategy_position_layout;
    QGroupBox *strategy_position_box;
    QWidget *verticalLayoutWidget_3;
    QGridLayout *strategy_selection_layout;
    QRadioButton *position_start_button;
    QRadioButton *position_penalty_button;
    QRadioButton *position_kick_button;
    QRadioButton *position_pass_button;
    QRadioButton *position_rebound_button;
    QRadioButton *position_deflection_button;
    QRadioButton *position_loose_ball_button;
    QWidget *strategy_move_tab;
    QWidget *verticalLayoutWidget_14;
    QVBoxLayout *strategy_move_layout;
    QGroupBox *strategy_move_box;
    QWidget *verticalLayoutWidget_15;
    QGridLayout *strategy_selection_layout_2;
    QRadioButton *move_middle_button;
    QRadioButton *move_straight_button;
    QRadioButton *move_sin_button;
    QRadioButton *strategy5_radio_button_2;
    QRadioButton *strategy3_radio_button_2;
    QRadioButton *strategy6_radio_button_2;
    QRadioButton *dd;
    QWidget *strategy_shoot_tab;
    QWidget *verticalLayoutWidget_16;
    QVBoxLayout *strategy_shoot_layout;
    QGroupBox *strategy_shoot_box;
    QWidget *verticalLayoutWidget_17;
    QGridLayout *strategy_selection_layout_3;
    QRadioButton *shoot_charge_ball_button;
    QRadioButton *shoot_dribble_shoot_button;
    QRadioButton *shoot_simple_shoot_button;
    QRadioButton *strategy5_radio_button_3;
    QRadioButton *strategy3_radio_button_3;
    QRadioButton *strategy6_radio_button_3;
    QRadioButton *dd_2;
    QWidget *test1;
    QWidget *verticalLayoutWidget_24;
    QVBoxLayout *strategy_shoot_layout_3;
    QGroupBox *strategy_shoot_box_3;
    QWidget *verticalLayoutWidget_25;
    QGridLayout *strategy_selection_layout_7;
    QRadioButton *shoot_button;
    QRadioButton *clear_button;
    QRadioButton *steal_button;
    QRadioButton *success_button;
    QRadioButton *complete_button;
    QRadioButton *dribble_to_position_button;
    QRadioButton *stop_button_2;
    QRadioButton *charge_ball_button;
    QWidget *test2;
    QWidget *verticalLayoutWidget_26;
    QVBoxLayout *strategy_shoot_layout_4;
    QGroupBox *strategy_shoot_box_4;
    QWidget *verticalLayoutWidget_27;
    QGridLayout *strategy_selection_layout_8;
    QRadioButton *block_button;
    QRadioButton *mark_button;
    QRadioButton *goalie_button;
    QRadioButton *velocity_button;
    QRadioButton *defend_point_button;
    QRadioButton *defend_lane_button;
    QRadioButton *defend_line_button;
    QRadioButton *pass_button;
    QWidget *test3;
    QWidget *verticalLayoutWidget_28;
    QVBoxLayout *strategy_shoot_layout_5;
    QGroupBox *strategy_shoot_box_5;
    QWidget *verticalLayoutWidget_29;
    QGridLayout *strategy_selection_layout_9;
    QRadioButton *receive_deflection_button;
    QRadioButton *dribble_to_region_button;
    QRadioButton *spin_to_region_button;
    QRadioButton *dribble_to_shoot_button;
    QRadioButton *active_def_button;
    QRadioButton *receive_pass_button;
    QRadioButton *position_button;
    QRadioButton *pass_and_receive_button;
    QSpacerItem *strategy_spacer;
    QWidget *log_tab;
    QWidget *verticalLayoutWidget_9;
    QVBoxLayout *log_layout;
    QGroupBox *field_record_layout;
    QTableWidget *time_table_widget;
    QTableWidget *score_table_widget;
    QGroupBox *log_group_box;
    QWidget *verticalLayoutWidget_5;
    QGridLayout *log_text_layout;
    QTextEdit *log_text_area;
    QWidget *referee_tab;
    QWidget *verticalLayoutWidget_12;
    QVBoxLayout *referee_layout;
    QGroupBox *ref_control_box;
    QWidget *gridLayoutWidget_2;
    QGridLayout *ref_control_layout;
    QRadioButton *ref_disable_button;
    QRadioButton *ref_stop_button;
    QRadioButton *ref_start_button;
    QRadioButton *ref_auto_button;
    QGroupBox *ref_score_control_panel;
    QWidget *horizontalLayoutWidget_2;
    QVBoxLayout *ref_score_control_layout;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *score_yellow_radio_button;
    QRadioButton *score_blue_radio_button;
    QHBoxLayout *score_button_layout;
    QPushButton *reset_score_button;
    QPushButton *minus_score_button;
    QPushButton *add_score_button;
    QSpacerItem *referee_spacer;
    QGroupBox *control_group_box;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *control_layout;
    QPushButton *start_button;
    QPushButton *pause_button;
    QPushButton *stop_button;
    QGroupBox *source_group_box;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *control_source_layout;
    QRadioButton *computer_button;
    QRadioButton *keyboard_button;
    QRadioButton *joystick_button;
    QTableWidget *generalTableWidget;
    QDockWidget *strategy_graphics_dockwidget;
    QWidget *strategy_graphics_widget;
    FieldView *strategy_graphics_view;
    QTextBrowser *coordinate_text;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1148, 776);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"\n"
"\n"
"\n"
"\n"
"/*****************************************************************************************/\n"
"/* The tab widget frame */\n"
"\n"
" QTabBar::tab {\n"
"    background: gray;\n"
"    border: 10px solid gray; \n"
"    color: white;\n"
"\n"
" }\n"
"\n"
" QTabBar::tab:selected {\n"
"    background: lightgray;\n"
"    border-color: #9B9B9B;\n"
"\n"
"    border-bottom-color: lightgrey; \n"
"    border-right-color: lightgrey;\n"
"    /*\n"
"    border-top-color: lightgrey; \n"
"    border-left-color: lightgrey; \n"
"    */\n"
" }\n"
"\n"
"/* make non-selected tabs look smaller */\n"
" QTabBar::tab:!selected {\n"
"     margin-top: 6px; \n"
" }\n"
"\n"
"/*****************************************************************************************/\n"
"QGroupBox::title { \n"
"    background: gray;\n"
"    color: white;\n"
"    padding: 10px;\n"
" }\n"
"\n"
"QGroupBox\n"
"{\n"
"    background-color: gray;\n"
"    margin-bottom: 2ex; /* leave space at the top for the title */\n"
"    border: 10px soli"
                        "d lightgray; \n"
"}\n"
"/*****************************************************************************************/\n"
"QPushButton\n"
"{\n"
"    color: white;\n"
"    background: gray;\n"
"    border-color: #9B9B9B;\n"
"    border-top:transparent;\n"
"    border-bottom:  transparent;\n"
"    border-right: transparent;\n"
"    border-left:  transparent;\n"
"    padding: 10px;\n"
"\n"
"}\n"
"\n"
" QPushButton::hover { \n"
"    background: lightgray;\n"
"    color: white;\n"
"    padding: 10px;\n"
" }\n"
"QPushButton:focus:active\n"
"{\n"
"    background: lightgray;\n"
"    color: white;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"\n"
"\n"
"/*****************************************************************************************/\n"
"\n"
"QRadioButton {\n"
"    color: white;\n"
"    background: gray;\n"
"    border-top: transparent;\n"
"    border-bottom: transparent;\n"
"    border-right: transparent;\n"
"    border-left: transparent;\n"
"    padding: 10px;\n"
"}\n"
" QRadioButton::hover { \n"
"    background: light"
                        "gray;\n"
"    color: white;\n"
"    padding: 10px;\n"
" }\n"
"\n"
"QRadioButton:checked\n"
"{\n"
"    background: lightgray;\n"
"    color: white;\n"
"    padding: 10px;\n"
"}\n"
"/*****************************************************************************************/\n"
"QCheckBox\n"
"{\n"
"    color: white;\n"
"    background: gray;\n"
"    border-top: transparent;\n"
"    border-bottom: transparent;\n"
"    border-right: transparent;\n"
"    border-left: transparent;\n"
"    padding: 10px;\n"
"\n"
"}\n"
" QCheckBox::hover { \n"
"    background: lightgray;\n"
"    color: white;\n"
"    padding: 10px;\n"
" }\n"
"\n"
"\n"
"/*****************************************************************************************/\n"
"QTableWidget\n"
"{\n"
"    color: white;\n"
"    background: lightgray;\n"
"    padding: 10px;\n"
"\n"
"}\n"
" QTableWidget::hover { \n"
"    background: lightgray;\n"
"    color: white;\n"
"    padding: 10px;\n"
" }\n"
"QHeaderView::section {\n"
"    color: white;\n"
"    background: grey;\n"
""
                        "    border-top: transparent;\n"
"    border-bottom: transparent;\n"
"    border-right: transparent;\n"
"    border-left: transparent;\n"
"}\n"
""));
        open = new QAction(MainWindow);
        open->setObjectName(QString::fromUtf8("open"));
        save = new QAction(MainWindow);
        save->setObjectName(QString::fromUtf8("save"));
        save_as = new QAction(MainWindow);
        save_as->setObjectName(QString::fromUtf8("save_as"));
        exit = new QAction(MainWindow);
        exit->setObjectName(QString::fromUtf8("exit"));
        preferences = new QAction(MainWindow);
        preferences->setObjectName(QString::fromUtf8("preferences"));
        strategy_central_widget = new QWidget(MainWindow);
        strategy_central_widget->setObjectName(QString::fromUtf8("strategy_central_widget"));
        MainWindow->setCentralWidget(strategy_central_widget);
        menu_bar = new QMenuBar(MainWindow);
        menu_bar->setObjectName(QString::fromUtf8("menu_bar"));
        menu_bar->setGeometry(QRect(0, 0, 1148, 25));
        file = new QMenu(menu_bar);
        file->setObjectName(QString::fromUtf8("file"));
        menu_Edit = new QMenu(menu_bar);
        menu_Edit->setObjectName(QString::fromUtf8("menu_Edit"));
        MainWindow->setMenuBar(menu_bar);
        status_bar = new QStatusBar(MainWindow);
        status_bar->setObjectName(QString::fromUtf8("status_bar"));
        MainWindow->setStatusBar(status_bar);
        control_dockwidget = new QDockWidget(MainWindow);
        control_dockwidget->setObjectName(QString::fromUtf8("control_dockwidget"));
        control_dockwidget->setMinimumSize(QSize(500, 100));
        control_widget = new QWidget();
        control_widget->setObjectName(QString::fromUtf8("control_widget"));
        control_panel = new QTabWidget(control_widget);
        control_panel->setObjectName(QString::fromUtf8("control_panel"));
        control_panel->setGeometry(QRect(10, 190, 501, 661));
        control_panel->setStyleSheet(QString::fromUtf8(""));
        control_panel->setMovable(false);
        general_tab = new QWidget();
        general_tab->setObjectName(QString::fromUtf8("general_tab"));
        horizontalLayoutWidget = new QWidget(general_tab);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 281, 601));
        general_layout = new QGridLayout(horizontalLayoutWidget);
        general_layout->setSpacing(2);
        general_layout->setContentsMargins(5, 5, 5, 5);
        general_layout->setObjectName(QString::fromUtf8("general_layout"));
        general_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        general_layout->setContentsMargins(0, 0, 0, 0);
        yrobot_group_box = new QGroupBox(horizontalLayoutWidget);
        yrobot_group_box->setObjectName(QString::fromUtf8("yrobot_group_box"));
        yrobot_group_box->setEnabled(true);
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu"));
        yrobot_group_box->setFont(font);
        yrobot_group_box->setStyleSheet(QString::fromUtf8(""));
        verticalLayoutWidget_8 = new QWidget(yrobot_group_box);
        verticalLayoutWidget_8->setObjectName(QString::fromUtf8("verticalLayoutWidget_8"));
        verticalLayoutWidget_8->setGeometry(QRect(10, 40, 111, 171));
        QFont font1;
        font1.setFamily(QString::fromUtf8("cmr10"));
        verticalLayoutWidget_8->setFont(font1);
        yrobot_layout = new QVBoxLayout(verticalLayoutWidget_8);
        yrobot_layout->setSpacing(2);
        yrobot_layout->setContentsMargins(5, 5, 5, 5);
        yrobot_layout->setObjectName(QString::fromUtf8("yrobot_layout"));
        yrobot_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        yrobot_layout->setContentsMargins(0, 0, 0, 0);
        yrobot1_button = new QCheckBox(verticalLayoutWidget_8);
        yrobot1_button->setObjectName(QString::fromUtf8("yrobot1_button"));
        yrobot1_button->setFont(font);
        yrobot1_button->setFocusPolicy(Qt::TabFocus);
        yrobot1_button->setChecked(true);

        yrobot_layout->addWidget(yrobot1_button);

        yrobot2_button = new QCheckBox(verticalLayoutWidget_8);
        yrobot2_button->setObjectName(QString::fromUtf8("yrobot2_button"));
        yrobot2_button->setFont(font);
        yrobot2_button->setChecked(true);

        yrobot_layout->addWidget(yrobot2_button);

        yrobot3_button = new QCheckBox(verticalLayoutWidget_8);
        yrobot3_button->setObjectName(QString::fromUtf8("yrobot3_button"));
        yrobot3_button->setFont(font);
        yrobot3_button->setFocusPolicy(Qt::TabFocus);
        yrobot3_button->setChecked(true);

        yrobot_layout->addWidget(yrobot3_button);

        yrobot4_button = new QCheckBox(verticalLayoutWidget_8);
        yrobot4_button->setObjectName(QString::fromUtf8("yrobot4_button"));
        yrobot4_button->setFocusPolicy(Qt::TabFocus);
        yrobot4_button->setChecked(true);

        yrobot_layout->addWidget(yrobot4_button);

        yrobot5_button = new QCheckBox(verticalLayoutWidget_8);
        yrobot5_button->setObjectName(QString::fromUtf8("yrobot5_button"));
        yrobot5_button->setFont(font);
        yrobot5_button->setChecked(true);

        yrobot_layout->addWidget(yrobot5_button);


        general_layout->addWidget(yrobot_group_box, 1, 0, 1, 1);

        brobot_group_box = new QGroupBox(horizontalLayoutWidget);
        brobot_group_box->setObjectName(QString::fromUtf8("brobot_group_box"));
        brobot_group_box->setEnabled(true);
        brobot_group_box->setFont(font);
        verticalLayoutWidget_7 = new QWidget(brobot_group_box);
        verticalLayoutWidget_7->setObjectName(QString::fromUtf8("verticalLayoutWidget_7"));
        verticalLayoutWidget_7->setGeometry(QRect(10, 40, 107, 171));
        brobot_layout = new QVBoxLayout(verticalLayoutWidget_7);
        brobot_layout->setSpacing(2);
        brobot_layout->setContentsMargins(5, 5, 5, 5);
        brobot_layout->setObjectName(QString::fromUtf8("brobot_layout"));
        brobot_layout->setContentsMargins(0, 0, 0, 0);
        brobot1_button = new QCheckBox(verticalLayoutWidget_7);
        brobot1_button->setObjectName(QString::fromUtf8("brobot1_button"));
        brobot1_button->setFont(font);
        brobot1_button->setChecked(true);

        brobot_layout->addWidget(brobot1_button);

        brobot2_button = new QCheckBox(verticalLayoutWidget_7);
        brobot2_button->setObjectName(QString::fromUtf8("brobot2_button"));
        brobot2_button->setFont(font);
        brobot2_button->setChecked(true);

        brobot_layout->addWidget(brobot2_button);

        brobot3_button = new QCheckBox(verticalLayoutWidget_7);
        brobot3_button->setObjectName(QString::fromUtf8("brobot3_button"));
        brobot3_button->setFont(font);
        brobot3_button->setChecked(true);

        brobot_layout->addWidget(brobot3_button);

        brobot4_button = new QCheckBox(verticalLayoutWidget_7);
        brobot4_button->setObjectName(QString::fromUtf8("brobot4_button"));
        brobot4_button->setFont(font);
        brobot4_button->setChecked(true);

        brobot_layout->addWidget(brobot4_button);

        brobot5_button = new QCheckBox(verticalLayoutWidget_7);
        brobot5_button->setObjectName(QString::fromUtf8("brobot5_button"));
        brobot5_button->setFont(font);
        brobot5_button->setChecked(true);

        brobot_layout->addWidget(brobot5_button);


        general_layout->addWidget(brobot_group_box, 1, 1, 1, 1);

        side_selection_box = new QGroupBox(horizontalLayoutWidget);
        side_selection_box->setObjectName(QString::fromUtf8("side_selection_box"));
        side_selection_box->setMaximumSize(QSize(16777215, 150));
        verticalLayoutWidget_10 = new QWidget(side_selection_box);
        verticalLayoutWidget_10->setObjectName(QString::fromUtf8("verticalLayoutWidget_10"));
        verticalLayoutWidget_10->setGeometry(QRect(10, 40, 111, 81));
        side_selection_layout = new QVBoxLayout(verticalLayoutWidget_10);
        side_selection_layout->setSpacing(2);
        side_selection_layout->setContentsMargins(5, 5, 5, 5);
        side_selection_layout->setObjectName(QString::fromUtf8("side_selection_layout"));
        side_selection_layout->setContentsMargins(0, 0, 0, 0);
        select_up_side = new QRadioButton(verticalLayoutWidget_10);
        select_up_side->setObjectName(QString::fromUtf8("select_up_side"));

        side_selection_layout->addWidget(select_up_side);

        select_down_side = new QRadioButton(verticalLayoutWidget_10);
        select_down_side->setObjectName(QString::fromUtf8("select_down_side"));

        side_selection_layout->addWidget(select_down_side);


        general_layout->addWidget(side_selection_box, 0, 1, 1, 1);

        color_group_box = new QGroupBox(horizontalLayoutWidget);
        color_group_box->setObjectName(QString::fromUtf8("color_group_box"));
        color_group_box->setMinimumSize(QSize(0, 125));
        color_group_box->setMaximumSize(QSize(16777215, 150));
        color_group_box->setLayoutDirection(Qt::LeftToRight);
        verticalLayoutWidget_6 = new QWidget(color_group_box);
        verticalLayoutWidget_6->setObjectName(QString::fromUtf8("verticalLayoutWidget_6"));
        verticalLayoutWidget_6->setGeometry(QRect(10, 40, 101, 81));
        color_layout = new QVBoxLayout(verticalLayoutWidget_6);
        color_layout->setSpacing(2);
        color_layout->setContentsMargins(5, 5, 5, 5);
        color_layout->setObjectName(QString::fromUtf8("color_layout"));
        color_layout->setSizeConstraint(QLayout::SetFixedSize);
        color_layout->setContentsMargins(0, 0, 0, 0);
        blue_color_button = new QRadioButton(verticalLayoutWidget_6);
        blue_color_button->setObjectName(QString::fromUtf8("blue_color_button"));
        blue_color_button->setChecked(true);

        color_layout->addWidget(blue_color_button);

        yellow_color_button = new QRadioButton(verticalLayoutWidget_6);
        yellow_color_button->setObjectName(QString::fromUtf8("yellow_color_button"));

        color_layout->addWidget(yellow_color_button);


        general_layout->addWidget(color_group_box, 0, 0, 1, 1);

        control_panel->addTab(general_tab, QString());
        human_control_tab = new QWidget();
        human_control_tab->setObjectName(QString::fromUtf8("human_control_tab"));
        horizontalLayoutWidget_3 = new QWidget(human_control_tab);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(0, 0, 351, 581));
        human_control_layout = new QVBoxLayout(horizontalLayoutWidget_3);
        human_control_layout->setContentsMargins(5, 5, 5, 5);
        human_control_layout->setObjectName(QString::fromUtf8("human_control_layout"));
        human_control_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        human_control_layout->setContentsMargins(0, 0, 0, 0);
        robot_selection_group_box = new QGroupBox(horizontalLayoutWidget_3);
        robot_selection_group_box->setObjectName(QString::fromUtf8("robot_selection_group_box"));
        robot_selection_group_box->setMinimumSize(QSize(0, 150));
        robot_selection_group_box->setMaximumSize(QSize(16777215, 200));
        verticalLayoutWidget_4 = new QWidget(robot_selection_group_box);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(10, 40, 221, 81));
        robot_selection_layout = new QGridLayout(verticalLayoutWidget_4);
        robot_selection_layout->setContentsMargins(5, 5, 5, 5);
        robot_selection_layout->setObjectName(QString::fromUtf8("robot_selection_layout"));
        robot_selection_layout->setSizeConstraint(QLayout::SetFixedSize);
        robot_selection_layout->setContentsMargins(0, 0, 0, 0);
        human_robot0_button = new QRadioButton(verticalLayoutWidget_4);
        human_robot0_button->setObjectName(QString::fromUtf8("human_robot0_button"));
        human_robot0_button->setChecked(true);

        robot_selection_layout->addWidget(human_robot0_button, 0, 0, 1, 1);

        human_robot1_button = new QRadioButton(verticalLayoutWidget_4);
        human_robot1_button->setObjectName(QString::fromUtf8("human_robot1_button"));

        robot_selection_layout->addWidget(human_robot1_button, 0, 1, 1, 1);

        human_robot2_button = new QRadioButton(verticalLayoutWidget_4);
        human_robot2_button->setObjectName(QString::fromUtf8("human_robot2_button"));

        robot_selection_layout->addWidget(human_robot2_button, 0, 2, 1, 1);

        human_robot3_button = new QRadioButton(verticalLayoutWidget_4);
        human_robot3_button->setObjectName(QString::fromUtf8("human_robot3_button"));

        robot_selection_layout->addWidget(human_robot3_button, 1, 0, 1, 1);

        human_robot4_button = new QRadioButton(verticalLayoutWidget_4);
        human_robot4_button->setObjectName(QString::fromUtf8("human_robot4_button"));

        robot_selection_layout->addWidget(human_robot4_button, 1, 1, 1, 1);

        human_robot5_button = new QRadioButton(verticalLayoutWidget_4);
        human_robot5_button->setObjectName(QString::fromUtf8("human_robot5_button"));

        robot_selection_layout->addWidget(human_robot5_button, 1, 2, 1, 1);


        human_control_layout->addWidget(robot_selection_group_box);

        human_control_spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        human_control_layout->addItem(human_control_spacer);

        control_panel->addTab(human_control_tab, QString());
        strategy_tab = new QWidget();
        strategy_tab->setObjectName(QString::fromUtf8("strategy_tab"));
        verticalLayoutWidget_11 = new QWidget(strategy_tab);
        verticalLayoutWidget_11->setObjectName(QString::fromUtf8("verticalLayoutWidget_11"));
        verticalLayoutWidget_11->setGeometry(QRect(0, 0, 481, 691));
        strategy_layout = new QVBoxLayout(verticalLayoutWidget_11);
        strategy_layout->setSpacing(2);
        strategy_layout->setContentsMargins(5, 5, 5, 5);
        strategy_layout->setObjectName(QString::fromUtf8("strategy_layout"));
        strategy_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        strategy_layout->setContentsMargins(0, 0, 0, 0);
        mode_box = new QGroupBox(verticalLayoutWidget_11);
        mode_box->setObjectName(QString::fromUtf8("mode_box"));
        mode_box->setMinimumSize(QSize(0, 120));
        mode_box->setMaximumSize(QSize(16777215, 1500));
        gridLayoutWidget = new QWidget(mode_box);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 40, 411, 51));
        mode_layout = new QGridLayout(gridLayoutWidget);
        mode_layout->setSpacing(2);
        mode_layout->setContentsMargins(5, 5, 5, 5);
        mode_layout->setObjectName(QString::fromUtf8("mode_layout"));
        mode_layout->setContentsMargins(0, 0, 0, 0);
        run_mode = new QRadioButton(gridLayoutWidget);
        run_mode->setObjectName(QString::fromUtf8("run_mode"));

        mode_layout->addWidget(run_mode, 0, 0, 1, 1);

        test_mode = new QRadioButton(gridLayoutWidget);
        test_mode->setObjectName(QString::fromUtf8("test_mode"));

        mode_layout->addWidget(test_mode, 0, 1, 1, 1);


        strategy_layout->addWidget(mode_box);

        strategy_type_tab = new QTabWidget(verticalLayoutWidget_11);
        strategy_type_tab->setObjectName(QString::fromUtf8("strategy_type_tab"));
        strategy_type_tab->setMinimumSize(QSize(0, 260));
        strategy_type_tab->setMovable(true);
        strategy_position_tab = new QWidget();
        strategy_position_tab->setObjectName(QString::fromUtf8("strategy_position_tab"));
        verticalLayoutWidget_13 = new QWidget(strategy_position_tab);
        verticalLayoutWidget_13->setObjectName(QString::fromUtf8("verticalLayoutWidget_13"));
        verticalLayoutWidget_13->setGeometry(QRect(0, 0, 461, 210));
        strategy_position_layout = new QVBoxLayout(verticalLayoutWidget_13);
        strategy_position_layout->setSpacing(2);
        strategy_position_layout->setContentsMargins(5, 5, 5, 5);
        strategy_position_layout->setObjectName(QString::fromUtf8("strategy_position_layout"));
        strategy_position_layout->setContentsMargins(0, 0, 0, 0);
        strategy_position_box = new QGroupBox(verticalLayoutWidget_13);
        strategy_position_box->setObjectName(QString::fromUtf8("strategy_position_box"));
        strategy_position_box->setMinimumSize(QSize(0, 200));
        strategy_position_box->setMaximumSize(QSize(16777215, 290));
        verticalLayoutWidget_3 = new QWidget(strategy_position_box);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(10, 10, 420, 176));
        strategy_selection_layout = new QGridLayout(verticalLayoutWidget_3);
        strategy_selection_layout->setSpacing(2);
        strategy_selection_layout->setContentsMargins(5, 5, 5, 5);
        strategy_selection_layout->setObjectName(QString::fromUtf8("strategy_selection_layout"));
        strategy_selection_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        strategy_selection_layout->setContentsMargins(0, 0, 0, 0);
        position_start_button = new QRadioButton(verticalLayoutWidget_3);
        position_start_button->setObjectName(QString::fromUtf8("position_start_button"));
        position_start_button->setChecked(true);

        strategy_selection_layout->addWidget(position_start_button, 0, 0, 1, 1);

        position_penalty_button = new QRadioButton(verticalLayoutWidget_3);
        position_penalty_button->setObjectName(QString::fromUtf8("position_penalty_button"));

        strategy_selection_layout->addWidget(position_penalty_button, 3, 0, 1, 1);

        position_kick_button = new QRadioButton(verticalLayoutWidget_3);
        position_kick_button->setObjectName(QString::fromUtf8("position_kick_button"));

        strategy_selection_layout->addWidget(position_kick_button, 0, 1, 1, 1);

        position_pass_button = new QRadioButton(verticalLayoutWidget_3);
        position_pass_button->setObjectName(QString::fromUtf8("position_pass_button"));

        strategy_selection_layout->addWidget(position_pass_button, 4, 0, 1, 1);

        position_rebound_button = new QRadioButton(verticalLayoutWidget_3);
        position_rebound_button->setObjectName(QString::fromUtf8("position_rebound_button"));

        strategy_selection_layout->addWidget(position_rebound_button, 3, 1, 1, 1);

        position_deflection_button = new QRadioButton(verticalLayoutWidget_3);
        position_deflection_button->setObjectName(QString::fromUtf8("position_deflection_button"));

        strategy_selection_layout->addWidget(position_deflection_button, 4, 1, 1, 1);

        position_loose_ball_button = new QRadioButton(verticalLayoutWidget_3);
        position_loose_ball_button->setObjectName(QString::fromUtf8("position_loose_ball_button"));

        strategy_selection_layout->addWidget(position_loose_ball_button, 5, 0, 1, 1);


        strategy_position_layout->addWidget(strategy_position_box);

        strategy_type_tab->addTab(strategy_position_tab, QString());
        strategy_move_tab = new QWidget();
        strategy_move_tab->setObjectName(QString::fromUtf8("strategy_move_tab"));
        verticalLayoutWidget_14 = new QWidget(strategy_move_tab);
        verticalLayoutWidget_14->setObjectName(QString::fromUtf8("verticalLayoutWidget_14"));
        verticalLayoutWidget_14->setGeometry(QRect(0, 0, 451, 211));
        strategy_move_layout = new QVBoxLayout(verticalLayoutWidget_14);
        strategy_move_layout->setSpacing(2);
        strategy_move_layout->setContentsMargins(5, 5, 5, 5);
        strategy_move_layout->setObjectName(QString::fromUtf8("strategy_move_layout"));
        strategy_move_layout->setContentsMargins(0, 0, 0, 0);
        strategy_move_box = new QGroupBox(verticalLayoutWidget_14);
        strategy_move_box->setObjectName(QString::fromUtf8("strategy_move_box"));
        strategy_move_box->setMinimumSize(QSize(0, 200));
        strategy_move_box->setMaximumSize(QSize(16777215, 290));
        verticalLayoutWidget_15 = new QWidget(strategy_move_box);
        verticalLayoutWidget_15->setObjectName(QString::fromUtf8("verticalLayoutWidget_15"));
        verticalLayoutWidget_15->setGeometry(QRect(10, 10, 411, 176));
        strategy_selection_layout_2 = new QGridLayout(verticalLayoutWidget_15);
        strategy_selection_layout_2->setSpacing(2);
        strategy_selection_layout_2->setContentsMargins(5, 5, 5, 5);
        strategy_selection_layout_2->setObjectName(QString::fromUtf8("strategy_selection_layout_2"));
        strategy_selection_layout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        strategy_selection_layout_2->setContentsMargins(0, 0, 0, 0);
        move_middle_button = new QRadioButton(verticalLayoutWidget_15);
        move_middle_button->setObjectName(QString::fromUtf8("move_middle_button"));
        move_middle_button->setChecked(true);

        strategy_selection_layout_2->addWidget(move_middle_button, 0, 0, 1, 1);

        move_straight_button = new QRadioButton(verticalLayoutWidget_15);
        move_straight_button->setObjectName(QString::fromUtf8("move_straight_button"));

        strategy_selection_layout_2->addWidget(move_straight_button, 3, 0, 1, 1);

        move_sin_button = new QRadioButton(verticalLayoutWidget_15);
        move_sin_button->setObjectName(QString::fromUtf8("move_sin_button"));

        strategy_selection_layout_2->addWidget(move_sin_button, 0, 1, 1, 1);

        strategy5_radio_button_2 = new QRadioButton(verticalLayoutWidget_15);
        strategy5_radio_button_2->setObjectName(QString::fromUtf8("strategy5_radio_button_2"));

        strategy_selection_layout_2->addWidget(strategy5_radio_button_2, 4, 0, 1, 1);

        strategy3_radio_button_2 = new QRadioButton(verticalLayoutWidget_15);
        strategy3_radio_button_2->setObjectName(QString::fromUtf8("strategy3_radio_button_2"));

        strategy_selection_layout_2->addWidget(strategy3_radio_button_2, 3, 1, 1, 1);

        strategy6_radio_button_2 = new QRadioButton(verticalLayoutWidget_15);
        strategy6_radio_button_2->setObjectName(QString::fromUtf8("strategy6_radio_button_2"));

        strategy_selection_layout_2->addWidget(strategy6_radio_button_2, 4, 1, 1, 1);

        dd = new QRadioButton(verticalLayoutWidget_15);
        dd->setObjectName(QString::fromUtf8("dd"));

        strategy_selection_layout_2->addWidget(dd, 5, 0, 1, 1);


        strategy_move_layout->addWidget(strategy_move_box);

        strategy_type_tab->addTab(strategy_move_tab, QString());
        strategy_shoot_tab = new QWidget();
        strategy_shoot_tab->setObjectName(QString::fromUtf8("strategy_shoot_tab"));
        verticalLayoutWidget_16 = new QWidget(strategy_shoot_tab);
        verticalLayoutWidget_16->setObjectName(QString::fromUtf8("verticalLayoutWidget_16"));
        verticalLayoutWidget_16->setGeometry(QRect(0, 0, 441, 210));
        strategy_shoot_layout = new QVBoxLayout(verticalLayoutWidget_16);
        strategy_shoot_layout->setSpacing(2);
        strategy_shoot_layout->setContentsMargins(5, 5, 5, 5);
        strategy_shoot_layout->setObjectName(QString::fromUtf8("strategy_shoot_layout"));
        strategy_shoot_layout->setContentsMargins(0, 0, 0, 0);
        strategy_shoot_box = new QGroupBox(verticalLayoutWidget_16);
        strategy_shoot_box->setObjectName(QString::fromUtf8("strategy_shoot_box"));
        strategy_shoot_box->setMinimumSize(QSize(0, 200));
        strategy_shoot_box->setMaximumSize(QSize(16777215, 290));
        verticalLayoutWidget_17 = new QWidget(strategy_shoot_box);
        verticalLayoutWidget_17->setObjectName(QString::fromUtf8("verticalLayoutWidget_17"));
        verticalLayoutWidget_17->setGeometry(QRect(10, 10, 411, 176));
        strategy_selection_layout_3 = new QGridLayout(verticalLayoutWidget_17);
        strategy_selection_layout_3->setSpacing(2);
        strategy_selection_layout_3->setContentsMargins(5, 5, 5, 5);
        strategy_selection_layout_3->setObjectName(QString::fromUtf8("strategy_selection_layout_3"));
        strategy_selection_layout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        strategy_selection_layout_3->setContentsMargins(0, 0, 0, 0);
        shoot_charge_ball_button = new QRadioButton(verticalLayoutWidget_17);
        shoot_charge_ball_button->setObjectName(QString::fromUtf8("shoot_charge_ball_button"));
        shoot_charge_ball_button->setChecked(true);

        strategy_selection_layout_3->addWidget(shoot_charge_ball_button, 0, 0, 1, 1);

        shoot_dribble_shoot_button = new QRadioButton(verticalLayoutWidget_17);
        shoot_dribble_shoot_button->setObjectName(QString::fromUtf8("shoot_dribble_shoot_button"));

        strategy_selection_layout_3->addWidget(shoot_dribble_shoot_button, 3, 0, 1, 1);

        shoot_simple_shoot_button = new QRadioButton(verticalLayoutWidget_17);
        shoot_simple_shoot_button->setObjectName(QString::fromUtf8("shoot_simple_shoot_button"));

        strategy_selection_layout_3->addWidget(shoot_simple_shoot_button, 0, 1, 1, 1);

        strategy5_radio_button_3 = new QRadioButton(verticalLayoutWidget_17);
        strategy5_radio_button_3->setObjectName(QString::fromUtf8("strategy5_radio_button_3"));

        strategy_selection_layout_3->addWidget(strategy5_radio_button_3, 4, 0, 1, 1);

        strategy3_radio_button_3 = new QRadioButton(verticalLayoutWidget_17);
        strategy3_radio_button_3->setObjectName(QString::fromUtf8("strategy3_radio_button_3"));

        strategy_selection_layout_3->addWidget(strategy3_radio_button_3, 3, 1, 1, 1);

        strategy6_radio_button_3 = new QRadioButton(verticalLayoutWidget_17);
        strategy6_radio_button_3->setObjectName(QString::fromUtf8("strategy6_radio_button_3"));

        strategy_selection_layout_3->addWidget(strategy6_radio_button_3, 4, 1, 1, 1);

        dd_2 = new QRadioButton(verticalLayoutWidget_17);
        dd_2->setObjectName(QString::fromUtf8("dd_2"));

        strategy_selection_layout_3->addWidget(dd_2, 5, 0, 1, 1);


        strategy_shoot_layout->addWidget(strategy_shoot_box);

        strategy_type_tab->addTab(strategy_shoot_tab, QString());
        test1 = new QWidget();
        test1->setObjectName(QString::fromUtf8("test1"));
        verticalLayoutWidget_24 = new QWidget(test1);
        verticalLayoutWidget_24->setObjectName(QString::fromUtf8("verticalLayoutWidget_24"));
        verticalLayoutWidget_24->setGeometry(QRect(0, 0, 441, 210));
        strategy_shoot_layout_3 = new QVBoxLayout(verticalLayoutWidget_24);
        strategy_shoot_layout_3->setSpacing(2);
        strategy_shoot_layout_3->setContentsMargins(5, 5, 5, 5);
        strategy_shoot_layout_3->setObjectName(QString::fromUtf8("strategy_shoot_layout_3"));
        strategy_shoot_layout_3->setContentsMargins(0, 0, 0, 0);
        strategy_shoot_box_3 = new QGroupBox(verticalLayoutWidget_24);
        strategy_shoot_box_3->setObjectName(QString::fromUtf8("strategy_shoot_box_3"));
        strategy_shoot_box_3->setMinimumSize(QSize(0, 200));
        strategy_shoot_box_3->setMaximumSize(QSize(16777215, 290));
        verticalLayoutWidget_25 = new QWidget(strategy_shoot_box_3);
        verticalLayoutWidget_25->setObjectName(QString::fromUtf8("verticalLayoutWidget_25"));
        verticalLayoutWidget_25->setGeometry(QRect(10, 10, 401, 176));
        strategy_selection_layout_7 = new QGridLayout(verticalLayoutWidget_25);
        strategy_selection_layout_7->setSpacing(2);
        strategy_selection_layout_7->setContentsMargins(5, 5, 5, 5);
        strategy_selection_layout_7->setObjectName(QString::fromUtf8("strategy_selection_layout_7"));
        strategy_selection_layout_7->setSizeConstraint(QLayout::SetDefaultConstraint);
        strategy_selection_layout_7->setContentsMargins(0, 0, 0, 0);
        shoot_button = new QRadioButton(verticalLayoutWidget_25);
        shoot_button->setObjectName(QString::fromUtf8("shoot_button"));
        shoot_button->setChecked(true);

        strategy_selection_layout_7->addWidget(shoot_button, 0, 0, 1, 1);

        clear_button = new QRadioButton(verticalLayoutWidget_25);
        clear_button->setObjectName(QString::fromUtf8("clear_button"));

        strategy_selection_layout_7->addWidget(clear_button, 3, 0, 1, 1);

        steal_button = new QRadioButton(verticalLayoutWidget_25);
        steal_button->setObjectName(QString::fromUtf8("steal_button"));

        strategy_selection_layout_7->addWidget(steal_button, 0, 1, 1, 1);

        success_button = new QRadioButton(verticalLayoutWidget_25);
        success_button->setObjectName(QString::fromUtf8("success_button"));

        strategy_selection_layout_7->addWidget(success_button, 4, 0, 1, 1);

        complete_button = new QRadioButton(verticalLayoutWidget_25);
        complete_button->setObjectName(QString::fromUtf8("complete_button"));

        strategy_selection_layout_7->addWidget(complete_button, 4, 1, 1, 1);

        dribble_to_position_button = new QRadioButton(verticalLayoutWidget_25);
        dribble_to_position_button->setObjectName(QString::fromUtf8("dribble_to_position_button"));

        strategy_selection_layout_7->addWidget(dribble_to_position_button, 5, 0, 1, 1);

        stop_button_2 = new QRadioButton(verticalLayoutWidget_25);
        stop_button_2->setObjectName(QString::fromUtf8("stop_button_2"));

        strategy_selection_layout_7->addWidget(stop_button_2, 5, 1, 1, 1);

        charge_ball_button = new QRadioButton(verticalLayoutWidget_25);
        charge_ball_button->setObjectName(QString::fromUtf8("charge_ball_button"));

        strategy_selection_layout_7->addWidget(charge_ball_button, 3, 1, 1, 1);


        strategy_shoot_layout_3->addWidget(strategy_shoot_box_3);

        strategy_type_tab->addTab(test1, QString());
        test2 = new QWidget();
        test2->setObjectName(QString::fromUtf8("test2"));
        verticalLayoutWidget_26 = new QWidget(test2);
        verticalLayoutWidget_26->setObjectName(QString::fromUtf8("verticalLayoutWidget_26"));
        verticalLayoutWidget_26->setGeometry(QRect(0, 0, 441, 210));
        strategy_shoot_layout_4 = new QVBoxLayout(verticalLayoutWidget_26);
        strategy_shoot_layout_4->setSpacing(2);
        strategy_shoot_layout_4->setContentsMargins(5, 5, 5, 5);
        strategy_shoot_layout_4->setObjectName(QString::fromUtf8("strategy_shoot_layout_4"));
        strategy_shoot_layout_4->setContentsMargins(0, 0, 0, 0);
        strategy_shoot_box_4 = new QGroupBox(verticalLayoutWidget_26);
        strategy_shoot_box_4->setObjectName(QString::fromUtf8("strategy_shoot_box_4"));
        strategy_shoot_box_4->setMinimumSize(QSize(0, 200));
        strategy_shoot_box_4->setMaximumSize(QSize(16777215, 290));
        verticalLayoutWidget_27 = new QWidget(strategy_shoot_box_4);
        verticalLayoutWidget_27->setObjectName(QString::fromUtf8("verticalLayoutWidget_27"));
        verticalLayoutWidget_27->setGeometry(QRect(10, 10, 391, 176));
        strategy_selection_layout_8 = new QGridLayout(verticalLayoutWidget_27);
        strategy_selection_layout_8->setSpacing(2);
        strategy_selection_layout_8->setContentsMargins(5, 5, 5, 5);
        strategy_selection_layout_8->setObjectName(QString::fromUtf8("strategy_selection_layout_8"));
        strategy_selection_layout_8->setSizeConstraint(QLayout::SetDefaultConstraint);
        strategy_selection_layout_8->setContentsMargins(0, 0, 0, 0);
        block_button = new QRadioButton(verticalLayoutWidget_27);
        block_button->setObjectName(QString::fromUtf8("block_button"));
        block_button->setChecked(true);

        strategy_selection_layout_8->addWidget(block_button, 0, 0, 1, 1);

        mark_button = new QRadioButton(verticalLayoutWidget_27);
        mark_button->setObjectName(QString::fromUtf8("mark_button"));

        strategy_selection_layout_8->addWidget(mark_button, 3, 0, 1, 1);

        goalie_button = new QRadioButton(verticalLayoutWidget_27);
        goalie_button->setObjectName(QString::fromUtf8("goalie_button"));

        strategy_selection_layout_8->addWidget(goalie_button, 0, 1, 1, 1);

        velocity_button = new QRadioButton(verticalLayoutWidget_27);
        velocity_button->setObjectName(QString::fromUtf8("velocity_button"));

        strategy_selection_layout_8->addWidget(velocity_button, 4, 0, 1, 1);

        defend_point_button = new QRadioButton(verticalLayoutWidget_27);
        defend_point_button->setObjectName(QString::fromUtf8("defend_point_button"));

        strategy_selection_layout_8->addWidget(defend_point_button, 3, 1, 1, 1);

        defend_lane_button = new QRadioButton(verticalLayoutWidget_27);
        defend_lane_button->setObjectName(QString::fromUtf8("defend_lane_button"));

        strategy_selection_layout_8->addWidget(defend_lane_button, 4, 1, 1, 1);

        defend_line_button = new QRadioButton(verticalLayoutWidget_27);
        defend_line_button->setObjectName(QString::fromUtf8("defend_line_button"));

        strategy_selection_layout_8->addWidget(defend_line_button, 5, 0, 1, 1);

        pass_button = new QRadioButton(verticalLayoutWidget_27);
        pass_button->setObjectName(QString::fromUtf8("pass_button"));

        strategy_selection_layout_8->addWidget(pass_button, 5, 1, 1, 1);


        strategy_shoot_layout_4->addWidget(strategy_shoot_box_4);

        strategy_type_tab->addTab(test2, QString());
        test3 = new QWidget();
        test3->setObjectName(QString::fromUtf8("test3"));
        verticalLayoutWidget_28 = new QWidget(test3);
        verticalLayoutWidget_28->setObjectName(QString::fromUtf8("verticalLayoutWidget_28"));
        verticalLayoutWidget_28->setGeometry(QRect(0, 0, 451, 211));
        strategy_shoot_layout_5 = new QVBoxLayout(verticalLayoutWidget_28);
        strategy_shoot_layout_5->setSpacing(2);
        strategy_shoot_layout_5->setContentsMargins(5, 5, 5, 5);
        strategy_shoot_layout_5->setObjectName(QString::fromUtf8("strategy_shoot_layout_5"));
        strategy_shoot_layout_5->setContentsMargins(0, 0, 0, 0);
        strategy_shoot_box_5 = new QGroupBox(verticalLayoutWidget_28);
        strategy_shoot_box_5->setObjectName(QString::fromUtf8("strategy_shoot_box_5"));
        strategy_shoot_box_5->setMinimumSize(QSize(0, 200));
        strategy_shoot_box_5->setMaximumSize(QSize(16777215, 290));
        verticalLayoutWidget_29 = new QWidget(strategy_shoot_box_5);
        verticalLayoutWidget_29->setObjectName(QString::fromUtf8("verticalLayoutWidget_29"));
        verticalLayoutWidget_29->setGeometry(QRect(10, 10, 401, 176));
        strategy_selection_layout_9 = new QGridLayout(verticalLayoutWidget_29);
        strategy_selection_layout_9->setSpacing(2);
        strategy_selection_layout_9->setContentsMargins(5, 5, 5, 5);
        strategy_selection_layout_9->setObjectName(QString::fromUtf8("strategy_selection_layout_9"));
        strategy_selection_layout_9->setSizeConstraint(QLayout::SetDefaultConstraint);
        strategy_selection_layout_9->setContentsMargins(0, 0, 0, 0);
        receive_deflection_button = new QRadioButton(verticalLayoutWidget_29);
        receive_deflection_button->setObjectName(QString::fromUtf8("receive_deflection_button"));

        strategy_selection_layout_9->addWidget(receive_deflection_button, 4, 1, 1, 1);

        dribble_to_region_button = new QRadioButton(verticalLayoutWidget_29);
        dribble_to_region_button->setObjectName(QString::fromUtf8("dribble_to_region_button"));

        strategy_selection_layout_9->addWidget(dribble_to_region_button, 3, 0, 1, 1);

        spin_to_region_button = new QRadioButton(verticalLayoutWidget_29);
        spin_to_region_button->setObjectName(QString::fromUtf8("spin_to_region_button"));

        strategy_selection_layout_9->addWidget(spin_to_region_button, 3, 1, 1, 1);

        dribble_to_shoot_button = new QRadioButton(verticalLayoutWidget_29);
        dribble_to_shoot_button->setObjectName(QString::fromUtf8("dribble_to_shoot_button"));
        dribble_to_shoot_button->setChecked(true);

        strategy_selection_layout_9->addWidget(dribble_to_shoot_button, 0, 0, 1, 1);

        active_def_button = new QRadioButton(verticalLayoutWidget_29);
        active_def_button->setObjectName(QString::fromUtf8("active_def_button"));

        strategy_selection_layout_9->addWidget(active_def_button, 0, 1, 1, 1);

        receive_pass_button = new QRadioButton(verticalLayoutWidget_29);
        receive_pass_button->setObjectName(QString::fromUtf8("receive_pass_button"));

        strategy_selection_layout_9->addWidget(receive_pass_button, 4, 0, 1, 1);

        position_button = new QRadioButton(verticalLayoutWidget_29);
        position_button->setObjectName(QString::fromUtf8("position_button"));

        strategy_selection_layout_9->addWidget(position_button, 5, 0, 1, 1);

        pass_and_receive_button = new QRadioButton(verticalLayoutWidget_29);
        pass_and_receive_button->setObjectName(QString::fromUtf8("pass_and_receive_button"));

        strategy_selection_layout_9->addWidget(pass_and_receive_button, 5, 1, 1, 1);


        strategy_shoot_layout_5->addWidget(strategy_shoot_box_5);

        strategy_type_tab->addTab(test3, QString());

        strategy_layout->addWidget(strategy_type_tab);

        strategy_spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        strategy_layout->addItem(strategy_spacer);

        control_panel->addTab(strategy_tab, QString());
        log_tab = new QWidget();
        log_tab->setObjectName(QString::fromUtf8("log_tab"));
        verticalLayoutWidget_9 = new QWidget(log_tab);
        verticalLayoutWidget_9->setObjectName(QString::fromUtf8("verticalLayoutWidget_9"));
        verticalLayoutWidget_9->setGeometry(QRect(0, 0, 501, 681));
        log_layout = new QVBoxLayout(verticalLayoutWidget_9);
        log_layout->setSpacing(2);
        log_layout->setContentsMargins(5, 5, 5, 5);
        log_layout->setObjectName(QString::fromUtf8("log_layout"));
        log_layout->setContentsMargins(0, 0, 0, 0);
        field_record_layout = new QGroupBox(verticalLayoutWidget_9);
        field_record_layout->setObjectName(QString::fromUtf8("field_record_layout"));
        field_record_layout->setMinimumSize(QSize(0, 150));
        field_record_layout->setMaximumSize(QSize(16777215, 150));
        time_table_widget = new QTableWidget(field_record_layout);
        if (time_table_widget->columnCount() < 1)
            time_table_widget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        time_table_widget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (time_table_widget->rowCount() < 1)
            time_table_widget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        time_table_widget->setVerticalHeaderItem(0, __qtablewidgetitem1);
        time_table_widget->setObjectName(QString::fromUtf8("time_table_widget"));
        time_table_widget->setGeometry(QRect(30, 40, 111, 71));
        time_table_widget->horizontalHeader()->setStretchLastSection(true);
        score_table_widget = new QTableWidget(field_record_layout);
        if (score_table_widget->columnCount() < 2)
            score_table_widget->setColumnCount(2);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Andale Mono"));
        font2.setPointSize(10);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font2);
        __qtablewidgetitem2->setBackground(QColor(0, 0, 0));
        score_table_widget->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font2);
        score_table_widget->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        if (score_table_widget->rowCount() < 1)
            score_table_widget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        score_table_widget->setVerticalHeaderItem(0, __qtablewidgetitem4);
        score_table_widget->setObjectName(QString::fromUtf8("score_table_widget"));
        score_table_widget->setGeometry(QRect(160, 40, 161, 71));
        score_table_widget->horizontalHeader()->setDefaultSectionSize(70);
        score_table_widget->horizontalHeader()->setMinimumSectionSize(15);
        score_table_widget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        score_table_widget->horizontalHeader()->setStretchLastSection(true);
        score_table_widget->verticalHeader()->setVisible(false);
        score_table_widget->verticalHeader()->setDefaultSectionSize(30);
        score_table_widget->verticalHeader()->setHighlightSections(true);
        score_table_widget->verticalHeader()->setMinimumSectionSize(15);

        log_layout->addWidget(field_record_layout);

        log_group_box = new QGroupBox(verticalLayoutWidget_9);
        log_group_box->setObjectName(QString::fromUtf8("log_group_box"));
        log_group_box->setMinimumSize(QSize(0, 280));
        log_group_box->setMaximumSize(QSize(16777215, 1000));
        verticalLayoutWidget_5 = new QWidget(log_group_box);
        verticalLayoutWidget_5->setObjectName(QString::fromUtf8("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(10, 40, 471, 451));
        log_text_layout = new QGridLayout(verticalLayoutWidget_5);
        log_text_layout->setObjectName(QString::fromUtf8("log_text_layout"));
        log_text_layout->setContentsMargins(0, 0, 0, 0);
        log_text_area = new QTextEdit(verticalLayoutWidget_5);
        log_text_area->setObjectName(QString::fromUtf8("log_text_area"));
        log_text_area->setMaximumSize(QSize(16777215, 16777215));

        log_text_layout->addWidget(log_text_area, 0, 0, 1, 1);


        log_layout->addWidget(log_group_box);

        control_panel->addTab(log_tab, QString());
        referee_tab = new QWidget();
        referee_tab->setObjectName(QString::fromUtf8("referee_tab"));
        verticalLayoutWidget_12 = new QWidget(referee_tab);
        verticalLayoutWidget_12->setObjectName(QString::fromUtf8("verticalLayoutWidget_12"));
        verticalLayoutWidget_12->setGeometry(QRect(0, 0, 451, 581));
        referee_layout = new QVBoxLayout(verticalLayoutWidget_12);
        referee_layout->setSpacing(2);
        referee_layout->setContentsMargins(5, 5, 5, 5);
        referee_layout->setObjectName(QString::fromUtf8("referee_layout"));
        referee_layout->setContentsMargins(0, 0, 0, 0);
        ref_control_box = new QGroupBox(verticalLayoutWidget_12);
        ref_control_box->setObjectName(QString::fromUtf8("ref_control_box"));
        ref_control_box->setMinimumSize(QSize(0, 150));
        ref_control_box->setMaximumSize(QSize(16777215, 150));
        gridLayoutWidget_2 = new QWidget(ref_control_box);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 40, 331, 92));
        ref_control_layout = new QGridLayout(gridLayoutWidget_2);
        ref_control_layout->setSpacing(2);
        ref_control_layout->setContentsMargins(5, 5, 5, 5);
        ref_control_layout->setObjectName(QString::fromUtf8("ref_control_layout"));
        ref_control_layout->setContentsMargins(0, 0, 0, 0);
        ref_disable_button = new QRadioButton(gridLayoutWidget_2);
        ref_disable_button->setObjectName(QString::fromUtf8("ref_disable_button"));

        ref_control_layout->addWidget(ref_disable_button, 0, 0, 1, 1);

        ref_stop_button = new QRadioButton(gridLayoutWidget_2);
        ref_stop_button->setObjectName(QString::fromUtf8("ref_stop_button"));

        ref_control_layout->addWidget(ref_stop_button, 1, 0, 1, 1);

        ref_start_button = new QRadioButton(gridLayoutWidget_2);
        ref_start_button->setObjectName(QString::fromUtf8("ref_start_button"));

        ref_control_layout->addWidget(ref_start_button, 0, 1, 1, 1);

        ref_auto_button = new QRadioButton(gridLayoutWidget_2);
        ref_auto_button->setObjectName(QString::fromUtf8("ref_auto_button"));

        ref_control_layout->addWidget(ref_auto_button, 1, 1, 1, 1);


        referee_layout->addWidget(ref_control_box);

        ref_score_control_panel = new QGroupBox(verticalLayoutWidget_12);
        ref_score_control_panel->setObjectName(QString::fromUtf8("ref_score_control_panel"));
        ref_score_control_panel->setMinimumSize(QSize(0, 175));
        ref_score_control_panel->setMaximumSize(QSize(16777215, 300));
        horizontalLayoutWidget_2 = new QWidget(ref_score_control_panel);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 40, 331, 101));
        ref_score_control_layout = new QVBoxLayout(horizontalLayoutWidget_2);
        ref_score_control_layout->setSpacing(2);
        ref_score_control_layout->setContentsMargins(5, 5, 5, 5);
        ref_score_control_layout->setObjectName(QString::fromUtf8("ref_score_control_layout"));
        ref_score_control_layout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        score_yellow_radio_button = new QRadioButton(horizontalLayoutWidget_2);
        score_yellow_radio_button->setObjectName(QString::fromUtf8("score_yellow_radio_button"));

        horizontalLayout_3->addWidget(score_yellow_radio_button);

        score_blue_radio_button = new QRadioButton(horizontalLayoutWidget_2);
        score_blue_radio_button->setObjectName(QString::fromUtf8("score_blue_radio_button"));
        score_blue_radio_button->setChecked(true);

        horizontalLayout_3->addWidget(score_blue_radio_button);


        ref_score_control_layout->addLayout(horizontalLayout_3);

        score_button_layout = new QHBoxLayout();
        score_button_layout->setSpacing(5);
        score_button_layout->setContentsMargins(5, 5, 5, 5);
        score_button_layout->setObjectName(QString::fromUtf8("score_button_layout"));
        reset_score_button = new QPushButton(horizontalLayoutWidget_2);
        reset_score_button->setObjectName(QString::fromUtf8("reset_score_button"));

        score_button_layout->addWidget(reset_score_button);

        minus_score_button = new QPushButton(horizontalLayoutWidget_2);
        minus_score_button->setObjectName(QString::fromUtf8("minus_score_button"));

        score_button_layout->addWidget(minus_score_button);

        add_score_button = new QPushButton(horizontalLayoutWidget_2);
        add_score_button->setObjectName(QString::fromUtf8("add_score_button"));

        score_button_layout->addWidget(add_score_button);


        ref_score_control_layout->addLayout(score_button_layout);


        referee_layout->addWidget(ref_score_control_panel);

        referee_spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        referee_layout->addItem(referee_spacer);

        control_panel->addTab(referee_tab, QString());
        control_group_box = new QGroupBox(control_widget);
        control_group_box->setObjectName(QString::fromUtf8("control_group_box"));
        control_group_box->setGeometry(QRect(10, -10, 135, 200));
        control_group_box->setMinimumSize(QSize(0, 0));
        control_group_box->setMaximumSize(QSize(16777215, 200));
        control_group_box->setFlat(false);
        verticalLayoutWidget_2 = new QWidget(control_group_box);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 40, 91, 125));
        control_layout = new QVBoxLayout(verticalLayoutWidget_2);
        control_layout->setSpacing(2);
        control_layout->setContentsMargins(5, 5, 5, 5);
        control_layout->setObjectName(QString::fromUtf8("control_layout"));
        control_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        control_layout->setContentsMargins(0, 0, 0, 0);
        start_button = new QPushButton(verticalLayoutWidget_2);
        start_button->setObjectName(QString::fromUtf8("start_button"));

        control_layout->addWidget(start_button);

        pause_button = new QPushButton(verticalLayoutWidget_2);
        pause_button->setObjectName(QString::fromUtf8("pause_button"));

        control_layout->addWidget(pause_button);

        stop_button = new QPushButton(verticalLayoutWidget_2);
        stop_button->setObjectName(QString::fromUtf8("stop_button"));
        stop_button->setDefault(false);

        control_layout->addWidget(stop_button);

        source_group_box = new QGroupBox(control_widget);
        source_group_box->setObjectName(QString::fromUtf8("source_group_box"));
        source_group_box->setGeometry(QRect(140, 0, 131, 191));
        verticalLayoutWidget = new QWidget(source_group_box);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 33, 111, 131));
        control_source_layout = new QVBoxLayout(verticalLayoutWidget);
        control_source_layout->setSpacing(2);
        control_source_layout->setContentsMargins(5, 5, 5, 5);
        control_source_layout->setObjectName(QString::fromUtf8("control_source_layout"));
        control_source_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        control_source_layout->setContentsMargins(0, 0, 0, 0);
        computer_button = new QRadioButton(verticalLayoutWidget);
        computer_button->setObjectName(QString::fromUtf8("computer_button"));
        computer_button->setChecked(true);

        control_source_layout->addWidget(computer_button);

        keyboard_button = new QRadioButton(verticalLayoutWidget);
        keyboard_button->setObjectName(QString::fromUtf8("keyboard_button"));

        control_source_layout->addWidget(keyboard_button);

        joystick_button = new QRadioButton(verticalLayoutWidget);
        joystick_button->setObjectName(QString::fromUtf8("joystick_button"));

        control_source_layout->addWidget(joystick_button);

        generalTableWidget = new QTableWidget(control_widget);
        if (generalTableWidget->columnCount() < 1)
            generalTableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        generalTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        if (generalTableWidget->rowCount() < 4)
            generalTableWidget->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        generalTableWidget->setVerticalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        generalTableWidget->setVerticalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        generalTableWidget->setVerticalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        generalTableWidget->setVerticalHeaderItem(3, __qtablewidgetitem9);
        generalTableWidget->setObjectName(QString::fromUtf8("generalTableWidget"));
        generalTableWidget->setGeometry(QRect(260, 0, 251, 171));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(generalTableWidget->sizePolicy().hasHeightForWidth());
        generalTableWidget->setSizePolicy(sizePolicy);
        generalTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        generalTableWidget->setShowGrid(true);
        generalTableWidget->setGridStyle(Qt::CustomDashLine);
        generalTableWidget->setSortingEnabled(false);
        generalTableWidget->setCornerButtonEnabled(true);
        generalTableWidget->horizontalHeader()->setVisible(false);
        generalTableWidget->horizontalHeader()->setHighlightSections(true);
        generalTableWidget->verticalHeader()->setVisible(false);
        generalTableWidget->verticalHeader()->setCascadingSectionResizes(true);
        generalTableWidget->verticalHeader()->setHighlightSections(true);
        generalTableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        generalTableWidget->verticalHeader()->setStretchLastSection(true);
        control_dockwidget->setWidget(control_widget);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), control_dockwidget);
        strategy_graphics_dockwidget = new QDockWidget(MainWindow);
        strategy_graphics_dockwidget->setObjectName(QString::fromUtf8("strategy_graphics_dockwidget"));
        strategy_graphics_dockwidget->setMinimumSize(QSize(620, 100));
        strategy_graphics_widget = new QWidget();
        strategy_graphics_widget->setObjectName(QString::fromUtf8("strategy_graphics_widget"));
        strategy_graphics_view = new FieldView(strategy_graphics_widget);
        strategy_graphics_view->setObjectName(QString::fromUtf8("strategy_graphics_view"));
        strategy_graphics_view->setGeometry(QRect(10, 20, 601, 731));
        coordinate_text = new QTextBrowser(strategy_graphics_widget);
        coordinate_text->setObjectName(QString::fromUtf8("coordinate_text"));
        coordinate_text->setGeometry(QRect(10, 0, 141, 21));
        strategy_graphics_dockwidget->setWidget(strategy_graphics_widget);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), strategy_graphics_dockwidget);

        menu_bar->addAction(file->menuAction());
        menu_bar->addAction(menu_Edit->menuAction());
        file->addAction(open);
        file->addAction(save);
        file->addAction(save_as);
        file->addAction(exit);
        menu_Edit->addAction(preferences);

        retranslateUi(MainWindow);

        control_panel->setCurrentIndex(0);
        strategy_type_tab->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        open->setText(QApplication::translate("MainWindow", "&Open", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        open->setWhatsThis(QApplication::translate("MainWindow", "Open config files to set the UI", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        open->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        save->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        save->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        save_as->setText(QApplication::translate("MainWindow", "Save &as", 0, QApplication::UnicodeUTF8));
        save_as->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        exit->setText(QApplication::translate("MainWindow", "&Exit", 0, QApplication::UnicodeUTF8));
        exit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        preferences->setText(QApplication::translate("MainWindow", "&Preferences", 0, QApplication::UnicodeUTF8));
        file->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menu_Edit->setTitle(QApplication::translate("MainWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        yrobot_group_box->setTitle(QApplication::translate("MainWindow", "Yellow Team", 0, QApplication::UnicodeUTF8));
        yrobot1_button->setText(QApplication::translate("MainWindow", "Robot0", 0, QApplication::UnicodeUTF8));
        yrobot2_button->setText(QApplication::translate("MainWindow", "Robot1", 0, QApplication::UnicodeUTF8));
        yrobot3_button->setText(QApplication::translate("MainWindow", "Robot2", 0, QApplication::UnicodeUTF8));
        yrobot4_button->setText(QApplication::translate("MainWindow", "Robot3", 0, QApplication::UnicodeUTF8));
        yrobot5_button->setText(QApplication::translate("MainWindow", "Robot4", 0, QApplication::UnicodeUTF8));
        brobot_group_box->setTitle(QApplication::translate("MainWindow", "Blue Team", 0, QApplication::UnicodeUTF8));
        brobot1_button->setText(QApplication::translate("MainWindow", "Robot0", 0, QApplication::UnicodeUTF8));
        brobot2_button->setText(QApplication::translate("MainWindow", "Robot1", 0, QApplication::UnicodeUTF8));
        brobot3_button->setText(QApplication::translate("MainWindow", "Robot2", 0, QApplication::UnicodeUTF8));
        brobot4_button->setText(QApplication::translate("MainWindow", "Robot3", 0, QApplication::UnicodeUTF8));
        brobot5_button->setText(QApplication::translate("MainWindow", "Robot4", 0, QApplication::UnicodeUTF8));
        side_selection_box->setTitle(QApplication::translate("MainWindow", "Side Selection", 0, QApplication::UnicodeUTF8));
        select_up_side->setText(QApplication::translate("MainWindow", "Up", 0, QApplication::UnicodeUTF8));
        select_down_side->setText(QApplication::translate("MainWindow", "Down", 0, QApplication::UnicodeUTF8));
        color_group_box->setTitle(QApplication::translate("MainWindow", "Color Selection", 0, QApplication::UnicodeUTF8));
        blue_color_button->setText(QApplication::translate("MainWindow", "Blue", 0, QApplication::UnicodeUTF8));
        yellow_color_button->setText(QApplication::translate("MainWindow", "Yellow", 0, QApplication::UnicodeUTF8));
        control_panel->setTabText(control_panel->indexOf(general_tab), QApplication::translate("MainWindow", "General", 0, QApplication::UnicodeUTF8));
        robot_selection_group_box->setTitle(QApplication::translate("MainWindow", "Control robot", 0, QApplication::UnicodeUTF8));
        human_robot0_button->setText(QApplication::translate("MainWindow", "#0", 0, QApplication::UnicodeUTF8));
        human_robot1_button->setText(QApplication::translate("MainWindow", "#1", 0, QApplication::UnicodeUTF8));
        human_robot2_button->setText(QApplication::translate("MainWindow", "#2", 0, QApplication::UnicodeUTF8));
        human_robot3_button->setText(QApplication::translate("MainWindow", "#3", 0, QApplication::UnicodeUTF8));
        human_robot4_button->setText(QApplication::translate("MainWindow", "#4", 0, QApplication::UnicodeUTF8));
        human_robot5_button->setText(QApplication::translate("MainWindow", "#5", 0, QApplication::UnicodeUTF8));
        control_panel->setTabText(control_panel->indexOf(human_control_tab), QApplication::translate("MainWindow", "Human", 0, QApplication::UnicodeUTF8));
        mode_box->setTitle(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        run_mode->setText(QApplication::translate("MainWindow", "Run Strategy", 0, QApplication::UnicodeUTF8));
        test_mode->setText(QApplication::translate("MainWindow", "Test Strategy", 0, QApplication::UnicodeUTF8));
        strategy_position_box->setTitle(QString());
        position_start_button->setText(QApplication::translate("MainWindow", "position_for_start", 0, QApplication::UnicodeUTF8));
        position_penalty_button->setText(QApplication::translate("MainWindow", "position_for_penalty", 0, QApplication::UnicodeUTF8));
        position_kick_button->setText(QApplication::translate("MainWindow", "position_for_kick", 0, QApplication::UnicodeUTF8));
        position_pass_button->setText(QApplication::translate("MainWindow", "position_for_pass", 0, QApplication::UnicodeUTF8));
        position_rebound_button->setText(QApplication::translate("MainWindow", "position_for_rebound", 0, QApplication::UnicodeUTF8));
        position_deflection_button->setText(QApplication::translate("MainWindow", "position_for_deflection", 0, QApplication::UnicodeUTF8));
        position_loose_ball_button->setText(QApplication::translate("MainWindow", "position_for_loose_ball", 0, QApplication::UnicodeUTF8));
        strategy_type_tab->setTabText(strategy_type_tab->indexOf(strategy_position_tab), QApplication::translate("MainWindow", "Position", 0, QApplication::UnicodeUTF8));
        strategy_move_box->setTitle(QString());
        move_middle_button->setText(QApplication::translate("MainWindow", "move_to_middle", 0, QApplication::UnicodeUTF8));
        move_straight_button->setText(QApplication::translate("MainWindow", "move_as_straight", 0, QApplication::UnicodeUTF8));
        move_sin_button->setText(QApplication::translate("MainWindow", "move_as_sin", 0, QApplication::UnicodeUTF8));
        strategy5_radio_button_2->setText(QString());
        strategy3_radio_button_2->setText(QString());
        strategy6_radio_button_2->setText(QString());
        dd->setText(QString());
        strategy_type_tab->setTabText(strategy_type_tab->indexOf(strategy_move_tab), QApplication::translate("MainWindow", "Move", 0, QApplication::UnicodeUTF8));
        strategy_shoot_box->setTitle(QString());
        shoot_charge_ball_button->setText(QApplication::translate("MainWindow", "charge_ball", 0, QApplication::UnicodeUTF8));
        shoot_dribble_shoot_button->setText(QApplication::translate("MainWindow", "dribble_shoot", 0, QApplication::UnicodeUTF8));
        shoot_simple_shoot_button->setText(QApplication::translate("MainWindow", "simple_shoot", 0, QApplication::UnicodeUTF8));
        strategy5_radio_button_3->setText(QString());
        strategy3_radio_button_3->setText(QString());
        strategy6_radio_button_3->setText(QString());
        dd_2->setText(QString());
        strategy_type_tab->setTabText(strategy_type_tab->indexOf(strategy_shoot_tab), QApplication::translate("MainWindow", "Shoot", 0, QApplication::UnicodeUTF8));
        strategy_shoot_box_3->setTitle(QString());
        shoot_button->setText(QString());
        clear_button->setText(QString());
        steal_button->setText(QString());
        success_button->setText(QString());
        complete_button->setText(QString());
        dribble_to_position_button->setText(QString());
        stop_button_2->setText(QString());
        charge_ball_button->setText(QString());
        strategy_type_tab->setTabText(strategy_type_tab->indexOf(test1), QApplication::translate("MainWindow", "Page", 0, QApplication::UnicodeUTF8));
        strategy_shoot_box_4->setTitle(QString());
        block_button->setText(QString());
        mark_button->setText(QString());
        goalie_button->setText(QString());
        velocity_button->setText(QString());
        defend_point_button->setText(QString());
        defend_lane_button->setText(QString());
        defend_line_button->setText(QString());
        pass_button->setText(QString());
        strategy_type_tab->setTabText(strategy_type_tab->indexOf(test2), QApplication::translate("MainWindow", "Page", 0, QApplication::UnicodeUTF8));
        strategy_shoot_box_5->setTitle(QString());
        receive_deflection_button->setText(QString());
        dribble_to_region_button->setText(QString());
        spin_to_region_button->setText(QString());
        dribble_to_shoot_button->setText(QString());
        active_def_button->setText(QString());
        receive_pass_button->setText(QString());
        position_button->setText(QString());
        pass_and_receive_button->setText(QApplication::translate("MainWindow", "pass_and_receive", 0, QApplication::UnicodeUTF8));
        strategy_type_tab->setTabText(strategy_type_tab->indexOf(test3), QApplication::translate("MainWindow", "Page", 0, QApplication::UnicodeUTF8));
        control_panel->setTabText(control_panel->indexOf(strategy_tab), QApplication::translate("MainWindow", "Computer", 0, QApplication::UnicodeUTF8));
        field_record_layout->setTitle(QApplication::translate("MainWindow", "Field Measure", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = time_table_widget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Time Left", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = score_table_widget->horizontalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Blue", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = score_table_widget->horizontalHeaderItem(1);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Yellow", 0, QApplication::UnicodeUTF8));
        log_group_box->setTitle(QApplication::translate("MainWindow", "System Log", 0, QApplication::UnicodeUTF8));
        control_panel->setTabText(control_panel->indexOf(log_tab), QApplication::translate("MainWindow", "Log", 0, QApplication::UnicodeUTF8));
        ref_control_box->setTitle(QApplication::translate("MainWindow", "Refbox Control", 0, QApplication::UnicodeUTF8));
        ref_disable_button->setText(QApplication::translate("MainWindow", "Disabled", 0, QApplication::UnicodeUTF8));
        ref_stop_button->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        ref_start_button->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        ref_auto_button->setText(QApplication::translate("MainWindow", "Auto", 0, QApplication::UnicodeUTF8));
        ref_score_control_panel->setTitle(QApplication::translate("MainWindow", "Score control", 0, QApplication::UnicodeUTF8));
        score_yellow_radio_button->setText(QApplication::translate("MainWindow", "Yellow", 0, QApplication::UnicodeUTF8));
        score_blue_radio_button->setText(QApplication::translate("MainWindow", "Blue", 0, QApplication::UnicodeUTF8));
        reset_score_button->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        minus_score_button->setText(QApplication::translate("MainWindow", "-", 0, QApplication::UnicodeUTF8));
        add_score_button->setText(QApplication::translate("MainWindow", "+", 0, QApplication::UnicodeUTF8));
        control_panel->setTabText(control_panel->indexOf(referee_tab), QApplication::translate("MainWindow", "Referee", 0, QApplication::UnicodeUTF8));
        control_group_box->setTitle(QApplication::translate("MainWindow", "Control", 0, QApplication::UnicodeUTF8));
        start_button->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        pause_button->setText(QApplication::translate("MainWindow", "Pause", 0, QApplication::UnicodeUTF8));
        stop_button->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        source_group_box->setTitle(QApplication::translate("MainWindow", "Control source", 0, QApplication::UnicodeUTF8));
        computer_button->setText(QApplication::translate("MainWindow", "Sever", 0, QApplication::UnicodeUTF8));
        keyboard_button->setText(QApplication::translate("MainWindow", "Keyboard", 0, QApplication::UnicodeUTF8));
        joystick_button->setText(QApplication::translate("MainWindow", "Joystick", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = generalTableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "System Status       ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = generalTableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Current Strategy             ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = generalTableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Control Source         ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = generalTableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Match Speed          ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STRATEGY_H
