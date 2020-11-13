#ifndef WLOOPSENSOR_H
#define WLOOPSENSOR_H

#include <QPair>
#include <QVector>
#include <QWidget>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTableWidget>
#include <QtMath>

enum sensorState {safe, unsafe, uninitialized};

class wloopSensor : public QObject
{ Q_OBJECT

public:

    wloopSensor(QJsonObject obj);
    void changeValue(qreal newVal);
    void changeValueSlider(qreal newVal);
    void setValItem(QTableWidgetItem * item);
    void setNameItem(QTableWidgetItem * item);
    QTableWidgetItem * getValItem();
    QTableWidgetItem * getNameItem();
    qreal getValue();
    QString getName();
    QString getUnits();
    enum sensorState getState();
    ~wloopSensor(){
        delete valItem;
        delete nameItem;
    }

signals:

    void valueChanged(QTableWidgetItem * item, qreal val, QString units);
    void stateChanged(QTableWidgetItem * item, sensorState state);

private:

    QPair<qreal,qreal> accRange, maxRange; //acceptableRange and maxRange for the sensors
    QString name, units;
    qreal value = 0;
    qint16 id;
    enum sensorState state = uninitialized;
    int valid = 1;
    QTableWidgetItem * valItem= nullptr, * nameItem = nullptr;




};

QVector<wloopSensor *> getSensorArray(QJsonDocument doc);

#endif // WLOOPSENSOR_H
