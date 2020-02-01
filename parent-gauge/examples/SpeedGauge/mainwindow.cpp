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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include "../../source/mytcp.h"
#include <QJsonDocument>
#include <QFile>


MainWindow::MainWindow(QWidget *parent, QString host, qint16 port) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //SETTING UP ALL COMPONENTS
    ui->setupUi(this);
    QcThemeItem mainTheme = QcThemeItem(":/styles/waterLoopThemeRETRO.txt");
    QcThemeItem newTheme = QcThemeItem(":/styles/waterLoopThemeNEW.txt");
    QcThemeItem midTheme = QcThemeItem(":/styles/waterLoopThemeMID.txt");

    //speedoMeter = new waterLoopGaugeItem(mainTheme, 250,"Speedometer","SPEED","Km/h",1, 400,300,200,50);
    baroMeter = new waterLoopGaugeItem(newTheme, 250, "Current", "LV CURR." ,"A", 1, 0, 60, 50, 0, 5);

    //ADDING COMPONENTS

    //voltMeter = new waterLoopGaugeItem(newTheme, 250, "Voltmeter", "", "V",2 ,1, 0.5, 0.25, 0.1);
    //ui->verticalLayout->addWidget(speedoMeter->getGauge());
    ui->verticalLayout->addWidget(baroMeter->getGauge());


    /////////GARICK THIS IS FOR YOU BABY


    loadConfigW(":/config/waterloop_desktop_config.json");




    //////////////////DON'T TOUCH UNDER HERE

    //SETTING UP CONNECTION
    tcpsocket = new QTcpSocket(this);
    //connect( tcpsocket, SIGNAL( readyRead()), this, SLOT(readTCPData()));
    //connect( tcpsocket, SIGNAL( disconnected() ), this , SLOT(disconnectTCP()));
    tcpsocket->connectToHost(host, port);
    tcpaddress = host;

    //STARTING CONNECTION AND ENTERING RUNNING LOOP

    if (tcpsocket->waitForConnected(2000)) {
        qDebug() << "Connected to Host";
        while (true){ //running loop
            if (tcpsocket->waitForReadyRead(3000)){
                bytesRead = tcpsocket->bytesAvailable();
                buffer = tcpsocket->readAll();
                while (bytesRead>0){
                    if (bytesLeft > 0 && bytesRead < bytesLeft){
                        stream.append(buffer);
                        bytesLeft-=bytesRead;
                        bytesRead = 0;
                        buffer.clear();
                    }
                    else if (bytesLeft > 0 && bytesRead > bytesLeft){
                        stream.append(buffer.left(bytesLeft));
                        data = QJsonDocument::fromJson(stream);
                        updateApp(data);
                        //possibly save data to a file should be ez pz
                        stream.clear();
                        bytesLeft=0;
                        bytesRead-=bytesLeft;
                    }
                    if (bytesRead < packet_size){
                        bytesLeft = packet_size - bytesRead;
                        bytesRead=0;
                        stream.append(buffer);
                    }
                    else{
                        stream.append(buffer.left(packet_size));
                        data = QJsonDocument::fromJson(stream);
                        updateApp(data);
                        //possibly save data to a file should be ez pz
                        stream.clear();
                        bytesLeft = 0;
                        bytesRead-= packet_size;
                    }
                }
            }
            else {
                qDebug() << "Heartbeat not recieved";
                tcpsocket->write("Close"); //ideally have a generator to generate JSON
                //depending on the messages we want to send (something like
                // generateMessage(1, other params)
                tcpsocket->close();
            }
        }

    }
    else{
        qDebug() << "An error has occured, unable to connect";
    }

}

void MainWindow::readTCPData() {
    //bytesRead = tcpsocket->read(buffer, buffer_size);
    for (int i = 0; i < bytesRead; i++){

    }
    if (!stream.isNull()){
        qDebug()<< stream;
    }
    data = QJsonDocument::fromJson(stream);


}

void MainWindow::sendCommand(){
    QByteArray command;
    command.resize(10);
    command = "test";
    tcpsocket->write(command);
    qDebug() << "Message to Client: " << command;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateApp(QJsonDocument &d){
    QJsonObject obj = d.object();
    //do bullshit here
    baroMeter->setCurrentValue(90);
}

void MainWindow::readUpdate(QJsonDocument &d){

    qDebug() << d.toJson();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    //speedoMeter->setCurrentValue(value);
    //voltMeter->setCurrentValue(value);
    baroMeter->setCurrentValue(value);
}

void MainWindow::loadConfigW(QString filePath) {
    QString val;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR: file not found" ;
        return;
    }
    else {
        val = file.readAll();

        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        qDebug() << doc;
        QJsonObject obj = doc.object();
        QJsonValue num_of_sensors = obj.value(QString("sensorCount"));
        qDebug() << num_of_sensors ;



        //QJsonObject app = num_of_sensors.toObject();
        //qDebug() << obj;
        //qDebug() << app["description"].toString();


    }

}



