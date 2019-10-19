#-------------------------------------------------
#
# Project created by QtCreator 2017-12-12T13:27:51
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

!defined(BULLET_INCLUDE_DIRECTORY, var) {
    BULLET_INCLUDE_DIRECTORY = "/usr/include/bullet"
}
TARGET = ProjectMaricaCarGenerator
TEMPLATE = app
INCLUDEPATH += includes $$BULLET_INCLUDE_DIRECTORY

defined(BULLET_LIBRARY_DIRECTORY, var) {
    LIBS += -L$$BULLET_LIBRARY_DIRECTORY
}

LIBS += -lBulletDynamics \
        -lBulletCollision \
        -lLinearMath

CONFIG += c++17

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
    sources/resources/model.cpp \
    sources/render_system/scene.cpp \
    sources/resources/unanimation.cpp \
    sources/object.cpp \
    sources/resources/resource_manager.cpp \
    sources/render_system/mesh.cpp \
    sources/openglsimulationwidget.cpp \
    sources/physics/physobject.cpp \
    sources/physics/physicworld.cpp \
    sources/grid.cpp \
    sources/render_system/scene_node.cpp \
    sources/render_system/drawable.cpp \
    sources/updatable.cpp \
    sources/render_system/renderer.cpp \
    sources/render_system/wireframe.cpp

HEADERS  += includes/mainwindow.h \
    includes/mainopenglwidget.h \
    includes/render_system/scene.h \
    includes/resources/model.h \
    includes/resources/unanimation.h \
    includes/object.h \
    includes/resources/resource_manager.h \
    includes/render_system/mesh.h \
    includes/openglsimulationwidget.h \
    includes/physics/physobject.h \
    includes/physics/physicworld.h \
    includes/grid.h \
    includes/render_system/scene_node.h \
    includes/render_system/drawable.h \
    includes/render_system/render_info.h \
    includes/updatable.h \
    includes/render_system/renderer.h \
    includes/render_system/wireframe.h

FORMS    += ui/mainwindow.ui

DISTFILES += \
    resources/shaders/defaultfragmentshader.frag \
    resources/shaders/defaultvertexshader.vert

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

resources.files  = resources
resources.path   = $$DESTDIR

INSTALLS       += resources
