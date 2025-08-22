#include "Tests/TestInstance.h"
#include "Tests/TestMaster.h"

namespace Core
{

// ---------------------------------------------------------------------------------------------------------------------
TestInstance::TestInstance(PVOIDFN pTest)
{
    TestMaster::Get().AddTest(pTest);
}

} // !Core

