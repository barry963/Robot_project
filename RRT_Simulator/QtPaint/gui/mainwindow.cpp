#include "gui/mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QToolBar *bar1 = this->addToolBar("Tools");
    QActionGroup *group1 = new QActionGroup(bar1);

    QAction *drawLineAction = new QAction("Line", bar1);
    drawLineAction->setIcon(QIcon(":/res/line.png"));
    drawLineAction->setToolTip(tr("Draw a line."));
    drawLineAction->setStatusTip(tr("Draw a line."));
    drawLineAction->setCheckable(true);
    drawLineAction->setChecked(true);
    group1->addAction(drawLineAction);
    bar1->addAction(drawLineAction);

    QAction *drawRectAction = new QAction("Rectangle", bar1);
    drawRectAction->setIcon(QIcon(":/res/rect.png"));
    drawRectAction->setToolTip(tr("Draw a rectangle."));
    drawRectAction->setStatusTip(tr("Draw a rectangle."));
    drawRectAction->setCheckable(true);
    group1->addAction(drawRectAction);
    bar1->addAction(drawRectAction);

    QAction *drawCircleAction = new QAction("Circle", bar1);
    drawCircleAction->setIcon(QIcon(":/res/circle.png"));
    drawCircleAction->setToolTip(tr("Draw a circle."));
    drawCircleAction->setStatusTip(tr("Draw a circle."));
    drawCircleAction->setCheckable(true);
    group1->addAction(drawCircleAction);
    bar1->addAction(drawCircleAction);

    // Start circle position
    QAction *drawStartCircleAction = new QAction("Circle", bar1);
    drawStartCircleAction->setIcon(QIcon(":/res/startcircle.png"));
    drawStartCircleAction->setToolTip(tr("Draw start position."));
    drawStartCircleAction->setStatusTip(tr("Draw start position."));
    drawStartCircleAction->setCheckable(true);
    group1->addAction(drawStartCircleAction);
    bar1->addAction(drawStartCircleAction);

    // Goal circle position
    QAction *drawGoalCircleAction = new QAction("Circle", bar1);
    drawGoalCircleAction->setIcon(QIcon(":/res/goalcircle.png"));
    drawGoalCircleAction->setToolTip(tr("Draw goal position."));
    drawGoalCircleAction->setStatusTip(tr("Draw goal position."));
    drawGoalCircleAction->setCheckable(true);
    group1->addAction(drawGoalCircleAction);
    bar1->addAction(drawGoalCircleAction);


    // =================== Save action
    QToolBar *bar2 = this->addToolBar("Tools2");
    QActionGroup *group2 = new QActionGroup(bar2);
    QAction *saveAction = new QAction("Save", bar2);
    saveAction->setIcon(QIcon(":/res/save.png"));
    saveAction->setToolTip(tr("Save to image"));
    saveAction->setStatusTip(tr("Save to image"));
    saveAction->setCheckable(false);
    saveAction->setChecked(false);
    group2->addAction(saveAction);
    bar2->addAction(saveAction);


    // =================== Load action
    QAction *loadAction = new QAction("Load", bar2);
    loadAction->setIcon(QIcon(":/res/load.png"));
    loadAction->setToolTip(tr("Load image"));
    loadAction->setStatusTip(tr("Load image"));
    loadAction->setCheckable(false);
    loadAction->setChecked(false);
    group2->addAction(loadAction);
    bar2->addAction(loadAction);


    // =================== Start action
    QAction *startAction = new QAction("Start", bar2);
    startAction->setIcon(QIcon(":/res/start.png"));
    startAction->setToolTip(tr("Start"));
    startAction->setStatusTip(tr("Start"));
    startAction->setCheckable(false);
    startAction->setChecked(false);
    group2->addAction(startAction);
    bar2->addAction(startAction);


    // =================== Stop action
    QAction *showAction = new QAction("Show Preformance", bar2);
    showAction->setIcon(QIcon(":/res/show.png"));
    showAction->setToolTip(tr("Show Preformance"));
    showAction->setStatusTip(tr("Show Preformance"));
    showAction->setCheckable(false);
    showAction->setChecked(false);
    group2->addAction(showAction);
    bar2->addAction(showAction);


    // =================== Reset action
    QAction *resetAction = new QAction("Reset", bar2);
    resetAction->setIcon(QIcon(":/res/reset.png"));
    resetAction->setToolTip(tr("Reset"));
    resetAction->setStatusTip(tr("Reset"));
    resetAction->setCheckable(false);
    resetAction->setChecked(false);
    group2->addAction(resetAction);
    bar2->addAction(resetAction);


    // Status bar
    QLabel *statusMsg = new QLabel;
    statusBar()->addWidget(statusMsg);


    paintWidget = new PaintWidget(this);

    gridLayout = new QGridLayout(this);
    gridLayout->addWidget(paintWidget, 0, 0, 1, 1);


