//-----------------------------------------------------------------------------
//
//		Функции для перевода строк в числа
//		(с) РГУПС, ВЖД 17/09/2016
//		Разработал: Притыкин Д.Е.
//
//-----------------------------------------------------------------------------
/*!
 *  \file
 *  \brief Функции для перевода строк в числа
 *  \copyright РГУПС, ВЖД
 *  \author Притыкин Д.Е.
 *  \date  17/09/2016
 */

#include	"convert.h"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool TextToDouble(QString text, double &value)
{
	bool validate = false;	// Флаг проверки корректности данных

	// Пытаемся выполнить конвертацию данных
	value = text.toDouble(&validate);
	
	// Если данные не вещественные - выход с предупреждением
	if (!validate)
    {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool TextToInt(QString text, int &value)
{
	bool validate = false;
	value = text.toInt(&validate);
	
	if (!validate)
    {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
QString EraseSpaces(QString str)
{
    QString result = "";

    for (int i = 0; i < str.length(); i++)
    {
        if (str.at(i) != QChar(' '))
            result += str.at(i);
    }

    return result;
}
