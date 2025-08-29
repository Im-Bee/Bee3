#include "Tests/TestMaster.h"

#include "Console.h"
#include "Memory/RawString.h"



namespace Core
{

// ---------------------------------------------------------------------------------------------------------------------
TestMaster::TestMaster()
    : m_uTestAmount(0)
    , m_pTestsBuf()
{ }

// ---------------------------------------------------------------------------------------------------------------------
void TestMaster::AddTest(PVOIDFN pTest, const CHAR8* pszTestName, USIZE uTestNameLen)
{ 
    m_pTestsBuf[m_uTestAmount++] = Test { pTest, pszTestName, uTestNameLen };
}

// ---------------------------------------------------------------------------------------------------------------------
void TestMaster::Run()
{ 
    Core::WriteToConsole("Starting tests...");

    UINT32 uTestsPassed = 0;
    for (USIZE i = 0; i < m_uTestAmount; ++i) {
        try {
            m_pTestsBuf[i].pTest();
            ++uTestsPassed;
        }
        catch (...)
        { 
            Core::WriteToConsole("Test not passed!: ");
            Core::WriteToConsoleN(m_pTestsBuf[i].pszTestName, m_pTestsBuf[i].uTestNameLen);
        }
    }

    CHAR8 pszPassed[16] = { 0 };
    Core::Memory::Int32ToString(uTestsPassed, pszPassed);
    Core::WriteToConsoleN(pszPassed, 16);
    Core::WriteToConsoleN(" tests passed out of ");
    CHAR8 pszOutOf[16] = { 0 };
    Core::Memory::Int32ToString(m_uTestAmount, pszOutOf);
    Core::WriteToConsole(pszOutOf, 16);
    Core::WriteToConsole("Tests finished...");
}

}

