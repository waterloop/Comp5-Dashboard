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

#include "qcgaugewidget.h"
#include <QDebug>

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcGaugeWidget::QcGaugeWidget(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(250,250);
}

QcBackgroundItem *QcGaugeWidget::addBackground(qreal position)
{
    QcBackgroundItem * item = new QcBackgroundItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcDegreesItem *QcGaugeWidget::addDegrees(qreal position)
{
    QcDegreesItem * item = new QcDegreesItem(this);
    item->setPosition(position);

    mItems.append(item);
    return item;
}


QcValuesItem *QcGaugeWidget::addValues(qreal position)
{
    QcValuesItem * item = new QcValuesItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcArcItem *QcGaugeWidget::addArc(qreal position)
{
    QcArcItem * item = new QcArcItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcImage *QcGaugeWidget::addImage(qreal position){
    QcImage * image = new QcImage(this);
    image->setPosition(position);
    mItems.append(image);
    return image;
}

QcColorBand *QcGaugeWidget::addColorBand(qreal position)
{
    QcColorBand * item = new QcColorBand(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcDynamicArcItem *QcGaugeWidget::addDynamicArc(qreal position) {
    QcDynamicArcItem *item = new QcDynamicArcItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcNeedleItem *QcGaugeWidget::addNeedle(qreal position)
{
    QcNeedleItem * item = new QcNeedleItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcLabelItem *QcGaugeWidget::addLabel(qreal position)
{
    QcLabelItem * item = new QcLabelItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcGlassItem *QcGaugeWidget::addGlass(qreal position)
{
    QcGlassItem * item = new QcGlassItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcAttitudeMeter *QcGaugeWidget::addAttitudeMeter(qreal position)
{
    QcAttitudeMeter * item = new QcAttitudeMeter(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

void QcGaugeWidget::addItem(QcItem *item,qreal position)
{
    // takes parentship of the item
    item->setParent(this);
    item->setPosition(position);
    mItems.append(item);
}

int QcGaugeWidget::removeItem(QcItem *item)
{
   return mItems.removeAll(item);
}

QList<QcItem *> QcGaugeWidget::items()
{
    return mItems;
}


void QcGaugeWidget::paintEvent(QPaintEvent */*paintEvt*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    foreach (QcItem * item, mItems) {
        item->draw(&painter);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcItem::QcItem(QObject *parent) :
    QObject(parent)
{

    parentWidget = qobject_cast<QWidget*>(parent);
    mPosition = 50;
}

int QcItem::type()
{
    return 50;
}

void QcItem::update()
{
    parentWidget->update();
}

qreal QcItem::position()
{
    return mPosition;
}

QRectF QcItem::rect()
{
    return mRect;
}

void QcItem::setPosition(qreal position)
{
    if(position>100)
        mPosition = 100;
    else if(position<0)
        mPosition = 0;
    else
        mPosition = position;
    update();
}

QRectF QcItem::adjustRect(qreal percentage)
{
    qreal r = getRadius(mRect);
    qreal offset =   r-(percentage*r)/100.0;
    QRectF tmpRect = mRect.adjusted(offset,offset,-offset,-offset);
    return tmpRect;
}

qreal QcItem::getRadius(const QRectF &tmpRect)
{
    qreal r = 0;
    if(tmpRect.width()<tmpRect.height())
        r = tmpRect.width()/2.0;
    else
        r = tmpRect.height()/2.0;
    return r;
}

QRectF QcItem::resetRect()
{
    mRect = parentWidget->rect();
    qreal r = getRadius(mRect);
    mRect.setWidth(2.0*r);
    mRect.setHeight(2.0*r);
    mRect.moveCenter(parentWidget->rect().center());
    return mRect;
}

QPointF QcItem::getPoint(qreal deg,const QRectF &tmpRect)
{
    qreal r = getRadius(tmpRect);
    qreal xx=cos(qDegreesToRadians(deg))*r;
    qreal yy=sin(qDegreesToRadians(deg))*r;
    QPointF pt;
    xx=tmpRect.center().x()-xx;
    yy=tmpRect.center().y()-yy;
    pt.setX(xx);
    pt.setY(yy);
    return pt;
}



qreal QcItem::getAngle(const QPointF&pt, const QRectF &tmpRect)
{
    qreal xx=tmpRect.center().x()-pt.x();
    qreal yy=tmpRect.center().y()-pt.y();
    return qRadiansToDegrees( atan2(yy,xx));
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcScaleItem::QcScaleItem(QObject *parent) :
    QcItem(parent)
{
    mMinDegree = -45;
    mMaxDegree = 225;
    mMinValue = 0;
    mMaxValue = 100;
}

void QcScaleItem::setValueRange(qreal minValue, qreal maxValue)
{
    if(!(minValue<maxValue))
        throw( InvalidValueRange);
    mMinValue = minValue;
    mMaxValue = maxValue;

}

void QcScaleItem::setDegreeRange(qreal minDegree, qreal maxDegree)
{
    if(!(minDegree<maxDegree))
        throw( InvalidValueRange);
    mMinDegree = minDegree;
    mMaxDegree = maxDegree;
}

qreal QcScaleItem::getDegFromValue(qreal v)
{
    qreal a = (mMaxDegree-mMinDegree)/(mMaxValue-mMinValue);
    qreal b = -a*mMinValue+mMinDegree;
    return a*v+b;
}


void QcScaleItem::setMinValue(qreal minValue)
{
    if(minValue>mMaxValue)
        throw (InvalidValueRange);
    mMinValue = minValue;
    update();
}


void QcScaleItem::setMaxValue(qreal maxValue)
{
    if(maxValue<mMinValue )
        throw (InvalidValueRange);
    mMaxValue = maxValue;
    update();
}

void QcScaleItem::setMinDegree(qreal minDegree)
{
    if(minDegree>mMaxDegree)
        throw (InvalidDegreeRange);
    mMinDegree = minDegree;
    update();
}
void QcScaleItem::setMaxDegree(qreal maxDegree)
{
    if(maxDegree<mMinDegree)
        throw (InvalidDegreeRange);
    mMaxDegree = maxDegree;
    update();
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcBackgroundItem::QcBackgroundItem(QObject *parent) :
    QcItem(parent)
{
    setPosition(88);
    mPen = Qt::NoPen;
    setPosition(88);
    mDynamic = false;
    mCurrentValue = 0;

    addColor(0.4,Qt::darkGray);
    addColor(0.8,Qt::black);

}


void QcBackgroundItem::draw(QPainter* painter)
{

    QRectF tmpRect = resetRect();
    painter->setBrush(Qt::NoBrush);

    if(!mDynamic){
    QLinearGradient linearGrad(tmpRect.topLeft(), tmpRect.bottomRight());
    for(int i = 0;i<mColors.size();i++){
        linearGrad.setColorAt(mColors[i].first,mColors[i].second);
    }
    painter->setPen(mPen);
    painter->setBrush(linearGrad);
    painter->drawEllipse(adjustRect(position()));
    }
    else{
        if (mCurrentValue>mWarningHigh || mCurrentValue < mWarningLow){
            painter->setBrush(mWarningColorFill);


        }
        else {
            painter->setBrush(mRegularColorFill);
        }
        painter->setPen(mPen);
        painter->drawEllipse(adjustRect(position()));
    }


}

void QcBackgroundItem::addColor(qreal position, const QColor &color)
{
    if(position<0||position>1)
        return;
      QPair<qreal,QColor> pair;
      pair.first = position;
      pair.second = color;
      mColors.append(pair);
      update();
}

void QcBackgroundItem::clearrColors()
{
    mColors.clear();
}
void QcBackgroundItem::setWarningValues(qreal low, qreal high){
    mWarningLow=low;
    mWarningHigh = high;
}
void QcBackgroundItem::setDynamic(bool b){
    mCurrentValue = 0;
    mDynamic = b;
}
void QcBackgroundItem::setDynamicColors(const QColor &regColor,const QColor &warnColor){
    mColors.clear();
    mRegularColorFill = regColor;
    mWarningColorFill = warnColor;
    update();
}

void QcBackgroundItem::setCurrentValue(qreal value){
    mCurrentValue = value;
    update();
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcGlassItem::QcGlassItem(QObject *parent) :
    QcItem(parent)
{
    setPosition(88);
}

void QcGlassItem::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect1 = adjustRect(position());
    QRectF tmpRect2 = tmpRect1;
    qreal r = getRadius(tmpRect1);
    tmpRect2.setHeight(r/2.0);
    painter->setPen(Qt::NoPen);

    QColor clr1 = Qt::gray ;
    QColor clr2 = Qt::white;
    clr1.setAlphaF(0.2);
    clr2.setAlphaF(0.4);

    QLinearGradient linearGrad1(tmpRect1.topLeft(), tmpRect1.bottomRight());
    linearGrad1.setColorAt(0.1, clr1);
    linearGrad1.setColorAt(0.5, clr2);

    painter->setBrush(linearGrad1);
    painter->drawPie(tmpRect1,0,16*180);
    tmpRect2.moveCenter(rect().center());
    painter->drawPie(tmpRect2,0,-16*180);
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcLabelItem::QcLabelItem(QObject *parent) :
    QcItem(parent)
{
    setPosition(50);
    mAngle = 270;
    mText = "%";
    mColor = Qt::black;
    mFont = "Meiryo UI";
    mFontSize = 0.1;
}

void QcLabelItem::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect = adjustRect(position());
    qreal r = getRadius(rect());
    QFont font(mFont, r * mFontSize);
    painter->setFont(font);
    painter->setPen(QPen(mColor));

    QPointF txtCenter = getPoint(mAngle,tmpRect);
    QFontMetrics fMetrics = painter->fontMetrics();
    QSize sz = fMetrics.size(Qt::TextDontClip, mText );
    QRectF txtRect(QPointF(0,0), sz );
    txtRect.moveCenter(txtCenter);

    painter->drawText( txtRect, Qt::TextDontClip ,mText );

}

void QcLabelItem::setAngle(qreal a)
{
    mAngle = a;
    update();
}

qreal QcLabelItem::angle()
{
    return mAngle;
}

void QcLabelItem::setText(const QString &text, bool repaint)
{
    mText = text;
    if(repaint)
        update();
}

QString QcLabelItem::text()
{
    return mText;
}

void QcLabelItem::setColor(const QColor &color)
{
    mColor = color;
    update();
}

QColor QcLabelItem::color()
{
    return mColor;
}

void QcLabelItem::setFont(QString str){
    mFont = str;
    update();
}

void QcLabelItem::setFontSize(qreal size){
    mFontSize = size;
    update();
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcArcItem::QcArcItem(QObject *parent) :
    QcScaleItem(parent)
{

    setPosition(80);
    mColor = Qt::black;
    mWidth = 0.025;
}

void QcArcItem::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect= adjustRect(position());
    qreal r = getRadius(tmpRect);

    QPen pen;
    pen.setColor(mColor);
    pen.setWidthF(r*mWidth);
    painter->setPen(pen);
    painter->drawArc(tmpRect,-16*(mMinDegree+180),-16*(mMaxDegree-mMinDegree));
}

void QcArcItem::setColor(const QColor &color)
{
    mColor = color;
    update();
}
void QcArcItem::setWidth(qreal width){
    mWidth = width;
    update();
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcColorBand::QcColorBand(QObject *parent) :
    QcScaleItem(parent)
{
    QColor tmpColor;
    tmpColor.setAlphaF(0.1);
    QPair<QColor,qreal> pair;

    pair.first = Qt::green;
    pair.second = 10;
    mBandColors.append(pair);

    pair.first = Qt::darkGreen;
    pair.second = 50;
    mBandColors.append(pair);

    pair.first = Qt::red;
    pair.second = 100;
    mBandColors.append(pair);

    mBandWidth = 0.04;
    mdynamic = false;
    mCurrentValue = 0;
    mCoveringColor = Qt::darkGray;
    mOpacity = 0.9;


    setPosition(50);
}

QPainterPath QcColorBand::createSubBand(qreal from, qreal sweep)
{
    QRectF tmpRect = adjustRect(position());
    QPainterPath path;
    path.arcMoveTo(tmpRect,180+from);
    path.arcTo(tmpRect,180+from,-sweep);
    return path;
}

void QcColorBand::draw(QPainter *painter)
{
    resetRect();
    qreal r = getRadius(rect());
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidthF(r * mBandWidth);
    painter->setBrush(Qt::NoBrush);
    qreal offset = getDegFromValue(mBandStartValue);
    for(int i = 0;i<mBandColors.size();i++){
        QColor clr = mBandColors[i].first;
        qreal sweep;
        if(i==0)
            sweep = getDegFromValue(mBandColors[i].second)-getDegFromValue(mMinValue);
        else
            sweep = getDegFromValue(mBandColors[i].second)-getDegFromValue(mBandColors[i-1].second);


        QPainterPath path = createSubBand(-offset,sweep);
        offset += sweep;
        pen.setColor(clr);
        painter->setPen(pen);
        painter->drawPath(path);
    }
}
void QcColorBand::setColors(const QList<QPair<QColor, qreal> > &colors)
{
    mBandColors = colors;
    update();
}

void QcColorBand::setWidth(qreal width){
    mBandWidth = width;
    update();
}

void QcColorBand::setDynamic(bool b){
    mdynamic = b;
    if (b){
        mBandColors.clear();
        QPair<QColor,qreal> pair;
        pair.first = Qt::transparent;
        if (mCurrentValue < 100){
            pair.second = mCurrentValue; //value from 0 to 100 (percentage based)
        }
        else{
            pair.second = 100;
        }
        mBandColors.append(pair);
        QColor tmpColor = mCoveringColor;
        tmpColor.setAlphaF(mOpacity);
        pair.first = tmpColor;
        pair.second = 100;
        mBandColors.append(pair);

    }
    else{
        QColor tmpColor;
        tmpColor.setAlphaF(0.1);
        QPair<QColor,qreal> pair;

        pair.first = Qt::green;
        pair.second = 10;
        mBandColors.append(pair);

        pair.first = Qt::darkGreen;
        pair.second = 50;
        mBandColors.append(pair);

        pair.first = Qt::red;
        pair.second = 100;
        mBandColors.append(pair);

        mBandWidth = 0.04;
        mdynamic = false;
        mCurrentValue = 0;

    }
    update();
}

void QcColorBand::setCurrentValue(qreal value){ //takes percentages (NOT ACTUAL VALUES)
    mCurrentValue = value;
    setDynamic(true);
}

void QcColorBand::setCoveringColor(QColor c){
    mCoveringColor = c;
    setDynamic(true);
}
void QcColorBand::setOpacity(qreal value){
    if (value > 1){
        mOpacity = 1;
    }
    else if (value < 0){
        mOpacity = 0;
    }
    else{
        mOpacity = value;
    }
    setDynamic(true);
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcImage::QcImage(QObject * parent) : QcItem(parent)
{
    setPosition(15);
    mAngle = 270;
    mWidth = 0;
    mHeight = 0;
}

void QcImage::draw(QPainter* painter){
    resetRect();
    QRectF tmpRect = adjustRect(position());
    qreal r = getRadius(rect());
    QPointF txtCenter = getPoint(mAngle,tmpRect);
    QPixmap disp = mImage.scaledToWidth(r*mWidth);
    painter->setRenderHint(QPainter::SmoothPixmapTransform,true);



    painter->drawPixmap(txtCenter.x()-mWidth/2-r*mWidth/2,txtCenter.y()-mHeight/2-disp.height()/2,disp);

}

void QcImage::setImage(QString filename){
    mImage = QPixmap(filename);
    update();

}

void QcImage::scale(qreal w){
    mWidth = w/100;
    update();

}

void QcImage::setDim(qreal w, qreal h){ //percentage based
    mWidth = w/100;
    mHeight = h/100;
    update();
}
void QcImage::setAngle(qreal angle){
    mAngle = angle;
    update();
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcDynamicArcItem::QcDynamicArcItem(QObject *parent) : QcScaleItem(parent)
{
    setPosition(80);
    mColor = Qt::black;
    mWidth = 1;
    mPercentage = 0;
}

void QcDynamicArcItem::draw(QPainter *painter) {
    resetRect();
    QRectF tmpRect= adjustRect(position());
    QPen pen;
    qreal r = getRadius(tmpRect);

    qreal percentage = mPercentage < 0 ? 0 : mPercentage > 1 ? 1 : mPercentage;

    pen.setColor(calculateColor(percentage));
    pen.setWidthF(r * mWidth);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    painter->drawArc(tmpRect, static_cast<int>(-16 * (mMinDegree + 180)), static_cast<int>(-16 * (mMaxDegree - mMinDegree) * (percentage)));
}

void QcDynamicArcItem::setPercentage(const qreal &percent) {
    this->mPercentage = percent;
    update();
}

void QcDynamicArcItem::setColor(const QColor &color) {
    this->mColor = color;
    this->usingSingleColor = true;
    update();
}

void QcDynamicArcItem::setColor(QList< QPair <qreal, QColor> > colors) {
    this->mColors = colors;

    qStableSort(mColors.begin(), mColors.end(), [](QPair<qreal, QColor> a, QPair<qreal, QColor> b) {
        return a.first < b.first;
    });

    this->usingSingleColor = false;
    update();
}

void QcDynamicArcItem::setWidth(const qreal &width) {
    this->mWidth = width;
    update();
}

QColor QcDynamicArcItem::calculateColor(qreal percentage) {
    int size = static_cast<int>(mColors.size());
    if (usingSingleColor || mColors.size() == 0) return mColor;

    int foundIndex = -1;
    // find the index
    for (int i = size - 1; i >= 0; i--) {
        if (mColors[static_cast<ulong>(i)].first < percentage) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == size - 1) return mColors[static_cast<ulong>(size - 1)].second;
    if (foundIndex == -1) return mColors[0].second;
    if (foundIndex == 0 && size == 1) return mColors[0].second;
    // interpolate
    auto lower = mColors[static_cast<ulong>(foundIndex)];
    auto upper = mColors[static_cast<ulong>(foundIndex + 1)];

    int a1, b1, c1, a2, b2, c2;
    lower.second.getHsv(&a1, &b1, &c1);
    upper.second.getHsv(&a2, &b2, &c2);

    auto relativePercentage = (percentage - lower.first) / (upper.first - lower.first);

    int a3 = a1 + static_cast<int>(relativePercentage * static_cast<qreal>(a2 - a1));
    int b3 = b1 + static_cast<int>(relativePercentage * static_cast<qreal>(b2 - b1));
    int c3 = c1 + static_cast<int>(relativePercentage * static_cast<qreal>(c2 - c1));

    QColor final;
    final.setHsv(a3,b3,c3);
    return final;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


QcDegreesItem::QcDegreesItem(QObject *parent) :
    QcScaleItem(parent)
{
    mStep = 10;
    mColor = Qt::black;
    mLength = 0.1;
    mWidth = 0.04;
    mSubDegree = false;
    setPosition(90);


}


void QcDegreesItem::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect = adjustRect(position());


    painter->setPen(mColor);
    qreal r = getRadius(tmpRect);
    for(qreal val = mMinValue;val<=mMaxValue;val+=mStep){
        qreal deg = getDegFromValue(val);
        QPointF pt = getPoint(deg,tmpRect);
        QPainterPath path;
        path.moveTo(pt);
        path.lineTo(tmpRect.center());
        pt = path.pointAtPercent(0);
        QPointF newPt = path.pointAtPercent(mLength);

        QPen pen;
        pen.setColor(mColor);
        if(!mSubDegree)
            pen.setWidthF(r * mWidth);

        painter->setPen(pen);
        painter->drawLine(pt,newPt);

    }
}

void QcDegreesItem::setStep(qreal step)
{
    mStep = step;
    update();
}

void QcDegreesItem::setColor(const QColor& color)
{
    mColor = color;
    update();
}

void QcDegreesItem::setSubDegree(bool b)
{
    mSubDegree = b;
    update();
}

void QcDegreesItem::setLength(qreal length){
    mLength = length;
    update();
}

void QcDegreesItem::setWidth(qreal width){
    if(mWidth > 1){
        mWidth = 0.04;
    }
    else if(mWidth < 0){
        mWidth = 0.04;
    }
    else{
        mWidth = width;
    }
    update();
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcNeedleItem::QcNeedleItem(QObject *parent) :
    QcScaleItem(parent)
{
    mCurrentValue = 0;
    mPrecision = 2;
    mColor = Qt::black;
    mLabel = NULL;
    mNeedleType = FeatherNeedle;
}

void QcNeedleItem::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect = adjustRect(position());
    painter->save();
    painter->translate(tmpRect.center());
    qreal deg = getDegFromValue( mCurrentValue);
    painter->rotate(deg+90.0);
    painter->setBrush(QBrush(mColor));
    painter->setPen(Qt::NoPen);

    QLinearGradient grad;

    switch (mNeedleType) {
    case QcNeedleItem::FeatherNeedle:
        createFeatherNeedle(getRadius(tmpRect));
        break;
    case QcNeedleItem::DiamonNeedle:
        createDiamonNeedle(getRadius(tmpRect));
        break;
    case QcNeedleItem::TriangleNeedle:
        createTriangleNeedle(getRadius(tmpRect));
        break;
    case QcNeedleItem::AttitudeMeterNeedle:
        createAttitudeNeedle(getRadius(tmpRect));
        break;
    case QcNeedleItem::CompassNeedle:
        createCompassNeedle(getRadius(tmpRect));
        grad.setStart(mNeedlePoly[0]);
        grad.setFinalStop(mNeedlePoly[1]);
        grad.setColorAt(0.9,Qt::red);
        grad.setColorAt(1,Qt::blue);
        painter->setBrush(grad);

        break;

    default:
        break;
    }
    painter->drawConvexPolygon(mNeedlePoly);
    painter->restore();
}

void QcNeedleItem::setCurrentValue(qreal value) //takes actual number values (NOT PERCENTAGES)
{
       if(value<mMinValue)
        mCurrentValue = mMinValue;
    else if(value>mMaxValue)
        mCurrentValue = mMaxValue;
    else
        mCurrentValue = value;


    if(mLabel!=0){
        QString tempTxt;
        if (mCurrentValue > qPow(10,mPrecision)){
            tempTxt = QString::number(static_cast<int>(mCurrentValue));
            mLabel->setText(tempTxt,false);
        }
        else if (mCurrentValue > 1){
            tempTxt = QString::number(static_cast<qreal>(static_cast<int>(qPow(10,mPrecision - floor(qLn(mCurrentValue)/qLn(10)))*mCurrentValue))/qPow(10,mPrecision - floor(qLn(mCurrentValue)/qLn(10))));

            mLabel->setText(tempTxt,false);
        }
        else {
            tempTxt = QString::number(static_cast<qreal>(static_cast<int>(qPow(10,mPrecision)*mCurrentValue))/qPow(10,mPrecision));
            mLabel->setText(tempTxt,false);
        }

        tempTxt.clear();


    }
/// This pull request is not working properly
//    if(mLabel!=0){
//        QString currentValue;
//        mLabel->setText( currentValue ,false);
//        mLabel->setText(currentValue.sprintf(mFormat.toStdString().c_str(), mCurrentValue),false);
//        Q_UNUSED(currentValue);
//    }
    update();
}

void QcNeedleItem::setPrecision(qreal value){
    mPrecision = value;
    update();
}

qreal QcNeedleItem::currentValue()
{
    return mCurrentValue;
}

void QcNeedleItem::setValueFormat(QString format){
    mFormat = format;
    update();
}

QString QcNeedleItem::currentValueFormat(){
    return mFormat;
}

void QcNeedleItem::setColor(const QColor &color)
{
    mColor = color;
    update();
}

QColor QcNeedleItem::color()
{
    return mColor;
}

void QcNeedleItem::setLabel(QcLabelItem *label)
{
    mLabel = label;
    update();
}

QcLabelItem *QcNeedleItem::label()
{
    return mLabel;
}


void QcNeedleItem::setNeedle(QcNeedleItem::NeedleType needleType)
{
    mNeedleType = needleType;
    update();
}


void QcNeedleItem::createDiamonNeedle(qreal r)
{
    QVector<QPointF> tmpPoints;
    tmpPoints.append(QPointF(0.0, 0.0));
    tmpPoints.append(QPointF(-r/20.0,r/20.0));
    tmpPoints.append(QPointF(0.0, r));
    tmpPoints.append(QPointF(r/20.0,r/20.0));
    mNeedlePoly = tmpPoints;
}

void QcNeedleItem::createTriangleNeedle(qreal r)
{
    QVector<QPointF> tmpPoints;
    tmpPoints.append(QPointF(0.0, r));
    tmpPoints.append(QPointF(-r/40.0, 0.0));
    tmpPoints.append(QPointF(r/40.0,0.0));
    mNeedlePoly = tmpPoints;
}

void QcNeedleItem::createFeatherNeedle(qreal r)
{
    QVector<QPointF> tmpPoints;
    tmpPoints.append(QPointF(0.0, r));
    tmpPoints.append(QPointF(-r/40.0, 0.0));
    tmpPoints.append(QPointF(-r/15.0, -r/5.0));
    tmpPoints.append(QPointF(r/15.0,-r/5));
    tmpPoints.append(QPointF(r/40.0,0.0));
    mNeedlePoly = tmpPoints;
}

void QcNeedleItem::createAttitudeNeedle(qreal r)
{
    QVector<QPointF> tmpPoints;
    tmpPoints.append(QPointF(0.0, r));
    tmpPoints.append(QPointF(-r/20.0, 0.85*r));
    tmpPoints.append(QPointF(r/20.0,0.85*r));
    mNeedlePoly = tmpPoints;
}

void QcNeedleItem::createCompassNeedle(qreal r)
{
    QVector<QPointF> tmpPoints;
    tmpPoints.append(QPointF(0.0, r));
    tmpPoints.append(QPointF(-r/15.0, 0.0));
    tmpPoints.append(QPointF(0.0, -r));
    tmpPoints.append(QPointF(r/15.0,0.0));
    mNeedlePoly = tmpPoints;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcValuesItem::QcValuesItem(QObject *parent) :
    QcScaleItem(parent)
{
    setPosition(70);
    mColor = Qt::black;
    mStep = 10;
    mfont = "Meiryo UI";
    mFontSize = 0.08;
    mDynamic = false;
    QColor c = Qt::lightGray;
    mColorLit = c;
    mColorUnlit = c.darker();

}


void QcValuesItem::draw(QPainter*painter)
{
    QRectF  tmpRect = resetRect();
    qreal r = getRadius(adjustRect(99));
    QFont font(mfont,0);
    font.setPointSizeF(mFontSize*r);

    painter->setFont(font);
    painter->setPen(mColorLit);
    if (not mDynamic){
    for(qreal val = mMinValue;val<=mMaxValue;val+=mStep){
        qreal deg = getDegFromValue(val);
        QPointF pt = getPoint(deg,tmpRect);
        QPainterPath path;
        path.moveTo(pt);
        path.lineTo(    tmpRect.center());
        QString strVal = QString::number(val);
        QFontMetrics fMetrics = painter->fontMetrics();
        QSize sz = fMetrics.size( Qt::TextSingleLine, strVal );
        QRectF txtRect(QPointF(0,0), sz );
        QPointF textCenter = path.pointAtPercent(1.0-position()/100.0);
        txtRect.moveCenter(textCenter);

        painter->drawText( txtRect, Qt::TextSingleLine, strVal );
    }
    }

    else{
        painter->setPen(mColorLit);
        for(qreal val = mMinValue;val<=mCurrentValue+0.0001;val+=mStep){
            qreal deg = getDegFromValue(val);
            QPointF pt = getPoint(deg,tmpRect);
            QPainterPath path;
            path.moveTo(pt);
            path.lineTo(    tmpRect.center());
            QString strVal = QString::number(val);
            QFontMetrics fMetrics = painter->fontMetrics();
            QSize sz = fMetrics.size( Qt::TextSingleLine, strVal );
            QRectF txtRect(QPointF(0,0), sz );
            QPointF textCenter = path.pointAtPercent(1.0-position()/100.0);
            txtRect.moveCenter(textCenter);

            painter->drawText( txtRect, Qt::TextSingleLine, strVal );
        }
        painter->setPen(mColorUnlit);
        qreal newStartValue = (static_cast<qreal>(static_cast<int>(100*mCurrentValue))/100) + (mStep - static_cast<qreal>(static_cast<int>(100*(mCurrentValue)) % static_cast<int>(100*mStep))/100);

        for(qreal val = newStartValue;val<=mMaxValue+0.001;val+=mStep){

            qreal deg = getDegFromValue(val);
            QPointF pt = getPoint(deg,tmpRect);
            QPainterPath path;
            path.moveTo(pt);
            path.lineTo(    tmpRect.center());
            QString strVal = QString::number(val);
            QFontMetrics fMetrics = painter->fontMetrics();
            QSize sz = fMetrics.size( Qt::TextSingleLine, strVal );
            QRectF txtRect(QPointF(0,0), sz );
            QPointF textCenter = path.pointAtPercent(1.0-position()/100.0);
            txtRect.moveCenter(textCenter);

            painter->drawText( txtRect, Qt::TextSingleLine, strVal );
        }


    }
}

void QcValuesItem::setStep(qreal step)
{
    mStep = step;
}


void QcValuesItem::setColor(const QColor& color)
{
    mColor = color;
}

void QcValuesItem::setFont(QString font){
    mfont = font;
}
void QcValuesItem::setFontSize(qreal value){
    if (value > 1){
        mFontSize = 1;
    }
    else if (value < 0){
        mFontSize = 0;
    }
    else {
        mFontSize = value;
    }

}

void QcValuesItem::setDynamic(bool b){
    mDynamic = b;

    update();
}

void QcValuesItem::setColorLit(const QColor &color){
    mColorLit = color;
    update();
}

void QcValuesItem::setColorUnlit(const QColor &color){
    mColorUnlit = color;
    update();
}
void QcValuesItem::setCurrentValue(qreal value){ //takes actual number values (NOT PERCENTAGES)
    if (value > mMaxValue){
        mCurrentValue = mMaxValue;
    }
    else if (value < mMinValue){
        mCurrentValue = mMinValue;
    }
    else{
        mCurrentValue = value;
    }

    update();
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcAttitudeMeter::QcAttitudeMeter(QObject *parent) :
    QcItem(parent)
{
    mPitch = 0;
    mRoll = 0;
}

void QcAttitudeMeter::setCurrentPitch(qreal pitch)
{
    mPitch=-pitch;
    update();
}

void QcAttitudeMeter::setCurrentRoll(qreal roll)
{
    mRoll = roll;
    update();
}

QPointF QcAttitudeMeter::getIntersection(qreal r, const QPointF &pitchPoint, const QPointF &pt)
{
    // refrence it to zero

    Q_UNUSED(r)
    qreal a = (pitchPoint.y()-pt.y())/(pitchPoint.x()-pt.x());
    qreal b = pt.y()-a*pt.x();
    return QPointF(0,a*0+b);
}

qreal QcAttitudeMeter::getStartAngle(const QRectF& tmpRect)
{
    qreal r = getRadius(tmpRect);
    QPointF pt1 = getPoint(mRoll,tmpRect);
    pt1.setY(pt1.y()-mPitchOffset);
    QPointF pitchPoint = QPointF(tmpRect.center().x(),tmpRect.center().y()-mPitchOffset);


    ///////////////////////////////////////
    QPainterPath path1;
    path1.moveTo(pitchPoint);
    path1.lineTo(getIntersection(r,pitchPoint,pt1)+QPointF(0,5));
    path1.lineTo(getIntersection(r,pitchPoint,pt1)+QPointF(0,-5));

    QPainterPath path2;
    path2.addEllipse(tmpRect);

    QPointF p = path1.intersected(path2).pointAtPercent(.5);
    return getAngle(p,tmpRect);
}

void QcAttitudeMeter::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect = adjustRect(position());
    qreal r = getRadius(tmpRect);
    if(mPitch<0)
        mPitchOffset = 0.0135*r*mPitch;
    else
        mPitchOffset = 0.015*r*mPitch;

    painter->setPen(Qt::NoPen);
    drawUpperEllipse(painter,tmpRect);
    drawLowerEllipse(painter,tmpRect);

    // Steps

    drawPitchSteps(painter,tmpRect);
    drawHandle(painter);

    drawDegrees(painter);

}

void QcAttitudeMeter::drawDegrees(QPainter *painter)
{
    resetRect();
    QRectF tmpRect = adjustRect(position());
    qreal r = getRadius(tmpRect);
    QPen pen;

    pen.setColor(Qt::white);
    painter->setPen(pen);
    for(int deg = 60;deg<=120;deg+=10){
        if(deg == 90)
            continue;
        drawDegree(painter,tmpRect,deg);
    }

    pen.setWidthF(r/30.0);
    painter->setPen(pen);
    drawDegree(painter,tmpRect,0);
    drawDegree(painter,tmpRect,90);
    drawDegree(painter,tmpRect,180);
    drawDegree(painter,tmpRect,30);
    drawDegree(painter,tmpRect,150);
}


void QcAttitudeMeter::drawDegree(QPainter * painter, const QRectF& tmpRect,qreal deg)
{
    QPointF pt1 = getPoint(deg,tmpRect);
    QPointF pt2 = tmpRect.center();
    QPainterPath path;
    path.moveTo(pt1);
    path.lineTo(pt2);
    QPointF pt = path.pointAtPercent(0.1);
    painter->drawLine(pt1,pt);
}


void QcAttitudeMeter::drawUpperEllipse(QPainter *painter, const QRectF &tmpRect)
{

    QLinearGradient radialGrad1(tmpRect.topLeft(),tmpRect.bottomRight());
    QColor clr1 = Qt::blue;
    clr1.setAlphaF(0.5);
    QColor clr2 = Qt::darkBlue;
    clr2.setAlphaF(0.5);
    radialGrad1.setColorAt(0, clr1);
    radialGrad1.setColorAt(.8, clr2);


    qreal offset = getStartAngle(tmpRect);
    qreal startAngle = 180-offset;
    qreal endAngle = offset-2*mRoll;
    qreal span =endAngle-startAngle;

    painter->setBrush(radialGrad1);
    painter->drawChord(tmpRect,16*startAngle,16*span);

}


void QcAttitudeMeter::drawLowerEllipse(QPainter *painter, const QRectF &tmpRect)
{
    QLinearGradient radialGrad2(tmpRect.topLeft(),tmpRect.bottomRight());
    QColor clr1 = QColor(139,119,118);
    QColor clr2 = QColor(139,119,101);
    radialGrad2.setColorAt(0, clr1);
    radialGrad2.setColorAt(.8, clr2);

    qreal offset = getStartAngle(tmpRect);
    qreal startAngle = 180+offset;
    qreal endAngle = offset-2*mRoll;
    qreal span =endAngle+startAngle;

    painter->setPen(Qt::NoPen);
    painter->setBrush(radialGrad2);
    painter->drawChord(tmpRect,-16*startAngle,16*span);

}

void QcAttitudeMeter::drawPitchSteps(QPainter *painter, const QRectF &tmpRect)
{
    qreal r = getRadius(tmpRect);
    QPointF center = tmpRect.center();
    painter->save();
    painter->translate(center.x(),center.y()-mPitchOffset);
    painter->rotate(mRoll);
    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidthF(r/40.0);

    painter->setPen(pen);
    for (int i = -30;i<=30;i+=10){
        QPointF pt1;
        pt1.setX(-0.01*r*abs(i));
        pt1.setY(r/70.0*i);
        QPointF pt2;
        pt2.setX(0.01*r*abs(i));
        pt2.setY(r/70.0*i);
        painter->drawLine(pt1,pt2);

        if(i==0)
            continue;

        // draw value
        QFont font("Meiryo UI",0, QFont::Bold);
        font.setPointSizeF(0.08*r);
        painter->setFont(font);
        QString strVal = QString::number(abs(i));
        QFontMetrics fMetrics = painter->fontMetrics();
        QSize sz = fMetrics.size( Qt::TextSingleLine, strVal );
        QRectF leftTxtRect(QPointF(0,0), sz );
        QRectF rightTxtRect(QPointF(0,0), sz );
        leftTxtRect.moveCenter(pt1-QPointF(0.1*r,0));
        rightTxtRect.moveCenter(pt2+QPointF(0.1*r,0));
        painter->drawText( leftTxtRect, Qt::TextSingleLine, strVal );
        painter->drawText( rightTxtRect, Qt::TextSingleLine, strVal );
    }
    painter->restore();
}

void QcAttitudeMeter::drawHandle(QPainter *painter)
{
    QRectF tmpRct = adjustRect(15);
    qreal r = getRadius(tmpRct);
    QPen pen;
    pen.setColor(Qt::gray);
    pen.setWidthF(0.25*r);
    painter->setPen(pen);
    painter->drawArc(tmpRct,0,-16*180);

    QPointF center = tmpRct.center();
    QPointF leftPt1 = center;
    QPointF leftPt2 = center;
    QPointF rightPt1 = center;
    QPointF rightPt2 = center;
    leftPt1.setX(center.x()-2*r);
    leftPt2.setX(center.x()-r);
    rightPt1.setX(center.x()+2*r);
    rightPt2.setX(center.x()+r);
    painter->drawLine(leftPt1,leftPt2);
    painter->drawLine(rightPt1,rightPt2);
    painter->drawEllipse(adjustRect(2));

    //
    QPointF pt1 = center;
    QPointF pt2 = center;
    // to get the real 100 % radius, without recomputing
    pt1.setY(center.y()+r);
    pt2.setY(center.y()+4*r);
    pen.setColor(Qt::gray);
    painter->setPen(pen);
    painter->drawLine(pt1,pt2);

    // trapezium
    painter->setPen(Qt::gray);
    painter->setBrush(Qt::gray);
    QPolygonF trapPoly;
    QPointF tmpPt = center;
    tmpPt.setX(center.x()-r);
    tmpPt.setY(center.y()+4*r);
    trapPoly.append(tmpPt);
    tmpRct = adjustRect(position());
    trapPoly.append(getPoint(290,tmpRct));
    trapPoly.append(getPoint(250,tmpRct));
    tmpPt = center;
    tmpPt.setX(center.x()+r);
    tmpPt.setY(center.y()+4*r);
    trapPoly.append(tmpPt);
    painter->drawPolygon(trapPoly);
    painter->drawChord(tmpRct,-16*70,-16*40);
}

