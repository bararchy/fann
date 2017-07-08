
SET( nativegui_SOURCES
	dataplot.cpp
	designtoolbar.cpp
	inputdatagroup.cpp
	main.cpp
	mainstackedwidget.cpp
	mainwindow.cpp
	networkmanager.cpp
	neuronpropertywidget.cpp
	outputpropertywidget.cpp
	testwidget.cpp
	trainpropertywidget.cpp
	trainwidget.cpp
	)

SET(nativegui_MOCS 
	dataplot.h
	mainwindow.h
	networkmanager.h
	neuronpropertywidget.h
	testwidget.h
	trainpropertywidget.h
	trainwidget.h
	)

SET( nativegui_FORMS 
	main_window.ui
	neuronproperty_widget.ui
	outputproperty_widget.ui
	test_widget.ui
	trainproperty_widget.ui
	train_widget.ui
	)

QT4_ADD_RESOURCES( icons_QRC_cpp icons.qrc )

SET(nativegui_FORMS_h "")
LINK_DIRECTORIES( ${FANN_PREFIX_DIR}/lib )

QT4_WRAP_UI( nativegui_FORMS_h ${nativegui_FORMS})
QT4_WRAP_CPP( nativegui_MOCS_cpp ${nativegui_MOCS})
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ../annGraphWidget ../dataStructure ../qwt-5.0.2 ${QT_QTOPENGL_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} )

QT4_AUTOMOC( ${nativegui_SOURCES} )

ADD_EXECUTABLE( nativegui  ${nativegui_SOURCES} ${nativegui_FORMS_h} ${nativegui_MOCS_cpp} ${icons_QRC_cpp})


TARGET_LINK_LIBRARIES( nativegui ${QT_QTCORE_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QT_QTGUI_LIBRARY} anngraphwidget datastructure cblas floatfann2 floatfann2_blas qwt )

INSTALL( TARGETS nativegui DESTINATION bin )

