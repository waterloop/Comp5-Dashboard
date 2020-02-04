/***************************************************************************
**                                                                        **
**  QcGauge, for instrumentation, and real time data measurement          **
**  visualization widget for Qt.                                          **
**  Copyright (C) 2015 Hadj Tahar Berrima                                 **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU Lesser General Public License as        **
**  published by the Free Software Foundation, either version 3 of the    **
**  License, or (at your option) any later version.                       **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU Lesser General Public License for more details.                   **
**                                                                        **
**  You should have received a copy of the GNU Lesser General Public      **
**  License along with this program.                                      **
**  If not, see http://www.gnu.org/licenses/.                             **
**                                                                        **
****************************************************************************
**           Author:  Hadj Tahar Berrima                                  **
**           Website: http://pytricity.com/                               **
**           Contact: berrima_tahar@yahoo.com                             **
**           Date:    1 dec 2014                                          **
**           Version:  1.0                                                **
****************************************************************************/

#ifndef QCGAUGEWIDGET_H
#define QCGAUGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QObject>
#include <QRectF>
#include <QtMath>
#include <QImage>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>

#if defined(QCGAUGE_COMPILE_LIBRARY)
#  define QCGAUGE_DECL  Q_DECL_EXPORT
#elif defined(QCGAUGE_USE_LIBRARY)
#  define QCGAUGE_DECL Q_DECL_IMPORT
#else
#  define QCGAUGE_DECL
#endif

class QcGaugeWidget;
class QcItem;
class QcBackgroundItem;
class QcDegreesItem;
class QcValuesItem;
class QcArcItem;
class QcColorBand;
class QcImage;
class QcDynamicArcItem;
class QcNeedleItem;
class QcLabelItem;
class QcGlassItem;
class QcAttitudeMeter;

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
class QCGAUGE_DECL QcGaugeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QcGaugeWidget(QWidget *parent = 0);    

    QcBackgroundItem* addBackground(qreal position);
    QcDegreesItem* addDegrees(qreal position);
    QcValuesItem* addValues(qreal position);
    QcArcItem* addArc(qreal position);
    QcColorBand* addColorBand(qreal position);
    QcDynamicArcItem* addDynamicArc(qreal position);
    QcImage* addImage(qreal position);
    QcNeedleItem* addNeedle(qreal position);
    QcLabelItem* addLabel(qreal position);
    QcGlassItem* addGlass(qreal position);
    QcAttitudeMeter* addAttitudeMeter(qreal position);


    void addItem(QcItem* item, qreal position);
    int removeItem(QcItem* item);
    QList <QcItem*> items();
    QList <QcItem*> mItems;

signals:

public slots:
private:
    void paintEvent(QPaintEvent *);

};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcItem : public QObject
{
    Q_OBJECT
public:
    explicit QcItem(QObject *parent = 0);
    virtual void draw(QPainter *) = 0;
    virtual int type();

    void setPosition(qreal percentage);
    qreal position();
    QRectF rect();
    enum Error{InvalidValueRange,InvalidDegreeRange,InvalidStep};


protected:
    QRectF adjustRect(qreal percentage);
    qreal getRadius(const QRectF &);
    qreal getAngle(const QPointF&, const QRectF &tmpRect);
    QPointF getPoint(qreal deg, const QRectF &tmpRect);
    QRectF resetRect();
    void update();

private:
    QRectF mRect;
    QWidget *parentWidget;
    qreal mPosition;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcScaleItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcScaleItem(QObject *parent = 0);

    void setValueRange(qreal minValue,qreal maxValue);
    void setDegreeRange(qreal minDegree,qreal maxDegree);
    void setMinValue(qreal minValue);
    void setMaxValue(qreal maxValue);
    void setMinDegree(qreal minDegree);
    void setMaxDegree(qreal maxDegree);
    qreal mMinDegree;
    qreal mMaxDegree;

signals:

public slots:


protected:

    qreal getDegFromValue(qreal);

    qreal mMinValue;
    qreal mMaxValue;


};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcBackgroundItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcBackgroundItem(QObject *parent = 0);
    void draw(QPainter*);
    void addColor(qreal position, const QColor& color);
    void clearrColors();
    void setDynamic(bool b);
    void setDynamicColors(const QColor &regColor, const QColor &warnColor);
    void setWarningValues(qreal low, qreal high);
    void setCurrentValue(qreal value);


private:
    QPen mPen;
    QList<QPair<qreal,QColor> > mColors;
    QLinearGradient mLinearGrad;
    bool mDynamic;
    qreal mWarningHigh;
    qreal mWarningLow;
    qreal mCurrentValue;
    QColor mWarningColorFill;
    QColor mRegularColorFill;


};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QcGlassItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcGlassItem(QObject *parent = 0);
    void draw(QPainter*);
};



///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcLabelItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcLabelItem(QObject *parent = 0);
    virtual void draw(QPainter *);
    void setAngle(qreal);
    qreal angle();
    void setText(const QString &text, bool repaint = true);
    QString text();
    void setColor(const QColor& color);
    QColor color();
    void setFont(QString str);
    void setFontSize(qreal size);

private:
    qreal mAngle;
    qreal mFontSize;//based on a percentage of the radius
    QString mFont;
    QString mText;
    QColor mColor;
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcArcItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcArcItem(QObject *parent = 0);
    void draw(QPainter*);
    void setColor(const QColor& color);
    void setWidth(qreal width);


private:
    QColor mColor;
    qreal mWidth;

signals:

