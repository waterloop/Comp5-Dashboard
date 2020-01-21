#ifndef MYTCP_H
#define MYTCP_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QJsonDocument>

class MyTCP : public QObject
{
    Q_OBJECT
public:
    explicit MyTCP(QObject *parent = 0, QString host="127.0.0.1",qint16 port=42002);
    bool connected();
    void sendCommand();
    QJsonDocument getData();



signals:

    void readyReadCustom();

public slots:

    void readTCPData();


private:
    QHostAddress tcpaddress;
    QTcpSocket *tcpsocket;
    QByteArray stream;
    QJsonDocument data;


};



#endif // MYTCP_H
