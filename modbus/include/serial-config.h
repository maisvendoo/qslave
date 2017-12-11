//------------------------------------------------------------------------------
//
//
//
//
//
//------------------------------------------------------------------------------
#ifndef SERIAL_CONFIG_H
#define SERIAL_CONFIG_H

#include    <QString>
#include    <QSerialPort>

/*!
 * \struct
 * \brief Serial port settings
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
struct serial_config_t
{
    /// Port name
    QString     portName;
    /// Baudrate
    int         baudrate;
    /// Data bits count
    int         dataBits;
    /// Stop bits count
    int         stopBits;
    /// Parity bit
    QString     parity;
    /// Flow control
    int         flowControl;
    /// Request timeout
    int         timeout;
    /// Requiest retries number
    int         retries;

    serial_config_t()
    {
        portName = "/dev/ttyUSB0";
        baudrate = 9600;
        dataBits = 8;
        stopBits = 1;
        parity = "None";
        flowControl = 0;
        timeout = 50;
        retries = 1;
    }

    /// Get parity setting by text name
    QSerialPort::Parity getPariry()
    {
        if (parity == "None")
            return QSerialPort::NoParity;

        if (parity == "Odd")
            return QSerialPort::OddParity;

        if (parity == "Even")
            return QSerialPort::EvenParity;

        return QSerialPort::UnknownParity;
    }
};

#endif // SERIAL_CONFIG_H
