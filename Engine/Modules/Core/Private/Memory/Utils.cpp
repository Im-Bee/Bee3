#include "Memory/Utils.h"

// ---------------------------------------------------------------------------------------------------------------------
void* operator new(unsigned long, void* pPtr) noexcept
{ return pPtr; }

