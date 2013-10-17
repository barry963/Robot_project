
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

/**    @file    - setting_windows.cpp
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


// if you want to bautify this, you can use QT style sheet

#include <QtGui>

#include "setting_window.h"
#include "xbeltree.h"

SettingMainWindow::SettingMainWindow()
{
    xbel_tree_ = new XbelTree;
    setCentralWidget(xbel_tree_);

    create_actions();
    create_menu();

    statusBar()->showMessage(tr("Ready"));
    setWindowTitle(tr("DOM Bookmarks"));
    resize(480, 320);
}

SettingMainWindow::~SettingMainWindow()
{
    delete xbel_tree_;
    xbel_tree_ = NULL;
}

// open file for the editing
void SettingMainWindow::open()
{
    QString file_name =
            QFileDialog::getOpenFileName(this, tr("Open Bookmark File"),
                                         QDir::currentPath(),
                                         tr("XBEL Files (*.xbel *.xml)"));
    if (file_name.isEmpty())
        return;

    QFile file(file_name);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file_name)
                             .arg(file.errorString()));
        return;
    }

    if (xbel_tree_->read(&file))
        statusBar()->showMessage(tr("File loaded"), 2000);
    current_file_name_ = file_name;
}

void SettingMainWindow::open_default()
{
    QString fileName = "config/parameter_settiings.xml";
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    if (xbel_tree_->read(&file))
        statusBar()->showMessage(tr("File loaded"), 2000);
    current_file_name_ = fileName;

}
void SettingMainWindow::save()
{
    QString file_name = current_file_name_;
    if (file_name.isEmpty())
        return;

    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file_name)
                             .arg(file.errorString()));
        return;
    }

    if (xbel_tree_->write(&file))
        statusBar()->showMessage(tr("File saved"), 2000);
    return;
}
void SettingMainWindow::saveAs()
{
    QString file_name =
            QFileDialog::getSaveFileName(this, tr("Save Bookmark File"),
                                         QDir::currentPath(),
                                         tr("XBEL Files (*.xbel *.xml)"));

    if (file_name.isEmpty())
        return;

    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file_name)
                             .arg(file.errorString()));
        return;
    }

    if (xbel_tree_->write(&file))
        statusBar()->showMessage(tr("File saved"), 2000);
}

void SettingMainWindow::about()
{
    QMessageBox::about(this, tr("Parameter Setting"),
                       tr("This is for new 2012 robot strategy software settings"));
}

void SettingMainWindow::create_actions()
{
    // create the instances of action ( if you want to know why go and see the how to create actions in Qt)
    open_act_ = new QAction(tr("&Open..."), this);
    open_act_->setShortcuts(QKeySequence::Open);
    connect(open_act_, SIGNAL(triggered()), this, SLOT(open()));

    save_act_ = new QAction(tr("Save"),this);
    save_act_->setShortcut(QKeySequence::Save);
    connect(save_act_,SIGNAL(triggered()),this,SLOT(save()));

    save_as_act_ = new QAction(tr("&Save As..."), this);
    save_as_act_->setShortcuts(QKeySequence::SaveAs);
    connect(save_as_act_, SIGNAL(triggered()), this, SLOT(saveAs()));

    exit_act_ = new QAction(tr("E&xit"), this);
    exit_act_->setShortcuts(QKeySequence::Quit);
    connect(exit_act_, SIGNAL(triggered()), this, SLOT(close()));

    about_act_ = new QAction(tr("&About"), this);
    connect(about_act_, SIGNAL(triggered()), this, SLOT(about()));

}

void SettingMainWindow::create_menu()
{
    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(open_act_);
    file_menu_->addAction(save_act_);
    file_menu_->addAction(save_as_act_);
    file_menu_->addAction(exit_act_);


    menuBar()->addSeparator();

    help_menu_ = menuBar()->addMenu(tr("&Help"));
    help_menu_->addAction(about_act_);
}
