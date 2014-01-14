#include "user_interface/strategy_control_window.h"
#include "ui_strategy.h"
#include "net/thread_tools/field_timer.h"
#include "net/net_webserver_send_thread.h"



#include <QTableWidgetItem>
#include <QString>
#include <Qt>
#include <QDebug>


void MainWindow::SetupVisionReceiveThread()
{
    // start the vision receive thread
    vision_receive_thread_ =new VisionReceiveThread(ui->strategy_graphics_view);
    vision_receive_thread_->start(QThread::TimeCriticalPriority);
}
void MainWindow::SetupRefboxReceiveThread()
{
    // start the reference box thread
    refbox_receive_thread_ =new RefboxReceiveThread();
    refbox_receive_thread_->start(QThread::NormalPriority);

}

void MainWindow::SetupStrategyThread()
{
     //SetupDefaultTeam();
    // we do not need the vision recieve thread as the strategy thread already has it radio sender Open Successed
    strategy_thread_ = new StrategyThread(ui->strategy_graphics_view,true);
    // this thread does not include the vision receive thread and the reference box thread, but it can send the ssl package to simulator through internet
    strategy_thread_->start(QThread::TimeCriticalPriority);
}

void MainWindow::SetupNetwebSendThread()
{
    // start broadcasting packets to web server..
    net_webserver_send_thread_ = new NetWebserverSendThread(ui->strategy_graphics_view);
    net_webserver_send_thread_->start(QThread::TimeCriticalPriority);
}
void MainWindow::SetupThread()
{
#define STRATEGY_MODE

#ifdef STRATEGY_MODE
    SetupVisionReceiveThread();//Lu_test
    SetupRefboxReceiveThread();//lu_test

    SetupNetwebSendThread(); //lu_test add

    SetupStrategyThread();//lu_test
#endif

#ifndef STRATEGY_MODE
    qDebug() << "NON STRATEGY_MODE";
    //无线接收
    radio_receive_thread0_=new RadioReceiveThread(ui->strategy_graphics_view,10003);
    radio_receive_thread0_->start(QThread::TimeCriticalPriority);

    //无线接收
    radio_receive_thread1_=new RadioReceiveThread(ui->strategy_graphics_view,10005);
    radio_receive_thread1_->start(QThread::TimeCriticalPriority);

    vision_receive_thread_ =new VisionReceiveThread(ui->strategy_graphics_view);
    vision_receive_thread_->start(QThread::TimeCriticalPriority);
    //vision send thread
    // vision_send_thread=new VisionSendThread(this);
    // vision_send_thread->start(QThread::TimeCriticalPriority);
#endif

}
void MainWindow::SetupGraphicsView()
{


    /// Setting up the views (QGraphicView) /////////////////////////////////
    ui->strategy_graphics_view->showNormal();
    ui->strategy_graphics_view->Initilize();


    ui->strategy_graphics_view->SetRobot(teamYellow,0,ui->yrobot1_button->isChecked());
    ui->strategy_graphics_view->SetRobot(teamYellow,0,false);


    ui->strategy_graphics_view->SetRobot(teamYellow,1,ui->yrobot2_button->isChecked());
    ui->strategy_graphics_view->SetRobot(teamYellow,2,ui->yrobot3_button->isChecked());
    ui->strategy_graphics_view->SetRobot(teamYellow,3,ui->yrobot4_button->isChecked());
    ui->strategy_graphics_view->SetRobot(teamYellow,4,ui->yrobot5_button->isChecked());

    ui->strategy_graphics_view->SetRobot(teamBlue,0,ui->brobot2_button->isChecked());
    ui->strategy_graphics_view->SetRobot(teamBlue,1,ui->brobot2_button->isChecked());
    ui->strategy_graphics_view->SetRobot(teamBlue,2,ui->brobot2_button->isChecked());
    ui->strategy_graphics_view->SetRobot(teamBlue,3,ui->brobot2_button->isChecked());
    ui->strategy_graphics_view->SetRobot(teamBlue,4,ui->brobot2_button->isChecked());

    ///////////////////////////////////////////////////////////////////////////////
}

