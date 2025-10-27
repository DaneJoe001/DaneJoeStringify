cmake_minimum_required(VERSION 3.20)
project(DaneJoeStringify VERSION 0.2.0 LANGUAGES CXX)
option(DANEJOE_STRINGIFY_BUILD_TESTS "Build tests for DaneJoeStringify" ${BUILD_TESTING})

add_library(DaneJoeStringify
  "source/danejoe/stringify/stringify.cpp"
)
add_library(DaneJoe::Stringify ALIAS DaneJoeStringify)

target_include_directories(DaneJoeStringify
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(DaneJoeStringify PUBLIC cxx_std_20)

# 版本与 SOVERSION
set_target_properties(DaneJoeStringify PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 0)

include(GNUInstallDirs)
install(TARGETS DaneJoeStringify
  EXPORT DaneJoeStringifyTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
install(DIRECTORY "include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# version header
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/danejoe/version")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/danejoe/version/stringify_version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/danejoe/version/stringify_version.h"
  @ONLY
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/include/danejoe/version/stringify_version.h"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/danejoe/version")

# export targets (no package config for now)
install(EXPORT DaneJoeStringifyTargets
  NAMESPACE DaneJoe::
  FILE DaneJoeStringifyTargets.cmake
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/DaneJoeStringify"
)

# Config + Version files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/DaneJoeStringifyConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_file("cmake/DaneJoeStringifyConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/DaneJoeStringifyConfig.cmake" @ONLY)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/DaneJoeStringifyConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/DaneJoeStringifyConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/DaneJoeStringify"
)

if(DANEJOE_STRINGIFY_BUILD_TESTS)
  include(CTest)
  enable_testing()
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
  else()
    # create a simple tests CMake if missing
  endif()
endif()
