#include "Memory/HeapAllocator.h"

#include "Exception.h"
#include "Memory/Aligment.h"
#include "Memory/OS.h"



namespace Core::Memory
{

// DynamicAllocator // -------------------------------------------------------------------------------------------------
HeapAllocator::HeapAllocator(USIZE uMem)
    : m_uReservedVirtMem(uMem)
    , m_pMemoryPage(OsDependent::ReserveMemory(m_uReservedVirtMem))
    , m_pFirstFree(m_pMemoryPage)
    , m_pLastBlock(reinterpret_cast<HeapAllocator::Block*>(m_pMemoryPage))
{ }

// ---------------------------------------------------------------------------------------------------------------------
HeapAllocator::~HeapAllocator()
{ 
    OsDependent::ReleasePage(m_pMemoryPage);
}

// ---------------------------------------------------------------------------------------------------------------------
void* HeapAllocator::Allocate(USIZE uSizeInBytes, USIZE uAlignTo)
{
    USIZE   uLockSize       = sizeof(HeapAllocator::Block);
    Block*  pLockedBlock    = nullptr;
    USIZE   uLenPrev;
    void*   pLocked;

    if (uSizeInBytes == 0 || uSizeInBytes >= 4'294'967'296 /* 4 GB, huge number */) {
        throw Exception("Tried to allocate 0 or more then 4GB of memory on once");
    }

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

    if (reinterpret_cast<USIZE>(m_pMemoryPage)                      > reinterpret_cast<USIZE>(pPtrToBlock) || 
        reinterpret_cast<USIZE>(m_pMemoryPage) + m_uReservedVirtMem < reinterpret_cast<USIZE>(pPtrToBlock)) {
        throw Exception("Pointer passed to deallocation is invalid, this allocator doesn't owns this memory!");
    }


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
USIZE HeapAllocator::GetAmountOfMemoryLeft() const noexcept
{
    return reinterpret_cast<USIZE>(m_pMemoryPage) + m_uReservedVirtMem - reinterpret_cast<USIZE>(m_pFirstFree);
}


} // !Memory
