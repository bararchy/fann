CONFIG += warn_on \
staticlib
TEMPLATE = lib

DESTDIR = .

HEADERS += network.h \
layer.h \
neuron.h \
 neurondescriptor.h \
 networkdescriptor.h \
 layerdescriptor.h \
 definitions.h \
 trainingparameters.h \
 rpropsigmoidoptimizedneuron.h \
 rpropsigmoidsymmetricoptimizedneuron.h \
 batchsigmoidoptimizedneuron.h \
 batchsigmoidsymmetricoptimizedneuron.h \
 traindata.h
SOURCES += network.cpp \
layer.cpp \
neuron.cpp \
 neurondescriptor.cpp \
 networkdescriptor.cpp \
 layerdescriptor.cpp \
 trainingparameters.cpp \
 rpropsigmoidoptimizedneuron.cpp \
 rpropsigmoidsymmetricoptimizedneuron.cpp \
 batchsigmoidoptimizedneuron.cpp \
 batchsigmoidsymmetricoptimizedneuron.cpp \
 traindata.cpp
TARGET = datastructure

CONFIG -= stl \
thread
QT -= gui

OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc

