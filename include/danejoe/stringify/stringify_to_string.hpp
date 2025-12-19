/**
 * @file stringify_to_string.hpp
 * @brief 字符串化工具库
 * @author DaneJoe001
 * @version 0.2.0
 * @date 2025-12-16
 */
#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <tuple>
#include <variant>
#include <cstddef>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <type_traits>

#include "danejoe/common/type_traits/std_type_traits.hpp"
#include "danejoe/common/type_traits/container_traits.hpp"
#include "danejoe/stringify/stringify_config.hpp"
#include "danejoe/stringify/stringify_traits.hpp"

 /// @brief 将变量并转为字符串
#define VARIABLE_NAME_TO_STRING(x) #x

/**
 * @namespace DaneJoe
 * @brief DaneJoe 命名空间
 */
namespace DaneJoe
{
    /**
     * @brief 通用to_string接口
     * @tparam T 类型
     * @param value 对象
     * @return 转换后的字符串
     */
    template<class T>
    std::string to_string(const T& value);
    /**
     * @brief 尝试将std::string转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        std::is_same<T, std::string>::value, int> = 0>
    std::string from_std_string(const T& value)
    {
        return value;
    }
    /**
     * @brief 尝试将std::string_view转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        is_std_string_view<T>::value, int> = 0>
    std::string from_std_string_view(const T& value)
    {
        return std::string(value);
    }
    /**
     * @brief 尝试将const char*转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        is_c_string<T>::value, int> = 0>
    std::string from_c_string(const T& value)
    {
        return std::string(value);
    }
    /**
     * @brief 尝试将枚举转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 枚举值的字符串
     */
    template<class T, std::enable_if_t<
        std::is_enum<T>::value, int> = 0>
    std::string from_enum(const T& value)
    {
        using U = typename std::underlying_type<T>::type;
        std::ostringstream oss;
        oss << StringifyConfigManager::get_config().enum_symbol.type_symbol.start_maker <<
            std::string(typeid(value).name()) <<
            StringifyConfigManager::get_config().enum_symbol.type_symbol.end_maker <<
            StringifyConfigManager::get_config().enum_symbol.value_symbol.start_maker <<
            std::to_string(static_cast<U>(value)) <<
            StringifyConfigManager::get_config().enum_symbol.value_symbol.end_maker;
        return oss.str();
    }
    /**
     * @brief 尝试将字符转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        std::is_same<T, char>::value ||
        std::is_same<T, unsigned char>::value, int> = 0>
    std::string from_char(const T & value)
    {
        return std::string(1, static_cast<char>(value));
    }
    /**
     * @brief 尝试将布尔转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        std::is_same<T, bool>::value, int> = 0>
    std::string from_bool(const T& value)
    {
        return value ?
            StringifyConfigManager::get_config().bool_symbol.true_symbol :
            StringifyConfigManager::get_config().bool_symbol.false_symbol;
    }
    /**
     * @brief 尝试将std::chrono::duration转为字符串
     * @tparam Period 时长类型
     * @param period 时长
     * @return 尝试转换后的字符串
     */
    template<class Period, std::enable_if_t<
        is_chrono_duration<Period>::value, int> = 0>
    std::string format_time_duration(const Period& period)
    {
        std::ostringstream oss;
        oss << period.count();
        if constexpr (is_chrono_microseconds<Period>::value)
        {
            oss << StringifyConfigManager::get_config().time_symbol.microsecond_symbol;
        }
        else if constexpr (is_chrono_milliseconds<Period>::value)
        {
            oss << StringifyConfigManager::get_config().time_symbol.millisecond_symbol;
        }
        else if constexpr (is_chrono_nanoseconds<Period>::value)
        {
            oss << StringifyConfigManager::get_config().time_symbol.nanosecond_symbol;
        }
        else if constexpr (is_chrono_seconds<Period>::value)
        {
            oss << StringifyConfigManager::get_config().time_symbol.second_symbol;
        }
        return oss.str();
    }
    /**
     * @brief 尝试将std::chrono::time_point转为字符串
     * @tparam Period 时长类型
     * @param period 时长
     * @param format 时间格式（strftime/put_time 格式字符串）
     * @return 尝试转换后的字符串
     */
    template<class Period, std::enable_if_t<
        is_chrono_time_point<Period>::value, int> = 0>
    std::string format_time_point(const Period& period, const std::string& format = "%Y-%m-%d %H:%M:%S")
    {
        std::time_t raw_time_t = std::chrono::system_clock::to_time_t(period);
        std::tm time_info;
        std::tm* time_info_ptr = nullptr;
#if defined(_WIN32)
        localtime_s(&time_info, &raw_time_t); // Windows
        time_info_ptr = &time_info;
#elif defined(__linux__)
        localtime_r(&raw_time_t, &time_info); // Linux
        time_info_ptr = &time_info;
#elif defined(__APPLE__)
        localtime_r(&raw_time_t, &time_info); // macOS
        time_info_ptr = &time_info;
#else
        time_info_ptr = std::localtime(&raw_time_t);
#endif
        std::ostringstream oss;
        oss << std::put_time(time_info_ptr, format.c_str());
        return oss.str();
    }
    /**
     * @brief 含有std::to_string分支
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        has_std_to_string<T>::value, int> = 0>
    std::string from_std_to_string(const T& value)
    {
        return std::to_string(value);
    }
    /**
     * @brief 含有to_string成员函数分支
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        has_member_to_string<T>::value, int> = 0>
    std::string from_member_to_string(const T& value)
    {
        return value.to_string();
    }
    /**
     * @brief 含有迭代器分支,但非字符串类型
     * @tparam T 容器类型
     * @param value 容器对象
     * @return 转换后的字符串
     */
    template <class T, std::enable_if_t<
        has_iterator<T>::value &&
        !std::is_same_v<T, std::string>, int> = 0>
    std::string from_has_iterator(const T& value)
    {
        std::ostringstream oss;
        oss << StringifyConfigManager::get_config().container_symbol.start_maker;
        for (auto iter = std::begin(value); iter != std::end(value);)
        {
            oss << to_string(*iter);
            if (++iter != std::end(value))
            {
                oss << StringifyConfigManager::get_config().container_symbol.element_separator;
                oss << StringifyConfigManager::get_config().container_symbol.space_maker;
            }
        }
        oss << StringifyConfigManager::get_config().container_symbol.end_maker;
        return oss.str();
    }
    /**
     * @brief 尝试将C数组转为字符串
     * @tparam T 元素类型
     * @param ptr 指针
     * @param count 元素数量
     * @return 转换后的字符串
     */
    template<class T>
    std::string from_c_ptr(const T* ptr, std::size_t count)
    {
        if (ptr == nullptr || count == 0)
        {
            return StringifyConfigManager::get_config().container_symbol.start_maker +
                StringifyConfigManager::get_config().container_symbol.end_maker;
        }
        std::ostringstream oss;
        oss << StringifyConfigManager::get_config().container_symbol.start_maker;
        for (std::size_t i = 0; i < count; ++i)
        {
            oss << DaneJoe::to_string(*(ptr + i));
            if (i + 1 < count)
            {
                oss << StringifyConfigManager::get_config().container_symbol.element_separator;
                oss << StringifyConfigManager::get_config().container_symbol.space_maker;
            }
        }
        oss << StringifyConfigManager::get_config().container_symbol.end_maker;
        return oss.str();
    }
    /**
     * @brief 尝试将C数组转为字符串
     * @tparam T 元素类型
     * @tparam N 元素数量
     * @param array 数组
     * @return 转换后的字符串
     */
    template<class T, std::size_t N>
    std::string from_c_array(const T(&array)[N])
    {
        return from_c_ptr(array, N);
    }
    /**
     * @brief 尝试将std::pair转为字符串
     * @tparam Key 键类型
     * @tparam Value 值类型
     * @param pair 键值对
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        is_std_pair<T>::value, int> = 0>
    std::string from_std_pair(const T& pair)
    {
        std::ostringstream oss;
        oss << StringifyConfigManager::get_config().pair_symbol.start_maker;
        oss << DaneJoe::to_string(pair.first);
        oss << StringifyConfigManager::get_config().pair_symbol.element_separator +
            StringifyConfigManager::get_config().pair_symbol.space_maker;
        oss << DaneJoe::to_string(pair.second);
        oss << StringifyConfigManager::get_config().pair_symbol.end_maker;
        return oss.str();
    }
    /**
     * @brief 尝试将std::optional转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        is_std_optional<T>::value, int> = 0>
    std::string from_std_optional(const T& value)
    {
        if (value.has_value())
        {
            return DaneJoe::to_string(value.value());
        }
        else
        {
            return StringifyConfigManager::get_config().null_value_symbol;
        }
    }
    /**
     * @brief 尝试将std::variant转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        is_std_variant<T>::value, int> = 0>
    std::string from_std_variant(const T& value)
    {
        if (value.valueless_by_exception())
        {
            return StringifyConfigManager::get_config().variant_valueless_placeholder;
        }
        else
        {
            return std::visit([](const auto& arg)
                {
                    return DaneJoe::to_string(arg);
                }, value);
        }
    }
    /**
     * @brief 尝试将std::tuple转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        is_std_tuple<T>::value, int> = 0>
    std::string from_std_tuple(const T& value)
    {
        std::ostringstream oss;
        oss << StringifyConfigManager::get_config().tuple_symbol.start_maker;
        std::apply([&](auto&&... args)
            {
                ((oss << DaneJoe::to_string(args) <<
                    StringifyConfigManager::get_config().tuple_symbol.element_separator <<
                    StringifyConfigManager::get_config().tuple_symbol.space_maker), ...);
            }, value);
        // 元素不为空时去掉最后一个元素分隔符
        std::string str = oss.str();
        if (str != StringifyConfigManager::get_config().tuple_symbol.start_maker)
        {
            str = str.substr(0, str.size() -
                StringifyConfigManager::get_config().tuple_symbol.element_separator.size() -
                StringifyConfigManager::get_config().tuple_symbol.space_maker.size());
        }
        str += StringifyConfigManager::get_config().tuple_symbol.end_maker;
        return str;
    }
    /**
     * @brief 尝试将类型转为字符串
     * @tparam T 类型
     * @param value 对象
     * @return 尝试转换后的字符串
     */
    template<class T, std::enable_if_t<
        has_stream_out<T>::value, int> = 0>
    std::string from_stream_out(const T& value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
    /**
     * @brief 无to_string分支
     * @tparam T 类型
     */
    template<class T>
    std::string from_fallback(const T& value)
    {
        return StringifyConfigManager::get_config().unsupported_type_place_holder;
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
     * @tparam T 元素类型
     * @param ptr 指针
     * @param count 元素数量
     * @return 转换后的字符串
     */
    template<class T>
    std::string to_string(const T* ptr, std::size_t count)
    {
        return from_c_ptr(ptr, count);
    }
    /**
     * @brief 尝试将时间点转为字符串
     * @tparam T 时间点类型
     * @param time_point 时间点
     * @param format 时间格式（strftime/put_time 格式字符串）
     * @return 转换后的字符串
     */
    template<class T, std::enable_if_t<
        is_chrono_time_point<T>::value, int> = 0>
    std::string to_string(const T& time_point, const std::string& format)
    {
        return format_time_point(time_point, format);
    }

}
