#ifndef CLASSICBLUETOOTHCLIENT_H
#define CLASSICBLUETOOTHCLIENT_H

#include "bluetoothclient.h"

#include <QObject>
#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>
#include <QBluetoothSocket>
#include <QtCore>

#include <iostream>
#include <fstream>

QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceInfo)

QT_USE_NAMESPACE

using namespace std;

#define PACKET_SIZE                     12

struct Packet {
    int recordNumber;
    double timeStamp;
    QJsonArray pressureMeasures;

    QJsonObject toJson() const {
        return {
            {"recordNumber", recordNumber},
            {"timeStamp", timeStamp},
            {"pressureMeasures", pressureMeasures}
        };
    }
};

struct Acquisition {
    int          serialNumber;
    int          samplingFrequency;
    QString      frequencyUnit;
    QString      scaleMode;
    double        versionNumber;
    int          batteryLevel;
    int          distanceBetweenX;
    int          distanceBetweenY;
    QString      distanceUnit;
    QString      pressureOrder;
    QString      pressureUnit;
    int          recordNumber;
    int          errorCode;
    int          weightValue;
    int          impedance;
    QJsonArray   measurePackets;

    QJsonObject toJson() const {
        return {
            {"serialNumber", serialNumber},
            {"samplingFrequency", samplingFrequency},
            {"frequencyUnit", frequencyUnit} ,
            {"scaleMode", scaleMode},
            {"versionNumber", versionNumber},
            {"batteryLevel", batteryLevel},
            {"distanceBetweenX", distanceBetweenX},
            {"distanceBetweenY", distanceBetweenY} ,
            {"distanceUnit", distanceUnit},
            {"pressureOrder", pressureOrder},
            {"pressureUnit", pressureUnit},
            {"recordNumber", recordNumber},
            {"errorCode", errorCode} ,
            {"weightValue", weightValue},
            {"impedance", impedance},
            {"measurePackets", measurePackets}
        };
    }
};

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
    QBluetoothSocket                *socket;
    QBluetoothLocalDevice           *localDevice;
    QByteArray                      openingPacket;
    QByteArray                      closingPacket;
    QList<QByteArray>               recordPacketList;
    Acquisition                     *acquisition;
    void extractPackets();

signals:
    void dataReceived();
    void doneProcessing();
    void processingFinished();

};

#endif // CLASSICBLUETOOTHCLIENT_H
