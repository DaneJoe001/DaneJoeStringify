cmake_minimum_required(VERSION 4.0.1)

project(DaneJoeStringify LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加编码设置
if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

set(TEST_MT_QUEUE on)

file(GLOB LIB_SOURCE
    "source/stringify/*.cpp"
)

file(GLOB TEST_SOURCE
    "source/test/*.cpp"
)

add_library(DaneJoeStringify STATIC ${LIB_SOURCE})

target_include_directories(DaneJoeStringify PUBLIC "include")

# Threads (for Linux/Unix)
if(NOT MSVC)
    find_package(Threads REQUIRED)
    target_link_libraries(DaneJoeStringify PUBLIC Threads::Threads)
endif()

# 创建测试可执行文件
if(TEST_MT_QUEUE)
    add_executable(DaneJoeStringifyTest ${TEST_SOURCE})
    # 设置测试的include目录
    target_include_directories(DaneJoeStringifyTest PRIVATE include)
    target_link_libraries(DaneJoeStringifyTest PRIVATE DaneJoeStringify)
endif()