void MainWindow::SetupGUIConnection()
{

    /// connect act for menus   ///////////////////////////////////////////////////
    connect(ui->open,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->save,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->save_as,SIGNAL(triggered()),this,SLOT(save_as()));
    connect(ui->exit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->preferences,SIGNAL(triggered()),this,SLOT(preferences()));

    /// set the state of strategy software which latter will be pased in the world structure
    connect(ui->start_button,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->stop_button,SIGNAL(clicked()),this,SLOT(stop()));
    connect(ui->pause_button,SIGNAL(clicked()),this,SLOT(pause()));

    /// set the method of controling robot
    connect(ui->computer_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));
    connect(ui->keyboard_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));
    connect(ui->joystick_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));

    /// set the color of the team
    connect(ui->blue_color_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));
    connect(ui->yellow_color_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));

    /// set the side of the team
    connect(ui->select_down_side,SIGNAL(clicked(bool)),this,SLOT(select_down_side(bool)));
    connect(ui->select_up_side,SIGNAL(clicked(bool)),this,SLOT(select_up_side(bool)));

    /// select the strategy mode
    connect(ui->run_mode,SIGNAL(clicked(bool)),this,SLOT(run_mode_selected(bool)));
    connect(ui->test_mode,SIGNAL(clicked(bool)),this,SLOT(test_mode_selected(bool)));

    /// select the referee box
    connect(ui->ref_start_button,SIGNAL(clicked(bool)),this,SLOT(start_referee(bool)));


    connect(ui->human_robot0_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));
    connect(ui->human_robot1_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));
    connect(ui->human_robot2_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));
    connect(ui->human_robot3_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));
    connect(ui->human_robot4_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));
    connect(ui->human_robot5_button,SIGNAL(clicked()),this,SLOT(specify_control_source()));

    /// Strategy slot /////////////////////////////////////////////////////////////////
    connect(ui->position_deflection_button,SIGNAL(clicked()),this,SLOT(select_position_strategy_type()));
    connect(ui->position_kick_button,SIGNAL(clicked()),this,SLOT(select_position_strategy_type()));
    connect(ui->position_loose_ball_button,SIGNAL(clicked()),this,SLOT(select_position_strategy_type()));
    connect(ui->position_pass_button,SIGNAL(clicked()),this,SLOT(select_position_strategy_type()));
    connect(ui->position_penalty_button,SIGNAL(clicked()),this,SLOT(select_position_strategy_type()));
    connect(ui->position_rebound_button,SIGNAL(clicked()),this,SLOT(select_position_strategy_type()));
    connect(ui->position_start_button,SIGNAL(clicked()),this,SLOT(select_position_strategy_type()));


    connect(ui->move_middle_button,SIGNAL(clicked()),this,SLOT(select_move_strategy_type()));
    connect(ui->move_sin_button,SIGNAL(clicked()),this,SLOT(select_move_strategy_type()));
    connect(ui->move_straight_button,SIGNAL(clicked()),this,SLOT(select_move_strategy_type()));


    connect(ui->shoot_charge_ball_button,SIGNAL(clicked()),this,SLOT(select_shoot_strategy_type()));
    connect(ui->shoot_simple_shoot_button,SIGNAL(clicked()),this,SLOT(select_shoot_strategy_type()));
    connect(ui->shoot_dribble_shoot_button,SIGNAL(clicked()),this,SLOT(select_shoot_strategy_type()));


    connect(ui->shoot_button,SIGNAL(clicked()),this,SLOT(select_page1_strategy_type()));
    connect(ui->steal_button,SIGNAL(clicked()),this,SLOT(select_page1_strategy_type()));
    connect(ui->clear_button,SIGNAL(clicked()),this,SLOT(select_page1_strategy_type()));
    connect(ui->charge_ball_button,SIGNAL(clicked()),this,SLOT(select_page1_strategy_type()));
    connect(ui->success_button,SIGNAL(clicked()),this,SLOT(select_page1_strategy_type()));
    connect(ui->complete_button,SIGNAL(clicked()),this,SLOT(select_page1_strategy_type()));
    connect(ui->dribble_to_position_button,SIGNAL(clicked()),this,SLOT(select_page1_strategy_type()));
    connect(ui->stop_button_2,SIGNAL(clicked()),this,SLOT(select_page1_strategy_type()));

    connect(ui->block_button,SIGNAL(clicked()),this,SLOT(select_page2_strategy_type()));
    connect(ui->goalie_button,SIGNAL(clicked()),this,SLOT(select_page2_strategy_type()));
    connect(ui->mark_button,SIGNAL(clicked()),this,SLOT(select_page2_strategy_type()));
    connect(ui->defend_point_button,SIGNAL(clicked()),this,SLOT(select_page2_strategy_type()));
    connect(ui->velocity_button,SIGNAL(clicked()),this,SLOT(select_page2_strategy_type()));
    connect(ui->defend_lane_button,SIGNAL(clicked()),this,SLOT(select_page2_strategy_type()));
    connect(ui->defend_line_button,SIGNAL(clicked()),this,SLOT(select_page2_strategy_type()));
    connect(ui->pass_button,SIGNAL(clicked()),this,SLOT(select_page2_strategy_type()));

    connect(ui->dribble_to_shoot_button,SIGNAL(clicked()),this,SLOT(select_page3_strategy_type()));
    connect(ui->active_def_button,SIGNAL(clicked()),this,SLOT(select_page3_strategy_type()));
    connect(ui->dribble_to_region_button,SIGNAL(clicked()),this,SLOT(select_page3_strategy_type()));
    connect(ui->spin_to_region_button,SIGNAL(clicked()),this,SLOT(select_page3_strategy_type()));
    connect(ui->receive_pass_button,SIGNAL(clicked()),this,SLOT(select_page3_strategy_type()));
    connect(ui->receive_deflection_button,SIGNAL(clicked()),this,SLOT(select_page3_strategy_type()));
    connect(ui->position_button,SIGNAL(clicked()),this,SLOT(select_page3_strategy_type()));
    connect(ui->pass_and_receive_button,SIGNAL(clicked()),this,SLOT(select_page3_strategy_type()));

    //    connect(ui->strategy1_radio_button,SIGNAL(clicked()),this,SLOT(strategy1_clicked()));
    //    connect(ui->strategy2_radio_button,SIGNAL(clicked()),this,SLOT(strategy2_clicked()));
    ///////////////////////////////////////////////////////////////////////////////////


    connect(ui->add_score_button,SIGNAL(clicked()),this,SLOT(add_score()));
    connect(ui->minus_score_button,SIGNAL(clicked()),this,SLOT(minus_score()));
    connect(ui->reset_score_button,SIGNAL(clicked()),this,SLOT(reset_score()));

    ///////////////////////////////////////////////////////////////////////////////



}
void MainWindow::SetupWindows()
{
    ui->setupUi(this);
    SetupSettingWindows();

}
void MainWindow::SetupWindowsComponent()
{


    /// UI dynamic statu settings///////////////////////////////////////////////////
    pause_status_=OFF;
    // seting the statu to be paused
    ui->pause_button->setEnabled(false);
    ui->stop_button->setEnabled(false);
    ui->computer_button->setChecked(true);

    QTableWidgetItem *status_name=new QTableWidgetItem(ui->stop_button->text());
    QTableWidgetItem *status_name1=new QTableWidgetItem(ui->position_kick_button->text());
    QTableWidgetItem *status_name2=new QTableWidgetItem(ui->computer_button->text());
    QTableWidgetItem *status_name3=new QTableWidgetItem("Normal");

    ui->generalTableWidget->setItem(0,0,status_name);
    ui->generalTableWidget->setItem(1,0,status_name1);
    ui->generalTableWidget->setItem(2,0,status_name2);
    ui->generalTableWidget->setItem(3,0,status_name3);

    ui->time_table_widget->setDisabled(true);

    QTableWidgetItem *blue_score=new QTableWidgetItem("0");
    blue_score->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    QTableWidgetItem *yellow_score=new QTableWidgetItem("0");
    yellow_score->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->score_table_widget->setItem(0,0,blue_score);
    ui->score_table_widget->setItem(0,1,yellow_score);

    // initialize the status of the strategy


}

