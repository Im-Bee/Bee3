#pragma once

#include "TestMaster.h"

#if _TESTS
#   define TEST(PTESTFUN)   void PTESTFUN##_TEST();                                                                            \
                            static Core::TestInstance PTESTFUN##_TESTSTRCT = Core::TestInstance(PTESTFUN##_TEST, #PTESTFUN);   \
                            void PTESTFUN##_TEST()
#else
#   define TEST(PTESTFUN)
#endif // !_TESTS

namespace Core
{

struct TestInstance
{
    TestInstance(PVOIDFN pTest, const CHAR8 szName[]);
    ~TestInstance() = default;
};

} // !Core
