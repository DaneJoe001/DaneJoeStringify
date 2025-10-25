#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <optional>
#include <variant>
#include <string_view>
#include <chrono>
#include <tuple>

#include "danejoe/stringify/stringify.hpp"
#include "demo_stringify.hpp"

namespace demo {

void run_stringify_demo()
{
    std::cout<<DaneJoe::format_title("Demo Stringify")<<std::endl;
    auto time_point = std::chrono::system_clock::now();
    std::cout << "time_point: " << DaneJoe::to_string(time_point) << std::endl;
    const char* c_str = "A C-style string";
    std::cout << "C-style string: " << DaneJoe::to_string(c_str) << std::endl;
    char c_array[] = "A char array";
    std::cout << "char array: " << DaneJoe::to_string(c_array) << std::endl;
    std::chrono::milliseconds duration(1500);
    std::cout << "Duration: " << DaneJoe::to_string(duration) << std::endl;
    std::cout << "std::string: " << DaneJoe::to_string(std::string("A std::string")) << std::endl;
    std::cout << "std::vector: " << DaneJoe::to_string(std::vector<int>{1, 2, 3}) << std::endl;
    std::cout <<"r_string: "<<"A r_string"<<std::endl;
    enum Color { RED, GREEN, BLUE };
    Color color = GREEN;
    std::cout << "Enum: " << DaneJoe::to_string(color) << std::endl;
    std::pair<std::string, int> my_pair = {"age", 30};
    std::cout << "std::pair: " << DaneJoe::to_string(my_pair) << std::endl;
    std::cout << "std::tuple: " << DaneJoe::to_string(std::make_tuple(1, "two", 3.0)) << std::endl;
    std::cout << "std::map: " << DaneJoe::to_string(std::map<std::string, int>{{"one", 1}, { "two", 2 }}) << std::endl;
    std::cout << "std::unordered_map: " << DaneJoe::to_string(std::unordered_map<std::string, int>{{"one", 1}, { "two", 2 }}) << std::endl;
    std::cout << "std::set: " << DaneJoe::to_string(std::set<int>{1, 2, 3}) << std::endl;
    std::cout << "std::unordered_set: " << DaneJoe::to_string(std::unordered_set<int>{1, 2, 3}) << std::endl;
    std::cout << "std::multiset: " << DaneJoe::to_string(std::multiset<int>{1, 2, 3}) << std::endl;
    int int_array[] = { 1, 2, 3 };
    std::cout << "C-style array: " << DaneJoe::to_string(int_array,sizeof(int_array)/sizeof(int)) << std::endl;
    std::cout<<DaneJoe::format_separator()<<std::endl;
    std::optional<int> optional_int = 42;
    std::cout << "std::optional: " << DaneJoe::to_string(optional_int) << std::endl;
    std::variant<int, std::string> variant_int = 42;
    std::cout << "std::variant: " << DaneJoe::to_string(variant_int) << std::endl;
    std::string_view sv = "A string_view";
    std::cout << "std::string_view: " << DaneJoe::to_string(sv) << std::endl;
    std::vector<int> vec = {1, 2, 3};
    std::tuple<int, std::string, double,std::vector<int>> my_tuple = {1, "two", 3.0,vec};
    std::cout << "std::tuple: " << DaneJoe::to_string(my_tuple) << std::endl;
    std::cout << DaneJoe::format_string_list(4, ". ", { "This is Item1!","This is Item2!" },true) << std::endl;
    std::cout<< "Formatted Capacity Size: " << DaneJoe::format_capacity_size(1024*1024*1024,DaneJoe::Stringify::StorageUnit::MegaByte) << std::endl;
}

} // namespace demo