void MainWindow::SetupWindowProperties()
{
    /// Setting window attributes //////////////////////////////////////////////////////
    setWindowTitle(tr("MainWindow"));
}

void MainWindow::TimerInitialization()
{
    TimerInit();
}
void MainWindow::SetupTimer()
{
    /// Setting up timer ///////////////////////////////////////////////////////////
    startTimer (33);
    oldTickCount=GetTickCount();
}

void MainWindow::SetupMode(bool thread_mode)
{
    thread_mode_ = thread_mode;
}
void MainWindow::SetupMode()
{
    thread_mode_ = false;
}
void MainWindow::SetupSettingWindows()
{
    // starting the setting window ///////////////////////////////////////////////
    setting_main_ = new SettingMainWindow();
}
void MainWindow::SetupAutoSelection()
{
    TestingSelection();
}

void MainWindow::TestingSelection()
{
    /***********These codes can make serious problem don't know why**********************/
    /// mimic the behavior of the human to initialize the strategy settings
    //emit strategy1_clicked();
    /// set the status of the radio buttons /////////////////////////////////
    //ui->location_mode->setChecked(true);
    //emit location_mode_selected(true);
    //emit specify_control_source();
    /************************************************************************************/
    ui->blue_color_button->setChecked(true);
    emit specify_control_source();
    ui->test_mode->setChecked(true);
    emit test_mode_selected(true);
    ui->ref_start_button->setChecked(true);
    emit start_referee(true);
    ui->select_up_side->setChecked(true);
    emit select_up_side(true);// blue side
    ui->position_kick_button->setChecked(true);
    emit select_position_strategy_type();

}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    TimerInitialization();
    SetupWindows();
    SetupMode();
    SetupThread();
    SetupWindowsComponent();
    SetupGraphicsView();
    SetupGUIConnection();
    SetupAutoSelection();
    SetupWindowProperties();
    SetupTimer();

}


