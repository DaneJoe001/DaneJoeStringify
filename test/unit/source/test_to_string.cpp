#include <gtest/gtest.h>

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "danejoe/stringify/stringify_to_string.hpp"

TEST(ToStringTest, StdToStringBranch_Int)
{
    EXPECT_EQ(DaneJoe::to_string(123), "123");
}

TEST(ToStringTest, BoolBranch_UsesConfig)
{
    DaneJoe::StringifyConfig config = DaneJoe::StringifyConfigManager::get_config();
    config.bool_symbol.true_symbol = "true";
    config.bool_symbol.false_symbol = "false";
    DaneJoe::StringifyConfigManager::set_config(config);

    EXPECT_EQ(DaneJoe::to_string(true), "true");
    EXPECT_EQ(DaneJoe::to_string(false), "false");
}

TEST(ToStringTest, StdStringViewBranch)
{
    const std::string_view sv = "abc";
    EXPECT_EQ(DaneJoe::to_string(sv), "abc");
}

TEST(ToStringTest, ContainerBranch_VectorInt)
{
    const std::vector<int> vec = { 1, 2 };
    EXPECT_EQ(DaneJoe::to_string(vec), "[1, 2]");
}

TEST(ToStringTest, PairBranch)
{
    const std::pair<int, int> p{ 1, 2 };
    EXPECT_EQ(DaneJoe::to_string(p), "{1: 2}");
}

TEST(ToStringTest, OptionalBranch_Empty)
{
    const std::optional<int> v = std::nullopt;
    EXPECT_EQ(DaneJoe::to_string(v), "<null>");
}
