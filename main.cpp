#include "poker.h"
#include "client.h"
#include "server.h"
#include "startwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StartWindow w;
    w.show();

    return a.exec();
}
