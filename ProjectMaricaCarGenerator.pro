#-------------------------------------------------
#
# Project created by QtCreator 2017-12-12T13:27:51
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectMaricaCarGenerator
TEMPLATE = app
INCLUDEPATH += includes

#LIBS += -lglu32 -lopengl32

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += sources/main.cpp\
    sources/mainwindow.cpp \
    sources/mainopenglwidget.cpp \
    sources/model.cpp \
    sources/scene.cpp \
    sources/unanimation.cpp \
    sources/object.cpp \
    sources/mesh.cpp

HEADERS  += includes/mainwindow.h \
    includes/mainopenglwidget.h \
    includes/scene.h \
    includes/model.h \
    includes/unanimation.h \
    includes/object.h \
    includes/mesh.h

FORMS    += ui/mainwindow.ui

DISTFILES += \
    resources/defaultfragmentshader.frag \
    resources/defaultvertexshader.vert

win32 {
    build_pass: CONFIG(debug, debug|release) {
        DESTDIR = $$OUT_PWD/debug/install
    }
    else: build_pass {
        DESTDIR = $$OUT_PWD/release/install
    }
}

unix {
    DESTDIR = $$OUT_PWD/install
}

ProjectMaricaCarGenerator.path = $$DESTDIR

resources.path   = $$DESTDIR
resources.files  = resources

INSTALLS       += resources
