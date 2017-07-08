TEMPLATE = app
CONFIG += warn_on \
	  qt
SOURCES += main.cpp \
mainwindow.cpp \
mainstackedwidget.cpp \
trainwidget.cpp \
neuronpropertywidget.cpp \
inputdatagroup.cpp \
outputpropertywidget.cpp \
 networkmanager.cpp \
 trainpropertywidget.cpp \
 dataplot.cpp \
 testwidget.cpp
HEADERS += mainwindow.h \
mainstackedwidget.h \
trainwidget.h \
neuronpropertywidget.h \
inputdatagroup.h \
 networkmanager.h \
 outputpropertywidget.h \
 trainpropertywidget.h \
 dataplot.h \
 testwidget.h \
 buffervector.h
FORMS += main_window.ui \
train_widget.ui \
neuronproperty_widget.ui \
trainproperty_widget.ui \
outputproperty_widget.ui \
 test_widget.ui
OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc


TARGET = ../../bin/nativeGui


CONFIG -= stl \
thread
DESTDIR = .








RESOURCES += icons.qrc

INCLUDEPATH += ../annGraphWidget \
../dataStructure \
../qwt-5.0.2
LIBS += ../annGraphWidget/libanngraphwidget.a \
../dataStructure/libdatastructure.a \
../qwt-5.0.2/libqwt-5.0.2.a \
-lcblas \
-lfloatfann2 \
-lfloatfann2_blas
TARGETDEPS += ../annGraphWidget/libanngraphwidget.a \
../dataStructure/libdatastructure.a \
../qwt-5.0.2/libqwt-5.0.2.a
