#include "field_analysor_parametres.h"

unsigned int giShootPowerMax = 30;//lu_test
unsigned int giShootPowerPass = 30;//lu_test add initial value

ParamSet::ParamSet(const QString & fileName, Format format, QObject * parent)
                : QSettings(fileName,format,parent)
{
}

void ParamSet::writeSettings()
{
        beginGroup("Shoot");
        setValue("giShootPowerMax", giShootPowerMax);
        setValue("giShootPowerPass", giShootPowerPass);
        endGroup();
}

void ParamSet::readSettings()
{
        beginGroup("Shoot");
        giShootPowerMax=value("giShootPowerMax", 63).toUInt();
        giShootPowerPass=value("giShootPowerPass", 10).toUInt();
        endGroup();
}

ParamSet::~ParamSet()
{
}

