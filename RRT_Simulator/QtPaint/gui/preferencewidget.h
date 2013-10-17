#ifndef PREFERENCEWIDGET_H
#define PREFERENCEWIDGET_H

#include <QWidget>

namespace Ui {
class PreferenceWidget;
}

class PreferenceWidget : public QWidget
{
    Q_OBJECT

signals:
    void rrtTextChangedSignal(QString name);
public:
    explicit PreferenceWidget(QWidget *parent = 0);
    ~PreferenceWidget();
    
private:
    Ui::PreferenceWidget *ui;
};

#endif // PREFERENCEWIDGET_H
