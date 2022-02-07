aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/data_structure src_ds)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/io src_io)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/misc src_misc)

set(SRC ${src_ds} ${src_io} ${src_misc} PARENT_SCOPE)
