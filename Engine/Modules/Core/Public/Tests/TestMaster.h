#pragma once

#include "ImportExportMacros.h"
#include "Types.h"

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

    void AddTest(PVOIDFN pTest, const CHAR8* pszTestName, USIZE uTestNameLen);

    void Run();
    
private:

    struct Test 
    {
        PVOIDFN         pTest;
        const CHAR8*    pszTestName;
        USIZE           uTestNameLen;
        bool            bPassed;
    };

private:

    USIZE m_uTestAmount;
    Test m_pTestsBuf[64];

};

}
