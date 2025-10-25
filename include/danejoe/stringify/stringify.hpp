#pragma once

/**
 * @file stringify.hpp
 * @brief 字符串化工具库
 * @author DaneJoe001
 * @note
 *   - 提供统一的 to_string 接口，将常见类型、容器、数组与 std::pair 转换为可读字符串
 *   - 支持检测：拥有成员函数 to_string() 的类型、可被 std::to_string(T) 处理的类型、标准容器及任意可迭代类型（具有 begin/end）、枚举类型（以底层值展示）、字符、布尔、字符串与 std::pair
 *   - 包含一个变量名转字符串的宏 VARIABLE_NAME_TO_STRING(x)，便于打印调试
 * @version 0.1.1
 * @date 2025-10-24
 */

#include <string>
#include <chrono>
#include <ctime>
#include <tuple>
#include <optional>
#include <variant>
#include <utility>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <typeinfo>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <type_traits>
#include <mutex>

/// @brief 将变量并转为字符串
#define VARIABLE_NAME_TO_STRING(x) #x

/**
 * @namespace DaneJoe
 * @brief 字符串化工具库
 */
namespace DaneJoe
{
    /**
     * @namespace DaneJoe::Stringify
     * @brief 字符串化工具库
     */
    namespace Stringify
    { 
        /**
         * @struct DelimiterSymbol
         * @brief 分隔符符号
         */
        struct DelimiterSymbol
        {
            /// @brief 开始符号
            std::string start_maker="[";
            /// @brief 结束符号
            std::string end_maker="]";
            /// @brief 元素分隔符
            std::string element_separator = ",";
            /// @brief 空格
            std::string space_maker=" ";
        };
        /**
         * @struct TimeSymbol
         * @brief 时间单位符号
         */
        struct TimeSymbol
        {
            /// @brief 秒
            std::string second_symbol="s";
            /// @brief 毫秒
            std::string millisecond_symbol="ms";
            /// @brief 微秒
            std::string microsecond_symbol="us";
            /// @brief 纳秒
            std::string nanosecond_symbol="ns";
        };
        /**
         * @struct StorageSymbol
         * @brief 容量单位符号
         */
        struct StorageSymbol
        {
            /// @brief 字节
            std::string byte_symbol="B";
            /// @brief 千字节
            std::string kilobyte_symbol="KB";
            /// @brief 兆字节
            std::string megabyte_symbol="MB";
            /// @brief 吉字节
            std::string gigabyte_symbol="GB";
            /// @brief 太字节
            std::string terabyte_symbol="TB";
            /// @brief 万字节
            std::string petabyte_symbol="PB";
            /// @brief 十万字节
            std::string exabyte_symbol="EB";
            /// @brief 百万字节
            std::string zettabyte_symbol="ZB";
            /// @brief 十亿字节
            std::string yottabyte_symbol = "YB";
            /// @brief 空占位
            std::string space_maker=" ";
        };
        /**
         * @struct BoolSymbol
         * @brief 布尔值符号
         */
        struct BoolSymbol
        {
            /// @brief 真
            std::string true_symbol="true";
            /// @brief 假
            std::string false_symbol="false";
        };
        /**
         * @struct EnumSymbol
         * @brief 枚举值符号
         */
        struct EnumSymbol
        {
            /// @brief 类型符号
            DelimiterSymbol type_symbol={"<",">",":"," "};
            /// @brief 值符号
            DelimiterSymbol value_symbol={"(",")",":"," "};
        };
        /**
         * @struct Config
         * @brief 配置
         */
        struct Config
        {
            /// @brief std::pair符号
            DelimiterSymbol pair_symbol={"{","}",":"," "};
            /// @brief std::tuple符号
            DelimiterSymbol tuple_symbol={"(",")",","," "};
            /// @brief 容器符号
            DelimiterSymbol container_symbol = { "[","]",","," " };
            /// @brief 枚举符号
            EnumSymbol enum_symbol={{"<",">",":"," "},{"(",")",":"," "}};
            /// @brief 空值占位符
            std::string empty_place_holder = "<empty>";
            /// @brief variant空值占位符
            std::string variant_valueless_placeholder="<valueless>";
            /// @brief 未支持类型占位符
            std::string unsupported_type_place_holder = "<unsupported type>";
            /// @brief 空值占位符
            std::string null_value_symbol = "<null>";
            /// @brief 省略号
            std::string ellipsis_symbol="...";
            /// @brief 时间单位符号
            TimeSymbol time_symbol={"s","ms","us","ns"};
            /// @brief 布尔值符号
            BoolSymbol bool_symbol={"true", "false"};
            /// @brief 存储单位
            int storage_units=1024;
            /// @brief 存储单位符号
            StorageSymbol storage_symbol=StorageSymbol();
            /// @brief 最大递归深度
            /// @note 暂未启用
            int max_depth=-1;
            /// @brief 最大字符串化元素数量
            /// @note 暂未启用
            /// @note 超过部分使用...表示
            int max_stringify_element_count=-1;
        };
        /**
         * @enum StorageUnit
         * @brief 存储单位
         */
        enum class StorageUnit
        {
            Byte=0,
            KiloByte,
            MegaByte,
            GigaByte,
            TeraByte,
            PetaByte,
            ExaByte,
            ZettaByte,
            YottaByte
        };
        /**
         * @class ConfigManager
         * @brief 配置管理器
         */
        class ConfigManager
        {
        public:
            /**
             * @brief 获取配置
             * @return 配置
             */
            static Config get_config();
            /**
             * @brief 设置配置
             * @param config 配置
             */
            static void set_config(const Config& config);
        private:
            /// @brief 配置
            static Config m_config;
            /// @brief 互斥锁
            static std::mutex m_mutex;
        };
    }

