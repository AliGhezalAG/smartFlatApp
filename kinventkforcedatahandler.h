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

private:
    double  measurementMultiplier;
    int     baseline1;
    int     baseline2;
    void    setMeasurementMultiplier(QByteArray &data);
    void    setBaseline(QByteArray &data);
};

#endif // KINVENTKFORCEDATAHANDLER_H
