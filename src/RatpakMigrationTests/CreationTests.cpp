#include "pch.h"

// std
#include <limits>

// old ratpak
#include "../CalcManager/Ratpack/ratpak.h"

// new ratpak
#include "../Ratpak/ratpak.h"

// test headers
#include "Tools.h"

TEST(CreationTests, IntegralToNum)
{
    constexpr auto RunInt32R10 = [](int value)
    {
        auto oldnum = i32tonum(value, 10);
        auto num = ratpak::number<10>{value};
        ASSERT_TRUE(is_same(oldnum, num));
        destroynum(oldnum);
    };
    RunInt32R10(0);
    RunInt32R10(10);
    RunInt32R10(1000);
    RunInt32R10(12345678);
    RunInt32R10(-1);
    RunInt32R10(-1000);
    RunInt32R10(-12345678);
    RunInt32R10(std::numeric_limits<int>::max());
    RunInt32R10(std::numeric_limits<int>::min() + 1);

    constexpr auto RunInt32R32 = [](int value)
    {
        auto oldnum = i32tonum(value, 32);
        auto num = ratpak::number<32>{value};
        ASSERT_TRUE(is_same(oldnum, num));
        destroynum(oldnum);
    };
    RunInt32R32(0);
    RunInt32R32(10);
    RunInt32R32(1000);
    RunInt32R32(12345678);
    RunInt32R32(-1);
    RunInt32R32(-1000);
    RunInt32R32(-12345678);
    RunInt32R32(std::numeric_limits<int>::max());
    RunInt32R32(std::numeric_limits<int>::min() + 1);

    constexpr auto RunUInt32R9 = [](uint32_t value)
    {
        auto oldnum = Ui32tonum(value, 9);
        auto num = ratpak::number<9>{value};
        ASSERT_TRUE(is_same(oldnum, num));
        destroynum(oldnum);
    };
    RunUInt32R9(0);
    RunUInt32R9(1);
    RunUInt32R9(321);
    RunUInt32R9(87654321ul);
    RunUInt32R9(std::numeric_limits<uint32_t>::max());
}

TEST(CreationTests, NumToIntegral)
{
    constexpr auto RunR10 = [](int value)
    {
        auto oldnum = i32tonum(value, 10);
        auto num = ratpak::number<10>(value);
        ASSERT_EQ(numtoi32(oldnum, 10), num.to_value<int32_t>());
        destroynum(oldnum);
    };
    RunR10(0);
    RunR10(10);
    RunR10(10000);
    RunR10(12345678);
    RunR10(-1);
    RunR10(-1000);
    RunR10(-12345678);
    RunR10(std::numeric_limits<int>::max());
    RunR10(std::numeric_limits<int>::min() + 1);

    constexpr auto RunR32 = [](int value)
    {
        auto oldnum = i32tonum(value, 32);
        auto num = ratpak::number<32>(value);
        ASSERT_EQ(numtoi32(oldnum, 32), num.to_value<int32_t>());
        destroynum(oldnum);
    };
    RunR32(0);
    RunR32(10);
    RunR32(10000);
    RunR32(12345678);
    RunR32(-1);
    RunR32(-1000);
    RunR32(-12345678);
    RunR32(std::numeric_limits<int>::max());
    RunR32(std::numeric_limits<int>::min() + 1);
}
