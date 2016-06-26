#-------------------------------------------------
#
# Project created by QtCreator 2016-06-16T14:43:33
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAPSYS_graph
TEMPLATE = app


SOURCES += main.cpp \
    graph.cpp \
    ../dialogsetranges.cpp \
    ../eval_mpf.cpp \
    ../functions_for_eval.cpp \
    ../settings.cpp \
    g_mainwindow.cpp

HEADERS  += \
        graph.h \
    ../dialogsetranges.h \
    ../eval_mpf.h \
    ../functions_for_eval.h \
    ../settings.h \
    g_mainwindow.h

FORMS    += \
    ../dialogsetranges.ui \
    g_mainwindow.ui

### MathGL

win32: LIBS += -L$$PWD/../libs/mathgl-2.3.5.Win10-mingw.i386/lib -llibmgl-qt5
win32: LIBS += -L$$PWD/../libs/mathgl-2.3.5.Win10-mingw.i386/lib -llibmgl

INCLUDEPATH += $$PWD/../libs/mathgl-2.3.5.Win10-mingw.i386/include
DEPENDPATH += $$PWD/../libs/mathgl-2.3.5.Win10-mingw.i386/include

### MPFR

win32: LIBS += -L$$PWD/../libs/mpfr-3.1.4/lib/ -llibmpfr

INCLUDEPATH += $$PWD/../libs/mpfr-3.1.4/include
DEPENDPATH += $$PWD/../libs/mpfr-3.1.4/include

### GNU MP

win32: LIBS += -L$$PWD/../libs/gmp-6.1.0/lib/ -llibgmp
win32: LIBS += -L$$PWD/../libs/gmp-6.1.0/lib/ -llibgmpxx

INCLUDEPATH += $$PWD/../libs/gmp-6.1.0/include
DEPENDPATH += $$PWD/../libs/gmp-6.1.0/include

OTHER_FILES += \
    tor.txt
