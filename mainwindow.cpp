#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


//////////////////////////
/// Hardware configuration
//////////////////////////



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , pUi(new Ui::MainWindow)
    , previousTime(QTime::currentTime())
    , noon(QTime(12, 0, 0, 0))
{
    pUi->setupUi(this);

    connect(&timerCheckTime, SIGNAL(timeout()),
            this, SLOT(onTimeToCheckTime()));

    timerCheckTime.start(1000);
}


MainWindow::~MainWindow() {
    delete pUi;
}


void
MainWindow::onTimeToCheckTime() {
    now = QTime::currentTime();
    pUi->editTime->setText(now.toString());
    if(previousTime < noon && now >= noon) {
    }
    previousTime = now;
}
