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




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //SETTING UP UI
    ui->setupUi(this);
    //---------------------------------------------------------------------------

    //LOADING THEMES
    QcThemeItem darkTheme = QcThemeItem(":/styles/waterLoopThemeDARK.txt");
    QcThemeItem loadTheme = QcThemeItem(":/styles/waterLoopThemeLOAD.txt");
    //----------------------------------------------------------------------------

    //LOADING IMAGES

    QPixmap logo(":/images/wloop_full_med.png");
    ui->logo->setPixmap(logo);
    ui->logo->setMask(logo.mask());
    ui->logo->show();

    QPixmap logo_small(":/images/wloop_full_small.png");
    //-------------------------------------------------------------------------

    //SETTING UP LOADER
    loader = new waterLoopGaugeItem(loadTheme, 500, "Current", "LOADING" ,"A", 0, 0, 60, 60, 0, 5);

    QcImage * im =  loader->getGauge()->addImage(10);
    im->setAngle(90);
    im->scale(70);
    im->setImage(":/images/wloop_icon_yellow_mid.png");

    timerLoad = new QTimer(this);
    connect(timerLoad, &QTimer::timeout, this, &MainWindow::moveLoadingGauge);
    timerLoad->start(10);

     ui->load_page_layout->addWidget(loader->getGauge(),0,0);
    //-------------------------------------------------------------------

    //SETTING UP TIME DISPLAY
     ui->time_value->setText(QTime::currentTime().toString("hh:mm"));

     timeThread = new QThread(this);

     //setting up timer
     timeDisplay = new QTimer(nullptr);
     timeDisplay->setInterval(1000);
     timeDisplay->moveToThread(timeThread);

     //required connections
     connect(timeDisplay, SIGNAL(timeout()), this, SLOT(updateTimeDisplay()));
     connect(timeThread, SIGNAL(started()), timeDisplay, SLOT(start()));
     connect(timeThread, SIGNAL(finished()),timeDisplay, SLOT(stop()));

     //starting thread
     timeThread->start();

    //------------------------------------------------------------------

    //CREATING COMPONENTS

    speedoMeter = new waterLoopGaugeItem(darkTheme, 150,"Speedometer","SPEED","m/s",1,0, 400,350,50,50);
    dlim = new waterLoopGaugeItem(darkTheme, 150,"DLIM","FREQ.","Hz",1,60, 300,280,100,40);
    highC = new waterLoopGaugeItem(darkTheme, 150,"HVCurrent","CURR.","A",1,0, 400,350,50,50);
    highV = new waterLoopGaugeItem(darkTheme, 150,"HVVoltage","VOLT.","V",1,0, 400,350,50,50);
    lowC = new waterLoopGaugeItem(darkTheme, 150,"LVCurrent","CURR.","A",1,0, 400,350,50,50);
    lowV = new waterLoopGaugeItem(darkTheme, 150,"LVVoltage","VOLT.","V",1,0, 400,350,50,50);
    //-----------------------------------------------------------------------------

    //SETTING UP APPLICATION
    ui->stackedWidget->setCurrentIndex(0);

    //SETTING UP TITLE BAR
    ui->main_window->layout()->setContentsMargins(0,0,0,0);
    ui->centralWidget->layout()->setContentsMargins(0,0,0,0);
    ui->title_bar_layout->layout()->setContentsMargins(0,0,0,0);
    ui->icon->setPixmap(logo_small);

    connect(ui->close_button,SIGNAL(released()), this, SLOT(closeWindow()));

    //----------------------------------------------------------------------

    //SETTING UP PROGRESS BAR

    ui->distance_travelled_bar->setValue(0);

    //----------------------------------------------------------------------

    //DETERMINING THE INITIAL HEALTH

    if(speedoMeter->getState() == 1 && dlim->getState() == 1 && lowC->getState()==1 && lowV->getState()==1 && highV->getState()==1 && highC->getState()==1){
        ui->pod_indicator->setStyleSheet("QLabel#pod_indicator{ background-color: #14ff65; color: #2b2b2b; border: 3px solid #14ff65; border-radius : 20px; }");
        ui->pod_indicator->setText("GOOD");
    }
    else{
        ui->pod_indicator->setStyleSheet("QLabel#pod_indicator{ background-color: #ff9494; color: #2b2b2b; border: 3px solid #ff9494; border-radius : 20px; }");
        ui->pod_indicator->setText("BAD");
    }

    //------------------------------------------------------------------------

    //CONNECTING MAIN GAUGES TO POD HEALTH

    connect(speedoMeter, SIGNAL(badState()), this, SLOT(updatePodHealthBAD()));
    connect(speedoMeter, SIGNAL(safeState()), this, SLOT(updatePodHealthGOOD()));
    connect(dlim, SIGNAL(badState()), this, SLOT(updatePodHealthBAD()));
    connect(dlim, SIGNAL(safeState()), this, SLOT(updatePodHealthGOOD()));
    connect(highC, SIGNAL(badState()), this, SLOT(updatePodHealthBAD()));
    connect(highC, SIGNAL(safeState()), this, SLOT(updatePodHealthGOOD()));
    connect(highV, SIGNAL(badState()), this, SLOT(updatePodHealthBAD()));
    connect(highV, SIGNAL(safeState()), this, SLOT(updatePodHealthGOOD()));
    connect(lowC, SIGNAL(badState()), this, SLOT(updatePodHealthBAD()));
    connect(lowC, SIGNAL(safeState()), this, SLOT(updatePodHealthGOOD()));
    connect(lowV, SIGNAL(badState()), this, SLOT(updatePodHealthBAD()));
    connect(lowC, SIGNAL(safeState()), this, SLOT(updatePodHealthGOOD()));


    //SETTING UP THE MAIN GAUGES DISPLAY
    ui->dlim_layout->addWidget(dlim->getGauge(),0);
    ui->dlim_layout->setStretch(0,1);
    ui->dlim_layout->setStretch(1,6);
    ui->dlimHV_layout->addWidget(highV->getGauge(),3,0);
    ui->dlimHV_layout->addWidget(highC->getGauge(),3,1);
    ui->buttonsLV_layout->addWidget(lowV->getGauge(),3,0);
    ui->buttonsLV_layout->addWidget(lowC->getGauge(),3,1);
    ui->speed_layout->addWidget(speedoMeter->getGauge(),0,0);

    //SETTING TAB ORDER

    setTabOrder(ui->ip_adress,ui->port);
    setTabOrder(ui->port,ui->connect_button);

    //TRYING INITIAL CONNECTION WITH DETERMINED ADRESS AND PORT

    tcp = new WLoopSocket(this);
    tcp->connectToHost(std_add, std_port);
    if(tcp->waitForConnected(1000)){
        qDebug() << "conected!";
    }



    //TESTING ONLY AFTER THIS POINT
    //ui->horizontalSlider->setVisible(false);








    /*
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 3; j++){
            battery[i][j] = new waterLoopGaugeItem(darkTheme, 150,"Speedometer","SPEED","m/s",1,0, 400,350,50,50);
            ui->battery_layout->addWidget(battery[i][j]->getGauge(),i,j);
        }
    }
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 3; j++){
            dlim[i][j] = new waterLoopGaugeItem(darkTheme, 150,"Speedometer","HI VOLT.","m/s",1,0, 400,350,50,50);
            ui->dlim_layout->addWidget(dlim[i][j]->getGauge(),i,j);
        }
    }
*/
    //ui->grid->addWidget(speedoMeter->getGauge(),0,1);





    //SETTING UP CONNECTION

    //connect( tcpsocket, SIGNAL( readyRead()), this, SLOT(readTCPData()));
    //connect( tcpsocket, SIGNAL( disconnected() ), this , SLOT(disconnectTCP()));


    //STARTING CONNECTION AND ENTERING RUNNING LOOP
