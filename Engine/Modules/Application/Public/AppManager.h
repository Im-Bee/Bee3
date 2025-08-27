#pragma once

#include "CoreMinimal.h"


namespace App
{

class API AppManager
{

    AppManager();

    AppManager(const AppManager&) = delete;
    AppManager(AppManager&&) = delete;
    
    AppManager& operator=(const AppManager&) = delete;
    AppManager& operator=(AppManager&&) = delete;

public:

    static AppManager& Get()
    {
        static AppManager instance = { };
        return instance;
    }
    
    ~AppManager() = default;

public:

    static bool AreWeAlive()
    { return m_AreWeAlive; }

    static void Die();

private:

    static volatile bool m_AreWeAlive;

};

} // !App
