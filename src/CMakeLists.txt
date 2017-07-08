ADD_SUBDIRECTORY( include ) 

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)
ADD_DEFINITIONS(-D_REENTRANT)
########### next target ###############

SET(floatfann_LIB_SRCS
floatfann.c
)

ADD_LIBRARY(floatfann2 SHARED ${floatfann_LIB_SRCS})

SET_TARGET_PROPERTIES(floatfann2 PROPERTIES VERSION 2.1.0 SOVERSION 2 )
INSTALL(TARGETS floatfann2 DESTINATION ${FANN_LIB_INSTALL_DIR} )


########### next target ###############

SET(doublefann_LIB_SRCS
doublefann.c
)

ADD_LIBRARY(doublefann2 SHARED ${doublefann_LIB_SRCS})

SET_TARGET_PROPERTIES(doublefann2 PROPERTIES VERSION 2.1.0 SOVERSION 2 )
INSTALL(TARGETS doublefann2 DESTINATION ${FANN_LIB_INSTALL_DIR} )


########### next target ###############

SET(fixedfann_LIB_SRCS
fixedfann.c
)

ADD_LIBRARY(fixedfann2 SHARED ${fixedfann_LIB_SRCS})

SET_TARGET_PROPERTIES(fixedfann2 PROPERTIES VERSION 2.1.0 SOVERSION 2 )
INSTALL(TARGETS fixedfann2 DESTINATION ${FANN_LIB_INSTALL_DIR} )


########### next target ###############

SET(fann_LIB_SRCS
floatfann.c
)

ADD_LIBRARY(fann2 SHARED ${fann_LIB_SRCS})

SET_TARGET_PROPERTIES(fann2 PROPERTIES VERSION 2.1.0 SOVERSION 2 )
INSTALL(TARGETS fann2 DESTINATION ${FANN_LIB_INSTALL_DIR} )

ADD_SUBDIRECTORY( optimized ) 
