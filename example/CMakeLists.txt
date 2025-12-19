cmake_minimum_required(VERSION 3.20)

add_executable(danejoe_stringify_example
  "${CMAKE_CURRENT_LIST_DIR}/source/main.cpp"
)

target_link_libraries(danejoe_stringify_example
  PRIVATE
    DaneJoe::Stringify
)

if(MSVC)
  target_compile_options(danejoe_stringify_example PRIVATE /utf-8)
endif()
