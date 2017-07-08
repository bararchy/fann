INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)
ADD_DEFINITIONS(-D_REENTRANT)
########### next target ###############

SET(floatfann_blas_LIB_SRCS
fann_blas_mimo.c
)

ADD_LIBRARY(floatfann2_blas SHARED ${floatfann_blas_LIB_SRCS})

SET_TARGET_PROPERTIES(floatfann2_blas PROPERTIES VERSION 2.1.0 SOVERSION 2 )
INSTALL(TARGETS floatfann2_blas DESTINATION ${FANN_LIB_INSTALL_DIR} )


########### next target ###############

#SET(doublefann_LIB_SRCS
#doublefann.c
#)
#
#ADD_LIBRARY(doublefann SHARED ${doublefann_LIB_SRCS})
#
#SET_TARGET_PROPERTIES(doublefann PROPERTIES VERSION 2.1.0 SOVERSION 2 )
#INSTALL(TARGETS doublefann DESTINATION ${FANN_LIB_INSTALL_DIR} )


########### next target ###############

#SET(fixedfann_LIB_SRCS
#fixedfann.c
#)

#ADD_LIBRARY(fixedfann SHARED ${fixedfann_LIB_SRCS})
#
#SET_TARGET_PROPERTIES(fixedfann PROPERTIES VERSION 2.1.0 SOVERSION 2 )
#INSTALL(TARGETS fixedfann DESTINATION ${FANN_LIB_INSTALL_DIR} )


########### next target ###############

#SET(fann_LIB_SRCS
#floatfann.c
#)

#ADD_LIBRARY(fann SHARED ${fann_LIB_SRCS})

#SET_TARGET_PROPERTIES(fann PROPERTIES VERSION 2.1.0 SOVERSION 2 )
#INSTALL(TARGETS fann DESTINATION ${FANN_LIB_INSTALL_DIR} )

