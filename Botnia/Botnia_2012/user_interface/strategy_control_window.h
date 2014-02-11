#ifndef STRATEGY_CONROL_WINDOW_H
#define STRATEGY_CONROL_WINDOW_H
#include <QWidget>
#include <QMainWindow>

//  internet communication files
#include "net/net_vision_receive_thread.h"
#include "net/net_radio_send_thread.h"
#include "net/net_radio_receive_thread.h"
#include "net/net_refbox_receive_thread.h"
#include "net/net_vision_send_thread.h"
#include "net/net_webserver_send_thread.h"

// straegy files
#include "control_hub/computer_control/intelligence/strategy_extutor/strategy_thread.h"
// user interface files
#include "user_interface/field_related/field_scene.h"
// setting windows
#include "user_interface/ui_setting/setting_window.h"




/// In many thread apllication people need to recursively include classes.
/// This class should be declared here to indicate the name
class VisionSendThread;
class StrategyThread;

namespace Ui
{
    class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    void SetupVisionReceiveThread();
    void SetupRefboxReceiveThread();
    void SetupNetwebSendThread();

    void SetupDefaultTeam();
    void SetupStrategyThread();
    void SetupThread();

    void SetupWindows();
    void SetupSettingWindows();
    void SetupMode();
    void SetupMode(bool thread_mode);
    void SetupWindowProperties();
    void SetupWindowsComponent();

    void SetupGraphicsView();
    void SetupGUIConnection();

    void SetupAutoSelection();
    void TestingSelection();


    void TimerInitialization();
    void SetupTimer();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void tick();
    enum status {OFF,ON};
    enum speed_num {SLOW2=-2,SLOW1,NORMAL,FAST1,FAST2};
//public slots:
//    void AddSystemLog(QString &info)
//    {
//        ui->log_text_area->append(info);
//    }

protected:
    void timerEvent ( QTimerEvent* );

private slots:
    // method for menu file
    void apply_changes();
    void open();
    void save();
    void save_as();
    void exit();
    // methods for menu edit
    void preferences();
    void start();
    void stop();
    void pause();
    void specify_control_source();
    void select_position_strategy_type();
    void select_move_strategy_type();
    void select_shoot_strategy_type();

    void select_page2_strategy_type();//lu_test add
    void select_page1_strategy_type();
    void select_page3_strategy_type();

    void speed_control(int value);
    void match_time_control();
    void excute_strategy();
    void add_score();
    void minus_score();
    void reset_score();

    // select the side of the team
    void select_up_side(bool);
    void select_down_side(bool);

    // when the state of the blue robot button has been changed
    void brobot5_changed(int);
    void brobot4_changed(int);
    void brobot3_changed(int);
    void brobot2_changed(int);
    void brobot1_changed(int);

    void yrobot5_changed(int);
    void yrobot4_changed(int);
    void yrobot3_changed(int);
    void yrobot2_changed(int);
    void yrobot1_changed(int);

    // strategy mode selection method
    void run_mode_selected(bool selected);
    void test_mode_selected(bool selected);

    // strategy selection method
    void strategy1_clicked();
    void strategy2_clicked();
    // set referee command
    void start_referee(bool selected);

    void coordinate_refresh(double x,double y);

private:
    Ui::MainWindow *ui;
    status pause_status_;
    //-------------------------------------
    bool thread_mode_;	//多线程方式
    //-------------------------------------
    unsigned long TickCount,oldTickCount;


    // various threading
    StrategyThread* strategy_thread_;
    VisionSendThread* vision_send_thread_;
    VisionReceiveThread* vision_receive_thread_;
    RadioSendThread* radio_send_thread_;
    RadioReceiveThread* radio_receive_thread0_;
    RadioReceiveThread* radio_receive_thread1_;
    RefboxReceiveThread* refbox_receive_thread_;
    NetWebserverSendThread* net_webserver_send_thread_;


    // setting window
    SettingMainWindow* setting_main_;
};


#endif // STRATEGY_H
















