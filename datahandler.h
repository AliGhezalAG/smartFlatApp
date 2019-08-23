#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include <iostream>
#include <fstream>

using namespace std;

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