MainWindow::~MainWindow()
{
    delete ui;
    // deal with the strategy thread
    strategy_thread_->set_thread_terminated(true);
    strategy_thread_->terminate();
    while(!strategy_thread_->isFinished()){}
    delete strategy_thread_;
    strategy_thread_ = NULL;

    // deal with the setting main window
    delete setting_main_;
    setting_main_ =NULL;
}





// start the strategy
void MainWindow::start()
{
    ui->start_button->setEnabled(false);
    ui->stop_button->setEnabled(true);
    ui->pause_button->setEnabled(true);
    QTableWidgetItem *status_name=new QTableWidgetItem();
    status_name->setText("Running");
    ui->generalTableWidget->setItem(0,0,status_name);
    StatusOnGUI.Status = RUN_START;
}

// stop the strategy
void MainWindow::stop()
{
    ui->start_button->setEnabled(true);
    ui->stop_button->setEnabled(false);
    ui->pause_button->setEnabled(false);
    ui->pause_button->setText("Pause");
    pause_status_=OFF;
    QTableWidgetItem *status_name=new QTableWidgetItem();
    status_name->setText(ui->stop_button->text());
    ui->generalTableWidget->setItem(0,0,status_name);
    StatusOnGUI.Status = RUN_STOP;

}

// pause the strategy
void MainWindow::pause()
{
    ui->start_button->setEnabled(false);
    ui->stop_button->setEnabled(true);
    ui->pause_button->setEnabled(true);

    if(pause_status_==OFF)
    {
        ui->pause_button->setText("Continue");
        pause_status_=ON;
        QTableWidgetItem *status_name=new QTableWidgetItem();
        status_name->setText("Pause");
        ui->generalTableWidget->setItem(0,0,status_name);
                StatusOnGUI.Status = RUN_PAUSE;
    }
    else
    {
        ui->pause_button->setText("Pause");
        pause_status_=OFF;

        QTableWidgetItem *status_name=new QTableWidgetItem();
        status_name->setText("Running");
        ui->generalTableWidget->setItem(0,0,status_name);

        StatusOnGUI.Status = RUN_START;
    }

}

