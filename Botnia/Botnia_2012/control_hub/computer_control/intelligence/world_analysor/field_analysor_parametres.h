#ifndef FIELD_ANALYSOR_PARAMETRES_H
#define FIELD_ANALYSOR_PARAMETRES_H



#include <QSettings>

class ParamSet : public QSettings
{
        Q_OBJECT

public:
        ParamSet(const QString & fileName, Format format, QObject * parent = 0);
        ~ParamSet();
        void writeSettings();
        void readSettings();
private:

};

extern unsigned int giShootPowerMax;
extern unsigned int giShootPowerPass;

#endif // FIELD_ANALYSOR_PARAMETRES_H
