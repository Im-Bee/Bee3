#pragma once

#ifdef __linux__
#   include "LinuxTypes.h"
#endif // !__linux__
       

namespace Core
{

// ---------------------------------------------------------------------------------------------------------------------
typedef void (*PVOIDFN)();

typedef char BYTE;

} // !Core
