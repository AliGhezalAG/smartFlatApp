#include "datahandler.h"

DataHandler::DataHandler(){}
DataHandler::~DataHandler(){}
void DataHandler::processData(QString &deviceAddress, QByteArray &receivedData){}

int DataHandler::byteArrayToInt(const QByteArray &bytes){
    QByteArray bytesToHex = bytes.toHex(0);
    bool ok;
    int hex = bytesToHex.toInt(&ok, 16);
    return hex;
}

double DataHandler::byteArrayToDouble(const QByteArray &bytes){
    QByteArray bytesToHex = bytes.toHex(0);
    double hex = bytesToHex.toDouble();
    return hex;
}
