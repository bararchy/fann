########### install files ###############

INSTALL_FILES( /include/fann FILES  fann.h doublefann.h fann_internal.h floatfann.h
fann_data.h fixedfann.h compat_time.h fann_activation.h fann_cascade.h
fann_error.h fann_train.h fann_io.h fann_cpp.h fann_generic.h fann_sparse.h
fann_som.h fann_gng.h fann_recurrent.h )
ADD_SUBDIRECTORY( optimized ) 

