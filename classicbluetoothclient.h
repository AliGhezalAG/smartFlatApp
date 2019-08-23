#ifndef CLASSICBLUETOOTHCLIENT_H
#define CLASSICBLUETOOTHCLIENT_H

#include "bluetoothclient.h"
#include "uttboarddatahandler.h"

#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>
#include <QBluetoothSocket>

class ClassicBluetoothClient : public BluetoothClient
{
    Q_OBJECT

public:
    ClassicBluetoothClient(QString address);
    ~ClassicBluetoothClient();
    void start();

private slots:
    void readSocket();
    void requestData();
    void processReceivedData();
    void deviceDisconnected();
    void stop();

private:
    QBluetoothSocket        *socket;
    QBluetoothLocalDevice   *localDevice;
    UttBoardDataHandler     *dataHandler;

signals:
    void dataReceived();
    void doneProcessing();

};

#endif // CLASSICBLUETOOTHCLIENT_H