    /**
     * @brief 通用to_string接口
     * @tparam T 类型
     * @param obj 对象
     * @return 转换后的字符串
     */
    template<class T>
    std::string to_string(const T& obj);
    /**
     * @brief 尝试将std::string转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<std::is_same<T, std::string>::value, int> = 0>
    std::string from_std_string(const T& obj)
     {
         return obj;
    }
    /**
     * @brief 尝试将std::string_view转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T>
    struct is_std_string_view:std::false_type{};
    /**
     * @brief 检测std::string_view类型
     */
    template<class CharT, class Traits>
    struct is_std_string_view<std::basic_string_view<CharT, Traits>> : std::true_type {}; 
    /**
     * @brief 尝试将std::string_view转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<is_std_string_view<T>::value, int> = 0>
    std::string from_std_string_view(const T& obj)
    {
        return std::string(obj);
    }
    /**
     * @brief 检测const char*类型
     */
    template<class T>
    struct is_c_string :std::bool_constant<std::is_pointer_v<std::remove_reference_t<T>>&& std::is_same_v<std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<T>>>, char>> {};
    /**
     * @brief 尝试将const char*转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<is_c_string<T>::value, int> = 0>
    std::string from_c_string(const T& value)
    {
        return std::string(value);
    }
    /**
     * @brief 尝试将枚举转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
    std::string from_enum(const T& value)
    {
        using U = typename std::underlying_type<T>::type;
        std::ostringstream oss;
        oss<<Stringify::ConfigManager::get_config().enum_symbol.type_symbol.start_maker<<
        std::string(typeid(value).name())<<
        Stringify::ConfigManager::get_config().enum_symbol.type_symbol.end_maker<<
        Stringify::ConfigManager::get_config().enum_symbol.value_symbol.start_maker<<
        std::to_string(static_cast<U>(value))<<
        Stringify::ConfigManager::get_config().enum_symbol.value_symbol.end_maker;
        return oss.str();
    }
    /**
     * @brief 尝试将字符转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<std::is_same<T, char>::value ||
        std::is_same<T, unsigned char>::value, int> = 0>
    std::string from_char(const T & value)
    {
        return std::string(1, static_cast<char>(value));
    }
    /**
     * @brief 尝试将布尔转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<std::is_same<T, bool>::value, int> = 0>
    std::string from_bool(const T& value)
    {
        return value ?
        Stringify::ConfigManager::get_config().bool_symbol.true_symbol :
        Stringify::ConfigManager::get_config().bool_symbol.false_symbol;
    }
    /**
     * @brief 判断是否为std::chrono::duration
     * @tparam T 类型
     * @note 错误类型分支
     */
     template<class T>
     struct is_chrono_duration:std::false_type{};
     /**
      * @brief 判断是否为std::chrono::duration
      * @tparam Rep 时长单位
      * @tparam Period 时长周期
      * @note 正确类型分支
      */
     template<class Rep, class Period>
     struct is_chrono_duration<std::chrono::duration<Rep, Period>> :std::true_type {};
     /**
      * @brief 判断是否为std::chrono::time_point
      * @tparam T 类型
      * @note 错误类型分支
      */
     template<class T>
     struct is_chrono_time_point:std::false_type{};
     /**
      * @brief 判断是否为std::chrono::time_point
      * @tparam T 类型
      * @note 正确类型分支
      */
      template<class T> 
      struct is_chrono_time_point<std::chrono::time_point<std::chrono::system_clock, T>> : std::true_type {};
     /**
      * @brief 判断时间单位是否为微秒
      * @tparam T 类型
      * @note 错误类型分支
      */
     template<typename T>
     struct is_chrono_microseconds:std::false_type{};
     /**
      * @brief 判断时间单位是否为微秒
      * @tparam Rep 时长单位
      * @tparam Period 时长周期
      * @note 正确类型分支
      */
     template<class T>
     struct is_chrono_microseconds<std::chrono::duration<T, std::micro>> :std::true_type{};
     /**
      * @brief 判断时间单位是否为毫秒
      * @tparam T 类型
      * @note 正确类型分支
      */
     template<typename T>
     struct is_chrono_milliseconds:std::false_type{};
     /**
      * @brief 判断时间单位是否为毫秒
      * @tparam Rep 时长单位
      * @tparam Period 时长周期
      * @note 正确类型分支
      */
     template<class T>
     struct is_chrono_milliseconds<std::chrono::duration<T, std::milli>> :std::true_type{};
     /**
      * @brief 判断时间单位是否为纳秒
      * @tparam T 类型
      * @note 正确类型分支
      */
     template<typename T>
     struct is_chrono_nanoseconds:std::false_type{};
     /**
      * @brief 判断时间单位是否为纳秒
      * @tparam Rep 时长单位
      * @tparam Period 时长周期
      * @note 正确类型分支
      */
     template<class T>
     struct is_chrono_nanoseconds<std::chrono::duration<T, std::nano>> :std::true_type{};
     /**
      * @brief 判断时间单位是否为秒
      * @tparam T 类型
      * @note 正确类型分支
      */
     template<typename T>
     struct is_chrono_seconds:std::false_type{};
     /**
      * @brief 判断时间单位是否为秒
      * @tparam Rep 时长单位
      * @tparam Period 时长周期
      * @note 正确类型分支
      */
     template<class T>
     struct is_chrono_seconds<std::chrono::duration<T, std::ratio<1>>> :std::true_type{};
     /**
      * @brief 尝试将std::chrono::duration转为字符串
      * @tparam Period 时长类型
      * @param period 时长
      * @return 尝试转换后的字符串
      */
     template<class Period, std::enable_if_t<is_chrono_duration<Period>::value, int> = 0>
     std::string format_time_duration(const Period& period)
     {
        std::ostringstream oss;
        oss<<period.count();
        if constexpr (is_chrono_microseconds<Period>::value)
        {
             oss<<Stringify::ConfigManager::get_config().time_symbol.microsecond_symbol;
        }
         else if constexpr (is_chrono_milliseconds<Period>::value)
         {
             oss<<Stringify::ConfigManager::get_config().time_symbol.millisecond_symbol;
         }
         else if constexpr (is_chrono_nanoseconds<Period>::value)
         {
             oss<<Stringify::ConfigManager::get_config().time_symbol.nanosecond_symbol;
         }
         else if constexpr (is_chrono_seconds<Period>::value)
         {
             oss<<Stringify::ConfigManager::get_config().time_symbol.second_symbol;
         }
         return oss.str();
     }
     /**
      * @brief 尝试将std::chrono::time_point转为字符串
      * @tparam Period 时长类型
      * @param period 时长
      * @return 尝试转换后的字符串
      */
     template<class Period, std::enable_if_t<is_chrono_time_point<Period>::value, int> = 0>
     std::string format_time_point(const Period& period,const std::string& format="%Y-%m-%d %H:%M:%S")
     {
         std::time_t raw_time_t = std::chrono::system_clock::to_time_t(period);
         std::tm time_info;
         std::tm* time_info_ptr=nullptr;
         #if defined(_WIN32)
             localtime_s(&time_info, &raw_time_t); // Windows
             time_info_ptr=&time_info;
         #elif defined(__linux__)
             localtime_r(&raw_time_t, &time_info); // Linux
             time_info_ptr=&time_info;
         #elif defined(__APPLE__)
             localtime_r(&raw_time_t, &time_info); // macOS
             time_info_ptr=&time_info;
         #else
             time_info_ptr=std::localtime(&raw_time_t);
         #endif
        std::ostringstream oss;
        oss<<std::put_time(time_info_ptr, format.c_str());
        return oss.str();
     }
    /**
     * @brief 无std::to_string分支
     * @tparam T 类型
     * @note 模板类::velue返回true或false
     */
    template <typename T, typename = void>
    struct has_std_to_string : std::false_type{};
    /**
     * @brief std::to_string分支
     * @tparam T 类型
     */
    template <typename T>
    struct has_std_to_string<T, std::void_t<decltype(std::to_string(std::declval<T>()))>> : std::true_type{};
    /**
     * @brief 含有std::to_string分支
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T,std::enable_if_t<has_std_to_string<T>::value, int> = 0>
    std::string from_std_to_string(const T& obj)
    {
        return std::to_string(obj);
    }
    /**
     * @brief 无to_string成员函数分支
     * @tparam T 类型
     */
    template <typename T, typename = void>
    struct has_member_to_string : std::false_type{};
    /**
     * @brief to_string成员函数分支
     * @tparam T 类型
     */
    template <typename T>
    struct has_member_to_string<T, std::void_t<decltype(std::declval<T>().to_string())>> : std::true_type{};
    /**
     * @brief 含有to_string成员函数分支
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T,std::enable_if_t<has_member_to_string<T>::value,int> = 0>
    std::string from_member_to_string(const T& obj)
    {
        return obj.to_string();
    }
    /**
     * @brief 无迭代器分支
     * @tparam T 类型
     */
    template <typename T, typename = void>
    struct has_iterator : std::false_type{};
    /**
     * @brief 迭代器分支
     * @tparam T 类型
     */
    template <typename T>
    struct has_iterator<T, std::void_t<
        decltype(std::begin(std::declval<T>())),
        decltype(std::end(std::declval<T>()))>> : std::true_type{};
    /**
     * @brief 含有迭代器分支,但非字符串类型
     * @tparam T 
     * @param obj 
     * @return std::string 
     */
    template <class T, std::enable_if_t<has_iterator<T>::value && !std::is_same_v<T, std::string>, int> = 0>
    std::string from_has_iterator(const T& obj)
    {
        std::ostringstream oss;
        oss<<Stringify::ConfigManager::get_config().container_symbol.start_maker;
        for (auto iter = std::begin(obj); iter != std::end(obj);)
        {
            oss<<to_string(*iter);
            if (++iter != std::end(obj))
            {
                oss<<Stringify::ConfigManager::get_config().container_symbol.element_separator;
                oss<<Stringify::ConfigManager::get_config().container_symbol.space_maker;
            }
        }
        oss<<Stringify::ConfigManager::get_config().container_symbol.end_maker;
        return oss.str(); 
    }
    /**
     * @brief 尝试将C数组转为字符串
     * @tparam T 
     * @param array 
     * @param count 
     * @return std::string 
     */
    template<class T>
    std::string from_c_ptr(const T* ptr, std::size_t count)
    {
        if (ptr == nullptr || count == 0)
        {
            return Stringify::ConfigManager::get_config().container_symbol.start_maker +
                   Stringify::ConfigManager::get_config().container_symbol.end_maker;
        }
        std::ostringstream oss;
        oss<<Stringify::ConfigManager::get_config().container_symbol.start_maker;
        for (std::size_t i = 0; i < count; ++i)
        {
            oss<<DaneJoe::to_string(*(ptr + i));
            if (i + 1 < count)
            {
                oss<<Stringify::ConfigManager::get_config().container_symbol.element_separator;
                oss<<Stringify::ConfigManager::get_config().container_symbol.space_maker;
            }
        }
        oss<<Stringify::ConfigManager::get_config().container_symbol.end_maker;
        return oss.str();
    }
    template<class T>
    struct is_c_array:std::false_type{};
    template<class T, std::size_t N>
    struct is_c_array<T[N]>:std::true_type{};
    template<class T, std::size_t N>
    std::string from_c_array(const T(&array)[N])
    {
        return from_c_ptr(array, N);
    }
    /**
     * @brief 检测std::pair类型
     */
    template<class T>
    struct is_std_pair:std::false_type{};
    /**
     * @brief 检测std::pair类型
     */
    template<class Key, class Value>
    struct is_std_pair<std::pair<Key,Value>> :std::true_type{};
    /**
     * @brief 尝试将std::pair转为字符串
     * @tparam Key 键类型
     * @tparam Value 值类型
     * @param pair 键值对
     * @return 尝试转换后的字符串
     */
    template<class T,std::enable_if_t<is_std_pair<T>::value, int> = 0>
    std::string from_std_pair(const T& pair)
    {
        std::ostringstream oss;
        oss<<Stringify::ConfigManager::get_config().pair_symbol.start_maker;
        oss<<DaneJoe::to_string(pair.first);
        oss<<Stringify::ConfigManager::get_config().pair_symbol.element_separator +
            Stringify::ConfigManager::get_config().pair_symbol.space_maker;
        oss<<DaneJoe::to_string(pair.second);
        oss<<Stringify::ConfigManager::get_config().pair_symbol.end_maker;
        return oss.str();
    }
    /**
     * @brief 检测std::optional类型
     */
    template<class T>
    struct is_std_optional:std::false_type{};
    /**
     * @brief 检测std::optional类型
     */
    template<class T>
    struct is_std_optional<std::optional<T>>:std::true_type{};
    /**
     * @brief 尝试将std::optional转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T,std::enable_if_t<is_std_optional<T>::value, int> = 0>
    std::string from_std_optional(const T& obj)
    {
        if (obj.has_value())
        {
            return DaneJoe::to_string(obj.value());
        }
        else
        {
            return Stringify::ConfigManager::get_config().null_value_symbol;
        }
    }
    /**
     * @brief 检测std::variant类型
     */
    template<class T>
    struct is_std_variant:std::false_type{};
    /**
     * @brief 检测std::variant类型
     */
    template<class ...Args>
    struct is_std_variant<std::variant<Args...>>:std::true_type{};
    /**
     * @brief 尝试将std::variant转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T,std::enable_if_t<is_std_variant<T>::value, int> = 0>
    std::string from_std_variant(const T& obj)
    {
        if(obj.valueless_by_exception())
        {
            return Stringify::ConfigManager::get_config().variant_valueless_placeholder;
        }
        else
        {
            return std::visit([](const auto& arg)
            {
                return DaneJoe::to_string(arg);
            }, obj);
        }
    }
    /**
     * @brief 检测std::tuple类型
     */
    template<class T>
    struct is_std_tuple:std::false_type{};
    /**
     * @brief 检测std::tuple类型
     */
    template<class ...Args>
    struct is_std_tuple<std::tuple<Args...>>:std::true_type{};
    /**
     * @brief 尝试将std::tuple转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T,std::enable_if_t<is_std_tuple<T>::value, int> = 0>
    std::string from_std_tuple(const T& obj)
    {
        std::ostringstream oss;
        oss<<Stringify::ConfigManager::get_config().tuple_symbol.start_maker;
        std::apply([&](auto&&... args)
        {
                ((oss<<DaneJoe::to_string(args) <<
                    Stringify::ConfigManager::get_config().tuple_symbol.element_separator <<
                    Stringify::ConfigManager::get_config().tuple_symbol.space_maker),...);
        }, obj);
        // 元素不为空时去掉最后一个元素分隔符
        std::string str=oss.str();
        if(str!=Stringify::ConfigManager::get_config().tuple_symbol.start_maker)
        {
            str=str.substr(0,str.size()-
            Stringify::ConfigManager::get_config().tuple_symbol.element_separator.size()-
            Stringify::ConfigManager::get_config().tuple_symbol.space_maker.size());
        }
        str += Stringify::ConfigManager::get_config().tuple_symbol.end_maker;
        return str;
    }
    /**
     * @brief 检测类型是否支持流输出
     */
    template<class T>
    struct has_stream_out:std::false_type{};
    template<class T>
    struct has_stream_out<std::basic_ostream<T>>:std::true_type{};
    /**
     * @brief 尝试将类型转为字符串
     * @tparam T 类型
     * @param obj 对象
     * @return 尝试转换后的字符串
     */
    template<class T,std::enable_if_t<has_stream_out<T>::value, int> = 0>
    std::string from_stream_out(const T& obj)
    {
        std::stringstream ss;
        ss<<obj;
        return ss.str();
    }
    /**
     * @brief 无to_string分支
     * @tparam T 类型
     */
    template<class T>
    std::string from_fallback(const T& value)
    {
        return Stringify::ConfigManager::get_config().unsupported_type_place_holder;
    }
    /**
     * @brief 尝试将变量转为字符串
     * @tparam T 类型
     * @param value 变量
     */
    template<class T>
    std::string to_string(const T& value)
    {
        if constexpr (is_std_string_view<T>::value)
        {
            return from_std_string_view(value);
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            return from_std_string(value);
        }
        else if constexpr (is_c_string<T>::value)
        {
            return from_c_string(value);
        }
        else if constexpr (std::is_enum_v<T>)
        {
            return from_enum(value);
        }
        else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, unsigned char>)
        {
            return from_char(value);
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return from_bool(value);
        }
        else if constexpr (has_member_to_string<T>::value)
        {
            return from_member_to_string(value);
        }
        else if constexpr (has_std_to_string<T>::value)
        {
            return from_std_to_string(value);
        }
        else if constexpr (is_chrono_duration<T>::value)
        {
            return format_time_duration(value);
        }
        else if constexpr (is_chrono_time_point<T>::value)
        {
            return format_time_point(value);
        }
        else if constexpr (is_std_pair<T>::value)
        {
            return from_std_pair(value);
        }
        else if constexpr (is_std_optional<T>::value)
        {
            return from_std_optional(value);
        }
        else if constexpr (is_std_variant<T>::value)
        {
            return from_std_variant(value);
        }
        else if constexpr (is_std_tuple<T>::value)
        {
            return from_std_tuple(value);
        }
        else if constexpr (has_iterator<T>::value && !std::is_same_v<T, std::string>)
        {
            return from_has_iterator(value);
        }
        else if constexpr (is_c_array<T>::value)
        {
            return from_c_array(value);
        }
        else if constexpr (has_stream_out<T>::value)
        {
            return from_stream_out(value);
        }
        else
        {
            return from_fallback(value);
        }
    }
    /**
     * @brief 尝试将C数组转为字符串
     * @tparam T 
     * @param array 
     * @param count 
     * @return std::string 
     */
    template<class T>
    std::string to_string(const T* array, std::size_t count)
    {
        return from_c_ptr(array, count);
    }
    /**
     * @brief 尝试将时间点转为字符串
     * @tparam T 
     * @param time_point 
     * @param format 
     * @return std::string 
     */
    template<class T,std::enable_if_t<is_chrono_time_point<T>::value, int> = 0>
    std::string to_string(const T& time_point,const std::string& format)
    {
        return format_time_point(time_point,format);
    }
    /**
     * @enum FormatPosition
     * @brief 格式化位置
     */
    enum class FormatPosition
    {
        /// @brief 左对齐
        LEFT,
        /// @brief 居中对齐
        CENTER,
        /// @brief 右对齐
        RIGHT
    };
    /**
     * @brief 格式化标题
     * @param title 标题
     * @param sign 分隔符
     * @param width 宽度
     * @param position 位置
     * @return std::string 
     */
    std::string format_title(const std::string& title, const std::string& sign = "=", std::size_t width = 50, FormatPosition position = FormatPosition::CENTER);
    /**
     * @brief 格式化分隔符
     * @param sign 分隔符
     * @param width 宽度
     * @return std::string 
     */
    std::string format_separator(const std::string& sign = "-", std::size_t width = 50);
    /**
     * @brief 格式化表格
     * @tparam T 
     * @param table 
     * @note 暂未做实现
     * @return std::string 
     */
    template<class T>
    std::string format_table(const std::vector<std::vector<T>>& table)
    {
        /// @todo 考虑表格对齐，单元格宽高限制处理等
        return "";
    }
    /**
     * @brief 格式化字符串列表
     * @param space_counter 空格数量
     * @param item_sign 项目符号
     * @param list 列表
     * @param is_add_index 是否添加索引
     * @return std::string 
     */
    std::string format_string_list(int space_counter, const std::string& item_sign, const std::vector<std::string>& list,bool is_add_index=false,int begin_index=1);
    /**
     * @brief 获取存储单位符号
     * @param unit 存储单位
     * @return std::string 
     */
    std::string get_storage_unit_symbol(Stringify::StorageUnit unit);
    /**
     * @brief 格式化容量大小
     * @param size 容量大小(Bytes)
     * @return std::string
     */
    std::string format_capacity_size(uint64_t size,Stringify::StorageUnit dest_unit,std::size_t precision=0);
}