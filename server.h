#ifndef SERVER_H
#define SERVER_H
#include "poker.h"

namespace Ui{
class Server;
}

class Server : public Poker{
    Q_OBJECT
public:
    Server(QWidget *parent = nullptr);
    ~Server();
    void initServer();
    void send(QByteArray*);
    void sendTo(QByteArray*,int);

private slots:
    void acceptConnection();
    void recvMessage(int);


private:
    bool call[3];
    QTcpSocket* readWriteSocket[2];
    QTcpServer* listenSocket;
    void requestSendAgain(int);
    void whoCallOrNot(int,bool);
    void getSequence();
    void shuffleCards();
    void callForLandlord();//决定谁先叫地主
    void detLandlord();//抢完地主后决定谁是地主
};


#endif // SERVER_H
