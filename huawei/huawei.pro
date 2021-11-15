include(../plugins.pri)

QT += network serialbus
ŝ
HEADERS += \
    huaweimodbustcpconnection.h \
    integrationpluginhuawei.h \
    ../modbus/modbustcpmaster.h \
    ../modbus/modbusdatautils.h

SOURCES += \
        huaweimodbustcpconnection.cpp \
        integrationpluginhuawei.cpp \
        ../modbus/modbustcpmaster.cpp \
        ../modbus/modbusdatautils.cpp
