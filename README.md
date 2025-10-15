# DaneJoe Stringify

一个轻量的 C++17 字符串化工具库，提供统一的 `to_string` 接口，将常见类型、容器、数组与 `std::pair` 转换为可读字符串。支持检测：
- 拥有成员函数 `to_string()` 的类型
- 可被 `std::to_string(T)` 处理的类型
- 标准容器及任意可迭代类型（具有 `begin/end`）
- 枚举类型（以底层值展示）
- 字符、布尔、字符串与 `std::pair`

包含一个变量名转字符串的宏 `VARIABLE_NAME_TO_STRING(x)`，便于打印调试。

## 特性
- 统一且可扩展的字符串化入口：`DaneJoe::to_string` / `DaneJoe::try_to_string`
- 自动分派：优先成员 `to_string()`，其次 `std::to_string`，否则提供类型名占位
- 容器友好：自动遍历并格式化为 `[a, b, c]`
- `std::pair` 友好：`{first:second}`
- 纯头文件 API；如需使用测试或生成静态库，可通过 CMake 构建

## 目录结构
```text
include/
  stringify/stringify.hpp   # 公开头文件
source/
  stringify/stringify.cpp   # 便于生成静态库的空实现
  test/main.cpp             # 简单示例/测试
CMakeLists.txt              # 构建脚本（生成静态库与测试）
README.md
```

## 快速开始
### 方式一：仅头文件引入
将 `include` 目录加入你的编译器包含路径，然后在代码中：
```cpp
#include "stringify/stringify.hpp"

#include <iostream>
#include <unordered_map>

int main() {
    std::unordered_map<int, std::string> map1{{1, "one"}, {2, "two"}, {3, "three"}};
    std::cout << DaneJoe::to_string(map1) << std::endl; // 输出: [ {1:one}, {2:two}, {3:three} ]
}
```

> 提示：库会对可迭代对象逐个递归调用 `to_string`，并以逗号分隔包装在 `[]` 中。

### 方式二：通过 CMake 构建并链接静态库
```bash
cmake -S . -B build/gcc-debug -D CMAKE_BUILD_TYPE=Debug
cmake --build build/gcc-debug -j
```
将目标 `DaneJoeStringify` 链接到你的可执行程序：
```cmake
add_subdirectory(path/to/library_danejoe_stringify)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE DaneJoeStringify)
target_include_directories(MyApp PRIVATE path/to/library_danejoe_stringify/include)
```

> 非 MSVC 平台会自动链接 `Threads::Threads`（见 `CMakeLists.txt`）。

## API 概览
头文件：`stringify/stringify.hpp`

- 宏
  - `VARIABLE_NAME_TO_STRING(x)`: 将变量名转换为字面字符串。

- 函数模板
  - `std::string DaneJoe::to_string(const T& value)`
    - 对字符串原样返回；
    - 对可迭代类型（具有 `begin/end`）输出 `[a, b, c]`；
    - 对 `std::pair<Key,Value>` 输出 `{first:second}`；
    - 其他类型委托给 `try_to_string`。
  - `std::string DaneJoe::to_string(const T* array, std::size_t count = 1)`
    - 指针数组格式化：空或 `count==0` 输出 `[]`；`count==1` 退化为元素 `to_string`；否则输出 `[e0, e1, ...]`。
  - `std::string DaneJoe::try_to_string(const T& obj)`
    - 若 `T` 是枚举：输出 `typeid(T).name()<underlying>`；
    - 若 `T` 是 `std::string`、`char`/`unsigned char`、`bool`：做相应转换；
    - 若类型具有 `to_string()`：优先调用；
    - 若 `std::to_string(T)` 可用：其次调用；
    - 否则输出 `typeid(T).name()<No to_string>`。

> 通过内部萃取 `has_member_to_string`、`has_std_to_string`、`has_iterator` 实现分派。

## 示例
```cpp
#include "stringify/stringify.hpp"
#include <vector>
#include <utility>
#include <iostream>

enum class Color : int { Red = 1, Green = 2 };

struct User {
    std::string name;
    int age;
    std::string to_string() const { return "User{" + name + ":" + std::to_string(age) + "}"; }
};

int main() {
    std::vector<int> v{1,2,3};
    std::pair<int, std::string> p{7, "seven"};
    Color c = Color::Green;
    User u{"alice", 30};

    std::cout << DaneJoe::to_string(v) << "\n";        // [1, 2, 3]
    std::cout << DaneJoe::to_string(p) << "\n";        // {7:seven}
    std::cout << DaneJoe::to_string(c) << "\n";        // 类名<2>
    std::cout << DaneJoe::to_string(&v[0], v.size()) << "\n"; // [1, 2, 3]
    std::cout << DaneJoe::to_string(u) << "\n";        // User{alice:30}
}
```

## 兼容性
- 要求 C++17 及以上编译器
- 在 MSVC、GCC/Clang 上均可使用（工程已提供相应 CMake 预设）

## 局限与注意事项
- 对枚举类型使用 `typeid(T).name()`，名称可能与编译器相关且未解码
- `char/unsigned char` 被当作单字符输出；若需整数形式请先显式转换
- 对于不满足成员或标准 `to_string` 的类型，将输出 `<No to_string>` 占位
- 迭代器检测基于 `std::begin/std::end`，自定义类型需提供相应 ADL 可见的重载

## 许可证
MIT（如需调整请在此处更新）

## 致谢
- 灵感来自常见的字符串化工具与 `std::to_string` 的局限性
- 欢迎 Issue/PR 改进容器分隔符/格式化策略
