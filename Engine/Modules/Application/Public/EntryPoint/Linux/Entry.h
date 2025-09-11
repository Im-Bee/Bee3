#pragma once

#include "CoreMinimal.h"
#include "IApplication.h"
#include "ImportExportAppFromExecutable.h"
#include "AppManager.h"


int main() 
{
    RUNTESTS();

    auto* pApp = ::App::ImportApp();

    ::App::AppInit(pApp);
    
    while (::App::AppManager::AreWeAlive()) {
        ::App::AppUpdate(pApp);
    }

    ::App::AppDestroy(pApp);

    ::App::AppDelete(pApp);

    return 0;
}
