#include "server.h"

void _swap(int &a,int &b){
    int tmp = a;
    a = b;
    b = tmp;
}

Server::Server(QWidget *parent) :
    Poker(parent)
{
//qDebug() << "in";
    this->setWindowTitle("Server");
    no = 2;
    connect(this,&Poker::iWantToCall,[=](){ whoCallOrNot(no,true);});
    connect(this,&Poker::iDontWantToCall,[=](){ whoCallOrNot(no,false);});
}

Server::~Server(){}

void Server::send(QByteArray* array){//广播
    this->readWriteSocket[0]->write(array->data());
    this->readWriteSocket[1]->write(array->data());
}

void Server::sendTo(QByteArray* array,int target){
    this->readWriteSocket[target]->write(array->data());
}

void Server::requestSendAgain(int to){
    QJsonObject json;QByteArray array;
    json.insert("type","sendAgain");
    QJsonDocument jsondoc(json);
    array = jsondoc.toJson();sendTo(&array,to);
qDebug() << "requestSendAgain";
}

void Server::recvMessage(int from){
    static bool confirmed = 0;
    QJsonObject json = QJsonDocument::fromJson(readWriteSocket[from]->readAll()).object();
    QString type = json.value("type").toString();
qDebug() << "received " << from << json << type << ",confirm=" << confirmed;
    if(type == ""){
        confirmed = !confirmed;
        requestSendAgain(from);
    }else if(type == "beginShuffle" && !confirmed){
    }else if(type == "beginShuffle" && confirmed){
qDebug() << "beginShuffle confirmed";
        shuffleCards();
    }else if(type == "callForLandlord" && !confirmed){
    }else if(type == "callForLandlord" && confirmed){
qDebug() << "callForLandlord confirmed";
        callForLandlord();
    }else if(type == "callLandlord"){
        bool doCall = json.value("call").toInt();
        showLandlord(from,doCall);
        whoCallOrNot(from,doCall);
        if(confirmed)
            detLandlord();
    }
    confirmed = !confirmed;
}

void Server::initServer(){
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,8888);
    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

void Server::acceptConnection(){
    static bool confirmed = 0;
    if(!confirmed){
        confirmed = 1;
        this->readWriteSocket[0] = this->listenSocket->nextPendingConnection();
        QObject::connect(this->readWriteSocket[0],&QTcpSocket::readyRead,[=](){ recvMessage(0); });
    }else{
        confirmed = 0;
        this->readWriteSocket[1] = this->listenSocket->nextPendingConnection();
        QObject::connect(this->readWriteSocket[1],&QTcpSocket::readyRead,[=](){ recvMessage(1); });
        QJsonObject json,json2;QByteArray array;
        json.insert("type","connected");json.insert("no",0);
        json2.insert("type","connected");json.insert("no",1);
        QJsonDocument jsondoc(json),jsondoc2(json2);
        array = jsondoc.toJson();sendTo(&array,0);array.clear();
        array = jsondoc2.toJson();sendTo(&array,1);
        emit this->connectOK();
    }
}

void Server::getSequence(){
    srand((unsigned)time(nullptr));
    cardRight = rand()%3;
    shuffleCards();
}

void Server::shuffleCards(){
//qDebug() << "shuffle";
    int tmp,vault[54];
    for(int i = 0;i < 54;i++){
        vault[i] = i+1;
    }
    QJsonDocument jsondoc,jsondoc2;
    QJsonArray jsonarray,jsonarray2;
    QJsonObject json,json2;
    QByteArray array;
    srand((unsigned)time(nullptr));
    for(int i = 0;i < 54;i++){
        tmp = rand()%54;
        _swap(vault[i],vault[tmp]);
    }
/*for(int i = 0;i < 54;i++){
    qDebug() << vault[i];
}*/
    for(int i = 0;i < 17;i++){
        cards.append(convertToCard(vault[i]));
    }
//qDebug() << cards.length();
    std::sort(cards.begin(),cards.end(),Comp);
    emit showCards();
    for(int i = 17;i < 34;i++){
        jsonarray.append(vault[i]);
    }
    json.insert("type","shuffle");
    json.insert("cards",jsonarray);
    jsondoc.setObject(json);
    array = jsondoc.toJson();
    this->readWriteSocket[0]->write(array);
//qDebug() << "send cards to 0";
    array.clear();
    for(int i = 34;i < 51;i++){
        jsonarray2.append(vault[i]);
    }
    json2.insert("type","shuffle");
    json2.insert("cards",jsonarray2);
    jsondoc2.setObject(json2);
    array = jsondoc2.toJson();
    this->readWriteSocket[1]->write(array);
//qDebug() << "send cards to 1";
}

void Server::callForLandlord(){
    cardRight = 2;
    if(cardRight == 2){
        next(cardRight);
        emit beginCallForLandlord();
    }
}

void Server::whoCallOrNot(int who,bool doCall){
    call[who] = 1;
    QJsonObject json;QByteArray array;
    json.insert("type","showLandlord");
    json.insert("who",who);
    json.insert("next",cardRight);
    next(cardRight);
    if(doCall)
        json.insert("call",1);
    else
        json.insert("call",0);
    QJsonDocument jsondoc(json);
    array = jsondoc.toJson();
    this->readWriteSocket[0]->write(array);
    this->readWriteSocket[1]->write(array);
}

void Server::detLandlord(){

}
