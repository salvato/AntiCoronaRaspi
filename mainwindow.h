#pragma once

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include "ledindicator.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onTimeToCheckTime();

private:
    Ui::MainWindow* pUi;
    QTime now;
    QTime previousTime;
    QTime noon;
    QTimer timerCheckTime;
};
