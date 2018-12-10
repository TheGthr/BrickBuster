# configuration Qt
QT       += core gui opengl widgets
TEMPLATE  = app

# ajout des libs au linker
win32 {
    win32-msvc* {
        LIBS     += opengl32.lib glu32.lib
    } else {
        LIBS     += -lopengl32 -lglu32
    }
}
else {
	LIBS     += -lGL -lGLU
}

# nom de l'exe genere
TARGET 	  = BrickBuster

# fichiers sources/headers
SOURCES	+= main.cpp myglwidget.cpp \
    palet.cpp \
    balle.cpp \
    brique.cpp \
    webcam.cpp \
    brickbusterwindow.cpp \
    background.cpp
HEADERS += myglwidget.h \
    palet.h \
    balle.h \
    brique.h \
    webcam.h \
    brickbusterwindow.h \
    background.h
CONFIG += c++

RESOURCES += \
    ressources.qrc

INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core2413 \
    -lopencv_highgui2413 \
    -lopencv_imgproc2413 \
    -lopencv_video2413 \
    -lopencv_features2d2413 \
    -lopencv_calib3d2413

FORMS += \
    brickbusterwindow.ui
