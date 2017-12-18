//------------------------------------------------------------------------------
//
//      Modbus slave device emulation
//      (c) maisvendoo, 11/12/2017
//      Developer: Dmitry Pritykin
//
//------------------------------------------------------------------------------
/*!
 * \file
 * \brief Modbus slave device emulation
 * \copyright maisvendoo
 * \author Dmitry Pritykin
 * \date 11/12/2017
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

    /// Set description
    void setDescription(QString description);

    /// Get description
    QString getDescription() const;

    /// Set memory model configuration
    void setMemoryConfig(DataType type, int count);

    /// Add discrete value
    void addDiscreteValue(DataType type, data_unit_t<bool> dv);

    /// Add register value
    void addRegisterValue(DataType type, data_unit_t<quint16> rv);

    /// Set device data
    void setCoil(quint16 address, bool value);
    void setDiscreteInput(quint16 address, bool value);
    void setInputRegister(quint16 address, quint16 value);
    void setHoldingRegisters(quint16 address, quint16 value);

    /// Get device data
    bool getCoil(quint16 address) const;
    bool getDiscreteInput(quint16 address) const;
    quint16 getInputRegister(quint16 address) const;
    quint16 getHoldingRegister(quint16 address) const;

    int getCoilsCount() const;
    int getHoldingRegistersCount() const;
    int getDiscreteInputsCount() const;
    int getInputRegistersCount() const;

    QString getCoilDescription(quint16 address) const;
    QString getHoldingRegisterDescription(quint16 address) const;
    QString getDiscreteInputDescription(quint16 address) const;
    QString getInputRegisterDescription(quint16 address) const;

protected:

    /// Slave ID
    quint8      id;

    /// Slave description
    QString     description;

    /// Coils
    QMap<quint16, data_unit_t<bool>>        coils;
    /// Discrete inputs
    QMap<quint16, data_unit_t<bool>>        discrete_inputs;
    /// Holding registers
    QMap<quint16, data_unit_t<quint16>>     holding_registers;
    /// Input registeres
    QMap<quint16, data_unit_t<quint16>>     input_registers;

    /// Check request data
    bool checkRequest(QByteArray data);

    /// Check function code
    bool checkFuncCode(quint8 func);

    /// Process Modbus function
    void processFunc(quint8 func, QByteArray data);

    /*!    Modbus functions     */

    void readDiscreteValues(QByteArray data, QMap<quint16, data_unit_t<bool>> &dv);
    void readRegisterValues(QByteArray data, QMap<quint16, data_unit_t<quint16>> &rv);

    void writeDiscreteValues(QByteArray data, QMap<quint16, data_unit_t<bool>> &dv);
    void writeRegisterValues(QByteArray data, QMap<quint16, data_unit_t<quint16>> &rv);

    /// Read coils
    void readCoils(QByteArray data);

    /// Read discrete inputs
    void readDiscreteInputs(QByteArray data);

    /// Read input registers
    void readInputRegisters(QByteArray data);

    /// Read holding registers
    void readHoldingRegisters(QByteArray data);

    /// Write single coil
    void writeSingleCoil(QByteArray data);

    /// Write multiplle coils
    void writeMultipleCoils(QByteArray data);

    /// Write single register
    void writeSingleRegister(QByteArray data);

    /// Write multiple registers
    void writeMultipleRegisters(QByteArray data);

signals:

    void sendData(QByteArray data);

    void logPrint(QString msg);

    void updateCoils(quint8 id);
    void updateHoldingRegisters(quint8 id);

public slots:

    /// Process data
    void processData(QByteArray data);
};

#endif // SLAVE_H
