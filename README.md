# DaneJoe Stringify

通用 to_string 库，支持 STL 容器等常见类型的字符串化。

## 构建
```bash
cmake -S . -B build --preset gcc-debug -DBUILD_TESTING=ON
cmake --build build
```

## 运行示例/测试
```bash
ctest --test-dir build -L unit --output-on-failure
```

## 作为依赖使用
CMake:
```cmake
find_package(DaneJoeStringify CONFIG REQUIRED)
add_executable(app main.cpp)
target_link_libraries(app PRIVATE DaneJoe::Stringify)
```

聚合包（推荐）：
```cmake
find_package(DaneJoe CONFIG REQUIRED COMPONENTS Stringify)
add_executable(app main.cpp)
target_link_libraries(app PRIVATE DaneJoe::Stringify)
```
