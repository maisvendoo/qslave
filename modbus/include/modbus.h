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

#ifndef MODBUS_H
#define MODBUS_H

#include    <QObject>
#include    <QSerialPort>
#include    <QMap>

#include    "serial-config.h"
#include    "slave.h"

/*!
 * \enum
 * \brief Constants for silence time interval calculation
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum
{
    SILENCE_INTERVAL_MULTIPLE = 28000000L, ///< 8 bits * 1000000 us * 3.5
    MIN_SILENCE_INTERVAL = 1750
};

/*!
 * \class
 * \brief Modbus network emulation class
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class ModbusNetwork : public QObject
{
    Q_OBJECT

public:

    explicit ModbusNetwork(QObject *parent = Q_NULLPTR);
    virtual ~ModbusNetwork();

    /// Network initialization
    void init(const serial_config_t &serial_config);

    /// Check connection state
    bool isConnected() const;

    /// Add slave
    void addSlave(Slave *slave);

    /// Get slaves list
    QMap<int, Slave *> getSlaves() const;

    /// Clean slaves list
    void clear();

protected:

    /// Serial port configuration
    serial_config_t sp_config;

    /// Serial port object
    QSerialPort     *serialPort;

    /// Connection flag
    bool            is_connected;

    /// Silence interval betwen receive adn transmit
    unsigned long silence_interval;

    /// Slaves list
    QMap<int, Slave *> slaves;

signals:

    /// Send received data array to all slaves
    void sendDataToSlaves(QByteArray data);

    /// Print message to log
    void logPrint(QString msg);

public slots:

    /// Open connection with serial port
    void openConnection();
    /// Close serial connection
    void closeConnection();
    /// Send data to master
    void sendData(QByteArray data);

protected slots:

    /// Receive data from serial port
    void receive();

    /// Serial port's errors processing
    void errorSerialPort(QSerialPort::SerialPortError error);

    /// Print log message form slave
    void logSlavePrint(QString msg);
};

#endif // MODBUS_H
