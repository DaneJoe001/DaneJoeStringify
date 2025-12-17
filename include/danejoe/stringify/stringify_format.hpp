/**
 * @file stringify_format.hpp
 * @brief 字符串化格式化
 * @author DaneJoe001
 * @version 0.2.0
 * @date 2025-12-16
 */
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "danejoe/stringify/stringify_config.hpp"

 /**
  * @namespace DaneJoe
  * @brief DaneJoe 命名空间
  */
namespace DaneJoe
{
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
     * @return 格式化后的标题
     */
    std::string format_title(
        const std::string& title,
        const std::string& sign = "=",
        std::size_t width = 50,
        FormatPosition position = FormatPosition::CENTER);
    /**
     * @brief 格式化分隔符
     * @param sign 分隔符
     * @param width 宽度
     * @return 格式化后的分隔符
     */
    std::string format_separator(
        const std::string& sign = "-",
        std::size_t width = 50);
    /**
     * @brief 格式化字符串列表
     * @param space_counter 空格数量
     * @param item_sign 项目符号
     * @param list 列表
     * @param is_add_index 是否添加索引
     * @param begin_index 起始索引
     * @return 格式化后的字符串列表
     */
    std::string format_string_list(
        int space_counter,
        const std::string& item_sign,
        const std::vector<std::string>& list,
        bool is_add_index = false,
        int begin_index = 1);
    /**
     * @brief 获取存储单位符号
     * @param unit 存储单位
     * @return 存储单位符号
     */
    std::string get_storage_unit_symbol(StorageUnit unit);
    /**
     * @brief 格式化容量大小
     * @param size 容量大小(Bytes)
     * @param dest_unit 目标单位
     * @param precision 精度
     * @return 格式化后的容量大小
     */
    std::string format_capacity_size(
        uint64_t size,
        StorageUnit dest_unit,
        std::size_t precision = 0);
}
