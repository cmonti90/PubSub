set(target PubSub)
set(exportedTarget PubSubTarget)

set(version 1.0.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(SOURCES Component.cpp
            Module.cpp
            QueueMngr.cpp
            Thread.cpp
            SimThread.cpp
            Time.cpp)

set(HEADERS QueueMngr.h
            Component.h
            SimComponent.h
            "Message.h"
            Time.h
            Thread.h
            SimThread.h
            Module.h
)

add_link_options(-lpthread)

add_library(${target} SHARED ${SOURCES})

set_property(TARGET ${target} PROPERTY VERSION ${version})
set_property(TARGET ${target} PROPERTY SOVERSION 1)
set_property(TARGET ${target} PROPERTY
  INTERFACE_PubSub_MAJOR_VERSION 1)
set_property(TARGET ${target} APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING PubSub_MAJOR_VERSION
)

target_include_directories(${target} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(TARGETS ${target}
        EXPORT ${exportedTarget}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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