public slots:


};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcImage : public QcItem
{
    Q_OBJECT
public:
    explicit QcImage(QObject * parent = 0);
    void draw(QPainter *);
    void setImage(QString filename);
    void setDim(qreal w, qreal h); //based on percentages!
    void scale(qreal w);//keeps ratio the same changes to new width percentage
    void setAngle(qreal angle);

private:

    QPixmap mImage;
    qreal mAngle;
    qreal mWidth, mHeight;

};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcDynamicArcItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcDynamicArcItem(QObject *parent = nullptr);
    void draw(QPainter*);
    void setColor(const QColor& color);

    /**
     * @brief setColor set multiple colors to transition colors at different percentages
     * @param colors
     */
    void setColor(QList< QPair <qreal, QColor> > colors);
    /**
     * @brief setWidth Set the relative width of the arc
     * @param width
     */
    void setWidth(const qreal& width);
    /**
     * @brief setPercentage sets the value of the arc
     * @param percent qreal between 0.0 and 1.0
     */
    void setPercentage(const qreal &percent);



private:
    QColor mColor;
    QList< QPair <qreal, QColor> > mColors;
    bool usingSingleColor;
    qreal mWidth;
    qreal mPercentage;


    QColor calculateColor(qreal percentage);
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


class QCGAUGE_DECL QcColorBand : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcColorBand(QObject *parent = 0);
    void draw(QPainter*);
    void setColors(const QList<QPair<QColor,qreal> >& colors);
    void setWidth(qreal width);
    void setDynamic(bool b);
    void setCurrentValue(qreal value);
    void setOpacity(qreal value); //from 0 to 1
    void setCoveringColor(QColor c);


private:
   QPainterPath createSubBand(qreal from,qreal sweep);
   QList<QPair<QColor,qreal> > mBandColors;
   qreal mBandStartValue;
   qreal mBandWidth; //percentage of the radius
   qreal mOpacity; //only for when dynamic, sets opacity of covering color
   QColor mCoveringColor; //sets the color which will cover the other color band
   bool mdynamic;
   qreal mCurrentValue; //percentage (from 0 to 100) of the gauge

};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
class QCGAUGE_DECL QcDegreesItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcDegreesItem(QObject *parent = 0);
    void draw(QPainter *painter);
    void setStep(qreal step);
    void setColor(const QColor& color);
    void setSubDegree(bool );
    void setLength(qreal length); //length is a percentage of distance from the outer radius of the degrees to the center
    void setWidth(qreal width); //takes width as percentage of radius

private:
    qreal mStep;
    QColor mColor;
    bool mSubDegree;

    qreal mLength; //length is a percentage of distance from the outer radius of the degrees to the center
    qreal mWidth; //width of each individual degree (given as a percentage of the circle radius)
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcNeedleItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcNeedleItem(QObject *parent = 0);
    void draw(QPainter*);
    void setCurrentValue(qreal value);
    qreal currentValue();
    void setValueFormat(QString format);
    QString currentValueFormat();
    void setColor(const QColor & color);
    QColor color();

    void setLabel(QcLabelItem*);
    QcLabelItem * label();
    void setPrecision(qreal value);
    enum NeedleType{DiamonNeedle,TriangleNeedle,FeatherNeedle,AttitudeMeterNeedle,CompassNeedle};//#

    void setNeedle(QcNeedleItem::NeedleType needleType);
private:
    QPolygonF mNeedlePoly;
    qreal mCurrentValue;
    QColor mColor;
    void createDiamonNeedle(qreal r);
    void createTriangleNeedle(qreal r);
    void createFeatherNeedle(qreal r);
    void createAttitudeNeedle(qreal r);
    void createCompassNeedle(qreal r);
    NeedleType mNeedleType;
    QcLabelItem *mLabel;
    QString mFormat;
    qreal mPrecision;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


class QCGAUGE_DECL QcValuesItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcValuesItem(QObject *parent = 0);
    void draw(QPainter*);
    void setStep(qreal step);
    void setColor(const QColor& color);
    void setFont(QString font);
    void setFontSize(qreal value);
    void setDynamic(bool b);
    void setColorUnlit(const QColor& color);
    void setColorLit(const QColor& color);
    void setCurrentValue(qreal value);

private:
    QString mfont;
    qreal mFontSize; //given as a value from 0 to 1 which represents percentage of radius
    qreal mStep;
    QColor mColor;

    bool mDynamic;
    QColor mColorUnlit;
    QColor mColorLit;
    qreal mCurrentValue;

};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcAttitudeMeter : public QcItem
{
    Q_OBJECT
public:
    explicit QcAttitudeMeter(QObject *parent = 0);

    void draw(QPainter *);
    void setCurrentPitch(qreal pitch);
    void setCurrentRoll(qreal roll);
private:
    qreal mRoll;
    qreal mPitch;
    qreal mPitchOffset;

    QPolygonF mHandlePoly;
    QPainterPath mStepsPath;

    QPointF getIntersection(qreal r,const QPointF& pitchPoint,const QPointF& pt);
    qreal getStartAngle(const QRectF& tmpRect);

    void drawDegrees(QPainter *);
    void drawDegree(QPainter * painter, const QRectF& tmpRect,qreal deg);
    void drawUpperEllipse(QPainter *,const QRectF&);
    void drawLowerEllipse(QPainter *,const QRectF&);
    void drawPitchSteps(QPainter *,const QRectF&);
    void drawHandle(QPainter *);
    void drawSteps(QPainter *,qreal);

};

#endif // QCGAUGEWIDGET_H
