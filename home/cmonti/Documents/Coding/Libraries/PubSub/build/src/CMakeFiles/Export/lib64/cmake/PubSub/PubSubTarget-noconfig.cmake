#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PubSub" for configuration ""
set_property(TARGET PubSub APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PubSub PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib64/libPubSub.so.1.0.0"
  IMPORTED_SONAME_NOCONFIG "libPubSub.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS PubSub )
list(APPEND _IMPORT_CHECK_FILES_FOR_PubSub "${_IMPORT_PREFIX}/lib64/libPubSub.so.1.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
