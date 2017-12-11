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

protected:


};

#endif // MODBUS_H