// check whether the control source is computer or human
void MainWindow::specify_control_source()
{
    QTableWidgetItem *control_source=new QTableWidgetItem();
    if(ui->blue_color_button->isChecked())
    {
        soccerteam = TEAM_BLUE;
    }
    else
    {
        soccerteam = TEAM_YELLOW;
    }


    world.setcolor(soccerteam);

    if(ui->computer_button->isChecked())
    { 
        ui->strategy_tab->setEnabled(true);

        control_source->setText(ui->computer_button->text());
        ui->generalTableWidget->setItem(2,0,control_source);
        if(ui->run_mode->isChecked())
        {
            run_mode_selected(true);
        }
        else
            if(ui->test_mode->isChecked())
            {
                test_mode_selected(true);
            }
    }
    else if(ui->keyboard_button->isChecked()||ui->joystick_button->isChecked())
    {
        QString control_color,control_num;
        ui->strategy_tab->setEnabled(true);
        if(ui->blue_color_button->isChecked())
            control_color=ui->blue_color_button->text();
        else
            control_color=ui->yellow_color_button->text();
        if(ui->human_robot0_button->isChecked())
            control_num=ui->human_robot0_button->text();
        if(ui->human_robot1_button->isChecked())
            control_num=ui->human_robot1_button->text();
        if(ui->human_robot2_button->isChecked())
            control_num=ui->human_robot2_button->text();
        if(ui->human_robot3_button->isChecked())
            control_num=ui->human_robot3_button->text();
        if(ui->human_robot4_button->isChecked())
            control_num=ui->human_robot4_button->text();
        if(ui->human_robot5_button->isChecked())
            control_num=ui->human_robot5_button->text();
        control_source->setText("Human("+control_color+" "+control_num+")");
        ui->generalTableWidget->setItem(2,0,control_source);
        ui->generalTableWidget->resizeColumnsToContents();

        StatusOnGUI.StrategyIndex = STRATEGY_JOYSTICK;
    }

}

// check the strategy type and represente it on the panel
void MainWindow::select_position_strategy_type()
{
    QTableWidgetItem *tacitic_name_item=new QTableWidgetItem();

    // TODO change them according to strategy
    QString qstring_tactic_name =ui->position_kick_button->text();


    //#GRAMMER if there is no instance of qbytearray, the data will be stored continuously
    // then the tactic_name will not be store correctly

        if(ui->position_deflection_button->isChecked())
        {
            StatusOnGUI.tactic_name_ = "position_for_deflection";
        }
        else if(ui->position_kick_button->isChecked())
        {
            StatusOnGUI.tactic_name_ = "position_for_kick";

        }
        else if(ui->position_loose_ball_button->isChecked())
        {
            StatusOnGUI.tactic_name_ = "position_for_loose_ball";
        }
        else if(ui->position_pass_button->isChecked())
        {
            StatusOnGUI.tactic_name_ = "position_for_pass";
        }
        else if(ui->position_penalty_button->isChecked())
        {
            StatusOnGUI.tactic_name_ = "position_for_penalty";
        }
        else if(ui->position_rebound_button->isChecked())
        {
            StatusOnGUI.tactic_name_ = "position_for_rebound";
        }
        else if(ui->position_start_button->isChecked())
        {
            StatusOnGUI.tactic_name_ = "position_for_start";
        }

}



void MainWindow::select_move_strategy_type()
{
    QTableWidgetItem *tacitic_name_item=new QTableWidgetItem();

    // TODO change them according to strategy
    QString qstring_tactic_name =ui->position_kick_button->text();

    if(ui->move_middle_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "move_to_middle";
    }
    else if(ui->move_sin_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "move_as_sin";
    }
    else if(ui->move_straight_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "move_as_straight";
    }
    tacitic_name_item->setText(qstring_tactic_name);
    ui->generalTableWidget->setItem(1,0,tacitic_name_item);
}

