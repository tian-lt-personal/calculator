#include "pch.h"

// std
#include <limits>

// old ratpak
#include "../CalcManager/Ratpack/ratpak.h"

// new ratpak
#include "../Ratpak/ratpak.h"

namespace
{

bool is_negative(PNUMBER pn)
{
    return pn->sign == -1 ? true : false;
}

template <size_t R>
bool is_same(PNUMBER pn, ratpak::number<R> n)
{
    if (n.neg != is_negative(pn))
    {
        return false;
    }
    if (n.exp != pn->exp)
    {
        return false;
    }
    if (n.mant.size() != pn->cdigit)
    {
        return false;
    }
    for (size_t i = 0; i < n.mant.size(); ++i)
    {
        if (n.mant[i] != pn->mant[i])
        {
            return false;
        }
    }
    return true;
}

} // namespace

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


