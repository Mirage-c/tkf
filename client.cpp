#include "client.h"

Client::Client(QWidget *parent) :
    Poker(parent)
{
    this->setWindowTitle("Client");
    cards.clear();
    connect(this,&Poker::iWantToCall,[=](){ iCallOrNot(true);});
    connect(this,&Poker::iDontWantToCall,[=](){ iCallOrNot(false);});
}

Client::~Client(){

}

void Client::send(const QByteArray* QBarray){
    this->readWriteSocket->write(QBarray->data());
}

void Client::sendAgain(){
qDebug() << "sendAgain";
    sendCallForLandlord();
}

void Client::recvMessage(){
    QJsonObject json = QJsonDocument::fromJson(readWriteSocket->readAll()).object();
    QString type = json.value("type").toString();
qDebug() << "Client" << no << "receive:" << type;
    if(type == "sendAgain"){
        sendAgain();
    }else if(type == "connected"){
        no = json.value("no").toInt();
        emit connectOK();
        sendBeginShuffle();
    }else if(type == "shuffle"){
        QJsonArray QJarray = json.value("cards").toArray();
        for(int i = 0;i < 17;i++){
            cards.append(convertToCard(QJarray[i].toInt()));
            std::sort(cards.begin(),cards.end(),Comp);
        }
        emit showCards();
        sendCallForLandlord();
    }else if(type == "showLandlord"){
        int who = json.value("who").toInt();
        bool call = json.value("call").toInt();
        int next = json.value("next").toInt();
        showLandlord(who,call);
        if(no == next)
            emit beginCallForLandlord();
    }else if(type == "detLandlord"){
        sendDetLandlord();
    }

}

void Client::connectHost(){
    this->readWriteSocket = new QTcpSocket;
    this->readWriteSocket->connectToHost(QHostAddress("127.0.0.1"),8888);
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void Client::sendBeginShuffle(){
    QJsonObject json;
    array.clear();
    json.insert("type","beginShuffle");
    QJsonDocument jsondoc(json);
    array = jsondoc.toJson();
    send(&array);
    qDebug() << "Client" << no << "send beginShuffle";
}

void Client::sendCallForLandlord(){
    QJsonObject json;
    array.clear();
    json.insert("type","callForLandlord");
    QJsonDocument jsondoc(json);
qDebug() << jsondoc;
    array = jsondoc.toJson();
    send(&array);
    qDebug() << "Client" << no << "send callForLandlord" << array;
}

void Client::sendDetLandlord(){
    QJsonObject json;
    array.clear();
    json.insert("type","detLandlord");
    QJsonDocument jsondoc(json);
    array = jsondoc.toJson();
    send(&array);
    qDebug() << "Client" << no << "send detLandlord";
}

void Client::iCallOrNot(bool doCall){
    QJsonObject json;QByteArray array;
    json.insert("type","callLandlord");
    if(doCall)
        json.insert("call",1);
    else
        json.insert("call",0);
    QJsonDocument jsondoc(json);
    array = jsondoc.toJson();
    send(&array);
}
