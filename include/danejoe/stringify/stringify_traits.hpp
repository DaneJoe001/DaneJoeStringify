/**
 * @file stringify_traits.hpp
 * @brief 字符串化特征工具
 * @author DaneJoe001
 * @version 0.2.0
 * @date 2025-12-16
 */
#pragma once

#include <utility>
#include <type_traits>
#include <string>
#include <ostream>

 /**
  * @namespace DaneJoe
  * @brief DaneJoe 命名空间
  */
namespace DaneJoe
{
    /**
     * @brief 判断类型是否支持std::to_string
     * @tparam T 类型
     * @note has_std_to_string<T>::value为true表示T支持std::to_string
     */
    template <typename T, typename = void>
    struct has_std_to_string : std::false_type {};
    /**
     * @brief has_std_to_string的匹配分支：当std::to_string对T可用时为true
     * @tparam T 类型
     */
    template <typename T>
    struct has_std_to_string<T,
        std::void_t<decltype(std::to_string(std::declval<T>()))>> : std::true_type {};
    /**
     * @brief 判断类型是否支持to_string成员函数
     * @tparam T 类型
     * @note has_member_to_string<T>::value为true表示T支持to_string成员函数
     */
    template <typename T, typename = void>
    struct has_member_to_string : std::false_type {};
    /**
     * @brief has_member_to_string的匹配分支：当T有to_string成员函数时为true
     * @tparam T 类型
     */
    template <typename T>
    struct has_member_to_string<T,
        std::void_t<decltype(std::declval<T>().to_string())>> : std::true_type {};
    /**
     * @brief 检测类型是否支持流输出
     * @tparam T 类型
     * @note has_stream_out<T>::value为true表示T支持流输出
     */
    template <typename T, typename = void>
    struct has_stream_out : std::false_type {};
    /**
     * @brief has_stream_out的匹配分支：当operator<<对T可用时为true
     * @tparam T 类型
     */
    template <typename T>
    struct has_stream_out<T,
        std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>> : std::true_type {};
}
