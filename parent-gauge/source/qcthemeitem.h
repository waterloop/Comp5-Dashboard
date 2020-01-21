#ifndef QCTHEMEITEM_H
#define QCTHEMEITEM_H

#include <QWidget>
#include <QPainter>
#include <QObject>
#include <QRectF>
#include <QtMath>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

class QcThemeItem
{
public:
    QcThemeItem();
    QcThemeItem(QString directory, QString fileName);
    QcThemeItem(QString filepath);

    QString font = "Arial";
    QColor mainColor = Qt::white;
    QColor mediumWarningColor =Qt::yellow;
    QColor noWarningColor = Qt::green ;
    QColor highWarningColor = Qt::red;
    QColor warningColor = Qt::gray;
    QColor backgroundColor = Qt::black ;
    qreal fontSizeValues;
    qreal valuesRadius = -1;
    qreal fontSizeLabel;
    qreal fontSizeNeedleLabel;
    qreal fontSizeUnits;
    qreal needleRadius = - 1;
    qreal needleCoverRadius = -1;
    qreal outerRingRadius = -1;
    qreal mainBackgroundRadius =-1;
    qreal arcRadius=-1;
    qreal arcWidth;
    qreal colorBandRadius =-1;
    qreal colorBandWidth;
    qreal mainDegreesRadius = -1;
    qreal subDegreesRadius =-1;
    qreal mainDegreesWidth;
    qreal subDegreesWidth;
    qreal mainDegreesLength;
    qreal subDegreesLength;
    qreal subDegreesAlpha = 1;
    bool needleVisible;
    qreal minDegree = -45;
    qreal maxDegree=225;
    bool dropShadow=false;
    qreal dropShadowRadius = 25;
    QColor dropShadowColor = QColor(0,0,0,40);
    qreal dropShadowAlpha = 0.4; //from 0 to 1
    qreal dropShadowHorOffset = 1; //negative values are to the left
    qreal dropShadowVertOffset = 2; //negative values are up
    qreal labelLocation = -1;
    qreal unitsLocation = -1;
    qreal dynamicColorArcRadius = -1;
    QList<QPair<qreal,QColor>> dynamicColorArc;
    qreal dynamicColorArcWidth = 0.1;

};
#endif // QCTHEMEITEM_H
