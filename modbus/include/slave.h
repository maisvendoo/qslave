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



protected:



};

#endif // SLAVE_H
