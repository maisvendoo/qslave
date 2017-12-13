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

    case INPUT_REGICTER:

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
void Slave::processFunc(quint8 func, QByteArray data)
{
    switch (func)
    {
    case MB_FUNC_READ_COILS:

        readCoils(data);

        break;

    default:

        break;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::readCoils(QByteArray data)
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

    char *buff = new char[size];
    memset(buff, 0, size);

    for (quint16 i = 0; i < count; i++)
    {
       if (coils[address + i].value)
       {
            buff[i / BYTE_SIZE] |= (1 << (i % BYTE_SIZE));
       }
    }

    reply.append(buff, size);

    delete [] buff;

    quint16 crc = calcCRC16(reply.data(), reply.size());

    reply.append(static_cast<char>(hiByte(crc)));
    reply.append(static_cast<char>(loByte(crc)));

    emit sendData(reply);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Slave::processData(QByteArray data)
{
    request_t   request;

    request.id = static_cast<quint8>(data.at(0));

    if (id == request.id)
    {
        request.func = static_cast<quint8>(data.at(1));
        processFunc(request.func, data);
    }
}
