//------------------------------------------------------------------------------
//
//      Modbus slave device emulation
//      (c) maisvendoo, 11/12/2017
//      Developer: Dmitry Pritykin
//
//------------------------------------------------------------------------------
/*!
 * \file
 * \brief
 * \copyright
 * \author
 * \date
 */

#ifndef SLAVE_H
#define SLAVE_H

#include    <QObject>
#include    <QMap>

#include    "request.h"
#include    "slave-data.h"

#if defined(MODBUS_LIB)
    #define SLAVE_EXPORT Q_DECL_EXPORT
#else
    #define SLAVE_EXPORT Q_DECL_IMPORT
#endif

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class SLAVE_EXPORT Slave : public QObject
{
    Q_OBJECT

public:

    explicit Slave(QObject *parent = Q_NULLPTR);
    virtual ~Slave();

    /// Set slave id
    void setID(int id);
    /// Get slave id
    quint8 getID() const;

    /// Set memory model configuration
    void setMemoryConfig(DataType type, int count);

    void setCoil(quint16 address, bool value);

protected:

    /// Slave ID
    quint8    id;

    /// Coils
    QMap<quint16, data_unit_t<bool>>        coils;
    /// Discrete inputs
    QMap<quint16, data_unit_t<bool>>        discrete_inputs;
    /// Holding registers
    QMap<quint16, data_unit_t<quint16>>     holding_registers;
    /// Input registeres
    QMap<quint16, data_unit_t<quint16>>     input_registers;

    /// Process Modbus function
    void processFunc(quint8 func, QByteArray data);

    /*!    Modbus functions     */

    /// Read coils
    void readCoils(QByteArray data);

signals:

    void sendData(QByteArray data);

public slots:

    /// Process data
    void processData(QByteArray data);
};

#endif // SLAVE_H
