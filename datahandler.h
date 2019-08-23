#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "init.h"

#include <iostream>
#include <fstream>

class DataHandler : public QObject
{
    Q_OBJECT

public:
    DataHandler();
    virtual ~DataHandler();
    virtual void processData(QString &deviceAddress, QByteArray &receivedData);

protected:
    ofstream    logFile;
    int         byteArrayToInt(const QByteArray &bytes);
    double      byteArrayToDouble(const QByteArray &bytes);

signals:
    void processingFinished();
};

#endif // DATAHANDLER_H
