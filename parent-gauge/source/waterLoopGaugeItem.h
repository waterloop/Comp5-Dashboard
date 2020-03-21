#ifndef WATERLOOPGAUGEITEM_H
#define WATERLOOPGAUGEITEM_H

#include "qcgaugewidget.h"
#include "qcthemeitem.h"
#include <QGraphicsDropShadowEffect>


class waterLoopGaugeItem : public QcGaugeWidget
{
    Q_OBJECT
public:
    waterLoopGaugeItem();
    waterLoopGaugeItem(QcThemeItem &theme, qreal size, QString type, QString label, QString units,qreal precision, qreal startValue, qreal maxValue, qreal warningValueHigh, qreal warningValueLow, qreal stepSize);
    QcGaugeWidget * getGauge();
    qint8 getState();
    qreal getCurrentValue();
    void setCurrentValue(qreal value);

signals:

    void stateChanged();
    void badState();
    void safeState();

private:

    virtual ~waterLoopGaugeItem() {}

    QcGaugeWidget * mainGauge = nullptr;
    QcNeedleItem * mSpeedNeedle = nullptr;
    QcColorBand * mDynamicColorBand = nullptr;
    QcColorBand * mDynamicColorBandDegrees = nullptr;
    QcValuesItem * lightUpValues =nullptr;
    QcBackgroundItem * mainBackground = nullptr;
    QcBackgroundItem * needleCover = nullptr;
    QGraphicsDropShadowEffect * dropShadow = nullptr;
    QcDynamicArcItem * positionIndicatior = nullptr;
    QString type;
    QString label;
    QString units;
    qreal maxValue;
    qreal startValue;
    qreal warningValueHigh;
    qreal warningValueLow;
    qreal stepSize;
    qreal currentValue;
    qint8 state = 0; //0 for good, 1 for medium, 2 for warning



};

#endif // WATERLOOPGAUGEITEM_H
