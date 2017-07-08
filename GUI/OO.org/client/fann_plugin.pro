TEMPLATE = app
OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
QT += xml
QMAKE_CXXFLAGS	= -O -fpic -fno-rtti -DUNX -DGCC -DLINUX -DCPPU_ENV=gcc3 \
-pedantic -Wno-long-long
CONFIG += warn_on \
          qt \
          debug

DEPENDPATH += .

INCLUDEPATH += . \
               .ui \
               $(FANN_SRC_DIR) \
               $(OO_SDK_HOME)/include/ \

FORMS +=  main_window.ui \
          activation_function.ui \
          about_dialog.ui \
          data_chooser.ui \
          network_console.ui \
          network_overview.ui \
          layers_settings.ui \
          resilient_propagation.ui \
          progress.ui \
          training_settings.ui \
          weights.ui \
          execution.ui \

HEADERS += main_window.h \
           activation_function.h \
           about_dialog.h \
           data_chooser.h \
           network_console.h \
           network_overview.h \
           layers_settings.h \
           office_client.h \
           ooorg_client.h \
           resilient_propagation.h \
           selection.h \
           strict_double_validator.h \
           strict_int_validator.h \
           execution.h \
           training_settings.h \
           weights.h \
           fann_translation.h \

SOURCES += main_window.cpp \
           main.cpp \
           activation_function.cpp \ 
           about_dialog.cpp \
           data_chooser.cpp \
           network_console.cpp \
           network_overview.cpp \
           ooorg_client.cpp \
           resilient_propagation.cpp \
           layers_settings.cpp \
           selection.cpp \
           strict_double_validator.cpp \
           strict_int_validator.cpp \
           execution.cpp \
           training_settings.cpp \
           util.cpp \
           weights.cpp \
           fann_translation.cpp \

LIBS +=  $(FANN_LIB_DIR)/libfann.a \
        -L$(OO_ORG_SDK)/linux/lib \
        -L$(OFFICE_HOME)/program \
        -L$(OO_SDK_HOME)../program \
        -lcppuhelpergcc3 \
        -lcppu \
        -lsalhelpergcc3 \
        -lsal \
        #-l libstlport_gcc.so.4.6 \

TRANSLATIONS += fann_plugin_pl.ts 

