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
    //RunInt32R10(0, 0);
    //RunInt32R10(1, 1);
    //RunInt32R10(1234, -4321);
    //RunInt32R10(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    RunInt32R10(std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::min() + 1); // test failed.
    //RunInt32R10(std::numeric_limits<int>::max(), std::numeric_limits<int>::min() + 1);
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