//    paintWidget->resize(100, 100);
//    paintWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setCentralWidget(paintWidget);


    // Dock widget 1
    //设置主窗体的第一个QDockWidget
    QDockWidget *preferenceDockWidget = new QDockWidget(this);
    //设置第一个QDockWidget的窗口名称
    preferenceDockWidget->setWindowTitle(tr("Preference"));
    //设置第一个QDockWidget的可停靠区域,全部可停靠
    preferenceDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    preferenceDockWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //设置第一个QDockWidget内的控件并设置该控件的属性

    preferenceWidget = new PreferenceWidget(this);


    QCalendarWidget *calendar = new QCalendarWidget;
    calendar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    calendar->setGridVisible(true);
    //将QCalendarWidget控件设置成QDockWidget的主控件 使其能随窗口大小改变而改变
    preferenceDockWidget->setWidget(preferenceWidget);
    //向主窗体中添加第一个QDockWidget控件 第一个参数表示初始显示的位置 第二个参数是要添加的QDockWidget控件
    this->addDockWidget(Qt::RightDockWidgetArea, preferenceDockWidget);

    // Dock widget 2
    QDockWidget *performanceDockWidget = new QDockWidget(this);
    //设置第一个QDockWidget的窗口名称
    performanceDockWidget->setWindowTitle(tr("Performance"));
    //设置第一个QDockWidget的可停靠区域,全部可停靠
    performanceDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    performanceDockWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //设置第一个QDockWidget内的控件并设置该控件的属性

    performanceWidget = new PerformanceWidget(this);

    //将QCalendarWidget控件设置成QDockWidget的主控件 使其能随窗口大小改变而改变
    performanceDockWidget->setWidget(performanceWidget);
    //向主窗体中添加第一个QDockWidget控件 第一个参数表示初始显示的位置 第二个参数是要添加的QDockWidget控件
    this->addDockWidget(Qt::RightDockWidgetArea, performanceDockWidget);



    connect(drawLineAction, SIGNAL(triggered()), this, SLOT(drawLineActionTriggered()));
    connect(drawRectAction, SIGNAL(triggered()), this, SLOT(drawRectActionTriggered()));
    connect(drawCircleAction, SIGNAL(triggered()), this, SLOT(drawCircleActionTriggered()));
    connect(drawStartCircleAction, SIGNAL(triggered()), this, SLOT(drawStartCircleActionTriggered()));
    connect(drawGoalCircleAction, SIGNAL(triggered()), this, SLOT(drawGoalCircleActionTriggered()));
    connect(this, SIGNAL(changeCurrentShape(Shape::Code)), paintWidget, SLOT(setCurrentShape(Shape::Code)));

    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveActionTriggered()));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadActionTriggered()));
    connect(startAction, SIGNAL(triggered()), this, SLOT(startActionTriggered()));
    connect(showAction, SIGNAL(triggered()), this, SLOT(stopActionTriggered()));
    connect(resetAction, SIGNAL(triggered()), this, SLOT(resetActionTriggered()));

    // when received the signal from the perference widget then send to pain widget
    connect(preferenceWidget, SIGNAL(rrtTextChangedSignal(QString)), paintWidget, SLOT(rrtTextChangedSlot(QString)));

    connect(this, SIGNAL(startSimulationSignal()), paintWidget, SLOT(startSimulationSlot()));
    connect(this, SIGNAL(stopSimulationSignal()), paintWidget, SLOT(stopSimulationSlot()));
    connect(this, SIGNAL(resetSimulationSignal()), paintWidget, SLOT(resetSimulationSlot()));


    // connect the iteration to the display panel
   // connect(paintWidget->getMyRRT(),SIGNAL(currentIterationChanged(int)),preferenceWidget,SLOT(currentIterationChangedSlot(int)));
    connect(paintWidget,SIGNAL(currentIteration(int)),performanceWidget,SLOT(currentIterationChangedSlot(int)));
    connect(paintWidget,SIGNAL(currentNodeCount(int)),performanceWidget,SLOT(currentNodecountChangedSlot(int)));
    connect(paintWidget,SIGNAL(currentPathLength(int)),performanceWidget,SLOT(currentPathLengthChangedSlot(int)));
}

void MainWindow::drawLineActionTriggered()
{
    emit changeCurrentShape(Shape::Line);
}

void MainWindow::drawRectActionTriggered()
{
    emit changeCurrentShape(Shape::Rect);
}

void MainWindow::drawCircleActionTriggered()
{
    emit changeCurrentShape(Shape::Circle);
}


void MainWindow::drawStartCircleActionTriggered()
{
    emit changeCurrentShape(Shape::StartCircle);
}


void MainWindow::drawGoalCircleActionTriggered()
{
    emit changeCurrentShape(Shape::GoalCircle);
}

void MainWindow::rrtTextChangedSlotMain(QString name)
{
    //qDebug() << "MainWindow " << name;
    emit rrtTextChangedSignalMain(name);
}

void MainWindow::edgeLengthChangedSlotMain(QString name)
{
    emit edgeLengthChangedMain(name);
}
void MainWindow::iterationChangedSlotMain(QString iteration)
{
    emit iterationChangedMain(iteration);
}
// Save current image to local file
void MainWindow::saveActionTriggered()
{
    QImage image(paintWidget->size(), QImage::Format_ARGB32);
    QPainter painter(&image);
    paintWidget->render(&painter);
    painter.end();

    bool ok = image.save("defaultbk.png");
    qDebug() << "saveActionTriggered" << ok;
}


// Load an image from local file system
void MainWindow::loadActionTriggered()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png)"));
//    if(path.length() == 0) {
//          QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
//    } else {
//          QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
//    }

    QPixmap* loadedPixmap = new QPixmap();
    loadedPixmap->load(path);
    paintWidget->setPixmap(loadedPixmap);

    //TODO: Automatically set start and goal position!


    paintWidget->update();
}


void MainWindow::startActionTriggered()
{
    emit startSimulationSignal();
}


void MainWindow::stopActionTriggered()
{
    emit stopSimulationSignal();
}


void MainWindow::resetActionTriggered()
{
    emit resetSimulationSignal();
}


