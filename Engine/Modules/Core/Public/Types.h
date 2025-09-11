#pragma once

#ifdef __linux__
#   include "LinuxTypes.h"
#endif // !__linux__
       

namespace Core
{

typedef unsigned long long int USIZE;
typedef          long long int ISIZE;

typedef char CHAR8;

typedef unsigned char UCHAR8;

typedef void (*PVOIDFN)();

typedef char BYTE;

} // !Core
