#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include "poker.h"
#include "server.h"
#include "client.h"

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private slots:
    void on_createButton_pressed();
    void on_joinButton_pressed();
    void hideSlot();

    void on_joinButton_released();

    void on_createButton_released();

private:
    Ui::StartWindow *ui;
    Poker* p;
};

#endif // STARTWINDOW_H
