#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


//////////////////////////
/// Hardware configuration
//////////////////////////


// PIN 	NRF24L01 	RPI         RPi-P1 Connector
// 1     GND         rpi-gnd 	(25)
// 2 	VCC         rpi-3v3 	(17)
// 3 	CE          rpi-gpio22 	(15)
// 4 	CSN         rpi-gpio8 	(24)
// 5 	SCK         rpi-sckl 	(23)
// 6 	MOSI        rpi-mosi 	(19)
// 7 	MISO        rpi-miso 	(21)
// 8 	IRQ         -            -


// Radio CE Pin, CSN Pin, SPI Speed
// See http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939 and the related enumerations for pin information.


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pRadio(Q_NULLPTR)
    , previousTime(QTime::currentTime())
    , noon(QTime(12, 0, 0, 0))
{
    ui->setupUi(this);
    // Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
    pRadio = new RF24(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);
    pRadio->begin();                           // Setup and configure rf radio
    pRadio->setChannel(radioChannel);
    pRadio->setPALevel(RF24_PA_MAX);
    pRadio->setDataRate(RF24_2MBPS);
    pRadio->setAutoAck(1);                     // Ensure autoACK is enabled
    pRadio->setRetries(5, 15);                 // Optionally, increase the delay between retries & # of retries
    pRadio->setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
    pRadio->enableDynamicAck();
    if(!pRadio->isChipConnected()) {
        qDebug() << "nRF24L01 not Connected: exiting...";
//        return;
//        exit(EXIT_FAILURE);
    }
    else
        pRadio->printDetails();
    connect(&timerCheckTime, SIGNAL(timeout()),
            this, SLOT(onTimeToCheckTime()));
    timerCheckTime.start(1000);
}


MainWindow::~MainWindow() {
    delete ui;
}


void
MainWindow::onTimeToCheckTime() {
    now = QTime::currentTime();
    if(previousTime < noon && now >= noon) {
        //Send command to play
        qDebug() << "Now !!!" << now.toString();
    }
    previousTime = now;
}
