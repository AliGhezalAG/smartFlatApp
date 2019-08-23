#include "classicbluetoothclient.h"

ClassicBluetoothClient::ClassicBluetoothClient(QString address) : BluetoothClient(address)
{
    qDebug() << "Create socket";
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    dataHandler = new UttBoardDataHandler();
}

ClassicBluetoothClient::~ClassicBluetoothClient()
{
    if (socket)
        socket->disconnectFromService();
    delete socket;
    delete dataHandler;
}

void ClassicBluetoothClient::start()
{
    qDebug() << "Connect to service";

    connect(socket, &QBluetoothSocket::connected, this, &ClassicBluetoothClient::requestData);
    connect(socket, &QBluetoothSocket::readyRead, this, &ClassicBluetoothClient::readSocket);
    connect(this, &ClassicBluetoothClient::dataReceived, this, &ClassicBluetoothClient::processReceivedData);
    connect(dataHandler, &DataHandler::processingFinished, this, &ClassicBluetoothClient::stop);
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
    dataHandler->processData(deviceAddress, receivedData);
}

void ClassicBluetoothClient::requestData()
{
    qDebug() << "Requesting data...";
    QByteArray startPrefix = QByteArray::fromHex(REQUEST_DATA_COMMAND);
    socket->write(startPrefix, 1024);
}

void ClassicBluetoothClient::readSocket()
{
    qDebug() << "Data received...";
    QByteArray data = socket->readAll();
    receivedData.append(data);
    qDebug() << data;
    if (data.endsWith(QByteArray::fromHex(END_OF_DATA_CODE))){
        emit dataReceived();
    }
}

void ClassicBluetoothClient::deviceDisconnected(){
    qInfo() << "Disconnected!";
    emit doneProcessing();
}
