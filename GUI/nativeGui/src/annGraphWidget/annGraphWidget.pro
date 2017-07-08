
CONFIG += warn_on \
 staticlib
TEMPLATE = lib

CONFIG -= stl \
thread

TARGET = anngraphwidget

DESTDIR = .

OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc

HEADERS += anngraphwidget.h \
node.h \
 connection.h \
 mimoneuronitem.h \
 inputlayeritem.h \
 defaultlayeritem.h \
 layeritem.h
SOURCES += anngraphwidget.cpp \
node.cpp \
 connection.cpp \
 mimoneuronitem.cpp \
 inputlayeritem.cpp \
 defaultlayeritem.cpp \
 layeritem.cpp


QT += opengl

