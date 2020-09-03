#ifndef POKER_H
#define POKER_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QJsonArray>
#include <QVector>
#include <QThread>

#define CARDWIDTH 60

namespace Ui {
class Poker;
}

struct Card{
    QLabel* label;
    int type,number;
};

void next(int&);
Card convertToCard(int);
bool Comp(const Card &a,const Card &b);

class Poker : public QMainWindow
{
    Q_OBJECT

public:
    Poker(QWidget *parent = nullptr);
    ~Poker();
    virtual void send(QByteArray*){}
    virtual void recvMessage(){}
    void showLandlord(int,bool);

public slots:
    void showSlot();
    void windowSlot();
    void callLandlordSlot();

protected:
    int cardRight,no;
    QVector<Card> cards;

signals:
    void connectOK();
    void showCards();
    void beginCallForLandlord();
    void iWantToCall();
    void iDontWantToCall();

private slots:
    void on_callButton_clicked();
    void on_notcallButton_clicked();

private:
    Ui::Poker* ui;
    bool isLeftOne(int);
};

#endif // POKER_H
