########### install files ###############

INSTALL_FILES( /include/fann/optimized/ FILES  fann_optimized_template.h fann_train_optimized_template.h )
ADD_SUBDIRECTORY( scalar )
ADD_SUBDIRECTORY( blas )
ADD_SUBDIRECTORY( sse )

