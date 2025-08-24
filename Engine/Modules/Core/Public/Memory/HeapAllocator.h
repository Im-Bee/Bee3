#pragma once

#include "CoreMinimal.h"
#include "IAllocator.h"

namespace Core::Memory
{

class API HeapAllocator 
{

public:
    
    HeapAllocator(USIZE uVirtualPages = 1);

    ~HeapAllocator();

public:

    void* Allocate(USIZE uSizeInBytes, USIZE uAlignTo);

    void Deallocate(void* pPtrToBlock);

    void* Reallocate(void* pOldAlloc, USIZE uSizeInBytes, USIZE uAlignTo);

    USIZE GetAmountOfMemoryLeft() const noexcept;

private: 

    void IsInRange(void* pPointer);

private:

    struct Block
    {
        USIZE uLen;
        USIZE uLenPrev;
        USIZE uIsFree;
    };

private:

    constexpr const static USIZE m_uMaxPageSize = 4096;

    USIZE m_uReservedVirtMem;
    void* m_pMemoryPage;

    void* m_pFirstFree;
    HeapAllocator::Block* m_pLastBlock;

};

}


