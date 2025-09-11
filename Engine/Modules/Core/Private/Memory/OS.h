#pragma once

#include "CoreMinimal.h"

namespace Core::OsDependent
{

USIZE GetSizeOfPage() noexcept;

void* ReserveMemory(USIZE uSizeInBytes);

void ReleasePage(void* pMemory);

void LockMemory(void* pWhere, USIZE uSizeInBytes);

void UnlockMemory(void* pWhere, USIZE uSizeInBytes);

} // !OsDependent
