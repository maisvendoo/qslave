//------------------------------------------------------------------------------
//
//      Modbus network software emulator
//      (c) maisvendoo, 11/12/2017
//      Development: Dmitry Pritykin
//
//------------------------------------------------------------------------------
/*!
 * \file
 * \brief Modbus network software emulator
 * \copyright maisvendoo
 * \author Dmitry Pritykin
 * \date 11/12/2017
 */

#include    "modbus.h"

#include    <QThread>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ModbusNetwork::ModbusNetwork(QObject *parent) : QObject(parent)
{
    serialPort = nullptr;
    is_connected = false;
    quiet_time = 5000;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ModbusNetwork::~ModbusNetwork()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ModbusNetwork::init(const serial_config_t &serial_config)
{
    // Save serial port configuration
    sp_config = serial_config;

    serialPort = new QSerialPort(this);

    connect(serialPort, &QSerialPort::readyRead,
            this, &ModbusNetwork::receive);

    connect(serialPort, &QSerialPort::errorOccurred,
            this, &ModbusNetwork::errorSerialPort);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool ModbusNetwork::isConnected() const
{
    return is_connected;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ModbusNetwork::addSlave(Slave *slave)
{
    connect(this, &ModbusNetwork::sendDataToSlaves, slave, &Slave::processData);
    connect(slave, &Slave::sendData, this, &ModbusNetwork::sendData);
    connect(slave, &Slave::logPrint, this, &ModbusNetwork::logSlavePrint);

    slaves.insert(slave->getID(), slave);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QMap<int, Slave *> ModbusNetwork::getSlaves() const
{
    return slaves;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ModbusNetwork::openConnection()
{
    // Check connection state
    if (is_connected)
    {
        if (serialPort->isOpen())
        {
            closeConnection();
        }
    }
    else
    {
        // Set port parameters
        serialPort->setPortName(sp_config.portName);
        serialPort->setBaudRate(static_cast<QSerialPort::BaudRate>(sp_config.baudrate));
        serialPort->setDataBits(static_cast<QSerialPort::DataBits>(sp_config.dataBits));
        serialPort->setStopBits(static_cast<QSerialPort::StopBits>(sp_config.stopBits));
        serialPort->setParity(static_cast<QSerialPort::Parity>(sp_config.getPariry()));

        quiet_time = (unsigned long) (QUIET_TIME_MULTIPLE / sp_config.baudrate);

        if (quiet_time < MIN_QUIET_TIME)
            quiet_time = MIN_QUIET_TIME;

        // Connection
        if (serialPort->open(QIODevice::ReadWrite))
        {
            is_connected = true;
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ModbusNetwork::closeConnection()
{
    serialPort->close();
    is_connected = false;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ModbusNetwork::sendData(QByteArray data)
{
    if (serialPort->isOpen())
    {
        QThread::usleep(quiet_time);

        serialPort->write(data);
        serialPort->flush();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ModbusNetwork::receive()
{
    QByteArray data = serialPort->readAll();

    emit sendDataToSlaves(data);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ModbusNetwork::errorSerialPort(QSerialPort::SerialPortError error)
{
    Q_UNUSED(error)
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ModbusNetwork::logSlavePrint(QString msg)
{
    emit logPrint(msg);
}
