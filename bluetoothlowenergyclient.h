#ifndef BLUETOOTHLOWENERGYCLIENT_H
#define BLUETOOTHLOWENERGYCLIENT_H

#include "bluetoothclient.h"
#include "kinventkforcedatahandler.h"
#include <QLowEnergyController>

class BluetoothLowEnergyClient : public BluetoothClient
{
    Q_OBJECT

public:
    BluetoothLowEnergyClient(QString address);
    ~BluetoothLowEnergyClient();
    void start();
    Q_INVOKABLE void read();
    Q_INVOKABLE void write(const QByteArray &data);
    Q_INVOKABLE void gererNotification(bool notification);
    QLowEnergyController::ControllerState getControllerState();

protected slots:
    void stop();
    void deviceDisconnected();
    void connecterService(QLowEnergyService *service);
    void ajouterService(QBluetoothUuid serviceUuid);
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);
    void serviceCharacteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void deviceDiscoverServices();
    void setTimeClock();
    void getMeasurementMultiplier();
    void getBaseline();
    void getData();

private:
    QLowEnergyController           *m_controller;
    QLowEnergyService              *m_service;
    QLowEnergyCharacteristic        m_txCharacteristic;
    QLowEnergyCharacteristic        m_rxCharacteristic;
    bool                            measurementMultiplierSet;
    bool                            baselineSet;
    bool                            timeClockSet;
    KinventKForceDataHandler        *dataHandler;

signals:
    void compteurChange();
    void processTimeClockFinished();
    void processMeasurementMultiplierFinished();
    void processBaselineFinished();
    void deviceConnected();
};

#endif // BLUETOOTHLOWENERGYCLIENT_H
