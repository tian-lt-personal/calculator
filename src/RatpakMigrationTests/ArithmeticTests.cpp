#include "pch.h"

// std
#include <limits>

// old ratpak
#include "../CalcManager/Ratpack/ratpak.h"

// new ratpak
#include "../Ratpak/ratpak.h"

// test headers
#include "Tools.h"

TEST(ArithmeticTests, AddNum)
{
    constexpr auto RunInt32R10 = [](int a, int b)
    {
        auto oa = i32tonum(a, 10);
        auto ob = i32tonum(b, 10);
        addnum(&oa, ob, 10);

        ratpak::number<10> na{a}, nb{b};
        auto nc = ratpak::add_num(na, nb);
        ASSERT_TRUE(is_same(oa, nc));

        destroynum(oa);
        destroynum(ob);
    };
    RunInt32R10(0, 0);
    RunInt32R10(1, 1);
    RunInt32R10(1234, -4321);
    RunInt32R10(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    RunInt32R10(std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::min() + 1); // test failed.
    RunInt32R10(std::numeric_limits<int>::max(), std::numeric_limits<int>::min() + 1);

    constexpr auto RunUInt32R54321 = [](uint32_t a, uint32_t b)
    {
        auto oa = Ui32tonum(a, 54321);
        auto ob = Ui32tonum(b, 54321);
        addnum(&oa, ob, 54321);

        ratpak::number<54321> na{a}, nb{b};
        auto nc = ratpak::add_num(na, nb);
        ASSERT_TRUE(is_same(oa, nc));

        destroynum(oa);
        destroynum(ob);
    };
    RunUInt32R54321(0, 0);
    RunUInt32R54321(1, 1);
    RunUInt32R54321(1234, 4321);
    RunUInt32R54321(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max());
}

TEST(ArithmeticTests, AddNum_Commutative)
{
    constexpr auto RunInt32R10 = [](int a, int b)
    {
        auto oa = i32tonum(a, 10);
        auto ob = i32tonum(b, 10);
        addnum(&oa, ob, 10);

        ratpak::number<10> na{a}, nb{b};
        auto nc1 = ratpak::add_num(na, nb);
        auto nc2 = ratpak::add_num(nb, na);
        ASSERT_EQ(nc1, nc2);
        ASSERT_EQ(nc2, nc1);
        ASSERT_TRUE(is_same(oa, nc1));
        destroynum(oa);
        destroynum(ob);
    };
    RunInt32R10(0, 0);
}

TEST(ArithmeticTests, DISABLED_Heavy_AddNum_ScanValues)
{
    constexpr auto RunInt32R10 = [](int a, int b)
    {
        auto oa = i32tonum(a, 10);
        auto ob = i32tonum(b, 10);
        addnum(&oa, ob, 10);

        ratpak::number<10> na{a}, nb{b};
        auto nc1 = ratpak::add_num(na, nb);
        auto nc2 = ratpak::add_num(nb, na);
        ASSERT_EQ(nc1, nc2);
        ASSERT_EQ(nc2, nc1);
        ASSERT_TRUE(is_same(oa, nc1));
        destroynum(oa);
        destroynum(ob);
    };

    for (uint32_t i = 0; i < static_cast<uint32_t>(std::numeric_limits<int>::max()); ++i)
    {
        RunInt32R10(static_cast<int>(i), static_cast<int>(i));
        RunInt32R10(static_cast<int>(i), -static_cast<int>(i));
        RunInt32R10(static_cast<int>(i), std::numeric_limits<int>::max() - static_cast<int>(i));
    }
}
