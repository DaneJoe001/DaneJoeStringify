#pragma once

/// @todo 引入时间to_string支持
/// @todo 添加分割线文本生成工具(换行符、缩进符、分隔符,嵌入文字方式枚举)

/**
 * @file stringify.hpp
 * @brief 字符串化工具库
 * @author DaneJoe001
 * @note
 *   - 提供统一的 to_string 接口，将常见类型、容器、数组与 std::pair 转换为可读字符串
 *   - 支持检测：拥有成员函数 to_string() 的类型、可被 std::to_string(T) 处理的类型、标准容器及任意可迭代类型（具有 begin/end）、枚举类型（以底层值展示）、字符、布尔、字符串与 std::pair
 *   - 包含一个变量名转字符串的宏 VARIABLE_NAME_TO_STRING(x)，便于打印调试
 */

#include <string>
#include <cstdint>
#include <iterator>
#include <typeinfo>
#include <type_traits>

/// @brief 将变量并转为字符串
#define VARIABLE_NAME_TO_STRING(x) #x

namespace DaneJoe
{
    /**
     * @brief 无std::to_string分支
     * @tparam T 类型
     * @note 模板类::velue返回true或false
     */
    template <typename T, typename = void>
    struct has_std_to_string : std::false_type
    {
    };
    /**
     * @brief std::to_string分支
     * @tparam T 类型
     */
    template <typename T>
    struct has_std_to_string<T, std::void_t<decltype(std::to_string(std::declval<T>()))>> : std::true_type
    {
    };
    /**
     * @brief 无to_string成员函数分支
     * @tparam T 类型
     */
    template <typename T, typename = void>
    struct has_member_to_string : std::false_type
    {
    };
    /**
     * @brief to_string成员函数分支
     * @tparam T 类型
     */
    template <typename T>
    struct has_member_to_string<T, std::void_t<decltype(std::declval<T>().to_string())>> : std::true_type
    {
    };
    /**
     * @brief 无迭代器分支
     * @tparam T 类型
     */
    template <typename T, typename = void>
    struct has_iterator : std::false_type
    {
    };
    /**
     * @brief 迭代器分支
     * @tparam T 类型
     */
    template <typename T>
    struct has_iterator<T, std::void_t<
        decltype(std::begin(std::declval<T>())),
        decltype(std::end(std::declval<T>()))>> : std::true_type
    {
    };
    /**
     * @brief std::pair特化to_string
     * @tparam Key 键类型
     * @tparam Value 值类型
     * @param pair 键值对
     * @return 转换后的字符串
     */
    template<class Key, class Value>
    std::string to_string(const std::pair<Key, Value>& pair);

    /**
     * @brief 尝试将变量转为字符串
     * @tparam T 类型
     * @param obj 变量
     * @return 尝试转换后的字符串
     */
    template<class T>
    std::string try_to_string(const T& obj)
    {
        std::string str;
        if constexpr (std::is_enum_v<T>)
        {
            using UnderlyingType = std::underlying_type_t<T>;
            str = typeid(obj).name() + std::string("<") + std::to_string(static_cast<UnderlyingType>(obj)) + std::string(">");
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            str = obj;
        }
        else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, unsigned char>)
        {
            str = std::string(1, static_cast<char>(obj));
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            str = obj ? "true" : "false";
        }
        else if constexpr (has_member_to_string<T>::value)
        {
            str = obj.to_string();
        }
        else if  constexpr (has_std_to_string<T>::value)
        {
            str = std::to_string(obj);
        }
        else
        {
            str = typeid(obj).name() + std::string("<No to_string>");
        }
        return str;
    }

    /**
     * @brief 尝试将变量转为字符串
     * @tparam T 类型
     * @param value 变量
     */
    template<class T>
    std::string to_string(const T& value)
    {
        std::string str;
        if constexpr (std::is_same_v<T, std::string>)
        {
            str = value;
        }
        else if constexpr (has_iterator<T>::value)
        {
            str += "[";
            for (auto begin = std::begin(value); begin != std::end(value); begin++)
            {
                str += DaneJoe::to_string(*begin);
                auto begin_copy = begin;
                if (++begin_copy != std::end(value))
                {
                    str += ", ";
                }
            }
            str += "]";
        }
        else
        {
            str = DaneJoe::try_to_string(value);
        }
        return str;
    }
    /**
     * @brief 尝试将数组转为字符串
     * @tparam T 类型
     * @param array 数组
     * @param count 数组长度
     * @return 尝试转换后的字符串
     */
    template<class T>
    std::string to_string(const T* array, std::size_t count = 1)
    {
        std::string str;
        if (array == nullptr || count == 0)
        {
            str = "[]";
        }
        else if (count == 1)
        {
            return DaneJoe::to_string(*array);
        }
        else
        {
            str += "[";
            for (int i = 0; i < count; i++)
            {
                str += DaneJoe::to_string(*(array + i));
                if (i < count - 1)
                {
                    str += ", ";
                }
            }
            str += "]";
        }
        return str;
    }

    /**
     * @brief 尝试将std::pair转为字符串
     * @tparam Key 键类型
     * @tparam Value 值类型
     * @param pair 键值对
     * @return 尝试转换后的字符串
     */
    template<class Key, class Value>
    std::string to_string(const std::pair<Key, Value>& pair)
    {
        std::string str = "{";
        str += DaneJoe::to_string(pair.first);
        str += ":";
        str += DaneJoe::to_string(pair.second);
        str += "}";
        return str;
    }
}