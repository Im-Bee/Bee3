#include "Memory/RawString.h"

namespace Core::Memory
{

// ---------------------------------------------------------------------------------------------------------------------
constexpr static const UINT32 PowsOf10Int32[] = 
{ 
    0u, 1u, 10u, 100u, 1'000u, 10'000u, 
    100'000u, 1'000'000u, 10'000'000u, 
    100'000'000u, 1'000'000'000u
};

// ---------------------------------------------------------------------------------------------------------------------
constexpr UINT32 Int32FindNearestPowOf10(INT32 number)
{
    constexpr USIZE uTableSize = sizeof(PowsOf10Int32) / sizeof(UINT32);
    USIZE uIndex = 0;

    while (uIndex < uTableSize) {
        if (number < PowsOf10Int32[uIndex]) {
            break;
        }

        ++uIndex;
    }

    return PowsOf10Int32[--uIndex];
}


// ---------------------------------------------------------------------------------------------------------------------
constexpr static const UINT64 PowsOf10Int64[] = 
{ 
    0u, 1u, 10u, 100u, 1'000u, 10'000u, 100'000u, 1'000'000u, 10'000'000u, 
    100'000'000u, 1'000'000'000u, 10'000'000'000u, 100'000'000'000u,
    1'000'000'000'000'000u, 10'000'000'000'000'000u,
    100'000'000'000'000'000u, 1'000'000'000'000'000'000u, 10'000'000'000'000'000'000u
};

// ---------------------------------------------------------------------------------------------------------------------
constexpr UINT64 Int64FindNearestPowOf10(INT64 number)
{
    constexpr USIZE uTableSize = sizeof(PowsOf10Int64) / sizeof(UINT64);
    USIZE uIndex = 0;

    while (uIndex < uTableSize) {
        if (number < PowsOf10Int64[uIndex]) {
            break;
        }

        ++uIndex;
    }

    return PowsOf10Int64[--uIndex];
}

// ---------------------------------------------------------------------------------------------------------------------
USIZE Int32ToString(INT32 number, CHAR8* pBuffer)
{
    USIZE uPtrPos = 0;

    if (number == 0) {
        pBuffer[uPtrPos++] = '0';
        pBuffer[uPtrPos++] = '\0';
        return uPtrPos;
    }
    if (number < 0) {
        number = -number;
        pBuffer[uPtrPos++] = '-';
    }
    
    INT32 div = Int32FindNearestPowOf10(number);

    INT32 farLeftDigit;
    while (div) 
    {
        farLeftDigit = number / div;
        number -= farLeftDigit * div;
        div /= 10;

        pBuffer[uPtrPos++] = '0' + farLeftDigit;
    }

    pBuffer[uPtrPos++] = '\0';

    return uPtrPos;
}

// ---------------------------------------------------------------------------------------------------------------------
USIZE Int64ToString(INT64 number, CHAR8* pBuffer)
{
    USIZE uPtrPos = 0;

    if (number == 0) {
        pBuffer[uPtrPos++] = '0';
        pBuffer[uPtrPos++] = '\0';
        return uPtrPos;
    }
    if (number < 0) {
        number = -number;
        pBuffer[uPtrPos++] = '-';
    }
    
    INT64 div = Int64FindNearestPowOf10(number);
    
    INT32 farLeftDigit;
    while (div) 
    {
        farLeftDigit = number / div;
        number -= farLeftDigit * div;
        div /= 10;

        pBuffer[uPtrPos++] = '0' + farLeftDigit;
    }


    pBuffer[uPtrPos++] = '\0';

    return uPtrPos;
}

} // !Core::Memory
