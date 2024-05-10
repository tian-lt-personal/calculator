#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <limits>
#include <ranges>
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

enum class sign_kind : int32_t
{
    positive,
    negative
};

inline void trim_mantissa(mantissa_type& mant)
{
    auto pos = std::ranges::find_if(std::ranges::reverse_view{mant}, [](mantissa_digit_type x) { return x != 0; });
    mant.erase(pos.base(), mant.cend());
}

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
    details::sign_kind sign = details::sign_kind::positive;

    constexpr number() noexcept(noexcept(details::mantissa_type())) = default;
    constexpr number(const number&) = default;
    constexpr number(number&&) noexcept = default;
    constexpr number& operator=(const number&) = default;
    constexpr number& operator=(number&&) noexcept = default;
    friend constexpr std::strong_ordering operator<=>(number, number) noexcept = default;

    constexpr number(size_t digits) : mant(digits) {}

    template <std::integral V>
    constexpr explicit number(V value)
    {
        if constexpr (std::is_signed_v<V>)
        {
#if _DEBUG
            if (value == std::numeric_limits<V>::min())
            {
                throw std::logic_error{"can't find the positive representation for the min value."};
            }
#endif
            if (value < 0)
            {
                sign = details::sign_kind::negative;
                value *= -1;
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
            if (sign == details::sign_kind::negative)
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
            if (sign == details::sign_kind::negative)
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

    // Calculate the overlap of the numbers after alignment, this includes necessary padding 0's
    // mdigits is the max count of the digits results used as a counter.
    const size_t cdigits = std::max(a.mant.size() + a.exp, b.mant.size() + b.exp) - std::min(a.exp, b.exp);
    number<R> c{static_cast<size_t>(cdigits + 1u)};
    c.exp = std::min(a.exp, b.exp);

    // Figure out the sign of the numbers
    auto compla = false;                 // compla is a flag to signal `a` is negative.
    auto complb = false;                 // complb is a flag to signal `b` is negative.
    details::mantissa_digit_type cy = 0; // cy is the value of a carry after adding two `digits`
    if (a.sign != b.sign)
    {
        cy = 1;
        compla = a.sign == details::sign_kind::negative;
        complb = b.sign == details::sign_kind::negative;
    }

    // Loop over all the digits, real and 0 padded. here we know `a` and `b` are aligned
    auto mexp = c.exp; // mexp is the exponent of the result.
    auto pa = a.mant.cbegin();
    auto pb = b.mant.cbegin();
    auto pc = c.mant.begin();
    for (size_t i = 0; i < cdigits; ++i, ++mexp)
    {
        // Get digit from a, taking padding into account.
        details::mantissa_digit_type da =
            (((mexp >= a.exp) && (cdigits - i + a.exp - c.exp > (cdigits - a.mant.size()))) ? *pa++ : 0);
        // Get digit from a, taking padding into account.
        details::mantissa_digit_type db =
            (((mexp >= b.exp) && (cdigits - i + b.exp - c.exp > (cdigits - b.mant.size()))) ? *pb++ : 0);

        // Handle complementing for a and b digit. Might be a better way, but
        // haven't found it yet.
        if (compla)
        {
            da = static_cast<details::mantissa_digit_type>(R) - 1 - da;
        }
        if (complb)
        {
            db = static_cast<details::mantissa_digit_type>(R) - 1 - db;
        }

        // Update carry as necessary
        cy = da + db + cy;
        *pc++ = static_cast<details::mantissa_digit_type>(cy % static_cast<details::mantissa_digit_type>(R));
        cy /= static_cast<details::mantissa_digit_type>(R);
    }

    // Handle carry from last sum as extra digit
    if (cy && !(compla || complb))
    {
        *pc++ = cy;
    }

    // Compute sign of result
    if (!(compla || complb))
    {
        c.sign = a.sign;
    }
    else if (cy)
    {
        c.sign = details::sign_kind::positive;
    }
    else
    {
        // In this particular case an overflow or underflow has occurred
        // and all the digits need to be complemented, at one time an
        // attempt to handle this above was made, it turned out to be much
        // slower on average.
        c.sign = details::sign_kind::negative;
        cy = 1;
        pc = c.mant.begin();
        for (size_t i = 0; i < cdigits; ++i)
        {
            cy = static_cast<details::mantissa_digit_type>(R) - 1 - *pc + cy;
            *pc++ = static_cast<details::mantissa_digit_type>(cy % static_cast<details::mantissa_digit_type>(R));
            cy /= static_cast<details::mantissa_digit_type>(R);
        }
    }

    // Remove leading zeros, remember digits are in order of
    // increasing significance. i.e. 100 would be 0,0,1
    details::trim_mantissa(c.mant);
    return c;
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
