# -------------------------------------------------
# Project created by QtCreator 2012-01-02T12:00:37
# -------------------------------------------------
# -------------------------------------------------
# QT creator 4.7.3 G++ 4.6.1
# -------------------------------------------------
QT += core
QT += gui
QT += network
QT += opengl
QT += xml

# ########### specify home directory #####
# google protobuf
unix:!macx:!symbian:LIBS += -L/usr/local/lib/ \
    -lprotobuf
unix:!macx:!symbian:PRE_TARGETDEPS += /usr/local/lib/libprotobuf.a

# qjson
unix:!macx:!symbian:LIBS += -L/usr/local/lib/ \
    -lqjson
unix:!macx:!symbian:PRE_TARGETDEPS += /usr/local/lib/libqjson.so

# java environment develop for mouth and ear moule
unix:!mac:!symbian:INCLUDEPATH += control_hub/module_interaction_tools/jdk1.7.0_09/include
unix:!mac:!symbian:INCLUDEPATH += control_hub/module_interaction_tools/jdk1.7.0_09/include/linux

# unix:!macx:!symbian:LIBS += -Lcontrol_hub/module_interaction_tools/jre1.7.0_09/lib/i386/client/ -ljvm
# unix:!macx:!symbian:PRE_TARGETDEPS += control_hub/module_interaction_tools/jre1.7.0_09/lib/i386/client/libjvm.so
# ############### for kinect##################
# for using ncurses in c languege
unix:!macx:!symbian:LIBS += -L/usr/local/lib/ \
    -lncurses

# for using freenect library in c languege
unix:!macx:!symbian:LIBS += -L/usr/local/lib/ \
    -lfreenect

# not using opengl in c languege
unix:!macx:!symbian:LIBS += -L/usr/local/lib/ \
    -lglut \
    -lXtst

## not using graph lib of c
#unix:!macx:!symbian:LIBS += -L/usr/local/lib/ \
#    -lgraph

# ###############################################
TARGET = Botnia_2012
CONFIG += console
TEMPLATE = app
SOURCES += main.cpp \
    control_hub/human_control/keyboard.cpp \
    net/net_radio_server.cpp \
    net/net_base.cpp \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_wrapper.pb.cc \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_refbox_log.pb.cc \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_geometry.pb.cc \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_detection.pb.cc \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_radio.pb.cc \
    net/net_radio_send_thread.cpp \
    net/net_radio_client.cpp \
    net/net_radio_date_structures.cpp \
    net/net_vision_server.cpp \
    net/net_vision_client.cpp \
    wireless/wireless_robot.cpp \
    vision/vision_kalman_filter.cpp \
    vision/kalman_filter_tools/kalman_filter_matrix.cpp \
    net/net_refbox_receive_thread.cpp \
    net/net_radio_receive_thread.cpp \
    net/net_refbox_client.cpp \
    user_interface/field_related/field_view.cpp \
    user_interface/field_related/field_scene.cpp \
    user_interface/field_related/field_robot.cpp \
    user_interface/field_related/field_item.cpp \
    user_interface/field_related/field_global_function.cpp \
    user_interface/field_related/field_ball.cpp \
    net/net_vision_receive_thread.cpp \
    control_hub/computer_control/knowledge_base/database/world_state/field_world_robot.cpp \
    control_hub/computer_control/knowledge_base/database/world_state/field_world_obstacle.cpp \
    control_hub/computer_control/knowledge_base/database/world_state/field_world.cpp \
    control_hub/computer_control/intelligence/world_analysor/field_analysor.cpp \
    control_hub/computer_control/intelligence/item_property_excutor/field_robot_tracker.cpp \
    control_hub/computer_control/intelligence/item_property_excutor/field_ball_tracker.cpp \
    control_hub/computer_control/intelligence/item_property_excutor/field_tracker.cpp \
    user_interface/field_related/paint_tools/paint_buffer.cpp \
    control_hub/computer_control/intelligence/world_analysor/field_analysor_parametres.cpp \
    control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.cpp \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_cmd.pb.cc \
    control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_vqueue.cpp \
    net/net_vision_send_thread.cpp \
    control_hub/debug_control/visual_debug/path.cpp \
    control_hub/debug_control/visual_debug/path_line.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/strategy_thread.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/play/play.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/strategy.cpp \
    user_interface/strategy_control_window.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/evaluation.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/goalie.cpp \
    control_hub/human_control/joystick_gao.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/thread_count_tools/hires_timer.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/thread_arismatic_tools/algorithm_base.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/special_tactics.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/defense_tactics.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/anticipation_tactics.cpp \
    control_hub/computer_control/knowledge_base/database/world_state/field_world_robot_goto_point.cpp \
    control_hub/computer_control/knowledge_base/algorithms/path_planner/path_planner.cpp \
    user_interface/cmd_ui.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/skill/skill.cpp \
    control_hub/computer_control/cerebellum/mostion_control.cpp \
    wireless/serial_server.cpp \
    user_interface/ui_setting/xbeltree.cpp \
    user_interface/ui_setting/setting_window.cpp \
    net/thread_tools/field_timer.cpp \
    wireless/port_operation.cpp \
    wireless/SLIP_operation.cpp \
    wireless/DECT_operation.cpp \
    wireless/transparent_operation.cpp \
    net/net_webserver_send_thread.cpp \
    net/web_data_model/webdatamodel.cpp \
