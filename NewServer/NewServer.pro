QT       += core gui network sql axcontainer printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    myserver.cpp \
    socketthread.cpp

HEADERS += \
    myserver.h \
    socketthread.h

FORMS += \
    myserver.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
