cmake_minimum_required(VERSION 3.20)
project(DaneJoeStringify VERSION 0.2.0 LANGUAGES CXX)
option(DANEJOE_STRINGIFY_BUILD_TESTS "Build tests for DaneJoeStringify" ${BUILD_TESTING})
option(DANEJOE_ALLOW_FETCH "Allow fetching DaneJoe deps from remote if not found locally" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS AND CMAKE_GENERATOR MATCHES "Ninja|Makefiles")
  add_custom_target(update_compile_commands_${PROJECT_NAME} ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/compile_commands.json"
            "${CMAKE_CURRENT_LIST_DIR}/compile_commands.json")
endif()

if(NOT TARGET DaneJoe::Common)
  find_package(DaneJoeCommon CONFIG QUIET)
  if(NOT DaneJoeCommon_FOUND)
    set(_COMMON_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/../common")
    if(EXISTS "${_COMMON_SRC_DIR}/CMakeLists.txt")
      add_subdirectory("${_COMMON_SRC_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/_deps/common")
    elseif(DANEJOE_ALLOW_FETCH)
      include(FetchContent)
      FetchContent_Declare(
        DaneJoeCommon
        GIT_REPOSITORY https://github.com/DaneJoe001/DaneJoeCommon.git
        GIT_TAG v0.1.1
      )
      FetchContent_MakeAvailable(DaneJoeCommon)
    endif()
  endif()
endif()

add_library(DaneJoeStringify
  "source/danejoe/stringify/stringify_config.cpp"
  "source/danejoe/stringify/stringify_format.cpp"
)
add_library(DaneJoe::Stringify ALIAS DaneJoeStringify)

set_target_properties(DaneJoeStringify PROPERTIES EXPORT_NAME Stringify)

target_include_directories(DaneJoeStringify
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(MSVC)
  target_compile_options(DaneJoeStringify PRIVATE /utf-8)
endif()

target_link_libraries(DaneJoeStringify
  PUBLIC
    DaneJoe::Common
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
  endif()
endif()
