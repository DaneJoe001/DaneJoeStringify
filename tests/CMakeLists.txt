cmake_minimum_required(VERSION 3.20)

add_executable(danejoe_stringify_demo
  "${CMAKE_CURRENT_LIST_DIR}/source/main.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/source/demo_stringify.cpp"
)

target_link_libraries(danejoe_stringify_demo PRIVATE DaneJoe::Stringify)
target_include_directories(danejoe_stringify_demo PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/include"
)

add_test(NAME stringify.demo COMMAND danejoe_stringify_demo)
