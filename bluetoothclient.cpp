#include "bluetoothclient.h"

BluetoothClient::BluetoothClient(QString address) : deviceAddress(address){}
BluetoothClient::~BluetoothClient(){}
void BluetoothClient::start(){}
void BluetoothClient::stop(){}
void BluetoothClient::processReceivedData(){}

void BluetoothClient::deviceDisconnected(){
    qInfo() << "Disconnected!";
    emit doneProcessing();
}
