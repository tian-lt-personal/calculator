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
    RunInt32R10(0, 1);
    RunInt32R10(-10, 10);
    RunInt32R10(12345678, 87654321);
    RunInt32R10(std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max());

    constexpr auto RunUInt32R137 = [](uint32_t a, uint32_t b)
    {
        auto oa = Ui32tonum(a, 137);
        auto ob = Ui32tonum(b, 137);
        addnum(&oa, ob, 137);

        ratpak::number<137> na{a}, nb{b};
        auto nc1 = ratpak::add_num(na, nb);
        auto nc2 = ratpak::add_num(nb, na);
        ASSERT_EQ(nc1, nc2);
        ASSERT_EQ(nc2, nc1);
        ASSERT_TRUE(is_same(oa, nc1));

        destroynum(oa);
        destroynum(ob);
    };
    RunUInt32R137(0, 0);
    RunUInt32R137(0, 1);
    RunUInt32R137(12345678, 87654321);
    RunUInt32R137(0, std::numeric_limits<uint32_t>::max());
}

TEST(ArithmeticTests, MulNum)
{
    constexpr auto RunInt32R10 = [](int a, int b)
    {
        auto oa = i32tonum(a, 10);
        auto ob = i32tonum(b, 10);
        mulnum(&oa, ob, 10);

        ratpak::number<10> na{a}, nb{b};
        auto nc = ratpak::mul_num(na, nb);
        ASSERT_TRUE(is_same(oa, nc));

        destroynum(oa);
        destroynum(ob);
    };
    RunInt32R10(0, 0);
    RunInt32R10(1, -1);
    RunInt32R10(10, 0);
    RunInt32R10(0, -10);
    RunInt32R10(12345678, -87654321);
    RunInt32R10(std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max());

    constexpr auto RunUInt32R654321 = [](uint32_t a, uint32_t b)
    {
        constexpr auto Radix = 654321u;
        auto oa = Ui32tonum(a, Radix);
        auto ob = Ui32tonum(b, Radix);
        mulnum(&oa, ob, Radix);

        ratpak::number<Radix> na{a}, nb{b};
        auto nc = ratpak::mul_num(na, nb);
        ASSERT_TRUE(is_same(oa, nc));

        destroynum(oa);
        destroynum(ob);
    };
    RunUInt32R654321(0, 0);
    RunUInt32R654321(0, 1);
    RunUInt32R654321(0, 12345678);
    RunUInt32R654321(1, 12345678);
    RunUInt32R654321(431234, 12345678);
    RunUInt32R654321(std::numeric_limits<uint32_t>::max(), 1);
}

TEST(ArithmeticTests, MulNum_Commutative)
{
    constexpr auto RunInt32R10 = [](int a, int b)
    {
        auto oa = i32tonum(a, 10);
        auto ob = i32tonum(b, 10);
        mulnum(&oa, ob, 10);

        ratpak::number<10> na{a}, nb{b};
        auto nc1 = ratpak::mul_num(na, nb);
        auto nc2 = ratpak::mul_num(nb, na);
        ASSERT_EQ(nc1, nc2);
        ASSERT_EQ(nc2, nc1);
        ASSERT_TRUE(is_same(oa, nc1));
    };
    RunInt32R10(0, 0);
    RunInt32R10(0, 1);
    RunInt32R10(-10, 10);
    RunInt32R10(-1234567, 54321);
    RunInt32R10(std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max());

    constexpr auto RunUInt32R887755 = [](uint32_t a, uint32_t b)
    {
        constexpr auto Radix = 887755u;
        auto oa = Ui32tonum(a, Radix);
        auto ob = Ui32tonum(b, Radix);
        mulnum(&oa, ob, Radix);

        ratpak::number<Radix> na{a}, nb{b};
        auto nc1 = ratpak::mul_num(na, nb);
        auto nc2 = ratpak::mul_num(nb, na);
        ASSERT_EQ(nc1, nc2);
        ASSERT_EQ(nc2, nc1);
        ASSERT_TRUE(is_same(oa, nc1));
    };
    RunUInt32R887755(0, 0);
    RunUInt32R887755(0, 1);
    RunUInt32R887755(10, 10);
    RunUInt32R887755(14234, 9123123);
    RunUInt32R887755(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max());
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
