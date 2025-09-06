#pragma once

#include "AppManager.h"
#include "IApplication.h"
#include "ImportExportAppFromExecutable.h"



#ifdef _TESTS
#    include "Tests/TestMaster.h"
#    include "Tests/Memory/HeapAllocatorTests.h"
#    define RUNTESTS() do { ::Core::TestMaster::Get().Run(); } while (0)
#else 
#    define RUNTESTS()
#endif // !_TESTS
       
#ifdef _TESTS_ONLY 
#    define RUNTESTS() do { ::Core::TestMaster::Get().Run(); return 0; } while (0)
#endif // !_TESTS_ONLY 

int main() 
{
    RUNTESTS();

    ::App::AppManager::Get();

    auto* pApp = ::App::ImportApp();

    ::App::AppInit(pApp);
    
    while (::App::AppManager::AreWeAlive()) {
        ::App::AppUpdate(pApp);
    }

    ::App::AppDestroy(pApp);

    ::App::AppDelete(pApp);

    ::App::AppManager::Get().~AppManager();

    return 0;
}
