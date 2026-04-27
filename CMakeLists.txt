cmake_minimum_required(VERSION 3.14)
project(libeventsub C)

set(CMAKE_C_STANDARD 11)

# Header-only library target
add_library(libeventsub INTERFACE)
target_include_directories(libeventsub INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Install rules (pour find_package en bonus)
include(GNUInstallDirs)
install(TARGETS libeventsub EXPORT libeventsub-targets)
install(FILES libeventsub.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT libeventsub-targets
    FILE libeventsub-config.cmake
    NAMESPACE libeventsub::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libeventsub
)

