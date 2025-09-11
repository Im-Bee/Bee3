#include "Memory/HeapAllocator.h"

#include "Exception.h"
#include "OS.h"
#include "Memory/Aligment.h"
#include "Memory/Bytes.h"
#include "OS.h"



namespace Core::Memory
{

// DynamicAllocator // -------------------------------------------------------------------------------------------------
HeapAllocator::HeapAllocator(USIZE uVirtualPages)
    : m_uReservedVirtMem(uVirtualPages * m_uMaxPageSize)
    , m_pMemoryPage(OsDependent::ReserveMemory(m_uReservedVirtMem))
    , m_pFirstFree(m_pMemoryPage)
    , m_pLastBlock(reinterpret_cast<HeapAllocator::Block*>(m_pMemoryPage))
{ }

// ---------------------------------------------------------------------------------------------------------------------
HeapAllocator::~HeapAllocator()
{ 
    if (m_pMemoryPage) {
        OsDependent::ReleasePage(m_pMemoryPage);
    }
}

// ---------------------------------------------------------------------------------------------------------------------
HeapAllocator::HeapAllocator(const HeapAllocator& other) 
    : m_uReservedVirtMem(other.m_uReservedVirtMem)
    , m_pMemoryPage(OsDependent::ReserveMemory(m_uReservedVirtMem))
{
    USIZE uLenToLockCopy = reinterpret_cast<USIZE>(other.m_pFirstFree) - reinterpret_cast<USIZE>(other.m_pMemoryPage);

    OsDependent::LockMemory(this->m_pMemoryPage, uLenToLockCopy);

    CopyBytes(reinterpret_cast<BYTE*>(this->m_pMemoryPage),
              reinterpret_cast<BYTE*>(other.m_pMemoryPage),
              uLenToLockCopy);
    
    m_pFirstFree = reinterpret_cast<void*>(reinterpret_cast<USIZE>(m_pMemoryPage) + uLenToLockCopy);
    m_pLastBlock = reinterpret_cast<HeapAllocator::Block*>(reinterpret_cast<USIZE>(m_pFirstFree) 
                                                           - other.m_pLastBlock->uLen);
}

// ---------------------------------------------------------------------------------------------------------------------
HeapAllocator::HeapAllocator(HeapAllocator&& other) 
    : m_uReservedVirtMem(other.m_uReservedVirtMem)
    , m_pMemoryPage(other.m_pMemoryPage)
    , m_pFirstFree(other.m_pFirstFree)
    , m_pLastBlock(other.m_pLastBlock)
{ 
    other.m_pMemoryPage = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
void* HeapAllocator::Allocate(USIZE uSizeInBytes, USIZE uAlignTo)
{
    USIZE   uLockSize       = sizeof(HeapAllocator::Block);
    Block*  pLockedBlock    = nullptr;
    USIZE   uLenPrev;
    void*   pLocked;

    uLockSize = uLockSize + AlignTo(uSizeInBytes, uAlignTo);
    uLockSize = AlignTo(uLockSize, alignof(HeapAllocator::Block));

    if (uLockSize >= (reinterpret_cast<USIZE>(m_pMemoryPage) + m_uReservedVirtMem 
                      - reinterpret_cast<USIZE>(m_pFirstFree))) {
        throw Exception("Allocator is full, can't allocate more!");
    }

    OsDependent::LockMemory(m_pFirstFree, uLockSize);

    pLockedBlock = reinterpret_cast<HeapAllocator::Block*>(m_pFirstFree);
    pLockedBlock->uLenPrev = m_pLastBlock->uLen;
    m_pLastBlock = pLockedBlock;
    
    pLockedBlock->uLen    = uSizeInBytes;
    pLockedBlock->uIsFree = false;

    pLocked = reinterpret_cast<void*>(AlignTo(reinterpret_cast<USIZE>(pLockedBlock) 
                                              + sizeof(HeapAllocator::Block), 
                                              uAlignTo));

    m_pFirstFree = reinterpret_cast<void*>(reinterpret_cast<USIZE>(pLockedBlock) + uLockSize);

    return pLocked; 
}

// ---------------------------------------------------------------------------------------------------------------------
void HeapAllocator::Deallocate(void* pPtrToBlock)
{ 
    Block* pBlockStruct;

    IsInRange(pPtrToBlock);

    pBlockStruct = reinterpret_cast<Block*>(reinterpret_cast<USIZE>(pPtrToBlock) - sizeof(HeapAllocator::Block));

    pBlockStruct->uIsFree = true;

    if (pBlockStruct != m_pLastBlock) {
        return;
    }

    USIZE uLenOfPrevBlock;
    while (m_pLastBlock->uIsFree) {
        uLenOfPrevBlock = m_pLastBlock->uLenPrev;

        OsDependent::UnlockMemory(m_pLastBlock, m_pLastBlock->uLen + sizeof(HeapAllocator::Block));
        
        m_pFirstFree = m_pLastBlock;
        if (m_pFirstFree == m_pMemoryPage) {
            break;
        }

        m_pLastBlock = reinterpret_cast<Block*>(reinterpret_cast<USIZE>(m_pLastBlock) 
                                                - uLenOfPrevBlock - sizeof(HeapAllocator::Block));
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void* HeapAllocator::Reallocate(void* pOldAlloc, USIZE uSizeInBytes, USIZE uAlignTo)
{ 
    Block*  pBlockStruct;
    USIZE   uLockSize = sizeof(HeapAllocator::Block);
    USIZE   uOldLen;

    IsInRange(pOldAlloc);

    pBlockStruct = reinterpret_cast<Block*>(reinterpret_cast<USIZE>(pOldAlloc) - sizeof(HeapAllocator::Block));

    if (pBlockStruct != m_pLastBlock) {
        return Allocate(uSizeInBytes, uAlignTo);
    }


    uOldLen     = pBlockStruct->uLen;
    uLockSize   = uLockSize + AlignTo(uSizeInBytes, uAlignTo);
    uLockSize   = AlignTo(uLockSize, alignof(HeapAllocator::Block));

    if (uLockSize <= uOldLen) {
        return pOldAlloc;
    }
    
    pBlockStruct->uLen = uLockSize;

    OsDependent::LockMemory(pBlockStruct, uLockSize);

    return pOldAlloc;
}

// ---------------------------------------------------------------------------------------------------------------------
USIZE HeapAllocator::GetAmountOfMemoryLeft() const noexcept
{
    return reinterpret_cast<USIZE>(m_pMemoryPage) + m_uReservedVirtMem - reinterpret_cast<USIZE>(m_pFirstFree);
}

// ---------------------------------------------------------------------------------------------------------------------
void HeapAllocator::IsInRange(void* pPointer) const
{
    if (reinterpret_cast<USIZE>(m_pMemoryPage)                      > reinterpret_cast<USIZE>(pPointer) || 
        reinterpret_cast<USIZE>(m_pMemoryPage) + m_uReservedVirtMem < reinterpret_cast<USIZE>(pPointer)) {
        throw Exception("Pointer passed to reallocation is invalid, this allocator doesn't owns this memory!");
    }
}

} // !Memory
