#include "wloopsensor.h"

wloopSensor::wloopSensor(QcThemeItem &theme, QString name, QString label, QString units,  QVector<qreal> accRange, QVector<qreal> maxRange, qint8 precision, bool gauge, qint16 size,qreal stepSize)
{
    this->theme = theme;
    this->name = name;
    this->label = label;
    this->units = units;
    if (accRange.length() != 2){
        qDebug() << " this is wrong your accRange is not right";
    }
    this->accRange = accRange;
    if (maxRange.length() != 2){
        qDebug() << "you are wrong maxrange is not right lol";
    }
    this->maxRange = maxRange;
    this->precision = precision;
    if (gauge){
        this->gauge = new waterLoopGaugeItem(theme, size, name, label, units, precision, maxRange[0], maxRange[1], accRange[1],accRange[0],stepSize);
    }


}