/*
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

    */

}

void MainWindow::readTCPData() {
    buffer = tcp->readAll();

    qDebug()<< buffer;

    data = QJsonDocument::fromJson(stream);


}

void MainWindow::closeWindow(){
    tcp->closeThread();
    timeThread->exit();
    this->close();
}


void MainWindow::sendCommand(){
    QByteArray command;
    command.resize(10);
    command = "test";
    tcp->write(command);
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
    dlim->setCurrentValue(value);
    lowC->setCurrentValue(value);
    lowV->setCurrentValue(value);
    highC->setCurrentValue(value);
    highV->setCurrentValue(value);
    speedoMeter->setCurrentValue(value);

    ui->distance_travelled_bar->setValue(value);
    QString dist;
    dist = "Distance Travelled: ";
    dist.append(QString::number(floor((static_cast<double>(value)/99)*travelDistance)));
    dist.append(" m");
    ui->distance_travelled_value->setText(dist);


}

void MainWindow::updatePodHealthGOOD(){

    if(speedoMeter->getState() == 1 && dlim->getState() == 1 && lowC->getState()==1 && lowV->getState()==1 && highV->getState()==1 && highC->getState()==1){
        ui->pod_indicator->setStyleSheet("QLabel#pod_indicator{ background-color: #14ff65; color: #2b2b2b; border: 3px solid #14ff65; border-radius : 20px; }");
        ui->pod_indicator->setText("GOOD");
    }

}

void MainWindow::updatePodHealthBAD(){
    ui->pod_indicator->setStyleSheet("QLabel#pod_indicator{ background-color: #ff9494; color: #2b2b2b; border: 3px solid #ff9494; border-radius : 20px; }");
    ui->pod_indicator->setText("BAD");

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

    connect(tcp,SIGNAL(readyRead()),this, SLOT(readTCPData()));


    //MOVE ONTO THE MAIN STUFF

    QTimer::singleShot(1000, this, SLOT(fadeOutConnectionScreen()));

}

void MainWindow::fadeOutConnectionScreen(){

    fadeOut(ui->buttons, SLOT(loadMainScreen()));
}

void MainWindow::attemptConnection(){

    tcpaddress = ui->ip_adress->text();
    tcpport = ui->port->text().toInt();

    if (tcpaddress == "alex" && tcpport == 0){
        initializeConnection();
        return;
    }

    tcp->connectToHost(tcpaddress, tcpport);

    connect(tcp, SIGNAL(connected()), this ,SLOT(initializeConnection()));

    ui->connection_status->setText("STATUS: CONNECTION FAILED");
    ui->ip_adress->setText("");
    ui->port->setText("");
    return;




}

void MainWindow::loadConnectionScreen(){
    if(tcp->state() == QAbstractSocket::ConnectedState){
        ui->stackedWidget->setCurrentIndex(3);
        fadeIn(ui->page4);
    }
    else{

    ui->stackedWidget->setCurrentIndex(2);

    fadeIn(ui->buttons);

    connect(ui->connect_button,SIGNAL(pressed()),this , SLOT(attemptConnection()));
    connect(ui->port,SIGNAL(returnPressed()), this, SLOT(attemptConnection()));

    }
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

void MainWindow::updateTimeDisplay(){
    ui->time_value->setText(QTime::currentTime().toString("hh:mm"));
}
