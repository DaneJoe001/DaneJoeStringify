#include "danejoe/stringify/stringify.hpp"

DaneJoe::Stringify::Config DaneJoe::Stringify::ConfigManager::m_config={};
std::mutex DaneJoe::Stringify::ConfigManager::m_mutex;

DaneJoe::Stringify::Config DaneJoe::Stringify::ConfigManager::get_config()
{
    return m_config;
}

void DaneJoe::Stringify::ConfigManager::set_config(const Stringify::Config& config)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_config = config;
}

std::string DaneJoe::format_title(const std::string& title, const std::string& sign, std::size_t width, FormatPosition position)
{
    std::ostringstream oss;
    if(title.size()>=width)
    {
        oss<<title;
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
            oss<<title;
            oss<<sign_str.substr(0, rest_count);
            break;
        }
        case FormatPosition::RIGHT:
        {
            oss<<sign_str.substr(0, rest_count);
            oss<<title;
            break;
        }
        case FormatPosition::CENTER:
        default:
        {
            oss<<sign_str.substr(0, rest_count / 2);
            oss<<title;
            oss<<sign_str.substr(0, rest_count - rest_count / 2);
            break;
        }
    }
    return oss.str();
}

std::string DaneJoe::format_separator(const std::string& sign, std::size_t width)
{
    return DaneJoe::format_title("", sign, width);
}

std::string DaneJoe::format_string_list(int space_counter, const std::string& item_sign, const std::vector<std::string>& list,bool is_add_index,int begin_index)
{
    
    std::ostringstream oss;
    for (const auto& item : list)
    {
        oss<<std::string(space_counter, ' ');
        if(is_add_index)
        {
            oss<<begin_index++;
        }
        oss<<item_sign<<item<<"\n";
    }
    std::string result=oss.str();
    if(!result.empty())
    {
        result.pop_back();
    }
    return result;
}

std::string DaneJoe::get_storage_unit_symbol(Stringify::StorageUnit unit)
{
    switch(unit)
    {
        case Stringify::StorageUnit::Byte:
            return Stringify::ConfigManager::get_config().storage_symbol.byte_symbol;
        case Stringify::StorageUnit::KiloByte:
            return Stringify::ConfigManager::get_config().storage_symbol.kilobyte_symbol;
        case Stringify::StorageUnit::MegaByte:
            return Stringify::ConfigManager::get_config().storage_symbol.megabyte_symbol;
        case Stringify::StorageUnit::GigaByte:
            return Stringify::ConfigManager::get_config().storage_symbol.gigabyte_symbol;
        case Stringify::StorageUnit::TeraByte:
            return Stringify::ConfigManager::get_config().storage_symbol.terabyte_symbol;
        case Stringify::StorageUnit::PetaByte:
            return Stringify::ConfigManager::get_config().storage_symbol.petabyte_symbol;
        case Stringify::StorageUnit::ExaByte:
            return Stringify::ConfigManager::get_config().storage_symbol.exabyte_symbol;
        case Stringify::StorageUnit::ZettaByte:
            return Stringify::ConfigManager::get_config().storage_symbol.zettabyte_symbol;
        case Stringify::StorageUnit::YottaByte:
            return Stringify::ConfigManager::get_config().storage_symbol.yottabyte_symbol;
        default:
            return "";}
}

std::string DaneJoe::format_capacity_size(uint64_t size,Stringify::StorageUnit dest_unit,std::size_t precision)
{

    std::string symbol=get_storage_unit_symbol(dest_unit);
    auto storage_units = Stringify::ConfigManager::get_config().storage_units;
    int exponent = static_cast<int>(dest_unit);
    if(dest_unit==Stringify::StorageUnit::Byte)
    {
        precision=0;
    }
    std::ostringstream oss;
    if (exponent <= 0)
    {
        oss
        <<std::to_string(size)
        <<Stringify::ConfigManager::get_config().storage_symbol.space_maker 
        <<symbol;
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
        <<std::fixed
        <<std::setprecision(precision)
        <<value
        <<Stringify::ConfigManager::get_config().storage_symbol.space_maker 
        <<symbol;
    }
    return oss.str();
}
