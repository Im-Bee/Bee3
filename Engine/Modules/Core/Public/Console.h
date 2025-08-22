#pragma once

#include "CoreMinimal.h"

namespace Core 
{

USIZE API WriteToConsoleN(const CHAR8* pszString, USIZE uLen) noexcept;

USIZE API WriteToConsole(const CHAR8* pszString, USIZE uLen) noexcept;

template<USIZE uLen>
inline USIZE API WriteToConsole(const CHAR8 (&pszString)[uLen]) noexcept
{ return WriteToConsole(pszString, uLen); }

template<USIZE uLen>
inline USIZE API WriteToConsoleN(const CHAR8 (&pszString)[uLen]) noexcept
{ return WriteToConsoleN(pszString, uLen); }

} // !Core
