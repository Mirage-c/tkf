#include "poker.h"
#include "ui_poker.h"

Poker::Poker(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::Poker){
    ui->setupUi(this);
    ui->callButton->hide();
    ui->notcallButton->hide();
    ui->ACall->hide();
    ui->BCall->hide();
    ui->ADontCall->hide();
    ui->BDontCall->hide();
    ui->iCall->hide();
    ui->iDontCall->hide();
//qDebug() << "OK";
    connect(this,SIGNAL(connectOK()),this,SLOT(windowSlot()));
    connect(this,SIGNAL(showCards()),this,SLOT(showSlot()));
    connect(this,SIGNAL(beginCallForLandlord()),this,SLOT(callLandlordSlot()));
}

Poker::~Poker()
{
}

void Poker::showSlot(){
    for(int x = 0; x < 17; x++) {
        QLabel *label = new QLabel(this);
        QString src = ":/images/" + QString::number(cards[x].type*13+cards[x].number) + ".png";
   // qDebug() << src;
        QImage* img = new QImage(src);
        label->setPixmap(QPixmap::fromImage(*img));
        label->setGeometry(x * CARDWIDTH,600,200,300);
        label->show();
        cards[x].label = label;
    }
}

void Poker::windowSlot(){
    this->show();
}

void Poker::callLandlordSlot(){
    ui->callButton->show();
    ui->notcallButton->show();
}

void Poker::showLandlord(int who,bool call){
    if(who == no){
        if(call)
            ui->iCall->show();
        else
            ui->iDontCall->show();
    }else if(isLeftOne(who)){
        if(call)
            ui->ACall->show();
        else
            ui->ADontCall->show();
    }else{
        if(call)
            ui->BCall->show();
        else
            ui->BDontCall->show();
    }
}


void Poker::on_callButton_clicked()
{
    ui->callButton->hide();
    ui->notcallButton->hide();
    ui->iCall->show();
    emit iWantToCall();
}

void Poker::on_notcallButton_clicked()
{
    ui->notcallButton->hide();
    ui->callButton->hide();
    ui->iDontCall->show();
    emit iDontWantToCall();
}

