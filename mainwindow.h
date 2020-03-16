#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <RF24.h>
#include <QTime>
#include <QTimer>

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
    Ui::MainWindow *ui;
    RF24* pRadio;
    // nRF24 Radio Addresses
    // Note: Addresses where the level shifts only one time (that is, 000FFFFFFF)
    //       can often be detected in noise and can give a false detection,
    //       which may give a raised Packet-Error-Rate.
    //       Addresses as a continuation of the preamble (hi-low toggling)
    //       raises the Packet-Error-Rate.
    const uint8_t
    pipes[6][5] = { // Note that NRF24L01(+) expects address LSB first
                      {0x70, 0xCD, 0xAB, 0xCD, 0xAB},
                      {0x71, 0xCD, 0xAB, 0xCD, 0xAB},
                      {0x72, 0xCD, 0xAB, 0xCD, 0xAB},
                      {0x73, 0xCD, 0xAB, 0xCD, 0xAB},
                      {0x74, 0xCD, 0xAB, 0xCD, 0xAB},
                      {0x75, 0xCD, 0xAB, 0xCD, 0xAB}
                  };
    const uint8_t radioChannel = 0x68;
    QTime now;
    QTime previousTime;
    QTime noon;
    QTimer timerCheckTime;
    //===============================================================
    // Commands
    //===============================================================
    typedef enum {
        playRequest = 0x10,
        stopRequest,
        paOnRequest,
        paOffRequest,
        setVolumeRequest,
        baseRequestACmd,
    } Commands;
    //===============================================================
};

#endif // MAINWINDOW_H
