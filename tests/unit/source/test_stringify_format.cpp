#include <gtest/gtest.h>

#include "danejoe/stringify/stringify_format.hpp"

TEST(StringifyFormatTest, FormatSeparator_Default)
{
    const std::string result = DaneJoe::format_separator();
    EXPECT_FALSE(result.empty());
}

TEST(StringifyFormatTest, FormatTitle_Center)
{
    const std::string result = DaneJoe::format_title("Title", "-", 10, DaneJoe::FormatPosition::CENTER);
    EXPECT_EQ(result.size(), 10);
}

TEST(StringifyFormatTest, FormatCapacitySize_Bytes)
{
    const std::string result = DaneJoe::format_capacity_size(12, DaneJoe::StorageUnit::Byte);
    EXPECT_NE(result.find("12"), std::string::npos);
}
