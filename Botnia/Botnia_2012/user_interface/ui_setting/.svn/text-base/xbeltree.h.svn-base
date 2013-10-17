// ************************************************************************************************
//     Copyright 2012 Gao Yuan (robotics-computing.tk)
//
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//
//     Additional license information:
//     This paticular file follows the BSD licence
//
//  **********************************************************************************************/

/**    @file    - xbeltree.h
  *
  *    @class   - XbelTree
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 10/15/2012
  *
  *    @details - This class used
  *
  *    Project  - %PROJECT%
  */

#ifndef XBELTREE_H
#define XBELTREE_H

#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>

class XbelTree : public QTreeWidget
{
    Q_OBJECT

public:
    XbelTree(QWidget *parent = 0);

    bool read(QIODevice *device);
    bool write(QIODevice *device);

private slots:
    void update_dom_element(QTreeWidgetItem *item, int column);

private:
    void parse_folder_element(const QDomElement &element,
                            QTreeWidgetItem *parentItem = 0);
    QTreeWidgetItem *create_item(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);
    QDomDocument dom_document;
    QHash<QTreeWidgetItem *, QDomElement> dom_element_for_item_;
    QIcon folder_icon_;
    QIcon bookmark_icon_;
};

#endif
