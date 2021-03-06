QT       += core gui network sql printsupport axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addactor.cpp \
    adddirector.cpp \
    addmovie.cpp \
    addprotagonist.cpp \
    addstudio.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    outputactor.cpp \
    outputdirector.cpp \
    outputprotagonist.cpp \
    outputstudio.cpp \
    paint.cpp \
    paintscene.cpp \
    storedprocedures.cpp \
    updateactor.cpp \
    updatedirector.cpp \
    updatemovie.cpp \
    updateprotagonist.cpp \
    updatestudio.cpp

HEADERS += \
    addactor.h \
    adddirector.h \
    addmovie.h \
    addprotagonist.h \
    addstudio.h \
    login.h \
    mainwindow.h \
    outputactor.h \
    outputdirector.h \
    outputprotagonist.h \
    outputstudio.h \
    paint.h \
    paintscene.h \
    storedprocedures.h \
    updateactor.h \
    updatedirector.h \
    updatemovie.h \
    updateprotagonist.h \
    updatestudio.h

FORMS += \
    addactor.ui \
    adddirector.ui \
    addmovie.ui \
    addprotagonist.ui \
    addstudio.ui \
    login.ui \
    mainwindow.ui \
    outputactor.ui \
    outputdirector.ui \
    outputprotagonist.ui \
    outputstudio.ui \
    paint.ui \
    storedprocedures.ui \
    updateactor.ui \
    updatedirector.ui \
    updatemovie.ui \
    updateprotagonist.ui \
    updatestudio.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
