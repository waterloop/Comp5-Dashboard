#ifndef WLOOPTCP_H
#define WLOOPTCP_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>

class wloopTCP : public QObject{
        Q_OBJECT

public:
    wloopTCP(QObject * parent = 0);


public slots:
    void readData();

private:
    QHostAddress tcpaddress;
    QTcpSocket *tcpsocket;

};

#endif // WLOOPTCP_H
