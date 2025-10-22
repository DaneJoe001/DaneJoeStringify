#include <iostream>
#include <unordered_map>
#include "danejoe/stringify/stringify.hpp"
#include "demo_stringify.hpp"

namespace demo {

void run_stringify_demo()
{
    std::unordered_map<int, std::string> map1 = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
    };
    std::cout << "map1: " << DaneJoe::to_string(map1) << std::endl;
}

} // namespace demo
