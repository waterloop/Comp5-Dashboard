#include "waterLoopGaugeItem.h"

waterLoopGaugeItem::waterLoopGaugeItem()
{
    mainGauge = new QcGaugeWidget;
    QcThemeItem theme = QcThemeItem();

}

waterLoopGaugeItem::waterLoopGaugeItem(QcThemeItem &theme, qreal size,QString type, QString label,QString units ,qreal precision, qreal startValue, qreal maxValue, qreal warningValueHigh, qreal warningValueLow, qreal stepSize){
    mainGauge = new QcGaugeWidget;
    mainGauge->setMaximumSize(size,size);
    mainGauge->setMinimumSize(size,size);
    this->maxValue = maxValue;
    this->startValue = startValue;
    this->stepSize = stepSize;
    this->warningValueLow = warningValueLow;
    this->warningValueHigh = warningValueHigh;
    if (theme.outerRingRadius>0){
        QcBackgroundItem *bkg1 = mainGauge->addBackground(theme.outerRingRadius);
        bkg1->clearrColors();
        bkg1->addColor(1.0, theme.mainColor);
    }
    if (theme.mainBackgroundRadius > 0){
        mainBackground= mainGauge->addBackground(theme.mainBackgroundRadius);
        mainBackground->clearrColors();
        mainBackground->addColor(1.0, theme.backgroundColor);
        mainBackground->setDynamic(true);
        mainBackground->setWarningValues(warningValueLow, warningValueHigh);
        mainBackground->setDynamicColors(theme.backgroundColor ,theme.warningColor);
    }
    if (theme.arcRadius>0){
        QcArcItem * arc =  mainGauge->addArc(theme.arcRadius);
        arc->setWidth(theme.arcWidth);
        arc->setColor(theme.mainColor);
        arc->setDegreeRange(theme.minDegree,theme.maxDegree);
    }
    if (theme.mainDegreesRadius>0){
        QcDegreesItem * majDeg = mainGauge->addDegrees(theme.mainDegreesRadius);
        majDeg->setStep(stepSize);
        majDeg->setValueRange(startValue,maxValue);
        majDeg->setColor(theme.mainColor);
        majDeg->setWidth(theme.mainDegreesWidth);
        majDeg->setLength(theme.mainDegreesLength);
        majDeg->setDegreeRange(theme.minDegree,theme.maxDegree);
    }
    if (theme.subDegreesRadius>0){
        QcDegreesItem * deg = mainGauge->addDegrees(theme.subDegreesRadius);
        deg->setStep(stepSize/10.0);
        deg->setValueRange(startValue,maxValue);
        deg->setWidth(theme.subDegreesWidth);
        deg->setLength(theme.subDegreesLength);
        QColor subColor = theme.mainColor;
        subColor.setAlphaF(theme.subDegreesAlpha);
        deg->setColor(subColor);
        deg->setDegreeRange(theme.minDegree,theme.maxDegree);
    }
    QColor tmpColor;

    QPair<QColor,qreal> pair;
    QList<QPair<QColor,qreal>> lst;

    pair.first = QColor(theme.highWarningColor);
    pair.second = 100* (warningValueLow-startValue)/(maxValue-startValue);
    lst.append(pair);

    pair.first = QColor(theme.noWarningColor);
    pair.second = 100 * (warningValueHigh-startValue)/(maxValue-startValue);
    lst.append(pair);

    pair.first = QColor(theme.highWarningColor);
    pair.second = 100;
    lst.append(pair);

    if (theme.colorBandRadius>0){
        QcColorBand * cb = mainGauge->addColorBand(theme.colorBandRadius);
        cb->setWidth(theme.colorBandWidth);
        cb->setColors(lst);
        cb->setDegreeRange(theme.minDegree,theme.maxDegree);
    }
    if (theme.colorBandRadius>0){
        mDynamicColorBand = mainGauge->addColorBand(theme.colorBandRadius);
        mDynamicColorBand->setWidth(theme.colorBandWidth + 0.01);
        mDynamicColorBand->setDynamic(true);
        mDynamicColorBand->setCoveringColor(theme.backgroundColor);
        if (theme.colorBandDynamic){
            mDynamicColorBand->setOpacity(0.75);
        }
        else {
            mDynamicColorBand->setOpacity(0);
        }

        mDynamicColorBand->setDegreeRange(theme.minDegree,theme.maxDegree);
    }

    if (theme.dynamicColorArcRadius>0){
        positionIndicatior = mainGauge->addDynamicArc(theme.dynamicColorArcRadius);
        positionIndicatior->setWidth(theme.dynamicColorArcWidth);

        QList<QPair<qreal,QColor>> lst;
        QPair<qreal, QColor> pair;
        pair.second = QColor(theme.highWarningColor);
        pair.first = 0;
        lst.append(pair);

        pair.second = QColor(theme.highWarningColor);
        pair.first = (warningValueLow-startValue)/(maxValue-startValue)-0.05;
        if (pair.first<0){
            pair.first = 0;
        }
        lst.append(pair);
        if (pair.first == 0){
            pair.first = 0;
        }
        else {
            pair.first = (warningValueLow-startValue)/(maxValue-startValue)+0.05;
        }
        pair.second = QColor(theme.noWarningColor);

        lst.append(pair);

        pair.second = QColor(theme.noWarningColor);
        pair.first = (warningValueHigh-startValue)/(maxValue-startValue)-0.05;
        lst.append(pair);


        pair.second = (theme.highWarningColor);
        pair.first = (warningValueHigh-startValue)/(maxValue-startValue)+0.05;
        if (pair.first>1){
            pair.first = 1;
            pair.second = QColor(theme.noWarningColor);
        }
        lst.append(pair);
        if (pair.first !=1){
            pair.second = QColor(theme.highWarningColor);
            pair.first = 1;
            lst.append(pair);
        }


        if (theme.dynamicColorArcCust){
            positionIndicatior->setColor(theme.dynamicColorArc);
        }
        else{
            positionIndicatior->setColor(lst);
        }

        positionIndicatior->setDegreeRange(theme.minDegree,theme.maxDegree);
    }

    if (theme.mainDegreesRadius>2 ){
        mDynamicColorBandDegrees = mainGauge->addColorBand(theme.mainDegreesRadius - 2);
        mDynamicColorBandDegrees->setWidth(theme.colorBandWidth);
        mDynamicColorBandDegrees->setDynamic(true);
        mDynamicColorBandDegrees->setCoveringColor(theme.backgroundColor);
        mDynamicColorBandDegrees->setOpacity(0.5);
        mDynamicColorBandDegrees->setDegreeRange(theme.minDegree,theme.maxDegree);
    }

    if (theme.valuesRadius>0){
        lightUpValues = mainGauge->addValues(theme.valuesRadius);
        lightUpValues->setValueRange(startValue,maxValue);
        lightUpValues->setCurrentValue(startValue);
        lightUpValues->setFont(theme.font);
        lightUpValues->setFontSize(theme.fontSizeValues);
        lightUpValues->setDynamic(theme.dynamicValues);
        lightUpValues->setStep(stepSize);
        lightUpValues->setColorLit(theme.mainColor);
        lightUpValues->setColorUnlit(theme.mainColor.darker());
        lightUpValues->setDegreeRange(theme.minDegree,theme.maxDegree);
    }

    if (theme.needleRadius>0){

        mSpeedNeedle = mainGauge->addNeedle(theme.needleRadius);

        if (theme.needleVisible){
            mSpeedNeedle->setColor(theme.mainColor);
        }
        else{
            mSpeedNeedle->setColor(Qt::transparent);
        }
        mSpeedNeedle->setValueRange(startValue,maxValue);
        mSpeedNeedle->setCurrentValue(startValue);
        mSpeedNeedle->setDegreeRange(theme.minDegree, theme.maxDegree);
        needleCover = mainGauge->addBackground(theme.needleCoverRadius);
        needleCover->clearrColors();
        needleCover->addColor(1.0, theme.backgroundColor);
        needleCover->setDynamic(true);
        needleCover->setWarningValues(warningValueLow,warningValueHigh);
        needleCover->setDynamicColors(theme.backgroundColor, theme.warningColor);
        QcLabelItem *lab = mainGauge->addLabel(0);
        lab->setText(QString::number(startValue));
        lab->setFont(theme.font);
        lab->setPosition(theme.needleLabelPosition);
        lab->setAngle(theme.needleLabelAngle);
        lab->setFontSize(theme.fontSizeNeedleLabel);
        lab->setColor(theme.mainColor);
        mSpeedNeedle->setLabel(lab);
        mSpeedNeedle->setPrecision(precision);
    }

    if (theme.unitsLocation>0){
    QcLabelItem * kmh = mainGauge->addLabel(theme.unitsLocation);
    kmh->setText(units);
    kmh->setColor(theme.mainColor);
    kmh->setFont(theme.font);
    kmh->setFontSize(theme.fontSizeUnits);
    }
    if (theme.labelLocation>0){
    QcLabelItem *descriptor = mainGauge->addLabel(theme.labelLocation);
    descriptor->setText(label);
    if (theme.labelFont!=""){
        descriptor->setFont(theme.labelFont);
    }
    else{
        descriptor->setFont(theme.font);
    }
    descriptor->setColor(theme.mainColor);
    descriptor->setFontSize(theme.fontSizeLabel);
    descriptor->setAngle(theme.labelAngle);
    }
    if (theme.dropShadow){
        dropShadow = new QGraphicsDropShadowEffect(mainGauge);
        dropShadow->setBlurRadius(theme.dropShadowRadius);
        dropShadow->setColor(theme.dropShadowColor);
        dropShadow->setOffset(QPointF(theme.dropShadowHorOffset,theme.dropShadowVertOffset));
        mainGauge->setGraphicsEffect(dropShadow);
    }

}


