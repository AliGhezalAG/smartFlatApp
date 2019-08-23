#include "kinventkforcedatahandler.h"

KinventKForceDataHandler::KinventKForceDataHandler(){}
KinventKForceDataHandler::~KinventKForceDataHandler(){}

void KinventKForceDataHandler::setMeasurementMultiplier(QByteArray &data)
{
    QByteArray val = QByteArray::fromHex(data.toHex(':'));
    measurementMultiplier = val.toInt()/2000000.0;
}

void KinventKForceDataHandler::processData(QString &deviceAddress, QByteArray &receivedData)
{
    int count = 0;
    std::time_t timeDate = std::time(nullptr);
    string timeDateStr = std::ctime(&timeDate);
    timeDateStr.erase(std::remove(timeDateStr.begin(), timeDateStr.end(), '\n'), timeDateStr.end());

    logFile.open ("results.csv", ios::out | ios::app);

    logFile << "Time/date, Device address, Measurement multiplier, Baseline 1, Baseline 2, Timestamp, Nb of measures, Base measure 1, Base measure 2, Measure 1, Measure 2" << endl;

    while(count < receivedData.size()){

        int entryDataNb = byteArrayToInt(receivedData.mid(count,3));

        if (entryDataNb == 0)
            break;

        int entryTimeStamp = byteArrayToInt(receivedData.mid(count+3,4));

        logFile << timeDateStr << ","
                << deviceAddress.toStdString() << ","
                << measurementMultiplier << ","
                << baseline1 << ","
                << baseline2 << ","
                << entryTimeStamp << ","
                << (entryDataNb/4) << ","
                <<  ","
                 <<  ","
                  <<  ","
                   << endl;

        for (int j = count + 7; j < count + static_cast<int>(entryDataNb) ; j+=4) {
            int baseMes1 = byteArrayToInt(receivedData.mid(j,2));
            int baseMes2 = byteArrayToInt(receivedData.mid(j+2,2));
            double mes1 = (baseline1 - baseMes1) * measurementMultiplier;
            double mes2 = (baseline2 - baseMes2) * measurementMultiplier;

            logFile << ","
                    << ","
                    << ","
                    << ","
                    << ","
                    << ","
                    << ","
                    << baseMes1 << ","
                    << baseMes2 << ","
                    << mes1 << ","
                    << mes2
                    << endl;
        }

        count += entryDataNb+7;
    }

    logFile.close();
}