void MainWindow::select_shoot_strategy_type()
{
    QTableWidgetItem *tacitic_name_item=new QTableWidgetItem();

    QString qstring_tactic_name =ui->position_kick_button->text();
    if(ui->shoot_charge_ball_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "shoot_charge_ball";
    }
    else if(ui->shoot_simple_shoot_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "shoot_simple_shoot";
    }
    else if(ui->shoot_dribble_shoot_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "shoot_dribble_shoot";
    }
}

void MainWindow::select_page2_strategy_type()
{
    if(ui->block_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "block";
    }
    else if(ui->goalie_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "goalie";
    }
    else if(ui->mark_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "mark";
    }
    else if(ui->defend_point_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "defend_point";
    }
    else if(ui->velocity_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "velocity";
    }
    else if(ui->defend_lane_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "defend_lane";
    }
    else if(ui->defend_line_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "defend_line";
    }
    else if(ui->pass_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "pass";
    }
}

void MainWindow::select_page1_strategy_type()
{
    if(ui->shoot_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "shoot";
    }
    else if(ui->steal_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "steal";
    }
    else if(ui->clear_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "clear";
    }
    else if(ui->charge_ball_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "charge_ball";
    }
    else if(ui->success_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "success";
    }
    else if(ui->complete_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "complete";
    }
    else if(ui->dribble_to_position_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "dribble_to_position";
    }
    else if(ui->stop_button_2->isChecked())
    {
        StatusOnGUI.tactic_name_ = "stop";
    }
}

void MainWindow::select_page3_strategy_type()
{
    if(ui->dribble_to_shoot_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "dribble_to_shoot";
    }
    else if(ui->active_def_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "active_def";
    }
    else if(ui->dribble_to_region_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "dribble_to_region";
    }
    else if(ui->spin_to_region_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "spin_to_region";
    }
    else if(ui->receive_pass_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "receive_pass";
    }
    else if(ui->receive_deflection_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "receive_deflection";
    }
    else if(ui->position_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "position";
    }
    else if(ui->pass_and_receive_button->isChecked())
    {
        StatusOnGUI.tactic_name_ = "pass_and_receive";
    }

//    tacitic_name_item->setText(qstring_tactic_name);
//    ui->generalTableWidget->setItem(1,0,tacitic_name_item);
}

void MainWindow::speed_control(int value)
{
    QTableWidgetItem *speed=new QTableWidgetItem();
    QString speed_num;
    speed_num.append(QString("%1").arg(value));
    speed->setText(speed_num);
    ui->generalTableWidget->setItem(0,3,speed);
}

void MainWindow::match_time_control()
{

}

void MainWindow::add_score()
{
    QTableWidgetItem *score=new QTableWidgetItem();
    QString score_num;
    score->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    if(ui->score_blue_radio_button->isChecked())
    {
        score_num.append(QString("%1").arg(ui->score_table_widget->takeItem(0,0)->text().toInt()+1));
        score->setText(score_num);
        ui->score_table_widget->setItem(0,0,score);
    }
    else
        if(ui->score_yellow_radio_button->isChecked())
        {
            score_num.append(QString("%1").arg(ui->score_table_widget->takeItem(0,1)->text().toInt()+1));
            score->setText(score_num);
            ui->score_table_widget->setItem(0,1,score);
        }
}

void MainWindow::minus_score()
{
    QTableWidgetItem *score=new QTableWidgetItem();
    QString score_num;
    score->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    if(ui->score_blue_radio_button->isChecked())
    {
        score_num.append(QString("%1").arg(ui->score_table_widget->takeItem(0,0)->text().toInt()-1));
        score->setText(score_num);
        ui->score_table_widget->setItem(0,0,score);
    }
    else
        if(ui->score_yellow_radio_button->isChecked())
        {

            score_num.append(QString("%1").arg(ui->score_table_widget->takeItem(0,1)->text().toInt()-1));
            score->setText(score_num);
            ui->score_table_widget->setItem(0,1,score);
        }
}

