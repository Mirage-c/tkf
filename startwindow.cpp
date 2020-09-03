#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::hideSlot(){
    this->hide();
}

void StartWindow::on_createButton_pressed()
{
    ui->joinButton->setEnabled(false);
//qDebug() << "Pressed";
    p = new Server;
//qDebug() << "Created";
    connect(p,SIGNAL(connectOK()),this,SLOT(hideSlot()));
//qDebug() << "Connected";
    static_cast<Server*>(p)->initServer();
}

void StartWindow::on_joinButton_pressed()
{
    ui->createButton->setEnabled(false);
    p = new Client;
    connect(p,SIGNAL(connectOK()),this,SLOT(hideSlot()));
    static_cast<Client*>(p)->connectHost();
}

void StartWindow::on_joinButton_released()
{
    ui->createButton->setEnabled(true);
}

void StartWindow::on_createButton_released()
{
    ui->joinButton->setEnabled(true);
}
