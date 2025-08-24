#pragma once

#include "CoreMinimal.h"

namespace Core::Memory
{

inline void API CopyBytes(BYTE* pDestination, const BYTE* pszSource, USIZE uAmount)
{
    while (uAmount--) {
        pDestination[uAmount] = pszSource[uAmount];
    }
}

template<USIZE uAmount>
inline constexpr USIZE CopyBytes(BYTE* pDestination, const BYTE (&szSource)[uAmount])
{ 
    CopyBytes(pDestination, szSource, uAmount);
    return uAmount;
}

} // !Core::Memory