#    control_hub/human_control/kinect.cpp \
#    control_hub/human_control/kinect_module/kinect_module.c \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/move_to_middle_tactic.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/move_as_sin_tactic.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/move_as_straight_tactic.cpp \
    control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.cpp
HEADERS += wireless/transparent_operation.h \
    wireless/wireless_parametres.h \
    control_hub/human_control/keyboard.h \
    net/net_radio_server.h \
    net/net_base.h \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_wrapper.pb.h \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_refbox_log.pb.h \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_geometry.pb.h \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_detection.pb.h \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_radio.pb.h \
    net/net_radio_send_thread.h \
    net/net_radio_client.h \
    net/net_radio_date_structures.h \
    control_hub/computer_control/knowledge_base/algorithms/numeric_operation_templates.h \
    net/net_vision_server.h \
    net/net_vision_client.h \
    wireless/wireless_robot.h \
    net/net_vision_receive_thread.h \
    net/net_vision_interface.h \
    vision/vision_kalman_filter.h \
    vision/kalman_filter_tools/kalman_filter_matrix.h \
    net/net_refbox_receive_thread.h \
    net/net_radio_receive_thread.h \
    net/net_refbox_client.h \
    net/net_refbox_command.h \
    user_interface/field_related/field_view.h \
    user_interface/field_related/field_size_constants.h \
    user_interface/field_related/field_scene.h \
    user_interface/field_related/field_robot.h \
    user_interface/field_related/field_item.h \
    user_interface/field_related/field_global_function.h \
    user_interface/field_related/field_constants.h \
    user_interface/field_related/field_base_constants.h \
    user_interface/field_related/field_ball.h \
    user_interface/field_related/motion_simulation_tools/field_vectors.h \
    user_interface/field_related/motion_simulation_tools/field_util.h \
    control_hub/computer_control/knowledge_base/database/world_state/field_world_robot.h \
    control_hub/computer_control/knowledge_base/database/world_state/field_world_obstacle.h \
    control_hub/computer_control/knowledge_base/database/world_state/field_world.h \
    control_hub/computer_control/intelligence/world_analysor/field_analysor.h \
    control_hub/computer_control/intelligence/item_property_excutor/field_robot_tracker.h \
    control_hub/computer_control/intelligence/item_property_excutor/field_ball_tracker.h \
    control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_geometry.h \
    net/net_vision_gui_interface.h \
    control_hub/computer_control/intelligence/item_property_excutor/field_tracker.h \
    net/thread_tools/field_timer.h \
    user_interface/field_related/paint_tools/paint_buffer.h \
    control_hub/computer_control/intelligence/world_analysor/field_analysor_robot.h \
    control_hub/computer_control/intelligence/world_analysor/field_analysor_parametres.h \
    control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h \
    net/message_serilization/radio_client_out_files/messages_robocup_ssl_cmd.pb.h \
    control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_vqueue.h \
    net/net_vision_send_thread.h \
    control_hub/debug_control/visual_debug/path.h \
    control_hub/debug_control/visual_debug/path_line.h \
    control_hub/computer_control/intelligence/strategy_extutor/strategy_thread.h \
    control_hub/computer_control/intelligence/strategy_extutor/play/play.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h \
    control_hub/computer_control/intelligence/strategy_extutor/strategy.h \
    user_interface/strategy_control_window.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h \
    control_hub/computer_control/intelligence/strategy_extutor/evaluation.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/goalie.h \
    control_hub/human_control/joystick_gao.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/thread_count_tools/hires_timer.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/thread_arismatic_tools/algorithm_base.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/special_tactics.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/defense_tactics.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/anticipation_tactics.h \
    control_hub/computer_control/knowledge_base/algorithms/path_planner/path_planner.h \
    control_hub/computer_control/knowledge_base/algorithms/path_planner/path_plan_tools/kdtree.h \
    control_hub/computer_control/knowledge_base/algorithms/path_planner/path_plan_tools/fast_alloc.h \
    user_interface/cmd_ui.h \
    control_hub/computer_control/intelligence/strategy_extutor/skill/skill.h \
    wireless/wireless_send_thread.h \
    control_hub/computer_control/cerebellum/mostion_control.h \
    wireless/serial_server.h \
    user_interface/ui_setting/xbeltree.h \
    user_interface/ui_setting/setting_window.h \
    wireless/port_operation.h \
    wireless/DECT_operation.h \
    wireless/SLIP_operation.h \
    wireless/transparent_operation.h \
    net/net_webserver_send_thread.h \
    net/web_data_model/webdatamodel.h \
#    control_hub/human_control/kinect.h \
#    control_hub/human_control/kinect_module/libfreenect.h \
#    control_hub/human_control/kinect_module/kinect_module.h \
#    control_hub/human_control/kinect_module/ncurses_for_kinect.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/move_to_middle_tactic.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/move_as_sin_tactic.h \
    control_hub/computer_control/intelligence/strategy_extutor/tactic/move_as_straight_tactic.h \
    control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h
OTHER_FILES += log.txt
FORMS += user_interface/strategy.ui
