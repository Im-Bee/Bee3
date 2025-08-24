#pragma once

#include "CoreMinimal.h"

namespace Core::Memory
{

USIZE API Int32ToString(INT32 number, CHAR8* pBuffer);

USIZE API Int64ToString(INT64 number, CHAR8* pBuffer);

inline void API CopyStr(CHAR8* pDestination, const CHAR8* pszSource, USIZE uAmount)
{
    while (uAmount--) {
        pDestination[uAmount] = pszSource[uAmount];
    }
}

template<USIZE uAmount>
inline constexpr USIZE CopyStr(CHAR8* pDestination, const CHAR8 (&szSource)[uAmount])
{ 
    CopyStr(pDestination, szSource, uAmount);
    return uAmount;
}

} // !Core::Memory

