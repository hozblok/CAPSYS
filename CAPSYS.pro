#-------------------------------------------------
#
# Project created by QtCreator 2016-04-12T18:03:37
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAPSYS
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
#    dialogsetranges.cpp \
    settings.cpp \
    linalg.cpp \
    xrays.cpp \
    source.cpp \
    eval_mpf.cpp \
    functions_for_eval.cpp \
    complex_mpf.cpp

HEADERS  += mainwindow.h \
#    dialogsetranges.h \
    settings.h \
    linalg.h \
    xrays.h \
    source.h \
    eval_mpf.h \
    functions_for_eval.h \
    complex_mpf.h

FORMS    += mainwindow.ui
#    dialogsetranges.ui

OTHER_FILES +=

#### LIBS ####

### MathGL

#win32: LIBS += -L$$PWD/libs/mathgl-2.3.5.Win10-mingw.i386/lib -llibmgl-qt5
#win32: LIBS += -L$$PWD/libs/mathgl-2.3.5.Win10-mingw.i386/lib -llibmgl

#INCLUDEPATH += $$PWD/libs/mathgl-2.3.5.Win10-mingw.i386/include
#DEPENDPATH += $$PWD/libs/mathgl-2.3.5.Win10-mingw.i386/include


### MPFR

win32: LIBS += -L$$PWD/libs/mpfr-3.1.4/lib/ -llibmpfr

INCLUDEPATH += $$PWD/libs/mpfr-3.1.4/include
DEPENDPATH += $$PWD/libs/mpfr-3.1.4/include

### GNU MP

win32: LIBS += -L$$PWD/libs/gmp-6.1.0/lib/ -llibgmp
win32: LIBS += -L$$PWD/libs/gmp-6.1.0/lib/ -llibgmpxx

INCLUDEPATH += $$PWD/libs/gmp-6.1.0/include
DEPENDPATH += $$PWD/libs/gmp-6.1.0/include

RESOURCES += \
    res.qrc
