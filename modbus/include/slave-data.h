//------------------------------------------------------------------------------
//
//
//
//
//
//------------------------------------------------------------------------------
/*!
 * \file
 * \brief Slave data structures
 * \copyright maisvendoo
 * \author Dmitry Pritykin
 * \date 13/12/2017
 */

#ifndef SLAVE_DATA_H
#define SLAVE_DATA_H

#include    <QtGlobal>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
template <class T>
struct data_unit_t
{
    quint16     address;
    T           value;
    QString     description;

    data_unit_t()
    {
        address = 0;
        value = 0;
        description = "";
    }
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum DataType
{
    COIL,
    DISCRETE_INPUT,
    HOLDING_REGISTER,
    INPUT_REGISTER
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum
{
    DI_INIT_ADDRESS = 0,
    IT_INIT_ADDRESS = 2,
    HL_INIT_ADDRESS = 5,
    CL_INIT_ADDRESS = 16
};

#endif // SLAVE_DATA_H
