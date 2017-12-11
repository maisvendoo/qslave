TEMPLATE = lib

DEFINES += MODBUS_LIB

TARGET = modbus

QT += core serialport

DESTDIR = ../../lib

INCLUDEPATH = ./include

HEADERS += $$files(./include/*.h)
SOURCES += $$files(./src/*.cpp)
