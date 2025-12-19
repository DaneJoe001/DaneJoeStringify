#include <gtest/gtest.h>

#include "danejoe/stringify/stringify_config.hpp"

TEST(StringifyConfigManagerTest, SetAndGetConfig_Works)
{
    DaneJoe::StringifyConfig config = DaneJoe::StringifyConfigManager::get_config();
    config.null_value_symbol = "<NULL>";
    config.bool_symbol.true_symbol = "TRUE";
    config.bool_symbol.false_symbol = "FALSE";

    DaneJoe::StringifyConfigManager::set_config(config);

    DaneJoe::StringifyConfig result = DaneJoe::StringifyConfigManager::get_config();
    EXPECT_EQ(result.null_value_symbol, "<NULL>");
    EXPECT_EQ(result.bool_symbol.true_symbol, "TRUE");
    EXPECT_EQ(result.bool_symbol.false_symbol, "FALSE");
}
