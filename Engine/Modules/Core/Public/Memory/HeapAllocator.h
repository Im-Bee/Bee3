#pragma once

#include "CoreMinimal.h"
#include "IAllocator.h"

namespace Core::Memory
{

class API HeapAllocator 
{
    HeapAllocator(USIZE uMem);

public:
    
    template<USIZE uVirtualPages = 1>
    constexpr inline HeapAllocator()
        : HeapAllocator(m_uMaxPageSize * uVirtualPages)
    { }

    ~HeapAllocator();

public:

    void* Allocate(USIZE uSizeInBytes, USIZE uAlignTo);

    void Deallocate(void* pPtrToBlock);

    USIZE GetAmountOfMemoryLeft() const noexcept;

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


