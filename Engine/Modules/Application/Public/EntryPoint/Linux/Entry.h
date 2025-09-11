#pragma once

#include "CoreMinimal.h"
#include "IApplication.h"
#include "ImportExportAppFromExecutable.h"
#include "AppManager.h"


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
