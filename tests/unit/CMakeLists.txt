cmake_minimum_required(VERSION 3.20)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_executable(danejoe_stringify_unit_tests
  "${CMAKE_CURRENT_LIST_DIR}/source/test_smoke.cpp"
)

target_link_libraries(danejoe_stringify_unit_tests
  PRIVATE
    DaneJoe::Stringify
    GTest::gtest_main
)

if(MSVC)
  target_compile_options(danejoe_stringify_unit_tests PRIVATE /utf-8)
endif()

gtest_discover_tests(danejoe_stringify_unit_tests
  DISCOVERY_TIMEOUT 30
  PROPERTIES
    LABELS unit
)
