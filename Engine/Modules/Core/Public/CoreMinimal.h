#pragma once

#if __linux__ 
#   include "LinuxMinimal.h"
#endif

#include "Exception.h"
#include "Assert.h"

#include "Memory/Utils.h"

#include "Tests/TestInstance.h"

#ifdef _TESTS
#   include "Tests/TestMaster.h"
#   define RUNTESTS() do { ::Core::TestMaster::Get().Run(); } while (0)
#else 
#   define RUNTESTS()
#endif // !_TESTS
       
#ifdef _TESTS_ONLY 
#   define RUNTESTS() do { ::Core::TestMaster::Get().Run(); return 0; } while (0)
#endif // !_TESTS_ONLY 
       
