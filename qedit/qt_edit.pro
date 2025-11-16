QT += quick core gui

CONFIG += c++17 console
TEMPLATE = app

SOURCES += \
    main.cpp \
    ContactModel.cpp \
    QueueWorker.cpp

HEADERS += \
    ContactModel.h \
    QueueWorker.h

RESOURCES += \
    qml.qrc

LIBS += -lrt

