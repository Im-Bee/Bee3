#pragma once


#include "Assert.h"
#include "Memory/HeapAllocator.h"
#include "Memory/Utils.h"
#include "Tests/TestInstance.h"



namespace Tests 
{

TEST(HeapAlloctor)
{
    Core::Memory::HeapAllocator da = { };

    int* pInt = reinterpret_cast<int*>(da.Allocate(sizeof(int) * 100, alignof(int)));
    int* pInt2 = reinterpret_cast<int*>(da.Allocate(sizeof(int) * 100, alignof(int)));
    int* pInt3 = reinterpret_cast<int*>(da.Allocate(sizeof(int) * 100, alignof(int)));
    da.Deallocate(pInt2);
    int* pInt4 = reinterpret_cast<int*>(da.Allocate(sizeof(int) * 10, alignof(int)));

    for (int i = 0; i < 100; ++i) {
        pInt[i] = i;
    }
    for (int i = 0; i < 100; ++i) {
        pInt3[i] = i;
    }

    pInt4[0] = 100;
    pInt4[99] = 100;


    ASSERT(reinterpret_cast<USIZE>(pInt ) % alignof(int), 0);
    ASSERT(reinterpret_cast<USIZE>(pInt2) % alignof(int), 0);
    ASSERT(reinterpret_cast<USIZE>(pInt3) % alignof(int), 0);
    ASSERT(reinterpret_cast<USIZE>(pInt4) % alignof(int), 0);

    ASSERT(pInt4[0], 100);
    ASSERT(pInt4[99], 100);

    ASSERT(pInt3[0], 0);
    ASSERT(pInt3[55], 55);
    ASSERT(pInt3[99], 99);
    
    da.Deallocate(pInt4);
    da.Deallocate(pInt3);

    class Object 
    {
    public:
        Object() 
            : a(1)
            , b(20)
            , c(30)
        { }
        ~Object() = default;

        int a = -1;
        int b = -1;
        int c = -1;
    };

    Object* pObj = reinterpret_cast<Object*>(da.Allocate(sizeof(Object) * 10, alignof(Object)));
    Object& object = *Core::Memory::PlaceAt(pObj);
    ASSERT(object.a, 1);
    ASSERT(object.b, 20);
    ASSERT(object.c, 30);
    ASSERT(reinterpret_cast<USIZE>(pObj) % alignof(Object), 0);
}

TEST(HeapAlloctorReallocs)
{
    Core::Memory::HeapAllocator da(8);

    USIZE uSize = 240;
    USIZE uSizeInBytes = uSize * sizeof(double);
    constexpr USIZE uAligment = alignof(double);

    double* pDoubleBuffer = reinterpret_cast<double*>(da.Allocate(uSizeInBytes, uAligment));

    for (int i = 0; i < uSize; ++i) {
        pDoubleBuffer[i] = static_cast<double>(i) + 0.5;
    }

    uSize = 480;
    uSizeInBytes = uSize * sizeof(double);
    double* pRealloc = reinterpret_cast<double*>(da.Reallocate(pDoubleBuffer, uSizeInBytes, uAligment));

    ASSERT(pRealloc, pDoubleBuffer);

    for (int i = 0; i < 240; ++i) {
        ASSERT(pDoubleBuffer[i], static_cast<double>(i) + 0.5);
    }

    for (int i = 0; i < uSize; ++i) {
        pDoubleBuffer[i] = static_cast<double>(i) + 0.5;
        ASSERT(pDoubleBuffer[i], static_cast<double>(i) + 0.5);
    }
}

TEST(HeapAlloctorAligment)
{
    Core::Memory::HeapAllocator da(16);

    double* pDoubleBuffer = reinterpret_cast<double*>(da.Allocate(240 * sizeof(double), alignof(double)));
    ASSERT(reinterpret_cast<USIZE>(pDoubleBuffer) % alignof(double), 0);
    double* pRealloc = reinterpret_cast<double*>(da.Reallocate(pDoubleBuffer, 240 * sizeof(double), alignof(double)));
    ASSERT(reinterpret_cast<USIZE>(pRealloc) % alignof(double), 0);

    class TestClass
    {
        UINT32 a, a1, a2;
        UINT32 buff[58];
        INT32 b;
        INT16 c;
        INT32 d;
        CHAR8 e;
    };

    TestClass* pClass = reinterpret_cast<TestClass*>(da.Allocate(240 * sizeof(TestClass), alignof(TestClass)));
}

} // !Tests

