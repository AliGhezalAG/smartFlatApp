#include "connexionhandler.h"

ConnexionHandler::ConnexionHandler()
{
    discoveredDevicesList = {};

    deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(5000);

    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &ConnexionHandler::addDevice);
    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &ConnexionHandler::processDevices);
    connect(deviceDiscoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            [=](QBluetoothDeviceDiscoveryAgent::Error error){ this->deviceScanError(error); });
}

ConnexionHandler::~ConnexionHandler()
{
    delete deviceDiscoveryAgent;
    discoveredDevicesList.clear();
}

void ConnexionHandler::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        qInfo() << "The Bluetooth adaptor is powered off, power it on before doing discovery." << endl;
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        qInfo() << "Writing or reading from the device resulted in an error." << endl;
    else
        qInfo() << "An unknown error has occurred." << endl;
}

void ConnexionHandler::addDevice(const QBluetoothDeviceInfo &device)
{
    QString deviceAddress = device.address().toString();
    if(bluetoothLowEnergyDevicesList.contains(deviceAddress) || classicBluetoothDevicesList.contains(deviceAddress)){
        discoveredDevicesList.append(deviceAddress);
        qInfo() << device.address().toString() << " : " << device.name();
    }
}

void ConnexionHandler::start()
{
    while(true){
        discoveredDevicesList.clear();
        deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        {
            QEventLoop loop;
            qInfo() << "start of loop";
            loop.connect(this, SIGNAL(scanProcessingEnded()), SLOT(quit()));
            deviceDiscoveryAgent->start();
            loop.exec();
        }
        qInfo() << "end of loop";
    }
}

void ConnexionHandler::processDevices()
{
    QString currentDeviceAddress = "";
    for(int i=0; i < discoveredDevicesList.size(); i++){
        currentDeviceAddress = discoveredDevicesList.at(i);
        BluetoothClient *currentClient = clientList[currentDeviceAddress];
        if(!currentClient){
            qInfo() << "New device discovered: " << currentDeviceAddress;
            if(bluetoothLowEnergyDevicesList.contains(currentDeviceAddress)){
                currentClient = new BluetoothLowEnergyClient(currentDeviceAddress);
                clientList[currentDeviceAddress] = currentClient;
            } else {
                qInfo() << "processing " << discoveredDevicesList.at(i);
                currentClient = new ClassicBluetoothClient(discoveredDevicesList.at(i));
                clientList[currentDeviceAddress] = currentClient;
            }
        } else {
            qInfo() << "Processing existing device: " << currentDeviceAddress;
        }

        {
            QEventLoop loop;
            loop.connect(currentClient, SIGNAL(doneProcessing()), SLOT(quit()));
            qDebug() << "start loop";
            currentClient->start();
            loop.exec();
        }

    }
    qInfo() << "this is the end dadada!";
    emit scanProcessingEnded();
}
