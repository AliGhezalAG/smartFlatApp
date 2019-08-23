#ifndef BLUETOOTHLOWENERGYCLIENT_H
#define BLUETOOTHLOWENERGYCLIENT_H

#include "bluetoothclient.h"
#include <QLowEnergyController>

#include <iostream>
#include <fstream>

using namespace std;

#define SERVICE_UUID                "{49535343-fe7d-4ae5-8fa9-9fafd205e455}"
#define CHARACTERISTIC_UUID         "{49535343-1e4d-4bd9-ba61-23c647249616}"

#define MAX_SIZE 20 // 20 octets de données max

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
    void processReceivedData();
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
    double                          measurementMultiplier;
    int                             baseline1;
    int                             baseline2;
    bool                            measurementMultiplierSet;
    bool                            baselineSet;
    bool                            timeClockSet;

signals:
    void compteurChange();
    void processTimeClockFinished();
    void processMeasurementMultiplierFinished();
    void processBaselineFinished();
    void deviceConnected();
};

#endif // BLUETOOTHLOWENERGYCLIENT_H
