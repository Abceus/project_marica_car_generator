#-------------------------------------------------
#
# Project created by QtCreator 2017-12-12T13:27:51
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

!defined(WITHOUT_SIMULATION, var) {
    WITHOUT_SIMULATION = 0
} else {
    WITHOUT_SIMULATION = 1
}

equals(WITHOUT_SIMULATION, 1) {
    DEFINES += "WITHOUT_SIMULATION=1"
}

equals(WITHOUT_SIMULATION, 0) {
    !defined(BULLET_INCLUDE_DIRECTORY, var) {
        BULLET_INCLUDE_DIRECTORY = "/usr/include/bullet"
    }
}
TARGET = ProjectMaricaCarGenerator
TEMPLATE = app
INCLUDEPATH += includes
QTPLUGIN     += qtga

equals(WITHOUT_SIMULATION, 0) {
    INCLUDEPATH += $$BULLET_INCLUDE_DIRECTORY
    defined(BULLET_LIBRARY_DIRECTORY, var) {
        LIBS += -L$$BULLET_LIBRARY_DIRECTORY
    }
}


equals(WITHOUT_SIMULATION, 0) {
    LIBS += -lBulletDynamics \
            -lBulletCollision \
            -lLinearMath
}

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
    sources/collision_object.cpp \
    sources/collision_parser.cpp \
    sources/errorsystem.cpp \
    sources/impl_errorsystem.cpp \
    sources/mainwindow.cpp \
    sources/mainopenglwidget.cpp \
    sources/physics/shapes/box.cpp \
    sources/physics/shapes/collection.cpp \
    sources/physics/shapes/cylinder.cpp \
    sources/physics/shapes/mesh.cpp \
    sources/physics/shapes/offset.cpp \
    sources/physics/shapes/sphere.cpp \
    sources/project_model.cpp \
    sources/render_system/batching.cpp \
    sources/resources/loaders/model_loader.cpp \
    sources/resources/loaders/texture_loader.cpp \
    sources/resources/model.cpp \
    sources/render_system/scene.cpp \
    sources/resources/resource_manager.cpp \
    sources/resources/resource_pointer.cpp \
    sources/resources/resource_submanager.cpp \
    sources/resources/unanimation.cpp \
    sources/resources/file.cpp \
    sources/object.cpp \
    sources/render_system/mesh.cpp \
    sources/grid.cpp \
    sources/render_system/scene_node.cpp \
    sources/render_system/drawable.cpp \
    sources/resources/wireframe_model.cpp \
    sources/updatable.cpp \
    sources/render_system/renderer.cpp \
    sources/render_system/wireframe.cpp \
    sources/render_system/box.cpp \
    sources/vector3d.cpp \
    sources/render_system/camera.cpp \
    sources/resources/loaders/image_loader.cpp \
    sources/resources/loaders/file_loader.cpp \
    ui/edit_widget.cpp

HEADERS  += includes/mainwindow.h \
    includes/collision_object.h \
    includes/collision_parser.h \
    includes/physics/shapes/box.h \
    includes/physics/shapes/collection.h \
    includes/physics/shapes/cylinder.h \
    includes/physics/shapes/mesh.h \
    includes/physics/shapes/offset.h \
    includes/physics/shapes/shape.h \
    includes/physics/shapes/sphere.h \
    includes/project_config.h \
    includes/project_model.h \
    includes/render_system/batching.h \
    includes/render_system/material.h \
    includes/resources/loaders/image_loader.h \
    includes/resources/loaders/model_loader.h \
    includes/errorsystem.h \
    includes/impl_errorsystem.h \
    includes/mainopenglwidget.h \
    includes/render_system/scene.h \
    includes/resources/loaders/loader.h \
    includes/resources/loaders/texture_loader.h \
    includes/resources/model.h \
    includes/resources/resource_manager.h \
    includes/resources/resource_pointer.h \
    includes/resources/resource_submanager.h \
    includes/resources/unanimation.h \
    includes/resources/file.h \
    includes/object.h \
    includes/render_system/mesh.h \
    includes/grid.h \
    includes/render_system/scene_node.h \
    includes/render_system/drawable.h \
    includes/resources/wireframe_model.h \
    includes/updatable.h \
    includes/render_system/renderer.h \
    includes/render_system/wireframe.h \
    includes/render_system/box.h \
    includes/vector3d.h \
    includes/render_system/camera.h \
    includes/version.h \
    includes/resources/loaders/file_loader.h \
    ui/edit_widget.h

equals(WITHOUT_SIMULATION, 0) {
    SOURCES += sources/openglsimulationwidget.cpp \
        sources/physics/physobject.cpp \
        sources/physics/physicworld.cpp

    HEADERS  += includes/openglsimulationwidget.h \
        includes/physics/physobject.h \
        includes/physics/physicworld.h
}

FORMS    += ui/mainwindow.ui \
    ui/edit_widget.ui

DISTFILES += \
    resources/shaders/colorfragmentshader.frag \
    resources/shaders/colorvertexshader.vert \
    resources/shaders/texturefragmentshader.frag \
    resources/shaders/texturevertexshader.vert

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
