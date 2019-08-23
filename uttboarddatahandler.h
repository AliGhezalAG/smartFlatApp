#ifndef UTTBOARDDATAHANDLER_H
#define UTTBOARDDATAHANDLER_H

#include "datahandler.h"
#include <QtCore>

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
    QString      address;
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
            {"address", address},
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

class UttBoardDataHandler : public DataHandler
{
    Q_OBJECT

public:
    UttBoardDataHandler();
    ~UttBoardDataHandler();
    void processData(QString &deviceAddress, QByteArray &receivedData);

private:
    QByteArray          openingPacket;
    QByteArray          closingPacket;
    QList<QByteArray>   recordPacketList;
    Acquisition         *acquisition;
    void extractPackets(QByteArray &receivedData);
    void processGeneralInfo(QString &deviceAddress);
    void processRecordPackets();
};

#endif // UTTBOARDDATAHANDLER_H
