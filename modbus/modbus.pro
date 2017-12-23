TEMPLATE = lib

TARGET = modbus

QT += core serialport

CONFIG += staticlib

DESTDIR = ../../lib

INCLUDEPATH = ./include

HEADERS += $$files(./include/*.h)
SOURCES += $$files(./src/*.cpp)
