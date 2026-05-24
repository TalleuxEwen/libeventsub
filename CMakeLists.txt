cmake_minimum_required(VERSION 3.14)
project(libeventsub C)

set(CMAKE_C_STANDARD 11)

add_library(libeventsub SHARED libeventsub.c)
target_include_directories(libeventsub PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)
install(TARGETS libeventsub EXPORT libeventsub-targets)
install(FILES libeventsub.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT libeventsub-targets
    FILE libeventsub-config.cmake
    NAMESPACE libeventsub::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libeventsub
)

