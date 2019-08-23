#include "bluetoothclient.h"

BluetoothClient::BluetoothClient(QString address) : deviceAddress(address){}
BluetoothClient::~BluetoothClient(){}
void BluetoothClient::start(){}
void BluetoothClient::stop(){}
void BluetoothClient::processReceivedData(){}

int BluetoothClient::byteArrayToInt(const QByteArray &bytes){
    QByteArray bytesToHex = bytes.toHex(0);
    bool ok;
    int hex = bytesToHex.toInt(&ok, 16);
    return hex;
}

double BluetoothClient::byteArrayToDouble(const QByteArray &bytes){
    QByteArray bytesToHex = bytes.toHex(0);
    double hex = bytesToHex.toDouble();
    return hex;
}

void BluetoothClient::deviceDisconnected(){
    qInfo() << "Disconnected!";
    emit doneProcessing();
}
