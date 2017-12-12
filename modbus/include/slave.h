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

#include    "request.h"

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

    void setID(int id);

    quint8 getID() const;

protected:

    /// Slave ID
    quint8    id;

public slots:

    /// Process data
    void processData(QByteArray data);
};

#endif // SLAVE_H
