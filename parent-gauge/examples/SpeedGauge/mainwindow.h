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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../source/qcgaugewidget.h"
#include "../../source/qcthemeitem.h"
#include "../../source/waterLoopGaugeItem.h"
#include "../../source/mytcp.h"
#include "../../source/wlooptcp.h"
#include "../../source/wloopsocket.h"
#include <QDebug>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QTextStream>
#include <QIODevice>
#include <QPixmap>
#include <QLabel>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QGridLayout>
#include <QStackedWidget>
#include <QBitmap>
#include <QVector>
#include <QTime>

#define read_length 0
#define read_buffer 1
#define buffer_size 2048
#define packet_size 1024
#define std_add "1.0.0.1"
#define std_port 0


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void sendCommand();


private slots:
    void closeWindow();

    void readUpdate(QJsonDocument &d);
    void on_horizontalSlider_valueChanged(int value);

    void updatePodHealthGOOD();
    void updatePodHealthBAD();

    void moveLoadingGauge();
    void loadInitializer();
    void fadeOutInitializer();
    void loadConnectionScreen();
    void attemptConnection();
    void fadeOutConnectionScreen();
    void initializeConnection();
    void moveGauge();
    void loadMainScreen();
    void updateTimeDisplay();
    void readTCPData();


private:
    void fadeOut(QWidget * w, const char* mem, int msec = 500);
    void fadeIn(QWidget *w, int msec = 500);
    void updateApp(QJsonDocument &d);


    int travelDistance = 300;
    QTimer * timerLoad;
    Ui::MainWindow *ui;
    waterLoopGaugeItem * speedoMeter;
    waterLoopGaugeItem * dlim;
    waterLoopGaugeItem * highV;
    waterLoopGaugeItem * highC;
    waterLoopGaugeItem * lowV;
    waterLoopGaugeItem * lowC;
    waterLoopGaugeItem * loader;
    int maxSpeed;
    QString tcpaddress;
    unsigned int tcpport;


    WLoopSocket * tcp;

    QThread * timeThread;
    QTimer * timeDisplay;


    QByteArray stream;
    QByteArray buffer;
    qint64 bytesLeft=0;
    qint64 bytesRead;
    QJsonDocument data;
    qreal loadpos = 0;





};

#endif // MAINWINDOW_H
