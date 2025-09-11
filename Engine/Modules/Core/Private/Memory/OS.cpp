#include "OS.h"

#include "Exception.h"
#include "Memory/RawString.h"


       
namespace Core::OsDependent
{

// ---------------------------------------------------------------------------------------------------------------------
//
#ifdef __linux__ 
//
// ---------------------------------------------------------------------------------------------------------------------

#include <sys/mman.h>
#include <unistd.h>

// ---------------------------------------------------------------------------------------------------------------------
USIZE GetSizeOfPage() noexcept
{
    return getpagesize();
}

// ---------------------------------------------------------------------------------------------------------------------
void* ReserveMemory(USIZE uSizeInBytes)
{
    void* pReservation;

    if (uSizeInBytes == 0 || uSizeInBytes >= 4'294'967'296 /* 4 GB, huge number */) {
        throw Exception("Tried to allocate 0 or more then 4GB of virtual memory on once");
    }

    if ((pReservation = mmap(NULL, 
                             uSizeInBytes,
                             PROT_WRITE | PROT_READ,
                             MAP_ANONYMOUS | MAP_PRIVATE,
                             -1,
                             0)) == MAP_FAILED) {
#ifdef _DEBUG
        CHAR8 pszExceptionMsg[64] = { 0 };
        USIZE uPos = Memory::CopyStr(pszExceptionMsg, "mmap failed errno = ");
        Memory::Int32ToString(errno, &pszExceptionMsg[uPos]);

        throw Exception(pszExceptionMsg);
#endif // !_DEBUG
 
        throw Exception("mmap failed (MAP_FAILED)");
    }
   
    return pReservation;
}

// ---------------------------------------------------------------------------------------------------------------------
void LockMemory(void* pWhere, USIZE uSizeInBytes)
{
    if (mlock(pWhere, uSizeInBytes) != 0) {
#ifdef _DEBUG
        CHAR8 pszExceptionMsg[64] = { 0 };
        USIZE uPos = Memory::CopyStr(pszExceptionMsg, "mlock failed errno = ");
        Memory::Int32ToString(errno, &pszExceptionMsg[uPos]);

        throw Exception(pszExceptionMsg);
#endif // !_DEBUG
       
        throw Exception("mlock failed");
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void UnlockMemory(void* pWhere, USIZE uSizeInBytes)
{
    if (munlock(pWhere, uSizeInBytes) != 0) {
#ifdef _DEBUG
        CHAR8 pszExceptionMsg[64] = { 0 };
        USIZE uPos = Memory::CopyStr(pszExceptionMsg, "munlock failed errno = ");
        Memory::Int32ToString(errno, &pszExceptionMsg[uPos]);

        throw Exception(pszExceptionMsg);
#endif // !_DEBUG
       
        throw Exception("munlock failed");
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void ReleasePage(void* pMemory)
{
    if (munmap(pMemory, OsDependent::GetSizeOfPage()) != 0) {
#ifdef _DEBUG
        CHAR8 pszExceptionMsg[64] = { 0 };
        USIZE uPos = Memory::CopyStr(pszExceptionMsg, "munmap failed errno = ");
        Memory::Int32ToString(errno, &pszExceptionMsg[uPos]);

        throw Exception(pszExceptionMsg);
#endif // !_DEBUG
       
        throw Exception("munmap failed");
    }
}

// ---------------------------------------------------------------------------------------------------------------------
//
#endif // !__linux__
//
// ---------------------------------------------------------------------------------------------------------------------

}