QcGaugeWidget * waterLoopGaugeItem::getGauge(){
    return this->mainGauge;
}

qint8 waterLoopGaugeItem::getState(){
    return this->state;
}

qreal waterLoopGaugeItem::getCurrentValue(){
    return this->currentValue;
}

void waterLoopGaugeItem::setCurrentValue(qreal value){
    currentValue = (startValue + (qreal) value / 99 * (maxValue-startValue));//taking values 0-99
    state = currentValue >= warningValueLow ? (currentValue>= warningValueHigh ? 2 : 1) : 0;

    if (mSpeedNeedle!=nullptr){
        mSpeedNeedle->setCurrentValue(currentValue); //needs actual value
    }
    if (mDynamicColorBandDegrees!=nullptr){
        mDynamicColorBandDegrees->setCurrentValue( ((qreal) value / 99 *100));
    }
    if (mDynamicColorBand!=nullptr){
        mDynamicColorBand->setCurrentValue( ((qreal) value / 99 *100)); //needs percentages

    }
    if (mainBackground!= nullptr){
        mainBackground->setCurrentValue(currentValue);
    }
    if (needleCover!=nullptr){
        needleCover->setCurrentValue(currentValue);
    }
    if (lightUpValues!=nullptr){
        lightUpValues->setCurrentValue(currentValue);
    }
    if (positionIndicatior!=nullptr){
        positionIndicatior->setPercentage(((qreal) value / 99));
    }

}