void MainWindow::reset_score()
{
    QTableWidgetItem *blue_score=new QTableWidgetItem("0");
    blue_score->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    QTableWidgetItem *yellow_score=new QTableWidgetItem("0");
    yellow_score->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->score_table_widget->setItem(0,0,blue_score);
    ui->score_table_widget->setItem(0,1,yellow_score);
}



void MainWindow::excute_strategy()
{

}




/////////// Timer related ///////////////////////////////////

void MainWindow::timerEvent ( QTimerEvent *event )
{
    tick();

    //event->ignore();
}

void MainWindow::tick ( )
{

    static int iFrameCount=0;
    double dStrategyFreq;
    QString s;

    ui->strategy_graphics_view->tick();

    iFrameCount++;

    TickCount= ::GetTickCount();
    if(TickCount-oldTickCount>1000)
    {
        dStrategyFreq=StatusOnGUI.dFreq;
        s=QString("Display Freq:%1\r\nStrategy Freq:%2")
                .arg(iFrameCount*1.0/(TickCount-oldTickCount)*1000,3,'f',1)
                .arg(dStrategyFreq,3,'f',1) ;
        oldTickCount=TickCount;

        iFrameCount=0;
    }


}


//////////////////////////////////////////////////////////////


void MainWindow::yrobot1_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamYellow,0,ui->yrobot1_button->isChecked());
}

void MainWindow::yrobot2_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamYellow,1,ui->yrobot2_button->isChecked());
}

void MainWindow::yrobot3_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamYellow,2,ui->yrobot3_button->isChecked());
}

void MainWindow::yrobot4_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamYellow,3,ui->yrobot4_button->isChecked());
}

void MainWindow::yrobot5_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamYellow,4,ui->yrobot5_button->isChecked());
}

void MainWindow::brobot1_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamBlue,0,ui->brobot1_button->isChecked());
}

void MainWindow::brobot2_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamBlue,1,ui->brobot2_button->isChecked());
}

void MainWindow::brobot3_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamBlue,2,ui->brobot3_button->isChecked());
}

void MainWindow::brobot4_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamBlue,3,ui->brobot4_button->isChecked());
}

void MainWindow::brobot5_changed(int)
{
    ui->strategy_graphics_view->SetRobot(teamBlue,4,ui->brobot5_button->isChecked());
}

// method of the actions

// open config file and apply it to the GUI
void MainWindow::open()
{
}

// save the current configeration
void MainWindow::save()
{
}

//svae the current configerration to some other place
void MainWindow::save_as()
{
}

// exit the program
void MainWindow::exit()
{
}

// apply the changes to the GUI
void MainWindow::apply_changes()
{

}

// change the parameters of the software
void MainWindow::preferences()
{
    setting_main_->show();
    setting_main_->open_default();
}

// method of the starategies

// not sure now
void MainWindow::strategy1_clicked()
{
    //runstatusMutex.lock();
    StatusOnGUI.tactic_name_ = "position_for_kick";

    //    StatusOnGUI.tactic_string = "position";
    //runstatusMutex.unlock();
}

void MainWindow::strategy2_clicked()
{
    //runstatusMutex.lock();
    StatusOnGUI.tactic_name_= "shot";
    //runstatusMutex.unlock();
}

// select the side of the competition team
// this idealy will flip the view
void MainWindow::select_down_side(bool selected)
{
    if(!selected)
        return;
    world.setside(SIDE_LEFT);

}
void MainWindow::select_up_side(bool selected)
{
    if(!selected)
        return;
    world.setside(SIDE_RIGHT);
}

// select the mode mode
void MainWindow::run_mode_selected(bool selected)
{
    if(!selected)
        return;
    StatusOnGUI.StrategyIndex =STRATEGY_RUN;
}

void MainWindow::test_mode_selected(bool selected)
{
    if(!selected)
        return;
    StatusOnGUI.StrategyIndex =STRATEGY_TEST;
}

// menully select "start" for referee box
void MainWindow::start_referee(bool selected)
{
    if(!selected)
        return;
    world.game_state = 's';

}

