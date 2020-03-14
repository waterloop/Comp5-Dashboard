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
#include "wloopsocket.h"
#include <QApplication>







MainWindow::MainWindow(QWidget *parent, QString host, qint16 port) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    //SETTING UP UI
    ui->setupUi(this);

    //LOADING THEMES
    QcThemeItem darkTheme = QcThemeItem(":/styles/waterLoopThemeDARK.txt");
    QcThemeItem loadTheme = QcThemeItem(":/styles/waterLoopThemeLOAD.txt");


    //SETTING UP LOADER
    loader = new waterLoopGaugeItem(loadTheme, 500, "Current", "LOADING" ,"A", 0, 0, 60, 60, 0, 5);


    QcImage * im =  loader->getGauge()->addImage(10);
    im->setAngle(90);
    im->scale(70);
    im->setImage(":/images/wloop_icon_yellow_mid.png");

    timerLoad = new QTimer(this);
    connect(timerLoad, &QTimer::timeout, this, &MainWindow::moveLoadingGauge);
    timerLoad->start(10);




    //ADDING COMPONENTS

    speedoMeter = new waterLoopGaugeItem(darkTheme, 300,"Speedometer","SPEED","m/s",1,0, 400,350,50,50);


    //voltMeter = new waterLoopGaugeItem(newTheme, 250, "Voltmeter", "", "V",2 ,1, 0.5, 0.25, 0.1);
    //ui->verticalLayout->addWidget(speedoMeter->getGauge());
    //ui->verticalLayout->addWidget(baroMeter->getGauge());
    //ui->grid->addWidget(loader->getGauge(),0,0);

    ui->stackedWidget->setCurrentIndex(0);

    QPixmap logo(":/images/wloop_full_med.png");
    ui->logo->setPixmap(logo);
    ui->logo->setMask(logo.mask());
    ui->logo->show();




    ui->load_page_layout->addWidget(loader->getGauge(),0,0);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 3; j++){
            battery[i][j] = new waterLoopGaugeItem(darkTheme, 200,"Speedometer","SPEED","m/s",1,0, 400,350,50,50);
            ui->battery_layout->addWidget(battery[i][j]->getGauge(),i,j);
        }
    }
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 3; j++){
            dlim[i][j] = new waterLoopGaugeItem(darkTheme, 200,"Speedometer","HI VOLT.","m/s",1,0, 400,350,50,50);
            ui->dlim_layout->addWidget(dlim[i][j]->getGauge(),i,j);
        }
    }

    //ui->grid->addWidget(speedoMeter->getGauge(),0,1);



    ui->horizontalSlider->setVisible(true);

    //SETTING UP CONNECTION
    tcpsocket = new QTcpSocket(this);
    //connect( tcpsocket, SIGNAL( readyRead()), this, SLOT(readTCPData()));
    //connect( tcpsocket, SIGNAL( disconnected() ), this , SLOT(disconnectTCP()));
    tcpsocket->connectToHost(host, port);
    tcpaddress = host;

    //STARTING CONNECTION AND ENTERING RUNNING LOOP

    if (tcpsocket->waitForConnected(500)) {
        qDebug() << "Connected to Host";
        while (true){ //running loop
            if (tcpsocket->waitForReadyRead(3000)){
                bytesRead = tcpsocket->bytesAvailable();
                qDebug() << bytesRead;
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
                        qDebug()<< stream;
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
                tcpsocket->write("close"); //ideally have a generator to generate JSON
                //depending on the messages we want to send (something like
                // generateMessage(1, other params)
                tcpsocket->close();
                break;
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
    //baroMeter->setCurrentValue(90);
}

void MainWindow::readUpdate(QJsonDocument &d){

    qDebug() << d.toJson();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    speedoMeter->setCurrentValue(value);
    //voltMeter->setCurrentValue(value);

    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 3; j++){
            battery[i][j]->setCurrentValue(value);
        }
    }
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 3; j++){
            dlim[i][j]->setCurrentValue(value);
        }
    }

}

void MainWindow::fadeIn(QWidget * w, int msec){
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    w->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(msec);
    a->setStartValue(0);
    a->setEndValue(0.99);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);


}

void MainWindow::fadeOut(QWidget *w, const char* mem, int msec){
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    w->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(msec);
    a->setStartValue(0.99);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a,SIGNAL(finished()),this, mem);
}

void MainWindow::moveGauge(){
    if (loadpos<99){

        loadpos+= static_cast<qreal>(QRandomGenerator::global()->bounded(1,5))/10;
        speedoMeter->setCurrentValue(loadpos);
    }
}
void MainWindow::loadMainScreen(){
    ui->stackedWidget->setCurrentIndex(3);

    fadeIn(ui->page4);

}
void MainWindow::initializeConnection(){
    ui->connection_status->setText("STATUS: CONNECTED\n INITIALIZING CONNECTION");

    //SEND FIRST ACKNOWLEDGMENT PACKAGE AND STUFF


    //MOVE ONTO THE MAIN STUFF

    QTimer::singleShot(1000, this, SLOT(fadeOutConnectionScreen()));

}

void MainWindow::fadeOutConnectionScreen(){

    fadeOut(ui->buttons, SLOT(loadMainScreen()));
}

void MainWindow::attemptConnection(){

    tcpaddress = ui->ip_adress->text();
    tcpport = ui->port->text().toInt();

    if (tcpaddress == "alex" && tcpport == 0000){
        initializeConnection();
        return;
    }

    tcpsocket = new WLoopSocket(this);
    tcpsocket->connectToHost(tcpaddress, tcpport);

    connect(tcpsocket, SIGNAL(connected()), this ,SLOT(initializeConnection()));

    ui->connection_status->setText("STATUS: CONNECTION FAILED");
    ui->ip_adress->setText("");
    ui->port->setText("");
    return;




}
void MainWindow::loadConnectionScreen(){

    ui->stackedWidget->setCurrentIndex(2);

    fadeIn(ui->buttons);

    connect(ui->connect_button,SIGNAL(pressed()),this , SLOT(attemptConnection()));
    connect(ui->port,SIGNAL(returnPressed()), this, SLOT(attemptConnection()));

}

void MainWindow::fadeOutInitializer(){

    fadeOut(ui->initializer, SLOT(loadConnectionScreen()));

}



void MainWindow::loadInitializer(){
    loader->getGauge()->setVisible(false);

    ui->stackedWidget->setCurrentIndex(1);

    fadeIn(ui->initializer);

    connect(ui->initialize_button, SIGNAL(pressed()), this, SLOT(fadeOutInitializer()));


}

void MainWindow::moveLoadingGauge(){
    if (loadpos<99){

        loadpos+= static_cast<qreal>(QRandomGenerator::global()->bounded(1,5))/10;
        loader->setCurrentValue(loadpos);
    }
    else {
        timerLoad->stop();

        fadeOut(loader->getGauge(), SLOT(loadInitializer()));

    }


}
