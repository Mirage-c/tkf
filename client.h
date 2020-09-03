#ifndef CLIENT_H
#define CLIENT_H
#include "poker.h"

namespace Ui{
class Client;
}

class Client : public Poker{
    Q_OBJECT
public:
    Client(QWidget *parent = nullptr);
    ~Client();
    void acceptConnection();
    void connectHost();
    void send(const QByteArray*);


private slots:
    void recvMessage();

private:
    QByteArray array;
    void sendAgain();
    QTcpSocket* readWriteSocket;
    void iCallOrNot(bool);
    void sendBeginShuffle();
    void sendCallForLandlord();
    void sendDetLandlord();
};

#endif // CLIENT_H
