aux_source_directory(. DEDUP_SRC)
add_library(DEDUP_LIB ${DEDUP_SRC})
target_link_libraries(UTIL_LIB)