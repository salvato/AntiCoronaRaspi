#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pigpiod_if2.h"// The header for using GPIO pins on Raspberry
#include <QDebug>
#include <QSettings>
#include <QThread>


//////////////////////////
/// Hardware configuration
//////////////////////////

// GPIO Numbers are Broadcom (BCM) numbers
// For Raspberry Pi GPIO pin numbering see
// https://pinout.xyz/
//
// +5V on pins 2 or 4 in the 40 pin GPIO connector.
// GND on pins 6, 9, 14, 20, 25, 30, 34 or 39
// in the 40 pin GPIO connector.


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , pUi(new Ui::MainWindow)
    , previousTime(QTime::currentTime())
    , noon(QTime(12, 0, 0, 0))
    , pinPaPower(26) // BCM 26: pin 37 in the 40 pins GPIO connector
    , pinPlay(23)    // BCM 23: pin 16 in the 40 pins GPIO connector
{
    noon = QTime::currentTime().addSecs(30);
    pUi->setupUi(this);

    paPowerStatus = 0;
    playStatus    = 0;

    gpioHostHandle = pigpio_start((char*)"localhost", (char*)"8888");
    if(gpioHostHandle >= 0) {
        if(set_mode(gpioHostHandle, pinPaPower, PI_OUTPUT) < 0) {
            pUi->statusBar->showMessage(QString("Unable to initialize GPIO%1 as Output")
                                       .arg(pinPaPower));
            gpioHostHandle = -1;
            return;
        }
        if(set_pull_up_down(gpioHostHandle, pinPaPower, PI_PUD_OFF) < 0) {
            pUi->statusBar->showMessage(QString("Unable to set GPIO%1 No Pull")
                                       .arg(pinPaPower));
            gpioHostHandle = -1;
            return;
        }
        if(set_mode(gpioHostHandle, pinPlay, PI_OUTPUT) < 0) {
            pUi->statusBar->showMessage(QString("Unable to initialize GPIO%1 as Output")
                                       .arg(pinPlay));
            gpioHostHandle = -1;
            return;
        }
        if(set_pull_up_down(gpioHostHandle, pinPlay, PI_PUD_OFF) < 0) {
            pUi->statusBar->showMessage(QString("Unable to set GPIO%1 No Pull")
                                       .arg(pinPlay));
            gpioHostHandle = -1;
            return;
        }
    }
    else {
        pUi->statusBar->showMessage(QString("Unable to initialize the Pi GPIO."));
        return;
    }
    pUi->editStartTime->setText(noon.toString());

    connect(&timerCheckTime, SIGNAL(timeout()),
            this, SLOT(onTimeToCheckTime()));

    timerCheckTime.start(1000);

    gpio_write(gpioHostHandle, pinPaPower, 1-paPowerStatus);
    gpio_write(gpioHostHandle, pinPlay,    1-playStatus);
    QThread::msleep(100);
    gpio_write(gpioHostHandle, pinPaPower, paPowerStatus);
    gpio_write(gpioHostHandle, pinPlay,    playStatus);

    if(paPowerStatus)
        pUi->powerButton->setText("Power OFF");
    else
        pUi->powerButton->setText("Power ON");
    if(playStatus)
        pUi->playButton->setText("Stop");
    else
        pUi->playButton->setText("Play");
}


MainWindow::~MainWindow() {
    delete pUi;
}


void
MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event)
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    if(gpioHostHandle >= 0)
        pigpio_stop(gpioHostHandle);
}


void
MainWindow::onTimeToCheckTime() {
    now = QTime::currentTime();
    pUi->editCurrentTime->setText(now.toString());
    if((paPowerStatus == 0) && now.secsTo(noon) < 10) {
        paPowerStatus = 1;
        gpio_write(gpioHostHandle, pinPaPower, paPowerStatus);
        pUi->powerButton->setText("Power OFF");
    }
    if((previousTime < noon) && (now >= noon)) {
        playStatus = 1;
        gpio_write(gpioHostHandle, pinPlay, playStatus);
        pUi->playButton->setText("Stop");
    }
    previousTime = now;
}


void
MainWindow::on_powerButton_clicked() {
    paPowerStatus = 1 - paPowerStatus;
    gpio_write(gpioHostHandle, pinPaPower, paPowerStatus);
    if(paPowerStatus)
        pUi->powerButton->setText("Power OFF");
    else
        pUi->powerButton->setText("Power ON");
}


void
MainWindow::on_playButton_clicked() {
    playStatus = 1 - playStatus;
    gpio_write(gpioHostHandle, pinPlay, playStatus);
    if(playStatus)
        pUi->playButton->setText("Stop");
    else
        pUi->playButton->setText("Play");
}
