#pragma once

#include "TestMaster.h"

#ifdef _TESTS
#   define TEST(PTESTFUN)   void PTESTFUN##_TEST();                                                                 \
                            static Core::TestInstance PTESTFUN##_TESTSTRCT = Core::TestInstance(PTESTFUN##_TEST);   \
                            void PTESTFUN##_TEST()
#else
#   define TEST(PTESTFUN)
#endif // !_TESTS

namespace Core
{

struct TestInstance
{
    TestInstance(PVOIDFN pTest);
    ~TestInstance() = default;
};

} // !Core
