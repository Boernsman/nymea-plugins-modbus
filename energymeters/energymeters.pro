include(../plugins.pri)

QT += serialport serialbus

HEADERS += \
    integrationpluginenergymeters.h \
    sdm630modbusrtuconnection.h \
    ../modbus/modbusdatautils.h

SOURCES += \
    integrationpluginenergymeters.cpp \
    sdm630modbusrtuconnection.cpp \
    ../modbus/modbusdatautils.cpp

