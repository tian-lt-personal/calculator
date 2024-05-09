#include <algorithm>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace ratpak
{

namespace details
{

using exponent_type = int32_t;
using mantissa_digit_type = uint32_t;
using mantissa_type = std::vector<mantissa_digit_type>;

} // namespace details

//-----------------------------------------------------------------------------
//
//  `number` type is a representation of a generic sized generic radix number
//
//-----------------------------------------------------------------------------
template <size_t R>
struct number
{
    details::mantissa_type mant;
    details::exponent_type exp = 0;
    bool neg = false;

    constexpr number() noexcept(noexcept(details::mantissa_type)) = default;
    constexpr number(const number&) = default;
    constexpr number(number&&) noexcept = default;
    constexpr number& operator=(const number&) = default;
    constexpr number& operator=(number&&) noexcept = default;

    template <std::integral V>
    constexpr explicit number(V value)
    {
        if constexpr (std::is_signed_v<V>)
        {
            if (value < 0)
            {
                neg = true;
                value = -value;
            }
        }
        do
        {
            mant.push_back(static_cast<details::mantissa_digit_type>(value % R));
            value /= R;
        } while (value);
    }

    template <std::integral V>
    V to_value()
    {
        if constexpr (std::is_unsigned_v<V>)
        {
            if (neg)
            {
                throw std::logic_error{"error: wrong sign."};
            }
        }
        V result = 0;
        size_t length = mant.size() + std::min(exp, 0);
        for (size_t i = 0; i < length; ++i)
        {
            result = result * R + mant[mant.size() - i - 1];
        }
        for (size_t i = 0; i < exp; ++i)
        {
            result *= R;
        }

        if constexpr (std::is_signed_v<V>)
        {
            if (neg)
            {
                return -result;
            }
        }
        return result;
    }
};

template <size_t R>
bool is_zero(number<R> num)
{
    return std::ranges::all_of(num.mant, [](int32_t x) { return x == 0; });
}

template <size_t R>
number<R> add_num(number<R> a, number<R> b)
{
    if (is_zero(a))
    {
        return b;
    }
    else if (is_zero(b))
    {
        return a;
    }

    size_t digits = std::max(a.mant.size() + a.exp, b.mant.size() + b.exp) - std::min(a.exp, b.exp);
    number<R> c{digits};
    c.exp = std::min(a.exp, b.exp);
    auto mexp = c.exp;

    // cy is the value of a carry after adding two `digits`
    details::mantissa_type cy = a.neg == b.neg ? 0 : 1;

    // ...
}

//-----------------------------------------------------------------------------
//
//  `rational` type is a representation radix  on 2 NUMBER types,
//  in the form of `p / q`
//
//-----------------------------------------------------------------------------
template <size_t R>
struct rational
{
    number<R> p, q;
};

} // namespace ratpak
