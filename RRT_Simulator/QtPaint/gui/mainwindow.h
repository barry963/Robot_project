#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "shape.h"
#include "paintwidget.h"
#include "preferencewidget.h"
#include "performancewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    PaintWidget* getPaintWidget(){
        return this->paintWidget;
    }

signals:
    void changeCurrentShape(Shape::Code newShape);
    void startSimulationSignal();
    void stopSimulationSignal();
    void resetSimulationSignal();

    // emit this signal when received signal from the perference widget
    void rrtTextChangedSignalMain(QString name);
    void edgeLengthChangedMain(QString length);
    void iterationChangedMain(QString iteration);

private slots:
    void drawLineActionTriggered();
    void drawRectActionTriggered();
    void drawCircleActionTriggered();
    void drawStartCircleActionTriggered();
    void drawGoalCircleActionTriggered();

    void saveActionTriggered();
    void loadActionTriggered();
    void startActionTriggered();
    void stopActionTriggered();
    void resetActionTriggered();

    void rrtTextChangedSlotMain(QString name);
    void edgeLengthChangedSlotMain(QString length);
    void iterationChangedSlotMain(QString iteration);



private:
    QGridLayout *gridLayout;
    PaintWidget *paintWidget;
    PreferenceWidget* preferenceWidget;
    PerformanceWidget* performanceWidget;
};

#endif // MAINWINDOW_H
