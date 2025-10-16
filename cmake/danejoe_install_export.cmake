# @brief 安装与导出封装函数
# @param PACKAGE 包名（用于生成 *Config*.cmake/*Targets.cmake）
# @param NAMESPACE 导出命名空间（例如 danejoe::）
# @param INSTALL_DIR 安装的 CMake 包目录（lib/cmake/<Package>）
# @param CONFIG_IN 包配置模板输入路径（*.cmake.in）
# @param CONFIG_OUT 生成的包配置输出路径（构建树中的 *.cmake）
# @param VERSION 包版本（写入 *ConfigVersion.cmake）
# @param INSTALL_EXPORT_FILE 安装树导出的 targets 文件名（默认 <Package>Targets.cmake）
# @param BUILD_EXPORT_FILE 可选：构建树导出的 targets 文件完整路径
# @param TARGETS 需要导出/安装的目标列表
# @note
#   - 统一安装导出流程：生成 Config/ConfigVersion、安装 Targets 与头文件、导出命名空间目标
#   - 便于多库复用，保持一致目录与命名
#   - 构建树导出仅在提供 BUILD_EXPORT_FILE 时启用

function(danejoe_install_export)
    cmake_parse_arguments(ARG "" "PACKAGE;NAMESPACE;INSTALL_DIR;CONFIG_IN;CONFIG_OUT;VERSION;INSTALL_EXPORT_FILE;BUILD_EXPORT_FILE" "TARGETS" ${ARGN})

    if(NOT ARG_PACKAGE)
        message(FATAL_ERROR "danejoe_install_export: PACKAGE is required")
    endif()
    if(NOT ARG_NAMESPACE)
        message(FATAL_ERROR "danejoe_install_export: NAMESPACE is required (e.g. danejoe::)")
    endif()
    if(NOT ARG_INSTALL_DIR)
        message(FATAL_ERROR "danejoe_install_export: INSTALL_DIR is required")
    endif()
    if(NOT ARG_CONFIG_IN OR NOT ARG_CONFIG_OUT)
        message(FATAL_ERROR "danejoe_install_export: CONFIG_IN and CONFIG_OUT are required")
    endif()
    if(NOT ARG_VERSION)
        message(FATAL_ERROR "danejoe_install_export: VERSION is required")
    endif()
    if(NOT ARG_INSTALL_EXPORT_FILE)
        set(ARG_INSTALL_EXPORT_FILE "${ARG_PACKAGE}Targets.cmake")
    endif()

    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    # @brief 构建树导出（可选）
    # @param BUILD_EXPORT_FILE 若设置，则导出构建树 targets 并注册 PACKAGE
    if(ARG_BUILD_EXPORT_FILE)
        export(
            TARGETS ${ARG_TARGETS}
            NAMESPACE ${ARG_NAMESPACE}
            FILE "${ARG_BUILD_EXPORT_FILE}"
        )
        export(PACKAGE ${ARG_PACKAGE})
    endif()

    # @brief 生成版本文件
    # @param VERSION 写入 <Package>ConfigVersion.cmake
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${ARG_PACKAGE}ConfigVersion.cmake"
        VERSION ${ARG_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    # @brief 生成安装用 Config
    # @param CONFIG_IN/CONFIG_OUT/INSTALL_DIR
    configure_package_config_file(
        "${ARG_CONFIG_IN}"
        "${ARG_CONFIG_OUT}"
        INSTALL_DESTINATION "${ARG_INSTALL_DIR}"
    )

    # @brief 安装库与头文件
    # @param TARGETS 安装导出的目标；头文件包含源码 include 与生成 include
    install(TARGETS ${ARG_TARGETS}
        EXPORT ${ARG_PACKAGE}Targets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    # @brief 安装导出文件与包配置
    # @param INSTALL_EXPORT_FILE 安装树 targets 文件名
    install(EXPORT ${ARG_PACKAGE}Targets
        NAMESPACE ${ARG_NAMESPACE}
        DESTINATION "${ARG_INSTALL_DIR}"
        FILE "${ARG_INSTALL_EXPORT_FILE}"
    )

    install(FILES
        "${ARG_CONFIG_OUT}"
        "${CMAKE_CURRENT_BINARY_DIR}/${ARG_PACKAGE}ConfigVersion.cmake"
        DESTINATION "${ARG_INSTALL_DIR}"
    )
endfunction()


