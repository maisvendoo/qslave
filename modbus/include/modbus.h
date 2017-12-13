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

#if defined(MODBUS_LIB)
    #define MODBUS_EXPORT Q_DECL_EXPORT
#else
    #define MODBUS_EXPORT Q_DECL_IMPORT
#endif

/*!
 * \class
 * \brief Modbus network emulation class
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class MODBUS_EXPORT ModbusNetwork : public QObject
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

protected:

    /// Serial port configuration
    serial_config_t sp_config;

    /// Serial port object
    QSerialPort     *serialPort;

    /// Connection flag
    bool            is_connected;

    ///
    unsigned long t35;

    /// Slaves list
    QMap<int, Slave *> slaves;

signals:

    void sendDataToSlaves(QByteArray data);

public slots:

    void openConnection();

    void closeConnection();

    void sendData(QByteArray data);

protected slots:

    /// Receive data from serial port
    void receive();

    /// Serial port's errors processing
    void errorSerialPort(QSerialPort::SerialPortError error);
};

#endif // MODBUS_H
