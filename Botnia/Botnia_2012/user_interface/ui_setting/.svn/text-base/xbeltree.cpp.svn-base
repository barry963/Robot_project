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

/**    @file    - xbeltree.cpp
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

#include <QtGui>

#include "xbeltree.h"


// constructor of the XbelTree
XbelTree::XbelTree(QWidget *parent)
    : QTreeWidget(parent)
{
    QStringList labels;
    labels << tr("Element") << tr("Values");

    header()->setResizeMode(QHeaderView::Stretch);
    setHeaderLabels(labels);

    folder_icon_.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folder_icon_.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmark_icon_.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
}

bool XbelTree::read(QIODevice *device)
{
    QString error_string;
    int error_line;
    int error_column;

    if (!dom_document.setContent(device, true, &error_string, &error_line,
                                &error_column)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(error_line)
                                 .arg(error_column)
                                 .arg(error_string));
        return false;
    }

    QDomElement root = dom_document.documentElement();
    if (root.tagName() != "xbel") {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("The file is not an XBEL file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "1.0") {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("The file is not an XBEL version 1.0 "
                                    "file."));
        return false;
    }

    clear();

    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(update_dom_element(QTreeWidgetItem*,int)));

    QDomElement child = root.firstChildElement("folder");
    while (!child.isNull()) {
        parse_folder_element(child);
        child = child.nextSiblingElement("folder");
    }

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(update_dom_element(QTreeWidgetItem*,int)));

    return true;
}

bool XbelTree::write(QIODevice *device)
{
    const int indentSize = 4;

    QTextStream out(device);
    dom_document.save(out, indentSize);
    return true;
}

void XbelTree::update_dom_element(QTreeWidgetItem *item, int column)
{
    QDomElement element = dom_element_for_item_.value(item);
    if (!element.isNull()) {
        if (column == 0) {
            QDomElement old_title_element = element.firstChildElement("title");
            QDomElement new_title_element = dom_document.createElement("title");
            QDomText new_title_text = dom_document.createTextNode(item->text(0));
            new_title_element.appendChild(new_title_text);
            element.replaceChild(new_title_element, old_title_element);
            item->child(0)->setText(1,item->text(0));
        } else {
            QDomText old_title_text = element.firstChild().toText();
            QDomText new_element_text = dom_document.createTextNode(item->text(1));
            element.replaceChild(new_element_text,old_title_text);
        }
    }
}

// a recursive function to deal with the analysis of the tree element
void XbelTree::parse_folder_element(const QDomElement &element,
                                  QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = create_item(element, parentItem);

    QString title = element.firstChildElement("title").text();
    if (title.isEmpty())
        title = QObject::tr("Folder");

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setIcon(0, folder_icon_);
    item->setText(0, title);

    bool folded = (element.attribute("folded") != "no");
    setItemExpanded(item, !folded);

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == "folder") {
            parse_folder_element(child, item);

        }
        else if (child.tagName() == "separator") {
            QTreeWidgetItem *child_item = create_item(child, item);
            child_item->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));
            child_item->setText(0, QString(30, 0xB7));
        }
        else
        {
            QString title = child.tagName();
            QTreeWidgetItem *childItem = create_item(child, item);


            if (title.isEmpty())
                title = QObject::tr("Folder");

            if (child.tagName()!="title")
                childItem->setFlags(item->flags() | Qt::ItemIsEditable);
            childItem->setIcon(0, bookmark_icon_);
            childItem->setText(0, title);
            childItem->setText(1, child.firstChild().nodeValue());
            child = child.nextSiblingElement();
        }
    }
}

QTreeWidgetItem *XbelTree::create_item(const QDomElement &element,
                                      QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item;
    if (parentItem) {
        item = new QTreeWidgetItem(parentItem);
    } else {
        item = new QTreeWidgetItem(this);
    }
    dom_element_for_item_.insert(item, element);
    return item;
}
