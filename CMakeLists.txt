# @brief 设置 CMake 最小版本
# @note 本库使用 CMake 3.20 及以上版本（支持 CMakePresets、现代安装目录变量等）
# @param VERSION 项目版本
cmake_minimum_required(VERSION 3.20)

# @brief 项目信息与全局设置
# @param semantic_version 使用语义化版本，便于导出包与安装兼容性管理
# @param language_only 仅开启 C++ 作为语言
# @param top_level_reuse 保持顶层可复用性：尽量避免全局编译选项污染
project(DaneJoeStringify VERSION 1.0.0 LANGUAGES CXX)

# @brief 引入标准安装目录变量
# @param install_dirs 使用 GNUInstallDirs 以跨平台一致的安装路径（lib/include 等）
include(GNUInstallDirs)

# @brief 选项开关
# @note 保持最小表面；测试默认禁用，避免作为子项目时污染
option(DANEJOE_STRINGIFY_BUILD_TESTS "Build tests" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# @brief MSVC 特定编译开关
# @note 统一 UTF-8 源码并禁止不安全函数告警
if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()


# @brief 生成版本头
# @param purpose 供运行时/编译期查询库版本
# @param visibility 在构建树与安装树均可见
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/version/stringify_version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/version/stringify_version.h"
    @ONLY
)

# @brief 定义库目标
# @param no_glob 显式列出源，避免 GLOB，便于增量构建与可维护性
# @param alias 真实目标名不带 ::，提供 ALIAS 以命名空间形式对外暴露
add_library(danejoe_stringify
    source/stringify/stringify.cpp
)

# @brief 对外命名空间别名
# @note 消费者以 danejoe::stringify 使用
add_library(danejoe::stringify ALIAS danejoe_stringify)

# @brief 以“目标特性”声明 C++17（现代 CMake 推荐）
# @param min_standard 声明最低标准为 C++17，消费者可使用更高标准
target_compile_features(danejoe_stringify PUBLIC cxx_std_17)

# @brief 公开包含目录（构建树 + 安装树）
# @param build_interface 构建树时公开源码与生成头路径
# @param install_interface 安装树时公开安装后的 include 路径
target_include_directories(danejoe_stringify
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# @brief 目标元数据
# @param export_name 对外导出名（与命名空间组合）
# @param version 项目版本
# @param soversion 主版本（静态库无链接影响，保留无害）
set_target_properties(danejoe_stringify PROPERTIES
    EXPORT_NAME stringify
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# @brief 测试（仅当显式开启时）
# @param gate 由 DANEJOE_STRINGIFY_BUILD_TESTS 控制开关
if(DANEJOE_STRINGIFY_BUILD_TESTS)
    enable_testing()
    add_executable(danejoe_stringify_test
        source/test/main.cpp
    )
    target_link_libraries(danejoe_stringify_test PRIVATE danejoe::stringify)
    add_test(NAME stringify.sample COMMAND danejoe_stringify_test)
endif()

# @brief 安装与导出封装（仅顶层工程执行，避免子项目时做安装/导出）
# @param package DaneJoeStringify 包名
# @param namespace 导出命名空间（danejoe::）
# @param install_dir 安装的 CMake 包目录
# @param config_in 包配置模板输入路径
# @param config_out 生成的包配置输出路径
# @param version 项目版本号
# @param build_export_file 构建树导出的 targets 文件
# @param install_export_file 安装树导出的 targets 文件名
if(PROJECT_IS_TOP_LEVEL)
    # @brief 安装与导出封装
    include(cmake/danejoe_install_export.cmake)
    danejoe_install_export(
        TARGETS danejoe_stringify
        PACKAGE DaneJoeStringify
        NAMESPACE danejoe::
        INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/DaneJoeStringify"
        CONFIG_IN  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/DaneJoeStringifyConfig.cmake.in"
        CONFIG_OUT "${CMAKE_CURRENT_BINARY_DIR}/DaneJoeStringifyConfig.cmake"
        VERSION    "${PROJECT_VERSION}"
        BUILD_EXPORT_FILE "${CMAKE_CURRENT_BINARY_DIR}/DaneJoeStringifyTargets.cmake"
        INSTALL_EXPORT_FILE "DaneJoeStringifyTargets.cmake"
    )
endif()