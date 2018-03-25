#-------------------------------------------------
#
# Project created by QtCreator 2017-07-14T12:19:10
#
#-------------------------------------------------

QT       += quick core opengl gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VArch
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += \
            $$PWD/../../libs/glew-1.13.0/include \
            $$PWD/../../libs/glm \
            $$PWD/../../libs/SOIL/src \
            $$PWD/../../src \
            $$PWD/src \

LIBS += \
        -L$$PWD/../../libs/glew-1.13.0/lib/Release/x64 -lglew32s -lglu32 -lopengl32 \
        -L$$PWD/../../libs/SOIL/lib -lSOIL \
        -L$$PWD/../../x64/Release -lVArch \

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    src/openglwidget.cpp \
    src/planetgenerator.cpp \
    src/treegenerator.cpp \
    src/cameraconfig.cpp \
    src/cameraconfig.cpp \
    src/openglwidget.cpp \
    src/planetgenerator.cpp \
    src/treegenerator.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
        mainwindow.h \
    src/openglwidget.h \
    src/planetgenerator.h \
    src/treegenerator.h \
    src/cameraconfig.h \
    src/cameraconfig.h \
    src/openglwidget.h \
    src/planetgenerator.h \
    src/treegenerator.h \
    mainwindow.h

FORMS += \
        mainwindow.ui

win32: RC_ICONS += icon.ico

RESOURCES += resources.qrc

DISTFILES += \
    qml/ImageButton.qmlc \
    qml/ProceduralPanel.qmlc \
    qml/VArch.qmlc \
    qml/qmldir \
    qml/ImageButton.qml \
    qml/PlanetPanel.qml \
    qml/ProceduralPanel.qml \
    qml/VArch.qml \
    qml/VColors.qml
