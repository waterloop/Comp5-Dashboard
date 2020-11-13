#ifndef WLOOP_SOCKET_H
#define WLOOP_SOCKET_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include <QTimer>

#define BEAT_INTERVAL 300
#define MAX_WAIT 500

enum{ //HEARTBEAT STATES
  heartBeatStarted,
    noHeartBeat
};


class WLoopSocket : public QTcpSocket {
  Q_OBJECT

public:

    enum heartBeatState { //HEARTBEAT STATES (i.e. if the heartbeat was ever started)
        heartBeatStarted,
        noHeartBeat
    };
    Q_ENUM(heartBeatState)
    explicit WLoopSocket(QObject *parent = nullptr): QTcpSocket(parent) {}

    void closeThread(){
        if (hbstate == WLoopSocket::noHeartBeat){
            return;
        }
        else if (hbstate== WLoopSocket::heartBeatStarted){
            heartBeat->exit();
        }
    }

    void startHeartBeat(){
       // SET UP HEART BEAT
       // CAN SPIN A THREAD THAT CONTINUOUSLY WRITES TO THE SOCKET

        if(hbstate == WLoopSocket::heartBeatStarted) return;

        if(!this->waitForConnected(MAX_WAIT)){
            qDebug() <<" not connected";
            emit disconnected();
            return;
        }

        hbstate = WLoopSocket::heartBeatStarted;

        this->heartBeatTimer = new QTimer(nullptr); // NO PARENT
        this->heartBeatTimer->setInterval(BEAT_INTERVAL); // in millisecs
        this->heartBeatTimer->setSingleShot(false); // fire repeatedly

        heartBeat = new QThread(this);
        heartBeatTimer->moveToThread(heartBeat);

        connect(heartBeatTimer, SIGNAL(timeout()), this, SLOT(sendHeartBeat()));
        connect(heartBeat,SIGNAL(started()),heartBeatTimer, SLOT(start()));
        connect(heartBeat,SIGNAL(finished()), heartBeatTimer, SLOT(stop()));

        heartBeat->start(); // START THE HEART BEAT
   }

    heartBeatState heartBeatState(){
        return hbstate;
    }

   ~WLoopSocket() {
        if(hbstate == WLoopSocket::heartBeatStarted){
            delete heartBeat;  delete heartBeatTimer; }
    } // DESTRUCTOR NEEDED because no parent?


private slots:
  void sendHeartBeat() {

    // SEND HEART BEAT
    write("BA DUM");


  }

private:
  QThread *heartBeat;
  QTimer *heartBeatTimer;
  enum heartBeatState hbstate = WLoopSocket::noHeartBeat;

};

#endif
