#-------------------------------------------------
#
# Project created by QtCreator 2018-05-07T20:37:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fixture
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=  \
    src/dialogs/newdialog.cpp \
    src/widgets/paintwidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/widgets/layermanager.cpp \
    src/items/drawing.cpp \
    src/items/layer.cpp \
    src/model/canvas.cpp \
    src/tools/tool.cpp \
    src/tools/selecttool.cpp \
    src/tools/pantool.cpp

HEADERS += \
    src/dialogs/newdialog.h \
    src/widgets/paintwidget.h \
    src/mainwindow.h \
    src/widgets/layermanager.h \
    src/items/drawing.h \
    src/items/layer.h \
    src/model/canvas.h \
    src/tools/tool.h \
    src/tools/selecttool.h \
    src/tools/pantool.h

FORMS += \
    src/mainwindow.ui \
    src/dialogs/newdialog.ui \

RESOURCES += \
    resources/resources.qrc

DISTFILES += \
    resources/checkers.png \
    resources/icons/angle-bottom.svg \
    resources/icons/angle-left.svg \
    resources/icons/angle-right.svg \
    resources/icons/angle-top.svg \
    resources/icons/close-white-dark.svg \
    resources/icons/close-white-lighter.svg \
    resources/icons/close-white.svg \
    resources/tools/select.svg \
    resources/tools/pan.svg \
    resources/basic.qss \
    resources/layermanager.qss \
    resources/paintwidget.qss
