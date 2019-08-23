#ifndef BLUETOOTHCLIENT_H
#define BLUETOOTHCLIENT_H

#include "init.h"

#include <QDebug>
#include "datahandler.h"

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
};

#endif // BLUETOOTHCLIENT_H
