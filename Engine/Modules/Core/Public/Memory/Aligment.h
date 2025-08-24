#pragma once

#include "CoreMinimal.h"

namespace Core::Memory
{

// ---------------------------------------------------------------------------------------------------------------------
inline constexpr USIZE AlignTo(USIZE uPtr, USIZE uTo)
{ return (uPtr + --uTo) & ~uTo; }

} // !Core::Memory

