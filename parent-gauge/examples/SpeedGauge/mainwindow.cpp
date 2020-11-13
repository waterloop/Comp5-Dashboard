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

    //CREATING LOG FILE

    if(! QDir("D:/Coding/Workspaces/WaterLoop/qT/parent-gauge/Comp5-Dashboard/parent-gauge/examples/SpeedGauge/resources/logs/"+ QDateTime::currentDateTime().toString("dd.MM.yyyy") ).exists()){
        QDir().mkdir("D:/Coding/Workspaces/WaterLoop/qT/parent-gauge/Comp5-Dashboard/parent-gauge/examples/SpeedGauge/resources/logs/"+ QDateTime::currentDateTime().toString("dd.MM.yyyy"));
    }
    QString logFileName = "D:/Coding/Workspaces/WaterLoop/qT/parent-gauge/Comp5-Dashboard/parent-gauge/examples/SpeedGauge/resources/logs/" +  QDateTime::currentDateTime().toString("dd.MM.yyyy")+ "/logWloop-";
    logFileName.append(QDateTime::currentDateTime().toString("hh.mm.ss"));
    logFileName.append(".txt");
    logFile.setFileName(logFileName);
    if(logFile.open(QIODevice::ReadWrite | QIODevice::Text)){
        logStream.setDevice(&logFile);
        qDebug() << "log file created";
        logStream <<"BEGINNING TO LOG\n";
    }
    else{
        qDebug() << "unable to create log file, proceeding without logging information";
    }

    //LOADING IMAGES

    QPixmap logo(":/images/wloop_full_med.png");
    ui->logo->setPixmap(logo);
    ui->logo->setMask(logo.mask());
    ui->logo->show();

    QPixmap logo_small(":/images/wloop_full_small.png");

    //-------------------------------------------------------------------------

    //LOADING SENSORS

    QString json_data;
    QFile json_file;
    json_file.setFileName(":/config/config.json");
    json_file.open(QIODevice::ReadOnly | QIODevice::Text);
    json_data = json_file.readAll();
    json_file.close();
    QJsonDocument startup_config = QJsonDocument::fromJson(json_data.toUtf8());
    json_data.clear();
    sensors = getSensorArray(startup_config);

    //--------------------------------------------------------------------------

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

     //establishing required connections
     connect(timeDisplay, SIGNAL(timeout()), this, SLOT(updateTimeDisplay()));
     connect(timeThread, SIGNAL(started()), timeDisplay, SLOT(start()));
     connect(timeThread, SIGNAL(finished()),timeDisplay, SLOT(stop()));

     //starting thread
     timeThread->start();

     //------------------------------------------------------------------

     //SETTING UP REFRESH RATE

     screenRefresh = new QThread(this);

     //setting up timer

     refreshTimer = new QTimer(nullptr);
     refreshTimer->setInterval(static_cast<int>(1000.0/fps));
     refreshTimer->moveToThread(screenRefresh);

     //establishing connections

     connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refreshScreen()));
     connect(screenRefresh, SIGNAL(started()), refreshTimer, SLOT(start()));
     connect(screenRefresh, SIGNAL(finished()), refreshTimer, SLOT(stop()));

     //starting thread

     screenRefresh->start();

     //-------------------------------------------------------------------------------

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

    //SETTING UP VALIDATORS FOR LINE EDITS

    ui->port->setValidator(new QIntValidator(1,65535,this));
    ui->max_dist_line_edit->setValidator(new QDoubleValidator(0,10000, 2,this));
    ui->max_time_line_edit->setValidator(new QDoubleValidator(0,10000, 2,this));
    ui->max_speed_line_edit->setValidator(new QDoubleValidator(0,10000, 2,this));

    //SETTING TAB ORDER

    setTabOrder(ui->ip_adress,ui->port);
    setTabOrder(ui->port,ui->connect_button);
    setTabOrder(ui->max_speed_line_edit, ui->max_dist_line_edit);
    setTabOrder(ui->max_dist_line_edit, ui->max_time_line_edit);

    //TRYING INITIAL CONNECTION WITH DETERMINED ADRESS AND PORT

    tcp = new WLoopSocket(this);

    //establishing required connections pre initialization

    connect(tcp, SIGNAL(disconnected() ), this, SLOT(checkNetworkConnectivity()));
    connect(tcp, SIGNAL(connected()), this, SLOT(checkNetworkConnectivity()));

    //trying initalizing the connection using default parameters
    tcp->connectToHost(std_add, std_port);
    tcp->startHeartBeat();

    //establishing post initialization connections
    connect(tcp, SIGNAL(readyRead()), this, SLOT(readTCPData()));

    //SETTING UP SENSOR TABLE

    ui->tableWidget->setRowCount(sensors.size());
    QTableWidgetItem * item;
    int sensors_size = sensors.size();
    for(int i = 0; i < sensors_size; i ++){
        item = new QTableWidgetItem(sensors[i]->getName());
        ui->tableWidget->setItem(i,0,item);
        sensors[i]->setNameItem(item);
        item = new QTableWidgetItem(QString::number(sensors[i]->getValue()).append(" ").append(sensors[i]->getUnits()));
        ui->tableWidget->setItem(i,1,item);
        sensors[i]->setValItem(item);
        connect(sensors[i], SIGNAL(stateChanged(QTableWidgetItem *, sensorState)), this, SLOT(changeTableItemBackgroundCOL(QTableWidgetItem *, sensorState)));
        connect(sensors[i], SIGNAL(valueChanged(QTableWidgetItem *, qreal, QString)), this, SLOT(changeTableItemVAL(QTableWidgetItem *, qreal, QString)));
    }

    //SETTING UP START FORM

    ui->send_form_layout->setAlignment(ui->send_command, Qt::AlignHCenter);

    //SETTING UP IMPORTANT BUTTONS

    connect(ui->stop_button, SIGNAL(released()), this, SLOT(processStop()) );
    connect(ui->start_button, SIGNAL(released()), this, SLOT(processStart()));


    //TESTING ONLY AFTER THIS POINT (ACTUAL GARBAGE LMAOOOOOO)

    //ui->stackedWidget->setCurrentIndex(4);

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

    //ui->connection_value->setText("NA");

    ping = QDateTime::currentMSecsSinceEpoch() - buffer.toLongLong();

    logStream << QTime::currentTime().toString("hh.mm.ss.zzz");
    logStream << ": ";
    logStream << buffer;
    logStream << endl;

}

