include(../plugins.pri)

QT += \
    serialport \
    serialbus \

SOURCES += \
    integrationplugindrexelundweiss.cpp \

HEADERS += \
    integrationplugindrexelundweiss.h \
    modbusregisterdefinition.h
