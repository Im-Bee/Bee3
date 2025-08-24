#pragma once

#include "CoreMinimal.h"
#include "Exception.h"

namespace Core::Memory
{

class IAllocator
{
public:
    
    IAllocator() = delete;
    ~IAllocator() = delete;

public:

    void* Allocate(USIZE uSizeInBytes, USIZE uAlignTo)
    { throw Exception("This function doesn't have implementation."); }

    void Deallocate(void* pPtr)
    { throw Exception("This function doesn't have implementation."); }

    void* Reallocate(void* pOldAlloc, USIZE uSizeInBytes, USIZE uAlignTo)
    { throw Exception("This function doesn't have implementation."); }

    USIZE GetAmountOfMemoryLeft() const noexcept
    { return 0; }

};

}
