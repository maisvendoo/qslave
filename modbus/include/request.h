//------------------------------------------------------------------------------
//
//      Master request structure
//      (c) maisvendoo, 11/12/2017
//      Deleloper: Dmitry Pritykin
//
//------------------------------------------------------------------------------
/*!
 * \file
 * \brief Master request structure
 * \copyright maisvendoo
 * \author Dmitry Pritykin
 * \date 11/12/2017
 */

#ifndef REQUEST_H
#define REQUEST_H

#include    <QtGlobal>

/*!
 * \struct
 * \brief Request data structure
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
struct request_t
{
    quint8      id;
    quint8      func;
    quint16     address;
    quint16     count;
    quint8      *data;
    quint16     crc;

    request_t()
    {
        id = 0;
        func = 0;
        address = 0;
        count = 0;
        data = nullptr;
        crc = 0;
    }
};

#endif // REQUEST_H
