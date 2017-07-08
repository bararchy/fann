
#HEADERS += anngraphwidget.h \
#node.h \
# connection.h \
# layeritem.h \
# mimoneuronitem.h

SET( anngraphwidget_SOURCES 
 anngraphwidget.cpp 
 node.cpp 
 connection.cpp 
 layeritem.cpp 
 mimoneuronitem.cpp
 defaultlayeritem.cpp
 inputlayeritem.cpp
)

SET(anngraphwidget_MOCS
anngraphwidget.h
defaultlayeritem.h
layeritem.h
mimoneuronitem.h
)
QT4_WRAP_CPP( anngraphwidget_MOCS_cpp ${anngraphwidget_MOCS})

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${QT_QTOPENGL_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})

QT4_AUTOMOC( ${anngraphwidget_SOURCES} )

ADD_LIBRARY( anngraphwidget SHARED ${anngraphwidget_SOURCES} ${anngraphwidget_MOCS_cpp})

TARGET_LINK_LIBRARIES( anngraphwidget ${QT_QTCORE_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QT_QTGUI_LIBRARY})
SET_TARGET_PROPERTIES( anngraphwidget PROPERTIES VERSION 0.1.0 SOVERSION 0 )

INSTALL( TARGETS anngraphwidget DESTINATION lib/ )

