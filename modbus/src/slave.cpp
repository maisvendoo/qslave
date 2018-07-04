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

#include    "slave.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Slave::Slave(QObject *parent) : QObject(parent)
{
    id = 0;
    description = "";
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Slave::~Slave()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::setID(int id)
{
    this->id = id;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
quint8 Slave::getID() const
{
    return id;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::setDescription(QString description)
{
    this->description = description;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QString Slave::getDescription() const
{
    return description;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::addDiscreteValue(DataType type, data_unit_t<bool> dv)
{
    switch (type)
    {
    case COIL:

        coils.insert(dv.address, dv);

        break;

    case DISCRETE_INPUT:

        discrete_inputs.insert(dv.address, dv);

        break;

    default:

        break;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::addRegisterValue(DataType type, data_unit_t<quint16> rv)
{
    switch (type)
    {
    case HOLDING_REGISTER:

        holding_registers.insert(rv.address, rv);

        break;

    case INPUT_REGISTER:

        input_registers.insert(rv.address, rv);

        break;

    default:

        break;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::setCoil(quint16 address, bool value)
{
    coils[address].value = value;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::setDiscreteInput(quint16 address, bool value)
{
    discrete_inputs[address].value = value;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::setInputRegister(quint16 address, quint16 value)
{
    input_registers[address].value = value;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::setHoldingRegisters(quint16 address, quint16 value)
{
    holding_registers[address].value = value;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool Slave::getCoil(quint16 address) const
{
    return coils[address].value;
}

QMap<quint16, data_unit_t<bool>> Slave::getCoils() const{
	return coils;
}

QMap<quint16, data_unit_t<bool>> Slave::getDiscreteInputs() const{
	return discrete_inputs;
}

QMap<quint16, data_unit_t<quint16>> Slave::getInputRegisters() const{
	return input_registers;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool Slave::getDiscreteInput(quint16 address) const
{
    return discrete_inputs[address].value;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
quint16 Slave::getInputRegister(quint16 address) const
{
    return input_registers[address].value;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
quint16 Slave::getHoldingRegister(quint16 address) const
{
    return holding_registers[address].value;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int Slave::getCoilsCount() const
{
    return coils.count();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int Slave::getHoldingRegistersCount() const
{
    return holding_registers.count();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int Slave::getDiscreteInputsCount() const
{
    return discrete_inputs.count();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int Slave::getInputRegistersCount() const
{
    return input_registers.count();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QString Slave::getCoilDescription(quint16 address) const
{
    return coils[address].description;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QString Slave::getHoldingRegisterDescription(quint16 address) const
{
    return holding_registers[address].description;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QString Slave::getDiscreteInputDescription(quint16 address) const
{
    return discrete_inputs[address].description;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QString Slave::getInputRegisterDescription(quint16 address) const
{
    return input_registers[address].description;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool Slave::checkRequest(QByteArray data)
{
    if (!checkFuncCode(data.at(FUNC)))
    {
        logPrint("ERROR: Invalid function code " + QString::number(data.at(FUNC)));
        return false;
    }

    quint8 crc_idx = data.size() - 2;
    quint16 crc = word(data.at(crc_idx), data.at(crc_idx + 1));

    if (crc != calcCRC16((quint8 *) data.data(), data.size() - 2))
    {
        logPrint("ERROR: Invalide CRC");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool Slave::checkFuncCode(quint8 func)
{
    switch (func)
    {
    case MB_FUNC_READ_COILS:
    case MB_FUNC_READ_DISCRETE_INPUTS:
    case MB_FUNC_READ_HOLDING_REGISTERS:
    case MB_FUNC_READ_INPUT_REGISTERS:
    case MB_FUNC_WRITE_COIL:
    case MB_FUNC_WRITE_HOLDING_REGISTER:
    case MB_FUNC_WRITE_MULTIPLE_COILS:
    case MB_FUNC_WRITE_MULTIPLE_REGISTERS:

        return true;

    default:

        return false;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::processFunc(quint8 func, QByteArray data)
{
    switch (func)
    {
    case MB_FUNC_READ_COILS:

        readCoils(data);

        break;

    case MB_FUNC_READ_DISCRETE_INPUTS:

        readDiscreteInputs(data);

        break;

    case MB_FUNC_READ_INPUT_REGISTERS:

        readInputRegisters(data);

        break;

    case MB_FUNC_READ_HOLDING_REGISTERS:

        readHoldingRegisters(data);

        break;

    case MB_FUNC_WRITE_COIL:

        writeSingleCoil(data);

        break;

    case MB_FUNC_WRITE_MULTIPLE_COILS:

        writeMultipleCoils(data);

        break;

    case MB_FUNC_WRITE_HOLDING_REGISTER:

        writeSingleRegister(data);

        break;

    case MB_FUNC_WRITE_MULTIPLE_REGISTERS:

        writeMultipleRegisters(data);

        break;

    default:

        break;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::readDiscreteValues(QByteArray data, QMap<quint16, data_unit_t<bool> > &dv)
{
    quint16 address = word(data.at(HI_ADDRESS), data.at(LO_ADDRESS));
    quint16 count = word(data.at(HI_COUNT), data.at(LO_COUNT));

    QByteArray reply;
    reply.clear();

    // Set Slave ID
    reply.append(static_cast<char>(data.at(ID)));

    // Set function code
    reply.append(static_cast<char>(data.at(FUNC)));

    // Calculate data size
    quint16 size = count / BYTE_SIZE;

    if (count % BYTE_SIZE != 0)
        size++;

    reply.append(static_cast<char>(size));

    // Write coils state to PDU
    char *buff = new char[size];
    memset(buff, 0, size);

    for (quint16 i = 0; i < count; i++)
    {
       if (dv[address + i].value)
       {
            buff[i / BYTE_SIZE] |= (1 << (i % BYTE_SIZE));
       }
    }

    reply.append(buff, size);

    delete [] buff;

    // CRC16 calculation
    quint16 crc = calcCRC16((quint8 *) reply.data(), reply.size());

    reply.append(static_cast<char>(hiByte(crc)));
    reply.append(static_cast<char>(loByte(crc)));

    // Reply to master
    emit sendData(reply);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::readRegisterValues(QByteArray data,
                               QMap<quint16, data_unit_t<quint16> > &rv)
{
    quint16 address = word(data.at(HI_ADDRESS), data.at(LO_ADDRESS));
    quint16 count = word(data.at(HI_COUNT), data.at(LO_COUNT));

    QByteArray reply;
    reply.clear();

    reply.append(data.at(ID));
    reply.append(data.at(FUNC));

    quint16 size = count * 2;

    reply.append(static_cast<char>(size));

    for (int i = 0; i < count; i++)
    {
        quint16 value = rv[address + i].value;
        reply.append(hiByte(value));
        reply.append(loByte(value));
    }

    // CRC16 calculation
    quint16 crc = calcCRC16((quint8 *) reply.data() , reply.size());

    reply.append(static_cast<char>(hiByte(crc)));
    reply.append(static_cast<char>(loByte(crc)));

    // Reply to master
    emit sendData(reply);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::writeDiscreteValues(QByteArray data,
                                QMap<quint16, data_unit_t<bool> > &dv)
{
    quint16 address = word(data.at(HI_ADDRESS), data.at(LO_ADDRESS));
    quint16 count = word(data.at(HI_COUNT), data.at(LO_COUNT));

    for (int i = 0; i < count; i++)
    {
        quint8 byte_idx = i / BYTE_SIZE;
        quint8 bit_idx = i % BYTE_SIZE;

        dv[address + i].value = getBit(data.at(7 + byte_idx), bit_idx);
    }

    QByteArray reply;
    reply.clear();

    reply.append(data.data(), LO_COUNT + 1);

    quint16 crc = calcCRC16((quint8 *) reply.data(), reply.size());

    reply.append(static_cast<char>(hiByte(crc)));
    reply.append(static_cast<char>(loByte(crc)));

    emit sendData(reply);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::writeRegisterValues(QByteArray data,
                                QMap<quint16, data_unit_t<quint16> > &rv)
{
    quint16 address = word(data.at(HI_ADDRESS), data.at(LO_ADDRESS));
    quint16 count = word(data.at(HI_COUNT), data.at(LO_COUNT));

    quint8 byte_idx = 7;

    for (int i = 0; i < count; i++)
    {
        quint16 value = word(data.at(byte_idx), data.at(byte_idx + 1));
        rv[address + i].value = value;

        byte_idx += 2;
    }

    QByteArray reply;
    reply.clear();

    reply.append(data.data(), LO_COUNT + 1);

    quint16 crc = calcCRC16((quint8 *) reply.data(), reply.size());

    reply.append(static_cast<char>(hiByte(crc)));
    reply.append(static_cast<char>(loByte(crc)));

    emit sendData(reply);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::readCoils(QByteArray data)
{
    readDiscreteValues(data, coils);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::readDiscreteInputs(QByteArray data)
{
    readDiscreteValues(data, discrete_inputs);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::readInputRegisters(QByteArray data)
{
    readRegisterValues(data, input_registers);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::readHoldingRegisters(QByteArray data)
{
    readRegisterValues(data, holding_registers);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::writeSingleCoil(QByteArray data)
{
    quint16 address = word(data.at(HI_ADDRESS), data.at(LO_ADDRESS));

    coils[address].value = static_cast<bool>(data.at(LO_ADDRESS + 1));

    QByteArray reply;
    reply.clear();

    reply.append(data);

    emit sendData(reply);

    emit updateCoils(id);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::writeMultipleCoils(QByteArray data)
{
    writeDiscreteValues(data, coils);

    emit updateCoils(id);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::writeSingleRegister(QByteArray data)
{
    quint16 address = word(data.at(HI_ADDRESS), data.at(LO_ADDRESS));

    holding_registers[address].value = word(data.at(LO_ADDRESS + 1),
                                            data.at(LO_ADDRESS + 2));

    QByteArray reply;
    reply.clear();

    reply.append(data);

    emit sendData(reply);

    emit updateHoldingRegisters(id);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::writeMultipleRegisters(QByteArray data)
{
    writeRegisterValues(data, holding_registers);

    emit updateHoldingRegisters(id);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::processData(QByteArray data)
{
    // Get slave id from received data
    quint8 id = static_cast<quint8>(data.at(0));

    // Check slvae id
    if (id == this->id)
    {
        // Check received data and process request
        if (checkRequest(data))
        {
            quint8 func = static_cast<quint8>(data.at(1));
            processFunc(func, data);
        }
    }
}
