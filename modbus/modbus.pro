TEMPLATE = lib

TARGET = modbus

QT += core serialport

CONFIG += staticlib

DESTDIR = ../../lib

CONFIG(debug, debug|release) {

    TARGET = $$join(TARGET,,,_d)

} else {

}

INCLUDEPATH = ./include

HEADERS += $$files(./include/*.h)
SOURCES += $$files(./src/*.cpp)
