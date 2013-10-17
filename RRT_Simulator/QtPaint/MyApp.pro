TARGET = MyApp
TEMPLATE = app
SOURCES += \
    main.cpp \
    gui/shape.cpp \
    gui/rect.cpp \
    gui/preferencewidget.cpp \
    gui/performancewidget.cpp \
    gui/paintwidget.cpp \
    gui/mainwindow.cpp \
    gui/line.cpp \
    gui/circle.cpp \
    rrt/rrt.cpp \
    rrt/mytree.cpp \
    rrt/mynode.cpp \
    rrt/miscmethods.cpp


HEADERS += \
    gui/shape.h \
    gui/rect.h \
    gui/preferencewidget.h \
    gui/performancewidget.h \
    gui/paintwidget.h \
    gui/mainwindow.h \
    gui/line.h \
    gui/circle.h \
    rrt/rrt.h \
    rrt/mytree.h \
    rrt/mynode.h \
    rrt/miscmethods.h



RESOURCES += res.qrc

FORMS += \
    preferencewidget.ui \
    performancewidget.ui
