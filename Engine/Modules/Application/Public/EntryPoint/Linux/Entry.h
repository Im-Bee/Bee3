#pragma once

#include "IApplication.h"
#include "ImportExportAppFromExecutable.h"



#ifdef _TESTS
#    include "Tests/TestMaster.h"
#    include "Tests/Memory/HeapAllocatorTests.h"
#    define RUNTESTS() do { ::Core::TestMaster::Get().Run(); } while (0)
#elif _TESTS_ONLY 
#    include "Tests/TestMaster.h"
#    include "Tests/Memory/HeapAllocatorTests.h"
#    define RUNTESTS() do { ::Core::TestMaster::Get().Run(); return 0; } while (0)
#else 
#    define RUNTESTS()
#endif // !_TESTS
       


int main() 
{
    RUNTESTS();

    auto* pApp = ::App::ImportApp();

    ::App::AppInit(pApp);

    ::App::AppUpdate(pApp);

    ::App::AppDestroy(pApp);

    ::App::AppDelete(pApp);

    return 0;
}
