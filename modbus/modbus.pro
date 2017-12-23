TEMPLATE = lib

#DEFINES += MODBUS_LIB

TARGET = modbus

QT += core serialport

CONFIG += staticlib

DESTDIR = ../../lib

INCLUDEPATH = ./include

HEADERS += $$files(./include/*.h)
SOURCES += $$files(./src/*.cpp)
