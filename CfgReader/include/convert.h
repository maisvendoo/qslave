//------------------------------------------------------------------------------
//
//		Функции для перевода строк в числа
//		(с) РГУПС, ВЖД 17/09/2016
//		Разработал: Притыкин Д.Е.
//
//------------------------------------------------------------------------------
/*!
 *  \file
 *  \brief Функции для перевода строк в числа
 *  \copyright РГУПС, ВЖД
 *  \author Притыкин Д.Е.
 *  \date  17/09/2016
 */

#ifndef		CONVERT_H
#define		CONVERT_H

#include	<QString>
#include    <QtGlobal>

/// Перевод текста в вещественное число
extern "C" Q_DECL_EXPORT bool TextToDouble(QString text, double &value);
/// Перевод текста в целое число
extern "C" Q_DECL_EXPORT bool TextToInt(QString text, int &value);
/// Удаление всех пробелов из строки
QString EraseSpaces(QString str);

#endif
