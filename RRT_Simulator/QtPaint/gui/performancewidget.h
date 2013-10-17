#ifndef PERFORMANCEWIDGET_H
#define PERFORMANCEWIDGET_H

#include <QWidget>

namespace Ui {
class PerformanceWidget;
}

class PerformanceWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PerformanceWidget(QWidget *parent = 0);
    ~PerformanceWidget();
    
private:
    Ui::PerformanceWidget *ui;

public slots:
    void currentIterationChangedSlot(int iteration);
    void currentNodecountChangedSlot(int nodecount);
    void currentPathLengthChangedSlot(int pathlength);
};

#endif // PERFORMANCEWIDGET_H
