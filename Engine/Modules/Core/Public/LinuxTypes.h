#pragma once

#include "LinuxSystemIncludes.h"

#ifdef __linux__

typedef int8_t  INT8;
typedef int32_t INT16;
typedef int32_t INT32;
typedef int64_t INT64;

typedef uint8_t  UINT8;
typedef uint32_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;

typedef unsigned long long int USIZE;
typedef          long long int ISIZE;

typedef char CHAR8;

typedef unsigned char UCHAR8;

#endif // !__linux__
