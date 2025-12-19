#include "danejoe/stringify/stringify_config.hpp"

std::string DaneJoe::to_string(StorageUnit unit)
{
    switch (unit)
    {
    case StorageUnit::Byte:
        return ENUM_TO_STRING(StorageUnit::Byte);
    case StorageUnit::KiloByte:
        return ENUM_TO_STRING(StorageUnit::KiloByte);
    case StorageUnit::MegaByte:
        return ENUM_TO_STRING(StorageUnit::MegaByte);
    case StorageUnit::GigaByte:
        return ENUM_TO_STRING(StorageUnit::GigaByte);
    case StorageUnit::TeraByte:
        return ENUM_TO_STRING(StorageUnit::TeraByte);
    case StorageUnit::PetaByte:
        return ENUM_TO_STRING(StorageUnit::PetaByte);
    case StorageUnit::ExaByte:
        return ENUM_TO_STRING(StorageUnit::ExaByte);
    case StorageUnit::ZettaByte:
        return ENUM_TO_STRING(StorageUnit::ZettaByte);
    case StorageUnit::YottaByte:
        return ENUM_TO_STRING(StorageUnit::YottaByte);
    case StorageUnit::Unknown:
    default:
        return ENUM_TO_STRING(StorageUnit::Unknown);
    }
}

template<>
DaneJoe::StorageUnit DaneJoe::enum_cast<DaneJoe::StorageUnit>(const std::string& enum_string)
{
    if (enum_string == ENUM_TO_STRING(StorageUnit::Byte))
    {
        return StorageUnit::Byte;
    }
    else if (enum_string == ENUM_TO_STRING(StorageUnit::KiloByte))
    {
        return StorageUnit::KiloByte;
    }
    else if (enum_string == ENUM_TO_STRING(StorageUnit::MegaByte))
    {
        return StorageUnit::MegaByte;
    }
    else if (enum_string == ENUM_TO_STRING(StorageUnit::GigaByte))
    {
        return StorageUnit::GigaByte;
    }
    else if (enum_string == ENUM_TO_STRING(StorageUnit::TeraByte))
    {
        return StorageUnit::TeraByte;
    }
    else if (enum_string == ENUM_TO_STRING(StorageUnit::PetaByte))
    {
        return StorageUnit::PetaByte;
    }
    else if (enum_string == ENUM_TO_STRING(StorageUnit::ExaByte))
    {
        return StorageUnit::ExaByte;
    }
    else if (enum_string == ENUM_TO_STRING(StorageUnit::ZettaByte))
    {
        return StorageUnit::ZettaByte;
    }
    else if (enum_string == ENUM_TO_STRING(StorageUnit::YottaByte))
    {
        return StorageUnit::YottaByte;
    }
    else if (enum_string == ENUM_TO_STRING(StorageUnit::Unknown))
    {
        return StorageUnit::Unknown;
    }
    return StorageUnit::Unknown;
}

DaneJoe::StringifyConfig DaneJoe::StringifyConfigManager::m_config = {};
std::mutex DaneJoe::StringifyConfigManager::m_mutex;

DaneJoe::StringifyConfig DaneJoe::StringifyConfigManager::get_config()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_config;
}

void DaneJoe::StringifyConfigManager::set_config(const StringifyConfig& config)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_config = config;
}
