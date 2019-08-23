#include "uttboarddatahandler.h"

UttBoardDataHandler::UttBoardDataHandler(){}
UttBoardDataHandler::~UttBoardDataHandler(){}

void UttBoardDataHandler::processData(QString &deviceAddress, QByteArray &receivedData)
{
    acquisition = new Acquisition();
    extractPackets(receivedData);
    processGeneralInfo(deviceAddress);
    processRecordPackets();

    auto doc = QJsonDocument(acquisition->toJson());
    logFile.open ("results.json", ios::out | ios::app);
    logFile << doc.toJson().constData();
    logFile.close();

    qDebug() << acquisition->toJson();

    emit processingFinished();
}

void UttBoardDataHandler::extractPackets(QByteArray &receivedData)
{
    qInfo() << "Extracting packets...";

    QByteArray openingPacketStartPrefix = QByteArray::fromHex(OPENING_PACKET_START_PREFIX);
    QByteArray openingPacketEndPrefix = QByteArray::fromHex(OPENING_PACKET_END_PREFIX);
    int openingPacketStartIndex = receivedData.indexOf(openingPacketStartPrefix)+2;
    int openingPacketEndIndex = receivedData.indexOf(openingPacketEndPrefix);
    openingPacket = receivedData.mid(openingPacketStartIndex, PACKET_SIZE);

    QByteArray closingPacketStartPrefix = QByteArray::fromHex(CLOSING_PACKET_START_PREFIX);
    int closingPacketStartIndex = receivedData.indexOf(closingPacketStartPrefix)+2;
    closingPacket = receivedData.mid(closingPacketStartIndex, PACKET_SIZE);

    QByteArray recordPacket = receivedData.mid(openingPacketEndIndex+2, closingPacketStartIndex-openingPacketEndIndex-4);
    recordPacketList = {};
    int recordPacketStartIndex = 2;

    while(recordPacketStartIndex < recordPacket.size()){
        recordPacketList.append(recordPacket.mid(recordPacketStartIndex, PACKET_SIZE));
        recordPacketStartIndex += 16;
    }
}

void UttBoardDataHandler::processGeneralInfo(QString &deviceAddress)
{
    acquisition->address = deviceAddress;
    acquisition->serialNumber = byteArrayToInt(openingPacket.mid(0, 4));
    acquisition->samplingFrequency = byteArrayToInt(openingPacket.mid(4, 1));
    acquisition->scaleMode = "Normal mode";
    acquisition->versionNumber = byteArrayToInt(openingPacket.mid(6, 1));
    acquisition->batteryLevel = byteArrayToInt(openingPacket.mid(7, 1));
    acquisition->distanceBetweenX = byteArrayToInt(openingPacket.mid(8, 2));
    acquisition->distanceBetweenY = byteArrayToInt(openingPacket.mid(10, 2));
    acquisition->distanceUnit = "Millimeters";
    acquisition->frequencyUnit = "Hz";
    acquisition->pressureOrder = "sensor1; sensor2; sensor3; sensor4";
    acquisition->pressureUnit = "Newton";

    acquisition->recordNumber = byteArrayToInt(closingPacket.mid(0, 4));
    acquisition->errorCode = byteArrayToInt(closingPacket.mid(4, 2));
    acquisition->weightValue = byteArrayToInt(closingPacket.mid(6, 2));
    acquisition->impedance = byteArrayToInt(closingPacket.mid(8, 2));
}

void UttBoardDataHandler::processRecordPackets()
{
    QListIterator<QByteArray> i(recordPacketList);
    QByteArray currentPacket;

    while (i.hasNext()){
        currentPacket = i.next();
        Packet *packet = new Packet();
        packet->recordNumber = byteArrayToInt(currentPacket.mid(0, 4));
        packet->timeStamp = packet->recordNumber / acquisition->samplingFrequency;
        QList<double> records = {};
        packet->pressureMeasures.append(QJsonValue(byteArrayToDouble(currentPacket.mid(4, 2))));
        packet->pressureMeasures.append(QJsonValue(byteArrayToDouble(currentPacket.mid(6, 2))));
        packet->pressureMeasures.append(QJsonValue(byteArrayToDouble(currentPacket.mid(8, 2))));
        packet->pressureMeasures.append(QJsonValue(byteArrayToDouble(currentPacket.mid(10, 2))));
        acquisition->measurePackets.append(packet->toJson());
    }
}
