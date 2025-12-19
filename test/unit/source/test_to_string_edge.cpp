#include <gtest/gtest.h>

#include <optional>
#include <string>
#include <vector>

#include "danejoe/stringify/stringify_config.hpp"
#include "danejoe/stringify/stringify_to_string.hpp"

namespace
{

struct StringifyConfigGuard
{
    DaneJoe::StringifyConfig old_config;

    explicit StringifyConfigGuard(const DaneJoe::StringifyConfig& new_config)
        : old_config(DaneJoe::StringifyConfigManager::get_config())
    {
        DaneJoe::StringifyConfigManager::set_config(new_config);
    }

    ~StringifyConfigGuard()
    {
        DaneJoe::StringifyConfigManager::set_config(old_config);
    }
};

TEST(ToStringEdgeTest, OptionalBranch_HasValue)
{
    const std::optional<int> v = 7;
    EXPECT_EQ(DaneJoe::to_string(v), "7");
}

TEST(ToStringEdgeTest, NestedContainer_VectorOfOptional)
{
    const std::vector<std::optional<int>> v = { 1, std::nullopt, 3 };
    EXPECT_EQ(DaneJoe::to_string(v), "[1, <null>, 3]");
}

TEST(ToStringEdgeTest, NullValueSymbol_IsUsedByOptional)
{
    DaneJoe::StringifyConfig config = DaneJoe::StringifyConfigManager::get_config();
    config.null_value_symbol = "<NULL>";

    StringifyConfigGuard guard(config);

    const std::optional<int> v = std::nullopt;
    EXPECT_EQ(DaneJoe::to_string(v), "<NULL>");
}

TEST(ToStringEdgeTest, ContainerDelimiterSymbols_AffectOutput)
{
    DaneJoe::StringifyConfig config = DaneJoe::StringifyConfigManager::get_config();
    config.container_symbol.start_maker = "{";
    config.container_symbol.end_maker = "}";
    config.container_symbol.element_separator = ";";
    config.container_symbol.space_maker = "";

    StringifyConfigGuard guard(config);

    const std::vector<int> v = { 1, 2, 3 };
    EXPECT_EQ(DaneJoe::to_string(v), "{1;2;3}");
}

} // namespace
