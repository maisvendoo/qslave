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

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class Slave : public QObject
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

    /// Add discrete value
    void addDiscreteValue(DataType type, data_unit_t<bool> dv);

    /// Add register value
    void addRegisterValue(DataType type, data_unit_t<quint16> rv);

    /// Set coil value
    void setCoil(quint16 address, bool value);
    /// Set discrete input value
    void setDiscreteInput(quint16 address, bool value);
    /// Set input register value
    void setInputRegister(quint16 address, quint16 value);
    /// Set holding register value
    void setHoldingRegisters(quint16 address, quint16 value);

    /// Get coil value
    bool getCoil(quint16 address) const;

	QMap<quint16, data_unit_t<bool>> getCoils() const;
	QMap<quint16, data_unit_t<bool>> getDiscreteInputs() const;
	QMap<quint16, data_unit_t<quint16>> getInputRegisters() const;

    /// Get discrete input value
    bool getDiscreteInput(quint16 address) const;
    /// Get input register value
    quint16 getInputRegister(quint16 address) const;
    /// Get holding register value
    quint16 getHoldingRegister(quint16 address) const;

    /// Get coils count
    int getCoilsCount() const;
    /// Get  holding registers count
    int getHoldingRegistersCount() const;
    /// Get discrete inputs count
    int getDiscreteInputsCount() const;
    /// Get input registers count
    int getInputRegistersCount() const;

    /// Get coil description
    QString getCoilDescription(quint16 address) const;
    /// Get input register description
    QString getHoldingRegisterDescription(quint16 address) const;
    /// Get discrete input description
    QString getDiscreteInputDescription(quint16 address) const;
    /// Get input register description
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

    /// Send data to master device
    void sendData(QByteArray data);

    /// Print message to log
    void logPrint(QString msg);

    /// Update coils values in user intarface
    void updateCoils(quint8 id);
    /// Update holding registers values in user interface
    void updateHoldingRegisters(quint8 id);

public slots:

    /// Process data
    void processData(QByteArray data);
};

#endif // SLAVE_H
