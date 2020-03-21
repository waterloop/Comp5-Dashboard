#ifndef WLOOP_SOCKET_H
#define WLOOP_SOCKET_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include <QTimer>

#define BEAT_INTERVAL 1500

class WLoopSocket : public QTcpSocket {
  Q_OBJECT

public:

  explicit WLoopSocket(QObject *parent = nullptr): QTcpSocket(parent) {}
     void closeThread(){
         heartBeat->exit();
     }
  virtual bool waitForConnected(int msecs = 30000) override {
    bool result = QAbstractSocket::waitForConnected(msecs);

    // SET UP HEART BEAT
    // CAN SPIN A THREAD THAT CONTINUOUSLY WRITES TO THE SOCKET

    this->heartBeatTimer = new QTimer(nullptr); // NO PARENT
    this->heartBeatTimer->setInterval(BEAT_INTERVAL); // in millisecs
    this->heartBeatTimer->setSingleShot(false); // fire repeatedly

    heartBeat = new QThread(this);
    heartBeatTimer->moveToThread(heartBeat);

    connect(heartBeatTimer, SIGNAL(timeout()), this, SLOT(sendHeartBeat()));
    connect(heartBeat,SIGNAL(started()),heartBeatTimer, SLOT(start()));
    connect(heartBeat,SIGNAL(finished()), heartBeatTimer, SLOT(stop()));

    heartBeat->start(); // START THE HEART BEAT


    return result;
  }
  ~WLoopSocket() { delete heartBeat;  delete heartBeatTimer; } // DESTRUCTOR NEEDED because no parent?

private slots:
  void sendHeartBeat() {

    // SEND HEART BEAT
    write("BA DUM");


  }

private:
  QThread *heartBeat;
  QTimer *heartBeatTimer;

};

#endif
