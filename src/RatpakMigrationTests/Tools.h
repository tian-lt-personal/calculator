#pragma once

// std
#include <cstdint>
#include <stdexcept>

// old ratpak
#include "../CalcManager/Ratpack/ratpak.h"

// new ratpak
#include "../Ratpak/ratpak.h"

inline ratpak::details::sign_kind sign_kind(PNUMBER pn)
{
    switch (pn->sign)
    {
    case 1:
        return ratpak::details::sign_kind::positive;
    case -1:
        return ratpak::details::sign_kind::negative;
    default:
        throw std::logic_error{"bad sign value."};
    }
}

template <size_t R>
bool is_same(PNUMBER pn, ratpak::number<R> n)
{
    if (n.sign != sign_kind(pn))
    {
        return false;
    }
    if (n.exp != pn->exp)
    {
        return false;
    }
    if (n.mant.size() != pn->cdigit)
    {
        return (pn->cdigit == 1 && pn->mant[0] == 0) && n.mant.size() == 0;
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
