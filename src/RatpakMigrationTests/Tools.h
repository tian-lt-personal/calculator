#pragma once

// std
#include <cstdint>

// old ratpak
#include "../CalcManager/Ratpack/ratpak.h"

// new ratpak
#include "../Ratpak/ratpak.h"

inline bool is_negative(PNUMBER pn)
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
