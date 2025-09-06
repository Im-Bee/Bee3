#include "Tests/TestInstance.h"
#include "Memory/RawString.h"
#include "Tests/TestMaster.h"

namespace Core
{

// ---------------------------------------------------------------------------------------------------------------------
TestInstance::TestInstance(PVOIDFN pTest, const CHAR8 szName[])
{ TestMaster::Get().AddTest(pTest, szName, Memory::ZStrLen(szName)); }

} // !Core

