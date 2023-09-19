set(target PubSub)
set(exportedTarget PubSubTarget)

set(version 1.0.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

file(GLOB_RECURSE SOURCES_C RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc)
file(GLOB_RECURSE SOURCES_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cxx)
file(GLOB_RECURSE HEADERS_H RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB_RECURSE HEADERS_HPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE HEADERS_HXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hxx)

list(APPEND SOURCES ${SOURCES_C} ${SOURCES_CPP} ${SOURCES_CXX})
list(APPEND HEADERS ${HEADERS_H} ${HEADERS_HPP} ${HEADERS_HXX})

add_link_options(-lpthread)

add_library(${target} SHARED ${SOURCES} ${SOURCES})

set_property(TARGET ${target} PROPERTY VERSION ${version})
set_property(TARGET ${target} PROPERTY SOVERSION 1)
set_property(TARGET ${target} PROPERTY
  INTERFACE_PubSub_MAJOR_VERSION 1)
set_property(TARGET ${target} APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING PubSub_MAJOR_VERSION
)

macro(includeSubDirs targ curdir instdir installDirs)
  target_include_directories(${targ} PUBLIC "$<BUILD_INTERFACE:${curdir}>"
                                            "$<INSTALL_INTERFACE:${instDir}>")

  list(APPEND ${installDirs} ${instdir})

  file(GLOB children RELATIVE ${curdir} ${curdir}/*)

  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      includeSubDirs(${targ} ${curdir}/${child} ${instdir}/${child} ${sourceDirs} ${installDirs})
    endif()
  endforeach()
endmacro()

includeSubDirs(${target} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_INSTALL_INCLUDEDIR} install_dirs)

install(TARGETS ${target}
        EXPORT ${exportedTarget}
        INCLUDES DESTINATION ${install_dirs}
)

macro(installIncludeFiles headerFile)
  get_filename_component(thisSubDir ${headerFile} DIRECTORY)

  install(FILES ${headerFile} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${thisSubDir})
endmacro()

foreach(headerFile ${HEADERS})
  installIncludeFiles(${headerFile})
endforeach()

install(EXPORT ${exportedTarget}
        FILE "${exportedTarget}.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${target}
        ) 

# Create imported target ${exportedTarget}
add_library(${exportedTarget} SHARED IMPORTED)

set_target_properties(${exportedTarget} PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
  )

configure_package_config_file( ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${target}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${target}
  )

# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${target}ConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
  )

install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/${target}Config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/${target}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${target}
          )




