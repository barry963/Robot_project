// ************************************************************************************************
//     Copyright 2012 Gao Yuan (robotics-computing.tk)
//
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//
//     Additional license information:
//
//  **********************************************************************************************/

/**    @file    - setting_windows.h
  *
  *    @class   - SettingMainWindow
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 10/15/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class XbelTree;

class SettingMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SettingMainWindow();
    ~SettingMainWindow();

public slots:
    // open file
    void open();
    // open default file which is stored in the relative path config/parameter_setting.xml
    void open_default();
    // save currently opened file
    void save();
    // save as another file
    void saveAs();
    // show about
    void about();

private:
    // create the instance of the action
    void create_actions();
    // create the view structure of menu
    void create_menu();

    // this parameter stores the currenly opened file name
    QString current_file_name_;
    // this stores the pointer of xbel_tree (a tree view of the xml setting file)
    XbelTree *xbel_tree_;
    // menus in the menubar
    QMenu *file_menu_;
    QMenu *help_menu_;
    // actions in the menu
    QAction *open_act_;
    QAction *save_act_;
    QAction *save_as_act_;
    QAction *exit_act_;
    QAction *about_act_;
};

#endif
