#include "classicbluetoothclient.h"

ClassicBluetoothClient::ClassicBluetoothClient(QString address) : BluetoothClient(address)
{
    qDebug() << "Create socket";
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
}

ClassicBluetoothClient::~ClassicBluetoothClient()
{
    if (socket)
        socket->disconnectFromService();
    delete socket;
    delete acquisition;
}

void ClassicBluetoothClient::start()
{
    qDebug() << "Connect to service";

    connect(socket, &QBluetoothSocket::connected, this, &ClassicBluetoothClient::requestData);
    connect(socket, &QBluetoothSocket::readyRead, this, &ClassicBluetoothClient::readSocket);
    connect(this, &ClassicBluetoothClient::dataReceived, this, &ClassicBluetoothClient::processReceivedData);
    connect(this, &ClassicBluetoothClient::processingFinished, this, &ClassicBluetoothClient::stop);
    connect(socket, &QBluetoothSocket::disconnected, this, &ClassicBluetoothClient::deviceDisconnected);

    QBluetoothAddress uttBoardAddress(deviceAddress);
    socket->connectToService(uttBoardAddress, 1);
    qDebug() << "Connect to service done";
}

void ClassicBluetoothClient::stop()
{
    qDebug() << "Disconnect from service";
    if (socket)
        socket->disconnectFromService();
    qDebug() << "Disconnect from service done!";
}

void ClassicBluetoothClient::processReceivedData()
{
    qDebug() << "Processing received data...";
    extractPackets();
    acquisition = new Acquisition();
    acquisition->serialNumber = byteArrayToInt(openingPacket.mid(0, 4));
    acquisition->samplingFrequency = byteArrayToInt(openingPacket.mid(4, 1));
    acquisition->scaleMode = "Normal mode";
    acquisition->versionNumber = byteArrayToInt(openingPacket.mid(6, 1));
    acquisition->batteryLevel = byteArrayToInt(openingPacket.mid(7, 1));
    acquisition->distanceBetweenX = byteArrayToInt(openingPacket.mid(8, 2));
    acquisition->distanceBetweenY = byteArrayToInt(openingPacket.mid(10, 2));

    acquisition->recordNumber = byteArrayToInt(closingPacket.mid(0, 4));
    acquisition->errorCode = byteArrayToInt(closingPacket.mid(4, 2));
    acquisition->weightValue = byteArrayToInt(closingPacket.mid(6, 2));
    acquisition->impedance = byteArrayToInt(closingPacket.mid(8, 2));

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

    auto doc = QJsonDocument(acquisition->toJson());
    logFile.open ("results.json", ios::out | ios::app);
    logFile << doc.toJson().constData();
    logFile.close();

    qInfo() << acquisition->toJson();
    emit processingFinished();
}

void ClassicBluetoothClient::extractPackets()
{
    qInfo() << "Extracting packets...";

    QByteArray openingPacketStartPrefix = QByteArray::fromHex("5049");
    QByteArray openingPacketEndPrefix = QByteArray::fromHex("4950");
    int openingPacketStartIndex = receivedData.indexOf(openingPacketStartPrefix)+2;
    int openingPacketEndIndex = receivedData.indexOf(openingPacketEndPrefix);
    openingPacket = receivedData.mid(openingPacketStartIndex, PACKET_SIZE);

    QByteArray closingPacketStartPrefix = QByteArray::fromHex("5052");
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

void ClassicBluetoothClient::requestData()
{
    qDebug() << "Requesting data...";
    QByteArray startPrefix = QByteArray::fromHex("5253");
    socket->write(startPrefix, 1024);
}

void ClassicBluetoothClient::readSocket()
{
    qDebug() << "Data received...";
    QByteArray data = socket->readAll();
    receivedData.append(data);
    qDebug() << data;
    if (data.endsWith(QByteArray::fromHex("5250"))){
        qInfo() << "this is the end!!";
        emit dataReceived();
    }

    QByteArray endPrefix = QByteArray::fromHex("5350");
    socket->write(endPrefix, 1024);
}

void ClassicBluetoothClient::deviceDisconnected(){
    qInfo() << "Disconnected!";
    emit doneProcessing();
}
