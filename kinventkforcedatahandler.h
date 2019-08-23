#ifndef KINVENTKFORCEDATAHANDLER_H
#define KINVENTKFORCEDATAHANDLER_H

#include "datahandler.h"

class KinventKForceDataHandler : public DataHandler
{
    Q_OBJECT

public:
    KinventKForceDataHandler();
    ~KinventKForceDataHandler();
    void processData(QString &deviceAddress, QByteArray &receivedData);
    void setMeasurementMultiplier(const QByteArray &data);
    void setBaseline(const QByteArray &data);

private:
    double  measurementMultiplier;
    int     baseline1;
    int     baseline2;

signals:
    void processMeasurementMultiplierFinished();
    void processBaselineFinished();
};

#endif // KINVENTKFORCEDATAHANDLER_H
