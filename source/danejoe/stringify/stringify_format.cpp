#include <sstream>
#include <iomanip>

#include "danejoe/stringify/stringify_format.hpp"

std::string DaneJoe::to_string(FormatPosition position)
{
    switch (position)
    {
    case FormatPosition::LEFT:
        return ENUM_TO_STRING(FormatPosition::LEFT);
    case FormatPosition::CENTER:
        return ENUM_TO_STRING(FormatPosition::CENTER);
    case FormatPosition::RIGHT:
        return ENUM_TO_STRING(FormatPosition::RIGHT);
    case FormatPosition::Unknown:
    default:
        return ENUM_TO_STRING(FormatPosition::Unknown);
    }
}

template<>
DaneJoe::FormatPosition DaneJoe::enum_cast<DaneJoe::FormatPosition>(const std::string& enum_string)
{
    if (enum_string == ENUM_TO_STRING(FormatPosition::LEFT))
    {
        return FormatPosition::LEFT;
    }
    else if (enum_string == ENUM_TO_STRING(FormatPosition::CENTER))
    {
        return FormatPosition::CENTER;
    }
    else if (enum_string == ENUM_TO_STRING(FormatPosition::RIGHT))
    {
        return FormatPosition::RIGHT;
    }
    else if (enum_string == ENUM_TO_STRING(FormatPosition::Unknown))
    {
        return FormatPosition::Unknown;
    }
    return FormatPosition::Unknown;
}

std::string DaneJoe::format_title(
    const std::string& title,
    const std::string& sign,
    std::size_t width,
    FormatPosition position)
{
    std::ostringstream oss;
    if (title.size() >= width)
    {
        oss << title;
        return oss.str();
    }
    auto rest_count = width - title.size();
    auto total_sign_count = rest_count / sign.size() + 1;
    std::string sign_str;
    for (std::size_t i = 0; i < total_sign_count; ++i)
    {
        sign_str += sign;
    }
    std::string result;
    switch (position)
    {
    case FormatPosition::LEFT:
    {
        oss << title;
        oss << sign_str.substr(0, rest_count);
        break;
    }
    case FormatPosition::RIGHT:
    {
        oss << sign_str.substr(0, rest_count);
        oss << title;
        break;
    }
    case FormatPosition::CENTER:
    case FormatPosition::Unknown:
    default:
    {
        oss << sign_str.substr(0, rest_count / 2);
        oss << title;
        oss << sign_str.substr(0, rest_count - rest_count / 2);
        break;
    }
    }
    return oss.str();
}

std::string DaneJoe::format_separator(
    const std::string& sign,
    std::size_t width)
{
    return DaneJoe::format_title("", sign, width);
}

std::string DaneJoe::format_string_list(
    int space_counter,
    const std::string& item_sign,
    const std::vector<std::string>& list,
    bool is_add_index,
    int begin_index)
{

    std::ostringstream oss;
    for (const auto& item : list)
    {
        oss << std::string(space_counter, ' ');
        if (is_add_index)
        {
            oss << begin_index++;
        }
        oss << item_sign << item << "\n";
    }
    std::string result = oss.str();
    if (!result.empty())
    {
        result.pop_back();
    }
    return result;
}

std::string DaneJoe::get_storage_unit_symbol(StorageUnit unit)
{
    switch (unit)
    {
    case StorageUnit::Byte:
        return StringifyConfigManager::get_config().storage_symbol.byte_symbol;
    case StorageUnit::KiloByte:
        return StringifyConfigManager::get_config().storage_symbol.kilobyte_symbol;
    case StorageUnit::MegaByte:
        return StringifyConfigManager::get_config().storage_symbol.megabyte_symbol;
    case StorageUnit::GigaByte:
        return StringifyConfigManager::get_config().storage_symbol.gigabyte_symbol;
    case StorageUnit::TeraByte:
        return StringifyConfigManager::get_config().storage_symbol.terabyte_symbol;
    case StorageUnit::PetaByte:
        return StringifyConfigManager::get_config().storage_symbol.petabyte_symbol;
    case StorageUnit::ExaByte:
        return StringifyConfigManager::get_config().storage_symbol.exabyte_symbol;
    case StorageUnit::ZettaByte:
        return StringifyConfigManager::get_config().storage_symbol.zettabyte_symbol;
    case StorageUnit::YottaByte:
        return StringifyConfigManager::get_config().storage_symbol.yottabyte_symbol;
    default:
        return "";
    }
}

std::string DaneJoe::format_capacity_size(
    uint64_t size,
    StorageUnit dest_unit,
    std::size_t precision)
{

    std::string symbol = get_storage_unit_symbol(dest_unit);
    auto storage_units = StringifyConfigManager::get_config().storage_units;
    int exponent = static_cast<int>(dest_unit);
    if (dest_unit == StorageUnit::Byte)
    {
        precision = 0;
    }
    std::ostringstream oss;
    if (exponent <= 0)
    {
        oss
            << std::to_string(size)
            << StringifyConfigManager::get_config().storage_symbol.space_maker
            << symbol;
    }
    else
    {
        uint64_t cardinal_number = 1;
        for (int i = 0; i < exponent; ++i)
        {
            cardinal_number *= storage_units;
        }
        double value = size / (double)cardinal_number;
        oss
            << std::fixed
            << std::setprecision(precision)
            << value
            << StringifyConfigManager::get_config().storage_symbol.space_maker
            << symbol;
    }
    return oss.str();
}

#ifdef DANEJOE_STRINGIFY_FORMAT_TABLE_ENABLE
namespace DaneJoe
{
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
        // TODO 考虑表格对齐，单元格宽高限制处理等
        return "";
    }
}
#endif