void MainWindow::closeWindow(){
    tcp->closeThread();
    timeThread->exit();
    screenRefresh->exit();
    logFile.close();
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
    delete timeThread;
    delete timeDisplay;
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
/*
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

    int sensor_size = sensors.size();
    for(int i = 0; i < sensor_size; i ++){
        sensors[i]->changeValueSlider(value);
    }


}
*/

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

void MainWindow::loadStartScreen(){
    ui->stackedWidget->setCurrentIndex(4);
    fadeIn(ui->page5);
    connect(ui->send_command, SIGNAL(released()), this, SLOT(fadeOutStartScreen()));

}

void MainWindow::fadeOutStartScreen(){

    QString temp = ui->max_dist_line_edit->text();
    if(temp.toDouble()>0) travelDistance = temp.toDouble();
    else travelDistance = -1;
    temp = ui->max_speed_line_edit->text();
    if(temp.toDouble()>0) maxSpeed = temp.toDouble();
    else maxSpeed = -1;
    temp = ui->max_time_line_edit->text();
    if(temp.toDouble()>0) maxTime = temp.toDouble() ;
    else maxTime = -1;
    qDebug() << "TRAVEL" << travelDistance;
    if(maxTime < 0 && maxSpeed < 0 && travelDistance < 0){
        runningState = 0;
        ui->stop_button->setText("DISCONNECT");
        fadeOut(ui->page5,SLOT(loadMainScreen()));
    }
    else if (maxTime < 0 || maxSpeed <0 || travelDistance <0){
        runningState = 0;
    }
    else{
        runningState = 1;
        ui->stop_button->setText("STOP");
        ui->start_button->setEnabled(false);
        fadeOut(ui->page5,SLOT(loadMainScreen()));
    }

}

void MainWindow::initializeConnection(){
    ui->connection_status->setText("STATUS: CONNECTED\n INITIALIZING CONNECTION");

    //SEND FIRST ACKNOWLEDGMENT PACKAGE AND STUFF

    tcp->startHeartBeat();


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


    ui->stackedWidget->setCurrentIndex(2);

    fadeIn(ui->buttons);

    connect(ui->connect_button,SIGNAL(pressed()),this , SLOT(attemptConnection()));
    connect(ui->port,SIGNAL(returnPressed()), this, SLOT(attemptConnection()));


}

void MainWindow::fadeOutInitializer(){
    if(tcp->state() == QAbstractSocket::ConnectedState){
        fadeOut(ui->initializer, SLOT(loadMainScreen()));
    }
    else fadeOut(ui->initializer, SLOT(loadConnectionScreen()));
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

void MainWindow::checkNetworkConnectivity(){
    if(tcp->state() == QAbstractSocket::ConnectedState){
        ui->connection_indicator->setStyleSheet("QLabel#connection_indicator { background-color: #14ff65; color: #2b2b2b; border: 3px solid #14ff65; border-radius : 20px; }");
        ui->connection_indicator->setText("CONNECTED");
        ui->start_button->setText("START");
        ui->stop_button->setEnabled(true);
        if(!runningState){
            ui->stop_button->setText("DISCONNECT");
        }
    }
    else{
        ui->connection_indicator->setStyleSheet("QLabel#connection_indicator { background-color: #ff9494; color: #2b2b2b; border: 3px solid #ff9494; border-radius : 20px; }");
        ui->connection_indicator->setText("ERROR");
        ui->start_button->setText("RECONNECT");
        ui->stop_button->setEnabled(false);
        ui->connection_value->setText("NA");

    }
}

void MainWindow::processStart(){
    if(ui->start_button->text() == "RECONNECT"){
        ui->connection_status->setText("STATUS: WAITING FOR INPUT");
        runningState = 0;
        loadConnectionScreen();
    }
    if(ui->start_button->text() == "START"){
        runningState = 0;
        ui->max_dist_line_edit->setText("");
        ui->max_time_line_edit->setText("");
        ui->max_speed_line_edit->setText("");
        fadeOut(ui->page4, SLOT(loadStartScreen()));
    }

    else{
        ui->stop_button->setText("STOP");
        //runningState = 1;
    }
}

void MainWindow::processStop(){
    if(ui->stop_button->text() == "DISCONNECT"){
        runningState = 0;
        ui->connection_status->setText("STATUS: WAITING FOR INPUT");
        tcp->disconnectFromHost();
        loadConnectionScreen();
    }
    else{
        runningState = 0;
        ui->start_button->setEnabled(true);
    }

}

void MainWindow::changeTableItemBackgroundCOL(QTableWidgetItem * item, sensorState state){
    QColor bcol;
    QColor tcol = QColor("2b2b2b");
    if(state == safe){
        bcol = QColor("#14ff65");
        item->setBackground(bcol);
        item->setTextColor(tcol);
    }
    else{
        bcol = QColor("#ff9494");
        item->setBackgroundColor(bcol);
        item->setTextColor(tcol);
    }

}

void MainWindow::changeTableItemVAL(QTableWidgetItem * item, qreal val, QString units){
    item->setText(QString::number(val).append(" ").append(units));
}

void MainWindow::refreshScreen(){
    qreal value = ui->horizontalSlider->value();
    dlim->setCurrentValue(value);
    lowC->setCurrentValue(value);
    lowV->setCurrentValue(value);
    highC->setCurrentValue(value);
    highV->setCurrentValue(value);
    speedoMeter->setCurrentValue(value);

    ui->distance_travelled_bar->setValue(value);
    ui->connection_value->setText(QString::number(ping) + "ms");

    QString dist;
    dist = "Distance Travelled: ";
    if(travelDistance < 0){
        dist.append(" NA");
    }
    else{
        dist.append(QString::number(floor((static_cast<double>(value)/99)*travelDistance)));
        dist.append(" m");
    }
    ui->distance_travelled_value->setText(dist);

    int sensor_size = sensors.size();
    for(int i = 0; i < sensor_size; i ++){
        sensors[i]->changeValueSlider(value);
    }


}
