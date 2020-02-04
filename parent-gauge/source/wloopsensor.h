#ifndef WLOOPSENSOR_H
#define WLOOPSENSOR_H

#include "waterLoopGaugeItem.h"
#include "qcthemeitem.h"

class wloopSensor
{
public:
    wloopSensor(QcThemeItem &theme, QString name, QString label, QString units,  QVector<qreal> accRange, QVector<qreal> maxRange, qint8 precision, bool gauge = false, qint16 size = 250, qreal stepsize = 10);

private:

    QcThemeItem theme;
    QVector<qreal> accRange, maxRange;
    QString name,label, units;
    qint8 precision;
    waterLoopGaugeItem * gauge = nullptr;




};

#endif // WLOOPSENSOR_H
