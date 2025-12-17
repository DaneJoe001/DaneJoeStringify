#include "danejoe/stringify/stringify_config.hpp"

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
