
#HEADERS += network.h \
#layer.h \
#neuron.h \
# neurondescriptor.h \
# networkdescriptor.h \
# layerdescriptor.h \
# definitions.h \
# trainingparameters.h \
# rpropsigmoidoptimizedneuron.h \
# rpropsigmoidsymmetricoptimizedneuron.h \
# batchsigmoidoptimizedneuron.h \
# batchsigmoidsymmetricoptimizedneuron.h

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )

SET ( datastructure_SOURCES 
 network.cpp 
 layer.cpp 
 neuron.cpp 
 neurondescriptor.cpp 
 networkdescriptor.cpp 
 layerdescriptor.cpp 
 trainingparameters.cpp 
 rpropsigmoidoptimizedneuron.cpp 
 rpropsigmoidsymmetricoptimizedneuron.cpp 
 batchsigmoidoptimizedneuron.cpp 
 batchsigmoidsymmetricoptimizedneuron.cpp
 traindata.cpp
)

QT4_AUTOMOC( ${datastructure_SOURCES} )

ADD_LIBRARY( datastructure SHARED ${datastructure_SOURCES} )

TARGET_LINK_LIBRARIES( datastructure ${QT_QTCORE_LIBRARY} )
SET_TARGET_PROPERTIES( datastructure PROPERTIES VERSION 0.1.0 SOVERSION 0 )

INSTALL( TARGETS datastructure DESTINATION lib/ )

