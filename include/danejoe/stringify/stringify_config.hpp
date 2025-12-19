/**
 * @file stringify_config.hpp
 * @brief 字符串化配置
 * @author DaneJoe001
 * @version 0.2.0
 * @date 2025-12-16
 */
#pragma once

#include <string>
#include <mutex>

#include "danejoe/common/enum/enum_convert.hpp"

 /**
  * @namespace DaneJoe
  * @brief DaneJoe 命名空间
  */
namespace DaneJoe
{
    /**
     * @struct DelimiterSymbol
     * @brief 分隔符符号
     */
    struct DelimiterSymbol
    {
        /// @brief 开始符号
        std::string start_maker = "[";
        /// @brief 结束符号
        std::string end_maker = "]";
        /// @brief 元素分隔符
        std::string element_separator = ",";
        /// @brief 空格
        std::string space_maker = " ";
    };
    /**
     * @struct TimeSymbol
     * @brief 时间单位符号
     */
    struct TimeSymbol
    {
        /// @brief 秒
        std::string second_symbol = "s";
        /// @brief 毫秒
        std::string millisecond_symbol = "ms";
        /// @brief 微秒
        std::string microsecond_symbol = "us";
        /// @brief 纳秒
        std::string nanosecond_symbol = "ns";
    };
    /**
     * @struct StorageSymbol
     * @brief 容量单位符号
     */
    struct StorageSymbol
    {
        /// @brief 字节
        std::string byte_symbol = "B";
        /// @brief 千字节
        std::string kilobyte_symbol = "KB";
        /// @brief 兆字节
        std::string megabyte_symbol = "MB";
        /// @brief 吉字节
        std::string gigabyte_symbol = "GB";
        /// @brief 太字节
        std::string terabyte_symbol = "TB";
        /// @brief 万字节
        std::string petabyte_symbol = "PB";
        /// @brief 十万字节
        std::string exabyte_symbol = "EB";
        /// @brief 百万字节
        std::string zettabyte_symbol = "ZB";
        /// @brief 十亿字节
        std::string yottabyte_symbol = "YB";
        /// @brief 空占位
        std::string space_maker = " ";
    };
    /**
     * @struct BoolSymbol
     * @brief 布尔值符号
     */
    struct BoolSymbol
    {
        /// @brief 真
        std::string true_symbol = "true";
        /// @brief 假
        std::string false_symbol = "false";
    };
    /**
     * @struct EnumSymbol
     * @brief 枚举值符号
     */
    struct EnumSymbol
    {
        /// @brief 类型符号
        DelimiterSymbol type_symbol = { "<",">",":"," " };
        /// @brief 值符号
        DelimiterSymbol value_symbol = { "(",")",":"," " };
    };
    /**
     * @enum StorageUnit
     * @brief 存储单位
     */
    enum class StorageUnit
    {
        /// @brief 字节
        Byte = 0,
        /// @brief 千字节
        KiloByte,
        /// @brief 兆字节
        MegaByte,
        /// @brief 吉字节
        GigaByte,
        /// @brief 太字节
        TeraByte,
        /// @brief 拍字节
        PetaByte,
        /// @brief 艾字节
        ExaByte,
        /// @brief 泽字节
        ZettaByte,
        /// @brief 尧字节
        YottaByte,
        /// @brief 未知单位
        Unknown
    };

    /**
     * @brief 获取存储单位枚举字符串（调试用）
     * @param unit 存储单位
     * @return 对应的枚举字符串
     */
    std::string to_string(StorageUnit unit);
    /**
     * @brief 从字符串转换为存储单位
     * @param enum_string 由 ENUM_TO_STRING 生成的枚举字符串
     * @return 对应的枚举值，未知时返回 StorageUnit::Unknown
     */
    template<>
    StorageUnit enum_cast<StorageUnit>(const std::string& enum_string);
    /**
     * @struct Config
     * @brief 配置
     */
    struct StringifyConfig
    {
        /// @brief std::pair符号
        DelimiterSymbol pair_symbol = { "{","}",":"," " };
        /// @brief std::tuple符号
        DelimiterSymbol tuple_symbol = { "(",")",","," " };
        /// @brief 容器符号
        DelimiterSymbol container_symbol = { "[","]",","," " };
        /// @brief 枚举符号
        EnumSymbol enum_symbol = { {"<",">",":"," "},{"(",")",":"," "} };
        /// @brief 空值占位符
        std::string empty_place_holder = "<empty>";
        /// @brief variant空值占位符
        std::string variant_valueless_placeholder = "<valueless>";
        /// @brief 未支持类型占位符
        std::string unsupported_type_place_holder = "<unsupported type>";
        /// @brief 空值占位符
        std::string null_value_symbol = "<null>";
        /// @brief 省略号
        std::string ellipsis_symbol = "...";
        /// @brief 时间单位符号
        TimeSymbol time_symbol = { "s","ms","us","ns" };
        /// @brief 布尔值符号
        BoolSymbol bool_symbol = { "true", "false" };
        /// @brief 存储单位
        int storage_units = 1024;
        /// @brief 存储单位符号
        StorageSymbol storage_symbol = StorageSymbol();
        /// @brief 最大递归深度
        /// @note 暂未启用
        int max_depth = -1;
        /// @brief 最大字符串化元素数量
        /// @note 暂未启用
        /// @note 超过部分使用...表示
        int max_stringify_element_count = -1;
    };
    /**
     * @class ConfigManager
     * @brief 配置管理器
     */
    class StringifyConfigManager
    {
    public:
        /**
         * @brief 获取配置
         * @return 配置
         */
        static StringifyConfig get_config();
        /**
         * @brief 设置配置
         * @param config 配置
         */
        static void set_config(const StringifyConfig& config);
    private:
        /// @brief 配置
        static StringifyConfig m_config;
        /// @brief 互斥锁
        static std::mutex m_mutex;
    };
}
