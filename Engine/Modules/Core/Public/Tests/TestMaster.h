#pragma once

#include "CoreMinimal.h"

namespace Core
{

class API TestMaster
{
    TestMaster();

    TestMaster(const TestMaster&) = delete;
    TestMaster(TestMaster&&) = delete;
    
    TestMaster& operator=(const TestMaster&) = delete;
    TestMaster& operator=(TestMaster&&) = delete;

public:

    static TestMaster& Get()
    {
        static TestMaster instance = { };
        return instance;
    }
    
    ~TestMaster() = default;

public:

    void AddTest(PVOIDFN pTest);

    void Run();

private:

    USIZE m_uTestAmount;
    PVOIDFN m_pTestsBuf[64];

};

}
