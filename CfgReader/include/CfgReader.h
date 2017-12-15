//------------------------------------------------------------------------------
//
//		Класс для чтения конфигурационных XML-файлов
//		(с) РГУПС, ВЖД 17/09/2016
//		Разработал: Притыкин Д.Е.
//
//------------------------------------------------------------------------------
/*!
 *  \file
 *  \brief Класс для чтения конфигурационных XML-файлов
 *  \copyright  РГУПС, ВЖД
 *  \author Притыкин Д.Е.
 *  \date 17/09/2016
 */

#ifndef CFGREADER_H
#define CFGREADER_H

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QString>
#include <QFile>

#if defined(CFGREADER_LIB)
# define CFGREADER_EXPORT Q_DECL_EXPORT
#else
# define CFGREADER_EXPORT Q_DECL_IMPORT
#endif

/*!
 *  \class CfgReader
 *  \brief Класс содержит функциональность для обработки конфигом формата XML
 */
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CFGREADER_EXPORT CfgReader
{
public:

    /// Конструктор
    CfgReader();
    /// Деструктор
	~CfgReader();

    /// Загрузка конфигурационного файла
    bool load(QString path);

    /// Найти первую секцию с заданным именем
	QDomNode getFirstSection(QString section);
    /// Найти следующую секцию с таким же именем
	QDomNode getNextSection();

    /// Найти нужное поле в секции
	QDomNode getField(QDomNode secNode, QString field);

    /// Прочесть поле конфига в текстовом формате
    bool getString(QString section, QString field, QString &value);
    /// Прочесть поле конфига в текстовом формате
    bool getString(QDomNode secNode, QString field, QString &value);
    /// Прочесть вещественное поле конфига
    bool getDouble(QString section, QString filed, double &value);
    /// Прочесть вещественное поле конфига
    bool getDouble(QDomNode secNode, QString field, double &value);
    /// Прочесть целочисленное поле конфига
    bool getInt(QString section, QString filed, int &value);
    /// Прочесть целочисленное поле конфига
    bool getInt(QDomNode secNode, QString field, int &value);
    /// Прочесть логическое поле конфига
    bool getBool(QString section, QString field, bool &value);
    /// Прочесть логическое поле конфига
    bool getBool(QDomNode secNode, QString field, bool &value);

private:

    /// Описатель XML-файла
    QFile *file;
    /// Имя XML-файла
	QString file_name;
    /// Описатель XML-документа
	QDomDocument domDoc;
    /// Описатель первого элемента в структуре документа
	QDomElement firstElement;
    /// Текущая нода документа, разбор которой производится
	QDomNode curNode;	
};

#endif // CFGREADER_H

