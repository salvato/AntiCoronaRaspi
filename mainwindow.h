#pragma once

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include "pigpiod_if2.h" // The header for using GPIO pins on Raspberry


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private slots:
    void onTimeToCheckTime();
    void on_powerButton_clicked();
    void on_playButton_clicked();

private:
    Ui::MainWindow* pUi;

    QTime  now;
    QTime  previousTime;
    QTime  noon;
    QTimer timerCheckTime;

    int    gpioHostHandle;
    int    pinPaPower;
    int    pinPlay;

    int    paPowerStatus;
    int    playStatus;
};
