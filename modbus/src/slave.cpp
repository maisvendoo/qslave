#include    "slave.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Slave::Slave(QObject *parent) : QObject(parent)
{

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
void Slave::setMemoryConfig(DataType type, int count)
{
    switch (type)
    {
    case COIL:

        coils.clear();

        for (int i = 0; i < count; i++)
        {
            data_unit_t<bool> coil;
            coil.address = static_cast<quint16>(CL_INIT_ADDRESS + i);
            coils.insert(coil.address, coil);
        }

        break;

    case DISCRETE_INPUT:

        discrete_inputs.clear();

        for (int i = 0; i < count; i++)
        {
            data_unit_t<bool> discrete_input;
            discrete_input.address = static_cast<quint16>(DI_INIT_ADDRESS + i);
            discrete_inputs.insert(discrete_input.address, discrete_input);
        }

        break;

    case INPUT_REGISTER:

        input_registers.clear();

        for (int i = 0; i < count; i++)
        {
            data_unit_t<quint16> input_register;
            input_register.address = static_cast<quint16>(IT_INIT_ADDRESS + i);
            input_registers.insert(input_register.address, input_register);
        }

        break;

    case HOLDING_REGISTER:

        holding_registers.clear();

        for (int i = 0; i < count; i++)
        {
            data_unit_t<quint16> holding_register;
            holding_register.address = static_cast<quint16>(HL_INIT_ADDRESS + i);
            holding_registers.insert(holding_register.address, holding_register);
        }

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
bool Slave::checkRequest(QByteArray data)
{

    return true;
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
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::writeMultipleCoils(QByteArray data)
{
    writeDiscreteValues(data, coils);
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
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::writeMultipleRegisters(QByteArray data)
{
    writeRegisterValues(data, holding_registers);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::processData(QByteArray data)
{
    quint8 id = static_cast<quint8>(data.at(0));

    if (id == this->id)
    {
        if (checkRequest(data))
        {
            quint8 func = static_cast<quint8>(data.at(1));
            processFunc(func, data);
        }
    }
}
