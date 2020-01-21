#include <iostream>
#include <QTextStream>
#include <QTcpServer>
#include <QIODevice>
#include <QAbstractSocket>
#include "mytcp.h"



MyTCP::MyTCP (QObject *parent, QString host, qint16 port) : QObject(parent)
{
    tcpsocket = new QTcpSocket(this);
    connect( tcpsocket, SIGNAL( readyRead()), this, SLOT(readTCPData()));
    //connect( tcpsocket, SIGNAL( disconnected() ), this , SLOT(disconnectTCP()));
    tcpsocket->connectToHost(host, port);
    tcpaddress = host;
    if (tcpsocket->waitForConnected()) {
        qDebug("Connected to Host");
    }
}

QJsonDocument MyTCP::getData(){
    return data;
}

bool MyTCP::connected(){
    return (tcpsocket->state() == QAbstractSocket::ConnectedState);

}

void MyTCP::readTCPData() {
    stream = tcpsocket->readAll();
    if (!stream.isNull()){
        qDebug()<< stream;
    }
    data = QJsonDocument::fromJson(stream);

    emit readyReadCustom();
}


void MyTCP::sendCommand() {
    QByteArray command;
    command.resize(10);
    command = "test";
    tcpsocket->write(command);

    qDebug() << "Message to Client: " << command;
}
