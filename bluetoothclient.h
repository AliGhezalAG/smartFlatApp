#ifndef BLUETOOTHCLIENT_H
#define BLUETOOTHCLIENT_H

#include <QObject>
#include <QDebug>
#include <iostream>
#include <fstream>

using namespace std;

class BluetoothClient : public QObject
{
    Q_OBJECT

public:
    BluetoothClient(QString address);
    virtual ~BluetoothClient();
    virtual void start();

protected slots:
    virtual void stop();
    virtual void processReceivedData();
    virtual void deviceDisconnected();

signals:
    void processingFinished();
    void doneProcessing();

protected:
    QString     deviceAddress;
    ofstream    logFile;
    QByteArray  receivedData;
    int byteArrayToInt(const QByteArray &bytes);
    double byteArrayToDouble(const QByteArray &bytes);
};

#endif // BLUETOOTHCLIENT_H
