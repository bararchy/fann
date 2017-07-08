# Set the name of the project
PROJECT (FANN)

# Display all outputs, including compiler lines
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# If a module is specified, the file <modulename>.cmake
#   is searched in the below path
SET(CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake/Modules 
)

# Internally set version number
SET(VERSION 2.1.99)

# 'DefineInsallationPaths' is not a standard CMake module
INCLUDE(DefineInstallationPaths)
FIND_PACKAGE(BLAS REQUIRED)


# Copies 'config.h' into '/src/include' after replacing any variables in the file
#   referenced as ${VAR} or @VAR@ with their CMake values
configure_file( ${CMAKE_SOURCE_DIR}/cmake/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/src/include/config.h )

# Add '/src/include' to the directories searched by the compiler for include files
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/src/include/)

# Copies 'fann.pc.cmake' to '/'. Here, only @VAR@ will be replaced, and so ${VAR}
#   will be ignored
configure_file( ${CMAKE_SOURCE_DIR}/cmake/fann.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/fann.pc @ONLY )

SET(PKGCONFIG_INSTALL_DIR /lib/pkgconfig)
SET(FANN_LIB_INSTALL_DIR ${LIB_INSTALL_DIR} CACHE PATH "" FORCE)
SET(ENABLE_SSE true CACHE BOOL "Enables or disables the compilation of the SSE library")
SET(ENABLE_BLAS ${BLAS_FOUND} CACHE BOOL "Enables or disables the compilation of the BLAS library")

########### install files ###############

#   Installs the listed files (>= third parameter) with the
#   given extension (second parameter) in the installation
#   directory (first parameter)
INSTALL(FILES fann.pc DESTINATION ${PKGCONFIG_INSTALL_DIR})
#INSTALL_FILES( ${PKGCONFIG_INSTALL_DIR} FILES  fann.pc )   # ANTIQUATED

# Adds a subdirectory to the build, where the source
#   CMakeLists.txt is in the parameter directory
ADD_SUBDIRECTORY( src ) 


#original Makefile.am contents follow:

#SUBDIRS = src
#
#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = fann.pc
#
#EXTRA_DIST = benchmarks examples MicrosoftVisualC++6.0 MicrosoftVisualC++.Net \
#	MicrosoftWindowsDll BorlandC++Builder6.0 src/include/config.h \
#	debian/changelog debian/compat debian/control debian/copyright \
#	debian/docs debian/libfann2-dev.dirs debian/libfann2-dev.examples \
#	debian/libfann2-dev.files debian/libfann2-dev.install \
#	debian/libfann2.dirs debian/libfann2.files debian/libfann2.install \
#	debian/libfann2-py.dirs  debian/libfann2-py.files \
#	debian/libfann2-py.install debian/rules \
#	doc/advancedusage.txt doc/gettingstarted.txt doc/theory.txt \
#	doc/fixedpointusage.txt doc/installation.txt doc/fann_en.pdf \
#	python
#
#dist-hook:
#	rm -rf `find $(distdir) -name "*~" -o -name ".#*" -o -name "CVS"`
#	(cd $(distdir)/examples && make clean)
#	(cd $(distdir)/python && make clean)
#	(cd $(distdir)/benchmarks && make clean)
#	zip -9 -r $(distdir).zip $(distdir)
#	tar chojf $(distdir).tar.bz2 $(distdir)

